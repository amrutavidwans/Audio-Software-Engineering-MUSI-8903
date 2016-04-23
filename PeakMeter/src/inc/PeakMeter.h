#if !defined(__PeakMeter_hdr_)
#define __PeakMeter_hdr_

class CPeakMeter{
    
public:
    void initPeakMeter(float fSamplingFreq, int iNumChannels);
    static void createInstance(CPeakMeter*& pcPM);
    static void destroyInstance(CPeakMeter*& pcPM);
    void process(float **ppfAudioData, int iNumOfFrames, float *pfPeakValue);
    void resetPeakMeterValues();
    void setAlphaRT(float fRelTime);
    void setAlphaAT(float fAttTime);
    float getAlphaAT();
    float getAlphaRT();
  
protected:
    CPeakMeter();
    virtual ~CPeakMeter();
private:
    float m_fSamplingFreq;
    int m_iNumChannels;
    float *m_pfPreviousVPPM;    // vppm at iHopLength
    float *m_pfVPPM;
    float m_fAlphaRT;
    float m_fAlphaAT;
    
};
#endif//(__PeakMeter_hdr_)