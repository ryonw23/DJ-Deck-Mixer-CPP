#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class WaveformDisplay    : public Component, 
                           public ChangeListener
{
public:
    WaveformDisplay(AudioFormatManager & formatManagerToUse,
                    AudioThumbnailCache & cacheToUse );
    ~WaveformDisplay();

    // Graphics initialisation
    void paint (Graphics&) override;
    void resized() override;

    // Function to check when source is changed
    void changeListenerCallback (ChangeBroadcaster *source) override;

    // Function to load given URL to generate waveform
    void loadURL(URL audioURL);
    
    // Reset waveform to empty state
    void clearWaveform();

    // Update position based on relative position - maps actual position to relative position (0-1)
    void setPositionRelative(double pos);

private:
    // Object to generate and display waveform
    AudioThumbnail audioThumb;
    
    // Indicate whether there is a file loaded
    bool fileLoaded;
    
    // Store current playback position as double
    double position;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
