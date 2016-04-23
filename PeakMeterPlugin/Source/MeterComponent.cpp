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

    float temp = (getPeakValue()*getHeight());
    g.setColour(juce::Colours::green);
    g.fillRect(0.F, 0.F, float(getWidth()), temp);
    
    
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

void MeterComponent::setValue(float &val){
    //convert to dB
    if(val<0.00001){
        val=0.00001;
    }
    else{
        val=20*log10f(val);
    }
    
    // values less than 12 dB are not to be displayed
    if (val < -12.0){
        val = -12.0;
    }
    
    // convert 0-1 RANGE
    val = (val+12.0)/12.0;
}

