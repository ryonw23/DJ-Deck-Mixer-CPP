#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player, 
                 AudioFormatManager & 	formatManagerToUse,
                 AudioThumbnailCache & 	cacheToUse,
                 std::vector<juce::File>& _queue)
: waveformDisplay(formatManagerToUse, cacheToUse), player(_player), queue(_queue)
{
    // Initialise and draw buttons
    addAndMakeVisible(playButton);
    addAndMakeVisible(pauseButton);
    addAndMakeVisible(clearButton);
    addAndMakeVisible(loadButton);
    
    // Initialise and draw sliders
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    
    // Initialise and draw sliders for bass and treble
    addAndMakeVisible(lowSlider);
    addAndMakeVisible(highSlider);

    // Initialise and draw waveform section
    addAndMakeVisible(waveformDisplay);

    // Initialise listeners for buttons
    playButton.addListener(this);
    pauseButton.addListener(this);
    clearButton.addListener(this);
    loadButton.addListener(this);

    // Initialise listeners for sliders
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);
    
    // Initialise listeners for bass and treble sliders
    lowSlider.addListener(this);
    highSlider.addListener(this);
    
    // Apply colour and styling to buttons
    playButton.setColour(juce::TextButton::buttonColourId, Colours::darkgreen);
    pauseButton.setColour(juce::TextButton::buttonColourId, Colours::darkgoldenrod);
    loadButton.setColour(juce::TextButton::buttonColourId, Colours::darkblue);
    clearButton.setColour(juce::TextButton::buttonColourId, Colours::firebrick);
    
    volSlider.setSliderStyle(juce::Slider::Rotary);
    speedSlider.setSliderStyle(juce::Slider::Rotary);
    posSlider.setSliderStyle(juce::Slider::Rotary);
    
    volSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    speedSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    posSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    
    // Initialise range, step and initial value of sliders
    volSlider.setRange(0.0, 100.0, 1.0);
    volSlider.setValue(50.0);
    
    speedSlider.setRange(0.000000001, 2.0, 0.05);
    speedSlider.setValue(1.0);
    
    posSlider.setRange(0.0, 1.0);
    posSlider.setValue(0.0);
    
    // Apply styling and colours for bass and treble sliders
    lowSlider.setSliderStyle(juce::Slider::LinearBarVertical);
    highSlider.setSliderStyle(juce::Slider::LinearBarVertical);
    
    highSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    lowSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);

    highSlider.setColour(juce::Slider::trackColourId, Colours::lightskyblue);
    lowSlider.setColour(juce::Slider::trackColourId, Colours::darkmagenta);
    
    // Initialise range, step and initial value of bass and treble sliders
    lowSlider.setRange(1.0, 49.0, 0.5);
    lowSlider.setValue(1.0);
    
    highSlider.setRange(1.0, 49.0, 0.5);
    highSlider.setValue(1.0);
    
    // Apply custom graphics to sliders
    posSlider.setLookAndFeel(&discGraphics);
    volSlider.setLookAndFeel(&customGraphics);
    speedSlider.setLookAndFeel(&customGraphics);
    
    // Initialise table columns for queue component
    deckQueue.getHeader().addColumn("Queue", 1, 200);
    deckQueue.getHeader().addColumn("Delete", 2, 200);
    deckQueue.setModel(this);
    
    // Initialise and draw table component
    addAndMakeVisible(deckQueue);
    
    // Apply styling for table component
    deckQueue.setColour(ListBox::backgroundColourId, Colour(1,1,30));
    deckQueue.getHeaderComponent()->setColour(TableHeaderComponent::backgroundColourId, Colours::slategrey);
    deckQueue.getHeaderComponent()->setColour(TableHeaderComponent::textColourId, Colour(20,20,20));
    deckQueue.repaint();

    // Initialise timer to be called back every 100 milliseconds
    startTimer(100);
}

DeckGUI::~DeckGUI()
{
    // Reset the look and feel of all sliders to the default JUCE style - prevents reference to invalid object
    posSlider.setLookAndFeel(nullptr);
    volSlider.setLookAndFeel(nullptr);
    speedSlider.setLookAndFeel(nullptr);
    lowSlider.setLookAndFeel(nullptr);
    highSlider.setLookAndFeel(nullptr);
    
    // Stop timer to prevent further callbacks
    stopTimer();
}

// Normal paint function will only create graphics under the items like buttons, sliders, etc.
void DeckGUI::paint (Graphics& g)
{
    // Initialise row height and buffer to apply positioning
    double rowH = getHeight() / 9;
    double buffer = 10;

    // Set background colour to very dark blue (almost black)
    g.fillAll (Colour(1,1,30));

    // Draw outline around a single deck with a bright pink colour
    g.setColour (Colours::mediumvioletred);
    g.drawRect (getLocalBounds(), 1);

    // Prepare text styling
    g.setColour (Colours::ghostwhite);
    g.setFont (14.0f);
        
    // All text for speed slider - labels and notches
    g.drawText("Speed", buffer * 2   , rowH + buffer      , getWidth()/4, rowH * 2, Justification::centredBottom);
    g.drawText("0.0x" , buffer * 6.5 , rowH*2.7 + buffer/2, getWidth()/4, rowH, Justification::topLeft);
    g.drawText("0.25x", buffer*2     , rowH*1.9 + buffer/2, getWidth()/4, rowH, Justification::topLeft);
    g.drawText("0.75x", buffer * 6.5 , rowH*1.1 + buffer/2, getWidth()/4, rowH, Justification::topLeft);
    g.drawText("1.25x", -buffer * 2.5, rowH*1.1 + buffer/2, getWidth()/4, rowH, Justification::topRight);
    g.drawText("1.75x", buffer*2     , rowH*1.9 + buffer/2, getWidth()/4, rowH, Justification::topRight);
    g.drawText("2.0x" , -buffer * 2.5, rowH*2.7 + buffer/2, getWidth()/4, rowH, Justification::topRight);

    // All text for volume slider - labels and notches
    g.drawText("Volume", buffer * 2, rowH*4 - buffer    , getWidth()/4, rowH*2 - buffer, Justification::centredTop);
    g.drawText("00", buffer * 7.5  , rowH*5.7 - buffer/2, getWidth()/4, rowH, Justification::topLeft);
    g.drawText("20", buffer * 4    , rowH*4.9 - buffer/2, getWidth()/4, rowH, Justification::topLeft);
    g.drawText("40", buffer * 7.5  , rowH*4.1 - buffer/2, getWidth()/4, rowH, Justification::topLeft);
    g.drawText("60", -buffer * 3.5 , rowH*4.1 - buffer/2, getWidth()/4, rowH, Justification::topRight);
    g.drawText("80",0              , rowH*4.9 - buffer/2, getWidth()/4, rowH, Justification::topRight);
    g.drawText("100", -buffer * 3.5, rowH*5.7 - buffer/2, getWidth()/4, rowH, Justification::topRight);

    // Prepare text for bass slider - labels and notches
    g.drawText("Bass", getWidth()*7/8         , rowH + buffer * 2, getWidth()/16, rowH*2, Justification::centredBottom);
    g.drawText("50 -", getWidth()*6/8 - buffer, rowH + buffer/2, getWidth()/8, rowH, Justification::topRight);
    g.drawText("40 -", getWidth()*6/8 - buffer, rowH*1.35 + buffer/2, getWidth()/8, rowH, Justification::topRight);
    g.drawText("30 -", getWidth()*6/8 - buffer, rowH*1.7 + buffer/2, getWidth()/8, rowH, Justification::topRight);
    g.drawText("20 -", getWidth()*6/8 - buffer, rowH*2.1 + buffer/2, getWidth()/8, rowH, Justification::topRight);
    g.drawText("10 -", getWidth()*6/8 - buffer, rowH*2.45 + buffer/2, getWidth()/8, rowH, Justification::topRight);
    g.drawText("00 -", getWidth()*6/8 - buffer, rowH*2.8 + buffer/2, getWidth()/8, rowH, Justification::topRight);
    
    // Prepare text for treble slider - labels and notches
    g.drawText("Treble", getWidth()*7/8       , rowH*4 - buffer * 3     , getWidth()/16, rowH*2 - buffer * 2, Justification::centredTop);
    g.drawText("50 -", getWidth()*6/8 - buffer, rowH*4 - buffer * 1.5   , getWidth()/8 , rowH, Justification::topRight);
    g.drawText("40 -", getWidth()*6/8 - buffer, rowH*4.35 - buffer * 1.5, getWidth()/8 , rowH, Justification::topRight);
    g.drawText("30 -", getWidth()*6/8 - buffer, rowH*4.7 - buffer * 1.5 , getWidth()/8 , rowH, Justification::topRight);
    g.drawText("20 -", getWidth()*6/8 - buffer, rowH*5.1 - buffer * 1.5 , getWidth()/8 , rowH, Justification::topRight);
    g.drawText("10 -", getWidth()*6/8 - buffer, rowH*5.45 - buffer * 1.5, getWidth()/8 , rowH, Justification::topRight);
    g.drawText("00 -", getWidth()*6/8 - buffer, rowH*5.8 - buffer * 1.5 , getWidth()/8 , rowH, Justification::topRight);
}

// Use paintOverChildren to ensure that graphics appear on top of items like buttons, sliders, etc.
void DeckGUI::paintOverChildren(Graphics &g)
{
    // Initialise row height and buffer to apply positioning - ensure its the same as in paint
    double rowH = getHeight() / 9;
    double buffer = 10;

    // Check if each button is being pressed
    // If a button is pressed, slightly reduce its bounds to create a visual press effect
    // If not pressed, set it to its normal size
    
    // Handle play button
    if(playButton.isDown())
    {
        playButton.setBounds(buffer*1.1, buffer*1.1, getWidth()/4 - buffer * 2.2, rowH - buffer * 2.2);
    }
    else
    {
        playButton.setBounds(buffer, buffer, getWidth()/4 - buffer * 2, rowH - buffer * 2);
    }

    // Handle pause button
    if(pauseButton.isDown())
    {
        pauseButton.setBounds(getWidth()/4 + buffer*1.1, buffer*1.1, getWidth()/4 - buffer * 2.2, rowH - buffer * 2.2);
    }
    else
    {
        pauseButton.setBounds(getWidth()/4 + buffer, buffer, getWidth()/4 - buffer * 2, rowH - buffer * 2);
    }

    // Handle load button
    if(loadButton.isDown())
    {
        loadButton.setBounds(getWidth()/2 + buffer*1.1, buffer*1.1, getWidth()/4 - buffer * 2.2, rowH - buffer * 2.2);
    }
    else
    {
        loadButton.setBounds(getWidth()/2 + buffer, buffer, getWidth()/4 - buffer * 2, rowH - buffer * 2);
    }

    // Handle clear button
    if(clearButton.isDown())
    {
        clearButton.setBounds(getWidth()*3/4 + buffer*1.1, buffer*1.1, getWidth()/4 - buffer * 2.2, rowH - buffer * 2.2);
    }
    else
    {
        clearButton.setBounds(getWidth()*3/4 + buffer, buffer, getWidth()/4 - buffer * 2, rowH - buffer * 2);
    }
    
    // Draw the time in minutes and seconds of the track
    g.setColour (Colours::ghostwhite);
    g.setFont (14.0f);
    
    // Current position of track
    g.drawText(printTime(currentPosition), getWidth()/4 - buffer * 2, rowH * 5 - buffer, getWidth()/2, rowH, Justification::centredBottom);
    g.drawText("/", getWidth()/4, rowH * 5 - buffer, getWidth()/2, rowH, Justification::centredBottom);
    
    // Total length of track
    g.drawText(printTime(totalTrackLength), getWidth()/4 + buffer * 2, rowH * 5 - buffer, getWidth()/2, rowH, Justification::centredBottom);
    
    // Call repaint to apply changes made
    repaint();
}

void DeckGUI::resized()
{
    // Initialise row height and buffer to apply positioning - ensure its the same as in paint
    double rowH = getHeight() / 9;
    double buffer = 10;
    
    // Place position slider in the middle of the deck
    posSlider.setBounds(getWidth()/4, rowH - buffer * 1.5, getWidth()/2, rowH * 5);
    
    // Place speed slider on top left
    speedSlider.setBounds(buffer * 2, rowH + buffer, getWidth()/4, rowH*2 - buffer);
    
    // Place volume slider on bottom left
    volSlider.setBounds(buffer * 2, rowH*4, getWidth()/4, rowH*2 - buffer);
        
    // Place bass slider on top right
    lowSlider.setBounds(getWidth()*7/8, rowH + buffer, getWidth()/16, rowH*2 - buffer);
    
    // Place treble slider on bottom left
    highSlider.setBounds(getWidth()*7/8, rowH*4 - buffer, getWidth()/16, rowH*2 - buffer);
    
    // Place waveform below main deck
    waveformDisplay.setBounds(0, rowH * 6, getWidth(), rowH);
    
    // Initialise column bounds of queue table
    deckQueue.getHeader().setColumnWidth(1, getWidth()*4/5 - buffer * 2);
    deckQueue.getHeader().setColumnWidth(2, getWidth()/5 + buffer * 1.9);
    
    // Place queue at the very bottom of the deck
    deckQueue.setBounds(0, rowH*7, getWidth(), rowH*2 + buffer);
}

void DeckGUI::buttonClicked(Button* button)
{
    // Check if the button clicked is the play button
    if (button == &playButton)
    {
        // Print to console that play button clicked
        std::cout << "Play button was clicked " << std::endl;
        
        // Call start function of DJAudioPlayer
        player->start();
    }
    // Check if the button clicked is the pause button
    else if (button == &pauseButton)
    {
        // Print to console that pause button clicked
        std::cout << "Pause button was clicked " << std::endl;
        
        // Call pause function of DJAudioPlayer
        player->pause();
    }
    // Check if the button clicked is the clear button
    else if (button == &clearButton)
    {
        // Print to console that clear button clicked
        std::cout << "Clear button was clicked " << std::endl;
        
        // Call stop function of DJAudioPlayer
        player->stop();
        
        // Remove current waveform
        waveformDisplay.clearWaveform();
        
        // Reset sliders to original values
        defaultSliders();
    }
    // Check if the button clicked is the load button
    else if (button == &loadButton)
    {
        // Print to console that load button clicked
        std::cout << "Load button was clicked " << std::endl;
        
        // Reset sliders to original values
        defaultSliders();
        
        // Set up file chooser to only allow files
        auto fileChooserFlags = FileBrowserComponent::canSelectFiles;
        
        // Launch file chooser dialog
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
        {
            // Obtain user selected file
            File chosenFile = chooser.getResult();
            
            // Check if chosen file is valid
            if (chosenFile.exists())
            {
                // Load file into DJAudioPlayer
                player->loadURL(URL{chooser.getResult()});
                
                // Load file into waveformDisplay
                waveformDisplay.loadURL(URL{chooser.getResult()});
            }
        });
    }
    // Handle button presses in table component (queue)
    else
    {
        // Convert button component id into integer - integer used to reference corresponding file in queue
        int id = std::stoi(button -> getComponentID().toStdString());
        
        // Print id to command line for debugging
        std::cout << id << std::endl;
        
        // Check if the button clicked is delete button in table component
        if (button->getButtonText() == "Delete")
        {
            // Obtain file from queue according to id
            juce::File selectedFile = queue[id];
            
            // Print to command line what file is being deleted from queue
            std::cout << "Deleting from queue: " << selectedFile.getFullPathName() << std::endl;
            
            // Remove file from queue
            deleteFromQueue(id);
            
            // Update the queue display to reflect changes
            updateQueue();
        }
    }
}

void DeckGUI::sliderValueChanged (Slider *slider)
{
    // Obtain current value of slider
    float value = slider->getValue();
    
    // Check if adjusted slider is volume slider
    if (slider == &volSlider)
    {
        // Set gain from DJAudioPlayer based on slider value - divide by 50 to map 0-100 to 0-2
        player->setGain(value/50);
    }
    
    // Check if adjusted slider is speed slider
    if (slider == &speedSlider)
    {
        // Set speed from DJAudioPlayer based on slider value
        player->setSpeed(value);
    }
    
    // Check if adjusted slider is position slider
    if (slider == &posSlider)
    {
        // Set position from DJAudioPlayer based on slider value
        player->setPositionRelative(value);
    }
    
    // Check if adjusted slider is bass slider
    if (slider == &lowSlider)
    {
        // Set low gain from DJAudioPlayer based on slider value
        player->setLowGain(value);
    }
    
    // Check if adjusted slider is treble slider
    if (slider == &highSlider)
    {
        // Set high gain from DJAudioPlayer based on slider value
        player->setHighGain(value);
    }
}

bool DeckGUI::isInterestedInFileDrag (const StringArray &files)
{
    // Print message to console for debugging
    std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
    
    // Return true to indicate component accepts file dragging
    return true;
}

void DeckGUI::filesDropped (const StringArray &files, int x, int y)
{
    // Print message to console for debugging
    std::cout << "DeckGUI::filesDropped" << std::endl;
    
    // Check that only 1 file is dragged
    if (files.size() == 1)
    {
        // Convert dragged file into JUCE File object then add to queue
        queue.push_back({File{files[0]}});
        
        // Update the queue display to reflect changes
        updateQueue();
    }
}

// Callback every 100 milliseconds
void DeckGUI::timerCallback()
{
    // Update position of waveform display based on DJAudioPlayer current position
    waveformDisplay.setPositionRelative(player->getPositionRelative());
    
    // Check if there is no track loaded in player
    if(!(player->isTrackLoaded()))
    {
        // Check if queue is not empty, load the next track automatically
        if (!queue.empty())
        {
            // Reset sliders to original values
            defaultSliders();
            
            // Call function to load the next track in queue
            loadNextTrack();
            
            // Update the queue (display)
            updateQueue();
        }
        // Reset position tracking after new track loaded (overwrite old track position values)
        currentPosition = 0.0;
        totalTrackLength = 0.0;
    }
    else
    {
        // Update position slider to follow track position - shows the pointer on position slider rotating as track plays
        posSlider.setValue(player->getPositionRelative());
        
        // Obtain current position and total track length of active track
        currentPosition = player->getActualPosition();
        totalTrackLength = player->getTotalLength();
    }
    
    // Check if track has finished playing
    if (player->getPositionRelative() >= 1.0)
    {
        // Print to command that track has ended
        std::cout << "Track Ended" << std::endl;
        
        // Check if the queue is not empty, 
        if (!queue.empty())
        {
            
            // Reset sliders to original values
            defaultSliders();
            
            // Call function to load the next track in queue
            loadNextTrack();
            
            // Update the queue (display)
            updateQueue();
            
            // Automatically start playing next track
            player->start();
        }
    }
}

// Obtain number of rows in table based on vector size
int DeckGUI::getNumRows()
{
    // Return size of queue vector
    return queue.size();
}

// Paint background of table component
void DeckGUI::paintRowBackground(Graphics & g, int rowNumber, int width, int height, bool rowIsSelected)
{
    // Check if a row is selected
    if(rowIsSelected)
    {
        // Set background colour of row to lighter colour
        g.fillAll(Colour(50,50,80));
    }
    else
    {
        // Set background colour of row to background colour of table
        g.fillAll(Colour(1,1,30));
    }
}

// Paint individual cells of table
void DeckGUI::paintCell(Graphics & g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    // Set outline and text to white
    g.setColour(Colours::ghostwhite);
    
    // Draw outline around cell
    g.drawRect(getLocalBounds(),2);
    
    // Check if its the first column
    if(columnId == 1)
    {
        // For the first column, draw text of the file name
        g.drawText (queue[rowNumber].getFileName(),10, 0, width-4, height, Justification::centredLeft,true);
    }
}

// Table component button listeners
Component* DeckGUI::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate)
{
    // Check if its the second column
    if(columnId == 2)
    {
        // Check if there is no existing component
        if(existingComponentToUpdate == nullptr)
        {
            // Create a new delete button
            TextButton* deleteButton = new TextButton{"Delete"};
            
            // Set colour of button to red
            deleteButton->setColour(TextButton::buttonColourId, Colours::firebrick);
            
            // Initialise listener for delete button
            deleteButton->addListener(this);
            
            // Assign unique id based on row number
            String id{std::to_string(rowNumber)};
            deleteButton->setComponentID(id);
            
            // Let the new delete button be the existing component to update
            existingComponentToUpdate = deleteButton;
        }
    }
    
    // Return updated/new component
    return existingComponentToUpdate;
}

void DeckGUI::deleteFromQueue(int id)
{
    // Chcek if there is any items in queue vector
    if(queue.size()>0)
    {
        // Remove item from queue based on input id
        queue.erase(queue.begin() + id);
    }
}

void DeckGUI::updateQueue()
{
    // Print to command line for debugging
    std::cout << "Updating queue" << std::endl;
    
    // Update display of queue in table component
    deckQueue.updateContent();
    repaint();
}

// Set sliders to default values
void DeckGUI::defaultSliders()
{
    volSlider.setValue(50.0);
    speedSlider.setValue(1.0);
    lowSlider.setValue(1.0);
    highSlider.setValue(1.0);
    posSlider.setValue(0.0);
}

// Function to receive time in seconds to minutes and seconds
std::string DeckGUI::printTime(double seconds)
{
    // Let minutes be the floor of seconds divided by 60
    double minutes = floor(seconds/60);
    
    // Let the remaining seconds be total seconds minus the number of minutes in seconds
    double remainingSeconds = seconds - (minutes * 60);
    
    // Initialise strings to be printed
    std::string mins;
    std::string secs;
    std::string minsSecs;
    
    // Handle the minutes string  - if it is a single digit add a '0' to the front
    mins = "0" + std::to_string((int) minutes);
    
    // Check if minutes is a double digit
    if(minutes>=10)
    {
        // Let the minutes string be the actual minutes string
        mins = std::to_string((int) minutes);
    }
    
    // Handle the seconds string - if it is a single digit add a '0' to the front
    secs = "0" + std::to_string((int) remainingSeconds);
    
    // Check if seconds is a double digit
    if(remainingSeconds>=10)
    {
        // Let the seconds string be the actual seconds string
        secs = std::to_string((int) remainingSeconds);
    }
    
    // Combine both minutes and seconds string separated by colon
    minsSecs = mins + ":" + secs;
    
    // Return the final combined string
    return minsSecs;
}

void DeckGUI::loadNextTrack()
{
    // Obtain next track from the queue in JUCE File object format
    juce::File nextTrack = queue.front();
    
    // Print next file loaded into player
    std::cout << "Next track loaded: " << nextTrack.getFullPathName() << std::endl;
    
    // Load new track into DJAudioPlayer and waveformDisplay
    player -> loadURL(URL{nextTrack});
    waveformDisplay.loadURL(URL{nextTrack});
    
    // Remove loaded track from queue
    queue.erase(queue.begin());
}
