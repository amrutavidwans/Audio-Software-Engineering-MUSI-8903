#include "RingBuffer.h"

class CPeakMeter{

public:
    void initPeakMeter(float fSamplingFreq, int iBlockLen, int iHopLength, int iNumChannels);
    void process(float **ppfAudioData, float *pfPeakValue);
    void resetPeakMeterValues();
    CPeakMeter();
    virtual ~CPeakMeter();
protected:
private:
    float m_fSamplingFreq;
    int m_iNumChannels;
    float m_fAlphaRT;
    float m_fAlphaAT;
    int m_iBlockLength;
    int m_iHopLength;
    float *m_pfPreviousVPPM;    // vppm at iHopLength
    CRingBuffer<float> **m_ppCRingBuff;
    
    
};