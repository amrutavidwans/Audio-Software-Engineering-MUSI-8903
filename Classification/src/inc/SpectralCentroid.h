#if !defined(__SpectralCentroid_hdr__)
#define __SpectralCentroid_hdr__

#include "Fft.h"

class CSpectralCentroid{

public:
    void initParams();
    void setParams(float fSamplingFreq, int iNumChan, int iBlockLength);
    float process(float *AudioSlice);
    void createInstance(CSpectralCentroid *&CSpecCentr);
    void destroyInstance(CSpectralCentroid *&CSpecCentr);
    void initSpectrumValues();
    
protected:
    CSpectralCentroid();
    virtual ~CSpectralCentroid();
    
private:
    float m_fSamplingFreq;
    int m_iNumChan;
    int m_iNxtPow2BlkLen;
    CFft *m_pCFft;
    float *m_pfSpectrum;
    float *m_pfMag;
    float m_fSpectralSum;
    float m_fSpectralCentroid;
    
};




#endif