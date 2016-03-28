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
    setSize (480, 260);
    
    //define parameters for slider object for Mod. Freq
    addAndMakeVisible(sModFreqinHz);
    
    //sModFreqinHz.setSliderStyle(Slider::LinearBarVertical);
    sModFreqinHz.setRange(5.0, 15.0);  // ModFreq can actually go from 0 to samplingRate/2 but limit it here to 5-15Hz to keep the vibrato effect
                                       // meaningful (as per DAFx book) and for slider resolution
    //sModFreqinHz.setTextBoxStyle(Slider::TextBoxBelow, true, 90, 10);
    sModFreqinHz.setPopupDisplayEnabled(true, this);
    sModFreqinHz.setTextValueSuffix("Hz");
    sModFreqinHz.setValue(processor.getParameter(CVibrato::VibratoParam_t::kParamModFreqInHz)); //though the editor is closed we need the changed slider values to
                                                                                                // display when the user opens the editor again
    sModFreqinHz.addListener(this);
    
    addAndMakeVisible(lModFreqInHz);
    lModFreqInHz.setText("Modulation Frequency", dontSendNotification);
    lModFreqInHz.attachToComponent(&sModFreqinHz, false);
    
    //define parameters for slider for Width
    addAndMakeVisible(sModWidthinSecs);
    //sModWidthinSecs.setSliderStyle(Slider::LinearBarVertical);
    sModWidthinSecs.setRange(5, 10);   // MaxModFreqInSec is set in the init in PluginProcessor to be 0.01 hence range set here to be 0.005-0.01
                                            //(as per DAFx book typical values for vibrato effect)
    //sModWidthinSecs.setTextBoxStyle(Slider::TextBoxBelow, true, 90, 0);
    sModWidthinSecs.setPopupDisplayEnabled(true, this);
    sModWidthinSecs.setTextValueSuffix("ms");
    sModWidthinSecs.setValue(processor.getParameter(CVibrato::VibratoParam_t::kParamModWidthInS));  //though the editor is closed we need the changed slider values to
                                                                                                    // display when the user opens the editor again
    sModWidthinSecs.addListener(this);
    
    addAndMakeVisible(lModWidthInSec);
    lModWidthInSec.setText("Modulation Width", dontSendNotification);
    lModWidthInSec.attachToComponent(&sModWidthinSecs, false);
    
    addAndMakeVisible(tProcessByPass);
    tProcessByPass.addListener(this);
    
    tProcessByPass.setButtonText("Bypass the plugin");
    
//    addAndMakeVisible(lProcessByPass);
//    lProcessByPass.setText("Bypass the plugin", dontSendNotification);
//    lProcessByPass.attachToComponent(&tProcessByPass, false);
    
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
    
    const int positionLeft=100;
    g.drawFittedText ("5Hz", positionLeft, 80, 20, 20, Justification::left, 1);
    g.drawFittedText ("15Hz", getWidth()-40, 80, 40, 20, Justification::left, 1);
    g.drawFittedText ("5ms", positionLeft, 170, 50, 20, Justification::left, 1);
    g.drawFittedText ("10ms", getWidth()-50, 170, 50, 20, Justification::left, 1);

}

void VibratoPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    const int sliderLeft = 20;
    sModFreqinHz.setBounds(sliderLeft, 60, getWidth()-sliderLeft-10, 20);
    sModWidthinSecs.setBounds(sliderLeft, 150, getWidth()-sliderLeft-10, 20);
    
    tProcessByPass.setBounds(200, 200, 150, 30);
    
    //sModFreqinHz.setBounds(40, 30, 20, getHeight()-90);
    //sModWidthinSecs.setBounds(100, 30, 20, getHeight()-90);
}

void VibratoPluginAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
    if (slider== &sModFreqinHz)
    {
        processor.m_bSliderValueChangeModFreq = 1;
        processor.m_fModFreqInHzVPAP = sModFreqinHz.getValue();
        //processor.VibratoPluginAudioProcessor::setParameter(CVibrato::VibratoParam_t::kParamModFreqInHz, sModFreqinHz.getValue()); // we want to update the changed values beofre calling the process and not within the process... so not doing this...
        //std::cout<< sModFreqinHz.getValue()<<std::endl;
    }
    else if (slider == &sModWidthinSecs)
    {
        processor.m_bSliderValueChangeModWidth = 1;
        processor.m_fModWidthInSecVPAP = sModWidthinSecs.getValue() / 1000;
        //processor.VibratoPluginAudioProcessor::setParameter(CVibrato::VibratoParam_t::kParamModWidthInS, sModWidthinSecs.getValue());  // we want to update the changed values beofre calling the process and not within the process... so not doing this...
        //std::cout<<sModWidthinSecs.getValue()<<std::endl;
    }
}

void VibratoPluginAudioProcessorEditor::buttonClicked (Button* buttonThatWasClicked)
{
    if (buttonThatWasClicked == &tProcessByPass)
    {
        processor.m_bProcessByPass = tProcessByPass.getToggleState();
    }
}