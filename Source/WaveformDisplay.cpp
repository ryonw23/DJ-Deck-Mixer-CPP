#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(AudioFormatManager & formatManagerToUse,
                                 AudioThumbnailCache & cacheToUse)
: audioThumb(1000, formatManagerToUse, cacheToUse), fileLoaded(false), position(0)
                          
{
    audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (Graphics& g)
{
    // Set background colour to standard very dark blue (almost black) - match across all components
    g.fillAll (Colour(1,1,30));

    // Draw outline around a single deck with a bright pink colour
    g.setColour (Colours::mediumvioletred);
    g.drawRect (getLocalBounds(), 1);

    // Check if there is a active file loaded
    if(fileLoaded)
    {
        // Set colour of waveform to bright purple
        g.setColour (Colour(191, 0, 255));
        
        // Draw the waveform output of loaded track
        audioThumb.drawChannel(g, getLocalBounds(), 0, audioThumb.getTotalLength(), 0, 1.0f);
        
        // Set colour of position pointer to bright green
        g.setColour(Colours::lightgreen);
        
        // Draw the position pointer moving with the track position
        g.drawRect(position * getWidth(), 0, 1, getHeight());
    }
    // If file is not loaded
    else
    {
        // Draw a text indicating file not loaded in white
        g.setColour(Colours::ghostwhite);
        g.setFont (20.0f);
        g.drawText ("File not loaded...", getLocalBounds(), Justification::centred, true);
    }
}

void WaveformDisplay::resized()
{
}

void WaveformDisplay::clearWaveform()
{
    // Call clear function to remove current waveform graphic
    audioThumb.clear();
    
    // Set file loaded flag to false
    fileLoaded = false;
    
    // Call repaint to apply changes made
    repaint();
}

void WaveformDisplay::loadURL(URL audioURL)
{
    // Call clear function to remove current waveform graphic
    audioThumb.clear();
    
    // Load audio file from URL as new source for waveform display
    // .setSource returns true if file is successfully loaded
    fileLoaded  = audioThumb.setSource(new URLInputSource(audioURL));
    
    // Check if file is loaded
    if (fileLoaded)
    {
        // Print to command line for debugging
        std::cout << "wfd: loaded! " << std::endl;
        
        // Call repaint to apply changes made (new waveform display)
        repaint();
    }
    else
    {
        // Print to command line for debugging
        std::cout << "wfd: not loaded! " << std::endl;
        
        // Do nothing else
    }
}

// Check if audio source has changed
void WaveformDisplay::changeListenerCallback (ChangeBroadcaster *source)
{
    // Print to command line for debugging
    std::cout << "wfd: change received! " << std::endl;

    // Call repaint to apply changes made (new audio source)
    repaint();
}

void WaveformDisplay::setPositionRelative(double pos)
{
    // Check if new position is different from current position
    if (pos != position)
    {
        // Update current position to new position
        position = pos;
        
        // Call repaint to apply changes made (new position)
        repaint();
    }
}




