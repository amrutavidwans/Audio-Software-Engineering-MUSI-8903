#include "PeakMeter.h"
#include <stdlib.h>
#include <math.h>

CPeakMeter::CPeakMeter ()
{
    this->resetPeakMeterValues();
    m_ppCRingBuff = 0;
    m_pfPreviousVPPM = 0;
}

CPeakMeter::~CPeakMeter(){
    
    for (int i=0; i<m_iNumChannels; i++) {
        delete m_ppCRingBuff[i];
    }
    delete [] m_ppCRingBuff;
    
    delete m_pfPreviousVPPM;
    this->resetPeakMeterValues();

}

void CPeakMeter::resetPeakMeterValues(){
    m_fSamplingFreq = 0;
    m_fAlphaRT = 0;
    m_fAlphaAT = 0;
    m_iBlockLength = 0;
    m_iHopLength = 0;
    m_iNumChannels = 0;
}

void CPeakMeter::initPeakMeter(float fSamplingFreq, int iBlockLen, int iHopLength, int iNumChannels){
    m_fSamplingFreq = fSamplingFreq;
    m_iBlockLength = iBlockLen;
    m_iNumChannels = iNumChannels;
    m_iHopLength = iHopLength;
    
    m_fAlphaRT = 1- exp(-2.2/(m_fSamplingFreq* 1.5));
    m_fAlphaAT = 1- exp(-2.2/(m_fSamplingFreq* 0.5));
    
    m_pfPreviousVPPM = new float [m_iNumChannels];
    
    m_ppCRingBuff = new CRingBuffer<float>*[iNumChannels];
    for (int c=0; c<m_iNumChannels; c++){
        m_ppCRingBuff[c]=new CRingBuffer<float>(m_iBlockLength);
    }
}


void CPeakMeter::process(float **ppfAudioData, float *pfPeakValue){
    for (int i = 0; i<m_iNumChannels; i++){
        float tempVal=m_pfPreviousVPPM[i];
        
        //filtering
        for (int j=0; j<m_iBlockLength; j++) {
            
           ppfAudioData[i][j] = std::abs(ppfAudioData[i][j]);
           if(ppfAudioData[i][j]< tempVal)
           {
               m_ppCRingBuff[i]->putPostInc(m_pfPreviousVPPM[i] * (1-m_fAlphaRT));
           }
           else
           {
               m_ppCRingBuff[i]->putPostInc(m_fAlphaAT*ppfAudioData[i][j]+(1-m_fAlphaAT)* m_pfPreviousVPPM[i]);
           }
            
            tempVal=m_ppCRingBuff[i]->get(m_ppCRingBuff[i]->getWriteIdx());
            
            if (j==m_iHopLength-1) {
                m_pfPreviousVPPM[i] = tempVal;
            }
            
        }
        
    }
    
    
    // find max value
    for (int i = 0; i<m_iNumChannels; i++){
        float MaxTempVal=0;
        float tempBuffVal=0;
        for (int j = 0;j<m_iBlockLength; j++){
            tempBuffVal = m_ppCRingBuff[i]->getPostInc();
            if (tempBuffVal>MaxTempVal){
                MaxTempVal=tempBuffVal;
            }
        }
        pfPeakValue[i]=MaxTempVal;
    }
    
        
    //convert to dB
    for (int i = 0; i<m_iNumChannels; i++){
        if(pfPeakValue[i]<0.00001){
            pfPeakValue[i]=0.00001;
        }
        else{
            pfPeakValue[i]=20*log10f(pfPeakValue[i]);
        }
    }
    
    
}