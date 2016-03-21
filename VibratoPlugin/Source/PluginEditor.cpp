/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
VibratoPluginAudioProcessorEditor::VibratoPluginAudioProcessorEditor (VibratoPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    //define parameters for slider object for Mod. Freq
    sModFreqinHz.setSliderStyle(Slider::LinearBarVertical);
    sModFreqinHz.setRange(0.0, 20.0);  // ModFreq can actually go from 0 to samplingRate/2 but limit it here to 20Hz to keep the slider resolution
    sModFreqinHz.setTextBoxStyle(Slider::TextBoxBelow, true, 90, 10);
    sModFreqinHz.setPopupDisplayEnabled(true, this);
    sModFreqinHz.setTextValueSuffix("Modulation Frequency (Hz)");
    sModFreqinHz.setValue(5.0);
    addAndMakeVisible(sModFreqinHz);
    sModFreqinHz.addListener(this);
    
    //define parameters for slider for Width
    sModWidthinSecs.setSliderStyle(Slider::LinearBarVertical);
    sModWidthinSecs.setRange(0.0, 0.5);   // MaxModFreqInSec is set in the init in PluginProcessor to be 0.5 hence range set here to be 0-0.5
    sModWidthinSecs.setTextBoxStyle(Slider::TextBoxBelow, true, 90, 0);
    sModWidthinSecs.setPopupDisplayEnabled(true, this);
    sModWidthinSecs.setTextValueSuffix("Modulation Width (sec)");
    sModWidthinSecs.setValue(0.0);
    addAndMakeVisible(sModWidthinSecs);
    sModWidthinSecs.addListener(this);
    
    
    
}

VibratoPluginAudioProcessorEditor::~VibratoPluginAudioProcessorEditor()
{
}

//==============================================================================
void VibratoPluginAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::white);

    g.setColour (Colours::black);
    g.setFont (15.0f);
    g.drawFittedText ("Vibrato Effect", 0, 0, getWidth(), 30, Justification::centred, 1);
}

void VibratoPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    sModFreqinHz.setBounds(40, 30, 20, getHeight()-60);
    sModWidthinSecs.setBounds(100, 30, 20, getHeight()-60);
}

void VibratoPluginAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
    if (slider== &sModFreqinHz)
    {   processor.VibratoPluginAudioProcessor::setParameter(CVibrato::VibratoParam_t::kParamModFreqInHz, sModFreqinHz.getValue());
        //std::cout<< sModFreqinHz.getValue()<<std::endl;
    }
    else if (slider == &sModWidthinSecs)
    {    processor.VibratoPluginAudioProcessor::setParameter(CVibrato::VibratoParam_t::kParamModWidthInS, sModWidthinSecs.getValue());
        //std::cout<<sModWidthinSecs.getValue()<<std::endl;
    }
}