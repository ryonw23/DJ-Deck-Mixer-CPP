#include <JuceHeader.h>
#include "PlaylistComponent.h"
#include "MainComponent.h"
#include "DeckGUI.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(DJAudioPlayer* _player,
                                     AudioFormatManager & formatManagerToUse,
                                     DeckGUI& deck1,
                                     DeckGUI& deck2)
: player(_player), deckGUI1(deck1), deckGUI2(deck2)
{
    // Initialise table columns for playlist component
    playlistTableComponent.getHeader().addColumn("Track Title", 1, 600);
    playlistTableComponent.getHeader().addColumn("Preview", 2, 200);
    playlistTableComponent.getHeader().addColumn("Queue Into Deck 1", 3, 200);
    playlistTableComponent.getHeader().addColumn("Queue Into Deck 2", 4, 200);
    playlistTableComponent.getHeader().addColumn("Delete", 5, 200);
    playlistTableComponent.setModel(this);
    
    // Initialise and draw table component
    addAndMakeVisible(playlistTableComponent);
    
    // Apply styling for table component
    playlistTableComponent.setColour(ListBox::backgroundColourId, Colour(1,1,30));
    
    // Styling for background
    playlistTableComponent.getHeaderComponent()->setColour(TableHeaderComponent::backgroundColourId, Colours::slategrey);
    
    // Styling for text
    playlistTableComponent.getHeaderComponent()->setColour(TableHeaderComponent::textColourId, Colour(20,20,20));
    
    // Initialise and draw button with styling
    addAndMakeVisible(loadIntoPlaylist);
    loadIntoPlaylist.setColour(TextButton::buttonColourId, Colours::darkblue);
    
    // Initialise listener for button
    loadIntoPlaylist.addListener(this);
}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    // Set background colour to very dark blue - standard background colour
    g.fillAll (Colour(1,1,30));
}

void PlaylistComponent::resized()
{
    // Initialise row height, column width and buffer to apply positioning
    double rowH = getHeight() / 8;
    double colW = getWidth() / 7;
    double buffer = 10;
    
    // Initialise column bounds of playlist table
    playlistTableComponent.getHeader().setColumnWidth(1, colW * 3);
    playlistTableComponent.getHeader().setColumnWidth(2, colW);
    playlistTableComponent.getHeader().setColumnWidth(3, colW);
    playlistTableComponent.getHeader().setColumnWidth(4, colW);
    playlistTableComponent.getHeader().setColumnWidth(5, colW);

    // Fill whole playlist component with playlist table, leave some space for load button
    playlistTableComponent.setBounds(0, 0, getWidth(), getHeight() - rowH - buffer);

    // Place load button at very bottom of playlist component
    loadIntoPlaylist.setBounds(0 + buffer, rowH * 7 - buffer, getWidth() - buffer * 2, rowH);
}

// Obtain number of rows in table based on vector size
int PlaylistComponent::getNumRows()
{
    // Return size of playlist vector
    return trackFiles.size();
}

// Paint background of table component
void PlaylistComponent::paintRowBackground(Graphics & g, int rowNumber, int width, int height, bool rowIsSelected)
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
void PlaylistComponent::paintCell(Graphics & g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    // Set outline and text to white
    g.setColour(Colours::ghostwhite);
    
    // Draw outline around cell
    g.drawRect(getLocalBounds(),2);
    
    // Check if its the first column
    if(columnId == 1)
    {
        // For the first column, draw text of the file name
        g.drawText (trackFiles[rowNumber].getFileName(),10, 0, width-4, height, Justification::centredLeft,true);
    }
}

// Table component button listeners
Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate)
{
    // Check if it is the second column
    if(columnId == 2)
    {
        // Check if there is no existing component
        if(existingComponentToUpdate == nullptr)
        {
            // Create a new preview button
            TextButton* previewButton = new TextButton{"Preview"};
            
            // Set colour of button to green
            previewButton->setColour(TextButton::buttonColourId, Colours::darkgreen);
            
            // Initialise listener for preview button
            previewButton -> addListener(this);
            
            // Assign unique id based on row number
            String id{std::to_string(rowNumber)};
            previewButton -> setComponentID(id);
                        
            // Let the new preview button be the existing component to update
            existingComponentToUpdate = previewButton;
        }
    }

    // Check if it is the third column
    if(columnId == 3)
    {
        // Check if there is no existing component
        if(existingComponentToUpdate == nullptr)
        {
            // Create a new button to queue track into Deck 1
            TextButton* load1 = new TextButton{"Queue into Deck 1"};
            
            // Set colour of button to dark blue
            load1->setColour(TextButton::buttonColourId, Colours::darkblue);
            
            // Initialise listener for load button
            load1 -> addListener(this);
            
            // Assign unique id based on row number
            String id{std::to_string(rowNumber)};
            load1 -> setComponentID(id);
                        
            // Let the new load button be the existing component to update
            existingComponentToUpdate = load1;
        }
    }

    // Check if it is the fourth column
    if(columnId == 4)
    {
        // Check if there is no existing component
        if(existingComponentToUpdate == nullptr)
        {
            // Create a new button to queue track into Deck 2
            TextButton* load2 = new TextButton{"Queue into Deck 2"};
            
            // Set colour of button to dark blue
            load2->setColour(TextButton::buttonColourId, Colours::darkblue);
            
            // Initialise listener for load button
            load2 -> addListener(this);
            
            // Assign unique id based on row number
            String id{std::to_string(rowNumber)};
            load2 -> setComponentID(id);
                        
            // Let the new load button be the existing component to update
            existingComponentToUpdate = load2;
        }
    }

    // Check if it is the fifth column
    if(columnId == 5)
    {
        // Check if there is no existing component
        if(existingComponentToUpdate == nullptr)
        {
            // Create a new delete button
            TextButton* deleteButton = new TextButton{"Delete"};
            
            // Set colour of button to firebrick red
            deleteButton->setColour(TextButton::buttonColourId, Colours::firebrick);
            
            // Initialise listener for delete button
            deleteButton -> addListener(this);
            
            // Assign unique id based on row number
            String id{std::to_string(rowNumber)};
            deleteButton -> setComponentID(id);
                        
            // Let the new delete button be the existing component to update
            existingComponentToUpdate = deleteButton;
        }
    }
    
    // Return updated/new component
    return existingComponentToUpdate;
}


void PlaylistComponent::deleteFromPlaylist(int id)
{
    // Chcek if there is any items in playlist track vector
    if(trackFiles.size()>0)
    {
        // Remove item from playlist vector based on input id
        trackFiles.erase(trackFiles.begin() + id);
    }
}

void PlaylistComponent::buttonClicked(Button * button)
{
    // Check if the load into playlist button is clicked
    if (button == &loadIntoPlaylist)
    {
        // Print to console that load into playlist button clicked
        std::cout << "Load into playlist button was clicked " << std::endl;
        
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
                // Push file into playlist vector
                trackFiles.push_back(chosenFile);
                
                // Update display of playlist in table component
                playlistTableComponent.updateContent();
                repaint();
            }
        });
    }
    // Handle button presses in table component (playlist)
    else
    {
        // Convert button component id into integer - integer used to reference corresponding file in playlist vector
        int id = std::stoi(button -> getComponentID().toStdString());
        
        // Print id to command line for debugging
        std::cout << id << std::endl;

        // Check if the button clicked is the preview button in table component
        if (button->getButtonText() == "Preview")
        {
            // Obtain file from playlist according to id
            juce::File selectedFile = trackFiles[id];
            
            // Print to command line what file is being previewed from playlist
            std::cout << "Previewing: " << selectedFile.getFullPathName() << std::endl;

            // Load file into DJAudioPlayer
            player->loadURL(URL{selectedFile});
            
            // Call preview function to play a 5 second snipper
            player->preview();
        }
        // Check if the button clicked is the queue into deck 1 button in table component
        else if (button->getButtonText() == "Queue into Deck 1")
        {
            // Obtain file from playlist according to id
            juce::File selectedFile = trackFiles[id];
            
            // Print to command line what file is being queued into deck 1
            std::cout << "Queueing into Deck 1: " << selectedFile.getFullPathName() << std::endl;
            
            // Push file into vector for queue 1 from MainComponent
            MainComponent::queue1.push_back(trackFiles[id]);
            
            // Update the queue display to reflect changes
            deckGUI1.updateQueue();
            
            // Print to command line the size of updated queue
            std::cout << "Size of queue 1: " << MainComponent::queue1.size() << std::endl;
        }
        // Check if the button clicked is the queue into deck 2 button in table component
        else if (button->getButtonText() == "Queue into Deck 2")
        {
            // Obtain file from playlist according to id
            juce::File selectedFile = trackFiles[id];
            
            // Print to command line what file is being queued into deck 2
            std::cout << "Queueing into Deck 2: " << selectedFile.getFullPathName() << std::endl;

            // Push file into vector for queue 2 from MainComponent
            MainComponent::queue2.push_back(trackFiles[id]);
            
            // Update queue display to reflect changes
            deckGUI2.updateQueue();
            
            // Print to command line the size of updated queue
            std::cout << "Size of queue 2: " << MainComponent::queue2.size() << std::endl;
        }
        // Check if the delete button clicked is delete button in table component
        else if (button->getButtonText() == "Delete")
        {
            // Obtain file from playlist according to id
            juce::File selectedFile = trackFiles[id];
            
            // Print to command line what file is being deleted
            std::cout << "Deleting from playlist: " << selectedFile.getFullPathName() << std::endl;
            
            // Remove file from playlist
            deleteFromPlaylist(id);
            
            // Update playlist display to reflect changes
            playlistTableComponent.updateContent();
            repaint();
        }
    }
}

bool PlaylistComponent::isInterestedInFileDrag (const StringArray &files)
{
    // Print message to console for debugging
    std::cout << "PlaylistComponent::isInterestedInFileDrag" << std::endl;
    
    // Return true to indicate component accepts file dragging
    return true;
}

void PlaylistComponent::filesDropped (const StringArray &files, int x, int y)
{
    // Print message to console for debugging
    std::cout << "PlaylistComponent::filesDropped" << std::endl;
    
    // Check that only 1 file is dragged
    if (files.size() == 1)
    {
        // Convert dragged file into JUCE File object then push to playlist
        trackFiles.push_back({File{files[0]}});
        
        // Update playlist display to reflect changes
        playlistTableComponent.updateContent();
        repaint();
    }
}
