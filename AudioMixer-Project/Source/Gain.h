/*
  ==============================================================================

    Gain.h
    Created: 11 Mar 2016 12:20:55am
    Author:  Milap Rane

  ==============================================================================
*/

#ifndef GAIN_H_INCLUDED
#define GAIN_H_INCLUDED
class CGain
{
    
    
public:
    
    void setGain(float iGainIndB);
    float getGain();
    static void createInstance(CGain*& pCGain);
    static void destroyInstance(CGain*& pCGain);
    float valtodB(float val);
    float dBtoval(float dB);
    void resetInstance();
    void process(float **Inputbuf, float **Outputbuf, int iBlockLength);
    void setParam(int iSampling,int NumChannels);
    
protected:
    CGain();
    virtual ~CGain();
    
    
private:
    
    int iNumChannels;
    int iSampFreq;
    float fGain;
    
    
};




#endif  // GAIN_H_INCLUDED
