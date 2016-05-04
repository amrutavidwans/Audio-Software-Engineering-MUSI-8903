#include "TimeRMS.h"
#include <cmath>

CTimeRMS::CTimeRMS(){
    this->initParams();
}

CTimeRMS::~CTimeRMS(){
    this->initParams();
}

void CTimeRMS::initParams(){
    m_fSamplingRate = 0;
    m_iBlockLength = 0;
    m_fSumSqrd = 0;
    m_fVrms = 0;
}

void CTimeRMS::setParams(float fSamplingRate, int iBlockLength){
    m_iBlockLength = iBlockLength;
    m_fSamplingRate = fSamplingRate;
}

void CTimeRMS::createInstance(CTimeRMS *&pCRMS){
    pCRMS = new CTimeRMS();
    pCRMS->initParams();
}

void CTimeRMS::destroyInstance(CTimeRMS *&pCRMS){
    delete[] pCRMS;
    pCRMS = 0;
}

void CTimeRMS::resetSumSqrd(){
    m_fSumSqrd = 0;
    m_fVrms = 0;
}

float CTimeRMS::process(float *pfAudioSlice){
    resetSumSqrd();
    for(int i=0;i<m_iBlockLength; i++){
        m_fSumSqrd += powf(pfAudioSlice[i], 2);
    }
    m_fVrms = sqrtf(m_fSumSqrd/m_iBlockLength);
    return m_fVrms;
}