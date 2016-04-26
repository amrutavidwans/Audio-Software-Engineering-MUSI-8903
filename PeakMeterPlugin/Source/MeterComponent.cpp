//
//  MeterComponent.cpp
//  VibratoPlugin
//
//  Created by Amruta Vidwans on 4/22/16.
//
//
#include "../JuceLibraryCode/JuceHeader.h"
#include "MeterComponent.h"
#include <math.h>


MeterComponent::MeterComponent(){
    m_fPeakVal = 0;
    m_fMaxPeakVal = 0;
}

MeterComponent::~MeterComponent(){
    m_fMaxPeakVal = 0;
    m_fPeakVal = 0;
}

void MeterComponent::setPeakValue(float val){
    m_fMaxPeakVal = val;
}

void MeterComponent::paint(Graphics& g){
    g.fillAll(juce::Colours::black);
    float temp = (getPeakValue());
    g.setColour(juce::Colours::green);
    g.fillRect(0.F, getHeight()*(temp),float(getWidth()), (1-temp)*float(getHeight()));
    //std::cout<< temp<<std::endl;
    
}

//void MeterComponent::resized(){
//    
//}

float MeterComponent::getPeakValue(){

    return m_fMaxPeakVal;
}


void MeterComponent::setValue(float val){
    val = 20*log10f(val);
    
    // check for values below -12dB... ignore them i.e. make them -12dB
    if (val < -12.0)
    {
        val = -12.0;
    }
    
    // scale the value from 0-1
    val = - val / 12;
    m_fPeakVal=val;
    
    // max value since last poll
    if (m_fPeakVal> m_fMaxPeakVal)
        m_fMaxPeakVal=m_fPeakVal;
}

