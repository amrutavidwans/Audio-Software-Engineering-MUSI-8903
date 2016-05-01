
#include "SpectralCentroid.h"
#include "Util.h"

CSpectralCentroid::CSpectralCentroid(){
    this->initParams();
    CFft::create(m_pCFft);
}

CSpectralCentroid::~CSpectralCentroid(){
    CFft::destroy(m_pCFft);
}

void CSpectralCentroid::initParams(){
    m_fSamplingFreq = 0;
    m_iNumChan = 0;
    m_pCFft = 0;
}

void CSpectralCentroid::createInstance(CSpectralCentroid *&CSpecCentr){
    this->initParams();
    CFft::create(m_pCFft);
}

void CSpectralCentroid::destroyInstance(CSpectralCentroid *&CSpecCentr){
    CFft::destroy(m_pCFft);
}


void CSpectralCentroid::setParams(float fSamplingFreq, int iNumChan, int iBlockLength){
    m_fSamplingFreq = fSamplingFreq;
    m_iNumChan = iNumChan;
    
    int iZPfactor = 1;
    
    if (!CUtil::isPowOf2(iBlockLength))
        m_iNxtPow2BlkLen =CUtil::nextPowOf2(iBlockLength);  // for fft to be power of 2
    else
        m_iNxtPow2BlkLen = iBlockLength;
    
    m_pCFft->init( (2*m_iNxtPow2BlkLen), iZPfactor, CFft::WindowFunction_t::kWindowHann, CFft::Windowing_t::kNoWindow); //, 2, 1);
}

float CSpectralCentroid::process(float **AudioSlice){
    
}