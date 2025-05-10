#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "PlaylistComponent.h"
#include "CustomLookAndFeel.h"
#include "DiscLookAndFeel.h"

//==============================================================================
/*
*/
class DeckGUI    : public Component,
                   public Button::Listener, 
                   public Slider::Listener, 
                   public FileDragAndDropTarget, 
                   public Timer,
                   public TableListBoxModel
{
public:
    DeckGUI(DJAudioPlayer* player, 
            AudioFormatManager & formatManagerToUse,
            AudioThumbnailCache & cacheToUse,
            std::vector<juce::File>& queue);
    ~DeckGUI();

    // Graphics initialisation
    void paint (Graphics&) override;
    void paintOverChildren (Graphics&) override;
    void resized() override;

    // Button and slider listeners
    void buttonClicked (Button *) override;
    void sliderValueChanged (Slider *slider) override;

    // File dragging listeners
    bool isInterestedInFileDrag (const StringArray &files) override;
    void filesDropped (const StringArray &files, int x, int y) override; 

    // Allow for ongoing logic checks/calls
    void timerCallback() override;
    
    // Table component functions
    int getNumRows() override;
    void paintRowBackground(Graphics & g, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell(Graphics & g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
    Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate) override;
    
    // Function to update queue upon any changes
    void updateQueue();
    
private:
    // Text upon opening file explorer when load button pressed
    juce::FileChooser fChooser{"Select a file..."};

    // Initialise buttons for deck GUI
    TextButton playButton{"PLAY"};
    TextButton pauseButton{"PAUSE"};
    TextButton clearButton{"CLEAR"};
    TextButton loadButton{"LOAD"};
  
    // Initialise sliders for deck GUI
    Slider volSlider;
    Slider speedSlider;
    Slider posSlider;
    
    // Initialise sliders for bass and treble in deck GUI
    Slider lowSlider;
    Slider highSlider;
    
    // Custom graphics for sliders
    CustomLookAndFeel customGraphics;
    DiscLookAndFeel discGraphics;
    
    // Waveform upon loading track into deck GUI
    WaveformDisplay waveformDisplay;

    // Initialise audio player pointer - points back to initialisation of DeckGUI in MainComponent
    DJAudioPlayer* player;
    
    // Table component of deck GUI (queue)
    TableListBox deckQueue;
    
    // Vector of queue according to deck
    std::vector<juce::File>& queue;
    
    // Function to remove items from queue vector
    void deleteFromQueue(int id);
    void loadNextTrack();
    
    // Function to reset any sliders back to original value
    void defaultSliders();
    
    // Functions and doubles to allow for printing time under position slider
    std::string printTime(double seconds);
    double currentPosition;
    double totalTrackLength;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
