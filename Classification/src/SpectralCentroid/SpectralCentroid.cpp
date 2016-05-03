
#include "SpectralCentroid.h"
#include "Util.h"
#include <iostream>


CSpectralCentroid::CSpectralCentroid(){
    this->initParams();
    CFft::create(m_pCFft);
}

CSpectralCentroid::~CSpectralCentroid(){
    delete[] m_pfSpectrum;
    delete[] m_pfMag;
    CFft::destroy(m_pCFft);
}

void CSpectralCentroid::initParams(){
    m_fSamplingFreq = 0;
    m_pfSpectrum = 0;
    m_pfMag = 0;
    m_fSpectralCentroid = 0;
    m_fSpectralSum = 0;
}

 void CSpectralCentroid::createInstance(CSpectralCentroid *&CSpecCentr){
    CSpecCentr= new CSpectralCentroid();
    CSpecCentr->initParams();
}

void CSpectralCentroid::destroyInstance(CSpectralCentroid *&CSpecCentr){
    delete[] CSpecCentr;
    CSpecCentr = 0;
}


void CSpectralCentroid::setParams(float fSamplingFreq, int iBlockLength){
    m_fSamplingFreq = fSamplingFreq;
    
    int iZPfactor = 1;
    
    if (!CUtil::isPowOf2(iBlockLength))
        m_iNxtPow2BlkLen =CUtil::nextPowOf2(iBlockLength);  // for fft to be power of 2
    else
        m_iNxtPow2BlkLen = iBlockLength;
    
    m_pCFft->init( m_iNxtPow2BlkLen, iZPfactor, CFft::WindowFunction_t::kWindowHann, CFft::Windowing_t::kNoWindow); //, 2, 1);
    
    m_pfSpectrum = new float [m_iNxtPow2BlkLen];
    m_pfMag = new float [m_iNxtPow2BlkLen];
    initSpectrumValues();
    
}

void CSpectralCentroid::initSpectrumValues(){
    std::memset(m_pfMag, 0, sizeof(float)*m_iNxtPow2BlkLen);
    std::memset(m_pfSpectrum, 0, sizeof(float)*m_iNxtPow2BlkLen);
    m_fSpectralSum = 0;
    m_fSpectralCentroid = 0;
}

float CSpectralCentroid::process(float *AudioSlice){
    

    
    initSpectrumValues();
    m_pCFft->doFft(m_pfSpectrum, AudioSlice);
    
    m_pCFft->getMagnitude(m_pfMag, m_pfSpectrum);
    
    for (int i=0; i<m_iNxtPow2BlkLen; i++){
        
        m_fSpectralSum += (m_pfMag[i]*m_pfMag[i]*m_iNxtPow2BlkLen);
        m_fSpectralCentroid += (i*m_pfMag[i]*m_pfMag[i]*m_iNxtPow2BlkLen);
        //std::cout<< AudioSlice[i] <<" "<<m_pfMag[i]*m_iNxtPow2BlkLen<<std::endl;
    }
    
    if (m_fSpectralSum == 0){
        return 0.F;
    }
    else{
        m_fSpectralCentroid = (m_fSpectralCentroid/m_fSpectralSum);
        return (((m_fSpectralCentroid*2)/m_iNxtPow2BlkLen)*(m_fSamplingFreq/2));
    }
    
}