/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public AudioAppComponent, public Button::Listener, public juce::ChangeListener
{
public:
    //==============================================================================
    enum TransportState
    {
        Stopped,
        Starting,
        Playing,
        Pausing,
        Paused,
        Stopping
    };
    
    TextButton openButton;
    TextButton playButton;
    TextButton stopButton;
    TextButton Tbox[20];
   // File Tboxfile[20];
    int count;
    
    AudioFormatManager formatManager;
    ScopedPointer<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource;
    TransportState state;
    
    
    
    //==============================================================================
    
    
    
    
    MainContentComponent()
    {   formatManager.registerBasicFormats();
        transportSource.addChangeListener(this);
        
        setSize (800, 600);
        addAndMakeVisible (&openButton);
        openButton.setButtonText ("Open...");
        openButton.addListener (this);
        
        addAndMakeVisible (&playButton);
        playButton.setButtonText ("Play");
        playButton.addListener (this);
        playButton.setColour (TextButton::buttonColourId, Colours::green);
        playButton.setEnabled (false);
        
        addAndMakeVisible (&stopButton);
        stopButton.setButtonText ("Stop");
        stopButton.addListener (this);
        stopButton.setColour (TextButton::buttonColourId, Colours::red);
        stopButton.setEnabled (false);
        
        count=0;

        // specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }

    ~MainContentComponent()
    {
        shutdownAudio();
    }

    //=======================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        // This function will be called when the audio device is started, or when
        // its settings (i.e. sample rate, block size, etc) are changed.

        // You can use this function to initialise any resources you might need,
        // but be careful - it will be called on the audio thread, not the GUI thread.

        // For more details, see the help for AudioProcessor::prepareToPlay()
        
        transportSource.prepareToPlay (samplesPerBlockExpected, sampleRate);
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        // Your audio-processing code goes here!

        // For more details, see the help for AudioProcessor::getNextAudioBlock()

        // Right now we are not producing any data, in which case we need to clear the buffer
        // (to prevent the output of random noise)
        
        if (readerSource == nullptr)
        {
            bufferToFill.clearActiveBufferRegion();
            return;
        }
        
        transportSource.getNextAudioBlock (bufferToFill);
    }

    void releaseResources() override
    {
        // This will be called when the audio device stops, or when it is being
        // restarted due to a setting change.

        // For more details, see the help for AudioProcessor::releaseResources()
        
          transportSource.releaseResources();
    }

    //=======================================================================
    void paint (Graphics& g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (Colours::black);


        // You can add your drawing code here!
    }

    void resized() override
    {
        // This is called when the MainContentComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
        openButton.setBounds(200, 200, 100, 100);
        playButton.setBounds(200, 300, 100, 100);
        stopButton.setBounds(200, 400 ,100, 100);
        
    }
    
    void changeState (TransportState newState)
    {
        if (state != newState)
        {
            state = newState;
            
            switch (state)
            {
                case Stopped:
                    playButton.setButtonText ("Play");
                    stopButton.setButtonText ("Stop");
                    stopButton.setEnabled (false);
                    transportSource.setPosition (0.0);
                    break;
                    
                case Starting:
                    transportSource.start();
                    break;
                    
                case Playing:
                    playButton.setButtonText ("Pause");
                    stopButton.setButtonText ("Stop");
                    stopButton.setEnabled (true);
                    break;
                    
                case Pausing:
                    transportSource.stop();
                    break;
                    
                case Paused:
                    playButton.setButtonText ("Resume");
                    stopButton.setButtonText ("Return to Zero");
                    break;
                    
                case Stopping:
                    transportSource.stop();
                    break;
            }
        }
    }
    
    
    void buttonClicked (Button* button) override
    {
        if (button == &openButton)  openButtonClicked();
        if (button == &playButton)  playButtonClicked();
        if (button == &stopButton)  stopButtonClicked();
        
    }

    void openButtonClicked()
    {

        
           FileChooser chooser ("Select a Wave file to play...",
                             File::nonexistent,
                             "*.wav");
             if (chooser.browseForFileToOpen())
             {
               File file (chooser.getResult());
               AudioFormatReader* reader = formatManager.createReaderFor (file);
            
             if (reader != nullptr)
             {
                ScopedPointer<AudioFormatReaderSource> newSource = new AudioFormatReaderSource (reader, true);
                transportSource.setSource (newSource, 0, nullptr, reader->sampleRate);
                playButton.setEnabled (true);
                readerSource = newSource.release();
                addAndMakeVisible(Tbox[count]);
                Tbox[count].setButtonText(file.getFileName());
                Tbox[count].setEnabled(true);
                Tbox[count].setBounds(20,50*(1+count), 50, 50);
                count++;
                 
            }
               
                 
          }
        
    }
    
    void playButtonClicked()
    {
        if ((state == Stopped) || (state == Paused))
            changeState (Starting);
        else if (state == Playing)
            changeState (Pausing);
    }
    
    void stopButtonClicked()
    {
        if (state == Paused)
            changeState (Stopped);
        else
            changeState (Stopping);
    }
    
    
    void changeListenerCallback (ChangeBroadcaster* source) override
    {
        if (source == &transportSource)
        {
            if (transportSource.isPlaying())
                changeState (Playing);
            else if ((state == Stopping) || (state == Playing))
                changeState (Stopped);
            else if (Pausing == state)
                changeState (Paused);
        }
    }


private:
    //==============================================================================

    // Your private member variables go here...
    
   


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
