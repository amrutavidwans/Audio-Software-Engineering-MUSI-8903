#include "PeakMeter.h"
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <stdio.h>


CPeakMeter::CPeakMeter ()
{
    this->resetPeakMeterValues();
    
}

CPeakMeter::~CPeakMeter(){
    
    delete [] m_pfPreviousVPPM;
    delete [] m_pfVPPM;
    m_pfPreviousVPPM = 0;
    m_pfVPPM = 0;

}

 void CPeakMeter::createInstance(CPeakMeter*& pcPM){
    
    pcPM = new CPeakMeter();
}

 void CPeakMeter::destroyInstance(CPeakMeter*& pcPM){
    delete pcPM;
    pcPM = 0;
}

void CPeakMeter::resetPeakMeterValues(){
    m_fSamplingFreq = 0;
    m_fAlphaRT = 0;
    m_fAlphaAT = 0;
    m_iNumChannels = 0;
}

void CPeakMeter::initPeakMeter(float fSamplingFreq, int iNumChannels){
    m_fSamplingFreq = fSamplingFreq;
    m_iNumChannels = iNumChannels;
    
    m_fAlphaRT = 1- exp(-2.2/(m_fSamplingFreq* 1.5));
    m_fAlphaAT = 1- exp(-2.2/(m_fSamplingFreq* 0.01));
    
    m_pfPreviousVPPM = new float [m_iNumChannels];
    m_pfVPPM = new float [m_iNumChannels];
    for (int i = 0; i< m_iNumChannels; i++){
        m_pfVPPM[i] = 0;
        m_pfPreviousVPPM[i] = 0;
    }
    
}

void CPeakMeter::setAlphaAT(float fAttTime){
    m_fAlphaAT = 1 - exp(-2.2/(m_fSamplingFreq*fAttTime));
}

void CPeakMeter::setAlphaRT(float fRelTime){
    m_fAlphaRT = 1 - exp(-2.2/(m_fSamplingFreq*fRelTime));
}

float CPeakMeter::getAlphaAT(){
    return m_fAlphaAT;
}

float CPeakMeter::getAlphaRT(){
    return m_fAlphaRT;
}

void CPeakMeter::process(float **ppfAudioData, int iNumOfFrames, float *pfPeakValue){
    
    for (int k=0; k<m_iNumChannels; k++){
        pfPeakValue[k]=0;
    }
    
    for (int j=0; j<iNumOfFrames; j++) {
        for (int i = 0; i<m_iNumChannels; i++){

        //filtering
           ppfAudioData[i][j] = fabs(ppfAudioData[i][j]);
           if(ppfAudioData[i][j]< m_pfPreviousVPPM[i])
           {
               m_pfVPPM[i] = m_pfPreviousVPPM[i] * (1-m_fAlphaRT);
           }
           else
           {
               m_pfVPPM[i] = m_fAlphaAT*ppfAudioData[i][j]+(1-m_fAlphaAT)* m_pfPreviousVPPM[i];
           }
            
            m_pfPreviousVPPM[i] = m_pfVPPM[i];
            
           if (pfPeakValue[i]<fabs(m_pfVPPM[i]))
            {
            pfPeakValue[i] = m_pfVPPM[i];
            //std::cout<< pfPeakValue[i]<<std::endl;
            }
            
        }
        
    }
}