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
}

VibratoPluginAudioProcessor::~VibratoPluginAudioProcessor()
{
    CVibrato::destroyInstance(m_pCVib);
}

//==============================================================================
const String VibratoPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int VibratoPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int VibratoPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

const String VibratoPluginAudioProcessor::getProgramName (int index)
{
    return String();
}


//==============================================================================
void VibratoPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    float fModFreqInSec = 0.5;
    // gets the sampling rate and number of channels from the host and initialises the vibrato object
    CVibrato::createInstance(m_pCVib);
    m_pCVib->initInstance(fModFreqInSec, sampleRate, getTotalNumInputChannels());
    
}

void VibratoPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    CVibrato::destroyInstance(m_pCVib);
}

double VibratoPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

void VibratoPluginAudioProcessor::processBlock (AudioSampleBuffer& buffer)
{
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // I've added this to avoid people getting screaming feedback
    // when they first compile the plugin, but obviously you don't need to
    // this code if your algorithm already fills all the output channels.
    for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    float **channelData = buffer.getArrayOfWritePointers();
    
    m_pCVib->process(channelData, channelData, buffer.getNumSamples());

}

void VibratoPluginAudioProcessor::processBlockBypassed (AudioBuffer<float>& buffer)
{
    for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    float **channelData = buffer.getArrayOfWritePointers();
    
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    /*for (int channel = 0; channel < getTotalNumInputChannels(); ++channel)
    {
        float* channelData = buffer.getWritePointer (channel);
        
    }*/

    
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
    m_pCVib->setParam(static_cast<CVibrato::VibratoParam_t>(parameterIndex), newValue);
}

float VibratoPluginAudioProcessor::getParameter (int parameterIndex)
{
    return m_pCVib->getParam(static_cast<CVibrato::VibratoParam_t>(parameterIndex));
}
