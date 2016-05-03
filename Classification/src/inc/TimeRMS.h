#if !defined(__TimeRMS_hdr__)
#define __TimeRMS_hdr__

class CTimeRMS{
    
public:
    void initParams();
    void setParams(float fSamplingRate, int iBlockLength);
    static void createInstance(CTimeRMS *&pCRMS);
    static void destroyInstance(CTimeRMS *&pCRMS);
    float process(float *pfAudioSlice);
    void resetSumSqrd();
    
protected:
    CTimeRMS();
    virtual ~CTimeRMS();
  
private:
    float m_fSamplingRate;
    int m_iBlockLength;
    float m_fVrms;
    float m_fSumSqrd;
    
};

#endif