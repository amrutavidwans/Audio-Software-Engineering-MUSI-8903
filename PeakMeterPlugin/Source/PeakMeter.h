#if !defined(__PeakMeter_hdr__)
#define __PeakMeter_hdr__

class CPeakMeter{

public:
    void initPeakMeter(float fSamplingFreq, int iNumChannels);
    void process(float **ppfAudioData, int iNumOfFrames, float *pfPeakValue);
    void resetPeakMeterValues();
    static void createInstance(CPeakMeter *& pcPM);
    static void destroyInstance(CPeakMeter *& pcPM);
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
    float m_fAlphaRT;
    float m_fAlphaAT;
    float *m_pfPreviousVPPM;    // vppm at iHopLength
    float *m_pfVPPM;
    
};

#endif