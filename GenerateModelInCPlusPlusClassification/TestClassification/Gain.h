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
    void createInstance(CGain*& pCGain);
    void destroyInstance(CGain*& pCGain);
    float valtodB(float val);
    float dBtoval(float dB);
    void resetInstance();
    void process(float **Inputbuf, float **Outputbuf, int iBlockLength);
    
protected:
    
    
    
private:
    CGain();
    virtual ~CGain();
    int iNumChannels;
    int iSampFreq;
    float fGainIndB;
    
    
};




#endif  // GAIN_H_INCLUDED
