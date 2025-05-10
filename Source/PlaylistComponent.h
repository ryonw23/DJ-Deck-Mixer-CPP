#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"

#include <vector>
#include <string>

class DeckGUI;

//==============================================================================
/*
*/
class PlaylistComponent  :  public juce::Component,
                            public TableListBoxModel,
                            public Button::Listener,
                            public FileDragAndDropTarget
{
public:
    PlaylistComponent(DJAudioPlayer* _player,
                      AudioFormatManager & formatManagerToUse,
                      DeckGUI& deck1,
                      DeckGUI& deck2);
    ~PlaylistComponent() override;

    // Graphics initialisation
    void paint (juce::Graphics&) override;
    void resized() override;
    
    // Table component functions
    int getNumRows() override;
    void paintRowBackground(Graphics & g, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell(Graphics & g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
    Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate) override;
    
    // File dragging listeners
    bool isInterestedInFileDrag (const StringArray &files) override;
    void filesDropped (const StringArray &files, int x, int y) override; 
    
    // Button listeners
    void buttonClicked(Button * button) override;

private:
    // Table component of playlistComponent
    TableListBox playlistTableComponent;
    
    // Vector to store JUCE File objects for playlist
    std::vector<juce::File> trackFiles;
    
    // Text upon opening file explorer when load button pressed
    juce::FileChooser fChooser{"Select a file..."};
    
    // Initialise button to add tracks to playlist
    TextButton loadIntoPlaylist{"ADD TO PLAYLIST"};
    
    // Initialise instance of DJAudioPlayer for preview player
    DJAudioPlayer* player;
    
    // Reference to deckGUI 1 and 2 for functions - updateQueue()
    DeckGUI& deckGUI1;
    DeckGUI& deckGUI2;

    // Function to remove items from playlist vector
    void deleteFromPlaylist(int id);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
