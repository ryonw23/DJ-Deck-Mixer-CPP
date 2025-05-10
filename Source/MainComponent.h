/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;
    
    // Initialise vectors that store queued files - have to be global to allow access from deckGUI AND PlaylistComponent
    static std::vector<juce::File> queue1;
    static std::vector<juce::File> queue2;

private:
    //==============================================================================
    // Your private member variables go here...
    AudioFormatManager formatManager;
    AudioThumbnailCache thumbCache{100}; 

    // Initialise deck 1 with audio manager and GUI
    DJAudioPlayer player1{formatManager};
    DeckGUI deckGUI1{&player1, formatManager, thumbCache, queue1};

    // Initialise deck 2 with audio manager and GUI
    DJAudioPlayer player2{formatManager};
    DeckGUI deckGUI2{&player2, formatManager, thumbCache, queue2};
    
    // Initialise playlist section with audio manager and GUI
    DJAudioPlayer previewPlayer{formatManager};
    PlaylistComponent playlistComponent{&previewPlayer, formatManager, deckGUI1, deckGUI2};
    
    // Initialise mixer audio source to take in input of all other audio sources
    MixerAudioSource mixerSource;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
