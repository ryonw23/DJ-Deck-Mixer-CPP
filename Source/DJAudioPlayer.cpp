#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(AudioFormatManager& _formatManager) 
: formatManager(_formatManager)
{
}

DJAudioPlayer::~DJAudioPlayer()
{
}

void DJAudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate) 
{
    // Initialise audio mixer source ONLY (all other audio sources have been layered/chained to final audio source resampleSource)
    mixerSource.addInputSource(&resampleSource, false);
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    // Obtain audio signals
    mixerSource.getNextAudioBlock(bufferToFill);
}

void DJAudioPlayer::releaseResources()
{
    // Let the source release anything no longer needed after playback stopped
    mixerSource.releaseResources();
    transportSource.releaseResources();
    bassfilterSource.releaseResources();
    treblefilterSource.releaseResources();
    resampleSource.releaseResources();
}

void DJAudioPlayer::loadURL(URL audioURL)
{
    // Create an AudioFormatReader for an input audio URL - read audio file and extract sample data
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    
    // Check if reader successfully created with valid file
    if (reader != nullptr)
    {
        // Create a unique pointer to an AudioFormatReaderSource - connecting audio reader with audio transport source, allows control of audio
        std::unique_ptr<AudioFormatReaderSource> newSource (new AudioFormatReaderSource (reader, true));
        
        // Set the transport source to use the new reader source - stream audio from the file from reader
        transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);
        
        readerSource.reset (newSource.release());
    }
}

void DJAudioPlayer::setGain(double gain)
{
    // Check if the input value is not between 0 and 2
    if (gain < 0 || gain > 2.0)
    {
        // Print to command line for debugging
        std::cout << "DJAudioPlayer::setGain gain should be between 0 and 2" << std::endl;
    }
    else
    {
        // Adjust the gain/volume of the audio source if input is valid
        transportSource.setGain(gain);
    }
}
void DJAudioPlayer::setSpeed(double ratio)
{
    // Check if the input value is not between 0 and 2
    if (ratio < 0 || ratio > 2.0)
    {
        // Print to command line for debugging
        std::cout << "DJAudioPlayer::setSpeed ratio should be between 0 and 5" << std::endl;
    }
    else
    {
        // Adjust the speed of the audio source if input is valid
        resampleSource.setResamplingRatio(ratio);
    }
}

void DJAudioPlayer::setPosition(double posInSecs)
{
    // Adjust the position of the audio source
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
    // Check if the input is not between 0 and 1
    if (pos < 0 || pos > 1.0)
    {
        // Print to command line for debugging
        std::cout << "DJAudioPlayer::setPositionRelative pos should be between 0 and 1" << std::endl;
    }
    else
    {
        // Initialise double to store position of track in seconds
        // based on position of track from 0 to 1 - basically map relative position to actual position
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        // Call setPosition based on seconds
        setPosition(posInSecs);
    }
}

void DJAudioPlayer::start()
{
    // Start audio playback if source is selected
    transportSource.start();
}
void DJAudioPlayer::pause()
{
    // Stop audio playback
    transportSource.stop();
}
void DJAudioPlayer::stop()
{
    // Remove currently loaded audio source - for clear button
    transportSource.setSource(nullptr, 0, nullptr, 0);
}

void DJAudioPlayer::preview()
{
    // Randomly set the playback position within the first 80% of the track's duration.
    transportSource.setPosition(((double) rand()/RAND_MAX)*transportSource.getLengthInSeconds() * 0.8);
    
    // Start audio playback from the random position
    transportSource.start();
    
    // Start timer to stop audio playback after 5 seconds
    startTimer(5000);
}

void DJAudioPlayer::timerCallback()
{
    // Print to command line for debugging
    std::cout << "Stopping preview after timeout." << std::endl;
    
    // Stop audio playback
    transportSource.stop();
    
    // Stop timer to prevent any other callback
    stopTimer();
}

double DJAudioPlayer::getPositionRelative()
{
    // Get current playback position as fraction of total track length - between 0 and 1
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

double DJAudioPlayer::getActualPosition()
{
    // Get current playback position in whole seconds (truncate to remove decimals)
    return trunc(transportSource.getCurrentPosition());
}

double DJAudioPlayer::getTotalLength()
{
    // Get total track length in seconds in whole seconds (truncate to remove decimals)
    return trunc(transportSource.getLengthInSeconds());
}

bool DJAudioPlayer::isTrackLoaded()
{
    // Check if track length is more than 0 (if track is loaded) - return true if more than 0, false if less
    return transportSource.getLengthInSeconds() > 0.0;
}

void DJAudioPlayer::setLowGain(float gain)
{
    // Check if a track is loaded by checking valid readerSource
    if (readerSource && readerSource->getAudioFormatReader())
    {
        // Obtain sample rate of loaded track
        auto sampleRate = readerSource->getAudioFormatReader()->sampleRate;
        
        // Check if sample rate is valid
        if (sampleRate > 0)
        {
            // Set low shelf filter coefficients to adjust bass frequencies
            bassfilterSource.setCoefficients(IIRCoefficients::makeLowShelf(sampleRate, 200.0, 1.0, gain));
            // '200.0'Hz is cutoff frequency - apply changes to all frequencies below 200.0
            // '1.0' is the sharpness of filter applied
            // 'gain' based on the input parameter of the function, adjusts volume of frequencies affected
        }
    }
}

void DJAudioPlayer::setHighGain(float gain)
{
    // Check if a track is loaded by checking valid readerSource
    if (readerSource && readerSource->getAudioFormatReader())
    {
        // Obtain sample rate of loaded track
        auto sampleRate = readerSource->getAudioFormatReader()->sampleRate;
        
        // Check if sample rate is valid
        if (sampleRate > 0)
        {
            // Set high shelf filter coefficients to adjust treble frequencies
            treblefilterSource.setCoefficients(IIRCoefficients::makeHighShelf(sampleRate, 5000.0, 1.0, gain));
            // '5000.0'Hz is cutoff frequency - apply changes to all frequencies above 5000.0
            // '1.0' is the sharpness of filter applied
            // 'gain' based on the input parameter of the function, adjusts volume of frequencies affected
        }
    }
}

