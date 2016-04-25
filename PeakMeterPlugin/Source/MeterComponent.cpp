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


void MeterComponent::paint(Graphics& g){
    g.fillAll(juce::Colours::black);
    float temp = (getPeakValue());
    g.setColour(juce::Colours::green);
    g.fillRect(0.F, getHeight()*(1-temp),float(getWidth()), float(getHeight()));
    //std::cout<< temp<<std::endl;
    
}

//void MeterComponent::resized(){
//    
//}

float MeterComponent::getPeakValue(){
    setValue(m_fPeakVal);
    return m_fPeakVal;
}
float MeterComponent::getMaxValue(){
    setValue(m_fMaxPeakVal);
    return m_fMaxPeakVal;
}

void MeterComponent::setValue(float val){
    if (val>=0.00001)
    val= 20*log10f(val);
    else
    {
      val=20*log10f(0.00001);
    }
    //std::cout<< val<<std::endl;
    // values less than -12 dB are not to be displayed
 
    // convert 0-1 RANGE
    val = (-val)/100;
    m_fPeakVal=val;
    std::cout<< m_fPeakVal<<std::endl;
    repaint();
    
    if (m_fPeakVal> m_fMaxPeakVal)
        m_fMaxPeakVal=m_fPeakVal;
}

