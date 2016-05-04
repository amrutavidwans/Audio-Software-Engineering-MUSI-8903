/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
//#include "AudioMixInstance.h"

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
    TextButton mixFilesButton;
    TextButton Tbox[20];
    File Tboxfile[20];
    int count;
    
    AudioFormatManager formatManager;
    ScopedPointer<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource;
    TransportState state;
    MixerAudioSource audioMix;
    
    
    
    
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
        
        
        addAndMakeVisible (&mixFilesButton);
        mixFilesButton.setButtonText ("Mix All Files");
        mixFilesButton.setEnabled(false);
        mixFilesButton.addListener (this);
        mixFilesButton.setColour (TextButton::buttonColourId, Colours::blue);
        mixFilesButton.setEnabled (false);
        
        for (int i=0; i<20;i++){
            Tbox[i].addListener(this);
        }
        
        MixerAudioSource audioMix;
        
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
        openButton.setBounds(200, 300, 100, 100);
        playButton.setBounds(300, 300, 100, 100);
        stopButton.setBounds(400, 300 ,100, 100);
        mixFilesButton.setBounds(500, 300, 100, 100);
        
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
        if (button==&Tbox[0]||button==&Tbox[1]||button==&Tbox[2]||button==&Tbox[3]||button==&Tbox[4]||button==&Tbox[5]||button==&Tbox[6]||button==&Tbox[7]||button==&Tbox[8]||button==&Tbox[9]||button==&Tbox[10]||button==&Tbox[11]||button==&Tbox[12]||button==&Tbox[13]||button==&Tbox[14]||button==&Tbox[15]||button==&Tbox[16]||button==&Tbox[17]||button==&Tbox[18]||button==&Tbox[19])
            fileButtonClicked();
        if (button == &mixFilesButton) mixFilesButtonClicked();
    }
    
    void mixFilesButtonClicked(){
        File file;
        file.create();
        int iSamp=0;
        for(int i=0;i<20;i++)
        {
            
        AudioFormatReader* reader = formatManager.createReaderFor (Tboxfile[i]);
        AudioFormatReaderSource* newSource = new AudioFormatReaderSource(reader, true);
            if(reader!= nullptr)
            {   iSamp=reader->sampleRate;
                transportSource.setSource (newSource, 0, nullptr, reader->sampleRate);
                audioMix.addInputSource(newSource, iSamp);
                readerSource.release();
                
                
            }
            else
                break;
        }
        stopButton.setEnabled(false);
        transportSource.setSource(dynamic_cast<PositionableAudioSource*> (&audioMix),0,nullptr,iSamp);
        
    }
    
    void writeFile (AudioFormatReader *auReader)
    {
        int size=500000;
        //AudioSampleBuffer* buffer=new AudioSampleBuffer(1,size);
       // buffer->clear();
    
        WavAudioFormat* test = new WavAudioFormat();
        File outputFile = File("/Users/milaprane/Desktop/test.wav");
        if (outputFile.exists()==true)
            outputFile.deleteFile();
        
        outputFile = File("/Users/milaprane/Desktop/test.wav");
        FileOutputStream* outputTo = outputFile.createOutputStream();
        AudioFormatWriter* writer = test->createWriterFor(outputTo, 44100, 1, 16,NULL, 0);
        writer->writeFromAudioReader(*auReader,0.F,auReader->lengthInSamples);
            delete writer;
        
        delete test;
        
    }
    
    
        
    
    
    void fileButtonClicked()
    {
        File file ;
        playButton.setEnabled(true);
        for(int i=0;i<20;i++)
        {
            if(Tbox[i].getState()==true)
            {
                file = Tboxfile[i].getFullPathName();
                break;
                
            }
        }
        for(int i=0;i<20;i++)
        {
            if(Tbox[i].getState()==true)
            {
                Tbox[i].setEnabled(true);
                
            }
            else
                Tbox[i].setEnabled(false);
        }
        


        AudioFormatReader* reader = formatManager.createReaderFor (file);
        ScopedPointer<AudioFormatReaderSource> newSource = new AudioFormatReaderSource (reader, true);
        transportSource.setSource (newSource, 0, nullptr, reader->sampleRate);
        readerSource = newSource.release();
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
                playButton.setEnabled (false);
                readerSource = newSource.release();
                addAndMakeVisible(Tbox[count]);
                Tbox[count].setButtonText(file.getFileName());
                Tbox[count].setEnabled(true);
                Tbox[count].setBounds(20,50*(1+count), 100, 50);
                Tboxfile[count]=file;
                count++;
            }
                 for(int i=0;i<20;i++)
                 {
                     
                 Tbox[i].setEnabled(true);
                     
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
        
        for(int i=0;i<20;i++)
        {
            
            Tbox[i].setEnabled(true);
        }
        mixFilesButton.setEnabled(true);
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
    AudioDeviceManager AudDevMan;
   


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
