
class CPeakMeter{

public:
    void initPeakMeter(float fSamplingFreq, int iNumChannels);
    void process(float **ppfAudioData, int iNumOfFrames, float *pfPeakValue);
    void resetPeakMeterValues();
    CPeakMeter();
    virtual ~CPeakMeter();
protected:
private:
    float m_fSamplingFreq;
    int m_iNumChannels;
    float m_fAlphaRT;
    float m_fAlphaAT;
    float *m_pfPreviousVPPM;    // vppm at iHopLength
    float *m_pfVPPM;
    
};