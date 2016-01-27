//
//  c_sinewave.h
//  MUSI8903
//
//  Created by Milap Rane on 1/26/16.
//
//
#include <math.h>
#ifndef c_sinewave_h
#define c_sinewave_h
#define PI 3.14159265

class c_sinewave
{   float freq,Amp,TimeInSec;
public:
    void SetSineWavParam(float ffreq,float fAmp,float fTimeInSec);
    float GetSineWave(float fSamplingFreq);
protected:
    c_sinewave ();
    ~c_sinewave()
}
#endif /* c_sinewave_h */
(
void c_sinewave::SetSineWavParam(float ffreq,float fAmp,float fTimeInSec)
{
    ffreq=freq;
    fAmp=Amp;
    fTimeInSec=TimeInSec;
}

 float c_sinewave::GetSineWave(float fSamplingFreq)
{   int iNosamples=TimeInSec * fSamplingFreq
    foutsine=new float[iNoSamples];
    for(int i=0;i<iNoSamples;i++)
    {
        foutsine = Amp * sin ( (2*PI*freq*i)/fSamplingFreq);
    }
    return foutsine;
}