#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <juce_dsp/juce_dsp.h>

class DJAudioPlayer : public AudioSource, public Timer {
public:

    DJAudioPlayer(AudioFormatManager& _formatManager);
    ~DJAudioPlayer();

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    // Initialise functions of buttons on deck
    void start();
    void pause();
    void loadURL(URL audioURL);
    void stop();
    
    // Initialise functions of sliders on deck
    void setGain(double gain);
    void setSpeed(double ratio);
    void setPosition(double posInSecs);
    void setPositionRelative(double pos);
    
    // Initialise audio functions of PlaylistComponent
    void preview();
    
    // Allow for ongoing logic checks/calls
    void timerCallback() override;

    // Function to check if DJAudioPlayer is pointing to an object (audio file)
    bool isTrackLoaded();

    // Functions to obtain track length information relative, actual and total
    double getPositionRelative();
    double getActualPosition();
    double getTotalLength();
    
    // Initialise functions for equaliser
    void setLowGain(float gain);
    void setHighGain(float gain);

private:
    AudioFormatManager& formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    
    // Initialise audio managers
    AudioTransportSource transportSource;
    
    // Initialise equaliser filters
    IIRFilterAudioSource bassfilterSource {&transportSource, false};
    IIRFilterAudioSource treblefilterSource {&bassfilterSource, false};
    
    // Initialise playback speed manager
    ResamplingAudioSource resampleSource{&treblefilterSource, false, 2};

    // Initialise mixer audio source to take in input of all other audio sources
    MixerAudioSource mixerSource;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DJAudioPlayer)
};




