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
    setSize (400, 170);
    
    //define parameters for slider object for Mod. Freq
    addAndMakeVisible(sModFreqinHz);
    
    //sModFreqinHz.setSliderStyle(Slider::LinearBarVertical);
    sModFreqinHz.setRange(5.0, 15.0);  // ModFreq can actually go from 0 to samplingRate/2 but limit it here to 5-15Hz to keep the vibrato effect
                                       // meaningful (as per DAFx book) and for slider resolution
    //sModFreqinHz.setTextBoxStyle(Slider::TextBoxBelow, true, 90, 10);
    sModFreqinHz.setPopupDisplayEnabled(true, this);
    sModFreqinHz.setTextValueSuffix("Hz");
    sModFreqinHz.setValue(5.0);
    sModFreqinHz.addListener(this);
    
    addAndMakeVisible(lModFreqInHz);
    lModFreqInHz.setText("Modulation Frequency", dontSendNotification);
    lModFreqInHz.attachToComponent(&sModFreqinHz, false);
    
    //define parameters for slider for Width
    addAndMakeVisible(sModWidthinSecs);
    //sModWidthinSecs.setSliderStyle(Slider::LinearBarVertical);
    sModWidthinSecs.setRange(0.005, 0.01);   // MaxModFreqInSec is set in the init in PluginProcessor to be 0.01 hence range set here to be 0.005-0.01
                                            //(as per DAFx book typical values for vibrato effect)
    //sModWidthinSecs.setTextBoxStyle(Slider::TextBoxBelow, true, 90, 0);
    sModWidthinSecs.setPopupDisplayEnabled(true, this);
    sModWidthinSecs.setTextValueSuffix("sec");
    sModWidthinSecs.setValue(0.005);
    sModWidthinSecs.addListener(this);
    
    addAndMakeVisible(lModWidthInSec);
    lModWidthInSec.setText("Modulation Width", dontSendNotification);
    lModWidthInSec.attachToComponent(&sModWidthinSecs, false);
    
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
    const int sliderLeft = 20;
    sModFreqinHz.setBounds(sliderLeft, 60, getWidth()-sliderLeft-10, 20);
    sModWidthinSecs.setBounds(sliderLeft, 120, getWidth()-sliderLeft-10, 20);
    //sModFreqinHz.setBounds(40, 30, 20, getHeight()-90);
    //sModWidthinSecs.setBounds(100, 30, 20, getHeight()-90);
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