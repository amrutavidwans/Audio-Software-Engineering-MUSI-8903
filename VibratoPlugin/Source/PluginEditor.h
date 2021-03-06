/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class VibratoPluginAudioProcessorEditor  : public AudioProcessorEditor, private SliderListener, public ButtonListener
{
public:
    VibratoPluginAudioProcessorEditor (VibratoPluginAudioProcessor&);
    ~VibratoPluginAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void sliderValueChanged (Slider* slider) override;
    
    void buttonClicked (Button* buttonThatWasClicked) override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    VibratoPluginAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VibratoPluginAudioProcessorEditor)
    
    //Slider references
    Slider sModFreqinHz;
    Label lModFreqInHz;
    Slider sModWidthinSecs;
    Label lModWidthInSec;
    ToggleButton tProcessByPass;
    
};


#endif  // PLUGINEDITOR_H_INCLUDED
