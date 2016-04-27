/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
VibratoPluginAudioProcessor::VibratoPluginAudioProcessor()
{
    // allocate memory to the vibrato object and initialize it with No. of Channels as 2 and sampling freq as 44100
    CVibrato::createInstance(m_pCVib);
    CPeakMeter::createInstance(m_pCPM);

}

VibratoPluginAudioProcessor::~VibratoPluginAudioProcessor()
{
    CVibrato::destroyInstance(m_pCVib);
    CPeakMeter::destroyInstance(m_pCPM);
    m_pCPM = 0;
    m_fModFreqInHzVPAP = 0;
    m_fModWidthInSecVPAP = 0;
    delete [] m_pfPeakVal;
    m_pfPeakVal = 0;
    m_bSliderValueChangeModFreq = 0;
    m_bSliderValueChangeModWidth = 0;
    m_iNumChan = 0;
    m_bBypassFlag = false;
}

//==============================================================================
const String VibratoPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool VibratoPluginAudioProcessor::acceptsMidi() const
{
    #if JucePlugin_WantsMidiInput
        return true;
    #else
        return false;
    #endif
}

bool VibratoPluginAudioProcessor::producesMidi() const
{
    #if JucePlugin_ProducesMidiOutput
        return true;
    #else
        return false;
    #endif
}

bool VibratoPluginAudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double VibratoPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int VibratoPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

void VibratoPluginAudioProcessor::setCurrentProgram (int index)
{
    
}

int VibratoPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

const String VibratoPluginAudioProcessor::getProgramName (int index)
{
    return String();
}

void VibratoPluginAudioProcessor::changeProgramName (int index, const String& newName)
{
    
}

//==============================================================================
void VibratoPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    float fMaxModFreqInSec = 0.01;
    // gets the sampling rate and number of channels from the host and initialises the vibrato object
    m_pCVib->initInstance(fMaxModFreqInSec, sampleRate, getTotalNumInputChannels());
    m_pCVib->setParam(CVibrato::VibratoParam_t::kParamModFreqInHz, 5.0);
    m_pCVib->setParam(CVibrato::VibratoParam_t::kParamModWidthInS, 0.01);
    
    m_pfPeakVal = new float [getTotalNumInputChannels()];
    m_pCPM->initPeakMeter(sampleRate, getTotalNumInputChannels());
    m_iNumChan = getTotalNumInputChannels();
    m_bBypassFlag=1; //When the plug in is opened
}

void VibratoPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    //CVibrato::destroyInstance(m_pCVib);
}


void VibratoPluginAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // I've added this to avoid people getting screaming feedback
    // when they first compile the plugin, but obviously you don't need to
    // this code if your algorithm already fills all the output channels.
    
    
    
    
    //maax value?
    
    if (m_bProcessByPass)
    {
        processBlockBypassed(buffer, midiMessages);
        m_bBypassFlag = true;
    }
    
    else
    {
        if (m_bBypassFlag) {
            m_pCVib->setParam(CVibrato::VibratoParam_t::kParamModFreqInHz, m_fModFreqBypass);
            m_pCVib->setParam(CVibrato::VibratoParam_t::kParamModWidthInS, m_fModWidthBypass);
            m_bBypassFlag = false;
        }
        
        if (m_bSliderValueChangeModFreq)
        {
            m_pCVib->setParam(CVibrato::VibratoParam_t::kParamModFreqInHz, m_fModFreqInHzVPAP);
            m_bSliderValueChangeModFreq = 0;
            
        }
        if (m_bSliderValueChangeModWidth)
        {
            m_pCVib->setParam(CVibrato::VibratoParam_t::kParamModWidthInS, m_fModWidthInSecVPAP);
            m_bSliderValueChangeModWidth = 0;
        }
        
        if (m_fModFreqInHzVPAP == 0.F)
        {
            m_pCVib->setParam(CVibrato::VibratoParam_t::kParamModFreqInHz, m_fModFreqInHzVPAP);
            m_pCVib->setParam(CVibrato::VibratoParam_t::kParamModWidthInS, m_fModWidthInSecVPAP);
        }
    }
      
    
    for (int i = getTotalNumOutputChannels(); i < getTotalNumOutputChannels(); ++i)
    {
        buffer.clear (i, 0, buffer.getNumSamples());
    }
    
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    float **channelData = buffer.getArrayOfWritePointers();
    
    //call vibrato
    m_pCVib->process(channelData, channelData, buffer.getNumSamples());
    // call peak meter
    m_pCPM->process(channelData, buffer.getNumSamples(), m_pfPeakVal);
    

}

void VibratoPluginAudioProcessor::processBlockBypassed (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    //    for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
    //        buffer.clear (i, 0, buffer.getNumSamples());
    //
    //    float **channelData = buffer.getArrayOfWritePointers();
    //
    //    // This is the place where you'd normally do the guts of your plugin's
    //    // audio processing...
    //    // call peak meter
    //    m_pCPM->process(channelData, buffer.getNumSamples(), m_pfPeakVal);
   
    m_pCVib->setParam(CVibrato::VibratoParam_t::kParamModFreqInHz, 0.F);
    m_pCVib->setParam(CVibrato::VibratoParam_t::kParamModWidthInS, 0.F);
    m_fModWidthBypass = m_fModWidthInSecVPAP;
    m_fModFreqBypass = m_fModFreqInHzVPAP;

    
    
}

//==============================================================================
bool VibratoPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* VibratoPluginAudioProcessor::createEditor()
{
    return new VibratoPluginAudioProcessorEditor (*this);
}

//==============================================================================
void VibratoPluginAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void VibratoPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VibratoPluginAudioProcessor();
}

//==============================================================================
// parameters get and set functions for vibrato

void VibratoPluginAudioProcessor::setParameter (int parameterIndex, float newValue)
{
    //std::cout<<static_cast<CVibrato::VibratoParam_t>(parameterIndex)<<"enumval"<<std::endl;
    m_pCVib->setParam(static_cast<CVibrato::VibratoParam_t>(parameterIndex), newValue);
}

float VibratoPluginAudioProcessor::getParameter (int parameterIndex)
{
    return m_pCVib->getParam(static_cast<CVibrato::VibratoParam_t>(parameterIndex));
}

bool VibratoPluginAudioProcessor::getProcessByPassState (){
    return m_bProcessByPass;
}

void VibratoPluginAudioProcessor::setProcessByPassState(bool flag){
    m_bProcessByPass=flag;
}

float VibratoPluginAudioProcessor::getPeakMeterValue(int channel){
    return m_pfPeakVal[channel];
}
