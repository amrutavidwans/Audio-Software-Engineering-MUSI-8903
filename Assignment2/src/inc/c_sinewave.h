//
//  c_sinewave.h
//  MUSI8903
//
//  Created by Milap Rane on 1/26/16.
//
//
#include <math.h>
#include "ErrorDef.h"
#ifndef c_sinewave_h
#define c_sinewave_h
#define PI 3.14159265

class c_sinewave
{   float freq,Amp,TimeInSec;
    int iNumChannels;
public:
    void SetSineWavParam(float ffreq,float fAmp,float fTimeInSec, int iChannels);
    void GetSineWave(float **foutsine,float fSamplingFreq);
    void GetSineSum(float **pc_sine, float *SineSum, float fSamplingFreq);
    Error_t create(c_sinewave *&pc_sinewave);
    Error_t destroy(c_sinewave *&pc_sinewave);
    Error_t init();
protected:
    c_sinewave();
    //~c_sinewave();
};

c_sinewave::c_sinewave()
{
    this->init();
}

Error_t c_sinewave::init()
{
    freq=441;
    Amp=1;
    TimeInSec=3;
    iNumChannels = 2;
    return kNoError;
}

Error_t c_sinewave::create (c_sinewave *&pc_sinewave)
{
    pc_sinewave = new c_sinewave;
    
    if (!pc_sinewave)
        return kUnknownError;
    
    
    return kNoError;
}

Error_t c_sinewave::destroy (c_sinewave *&pc_sinewave)
{
    if (!pc_sinewave)
        return kUnknownError;
    
    // pCombFilt->reset ();
    
    delete pc_sinewave;
    pc_sinewave = 0;
    
    return kNoError;
    
}

void c_sinewave::GetSineSum(float **pc_sine, float *SineSum, float fSamplingFreq){
    int iNoSamples=TimeInSec * (fSamplingFreq);
    for (int i=0; i<iNumChannels; i++) {
        for (int j=0; j<iNoSamples; j++){
            (*SineSum) = (*SineSum) + fabs(pc_sine[i][j]);
        }
    }
}

void c_sinewave::SetSineWavParam (float ffreq,float fAmp,float fTimeInSec, int iChannels)
{
    freq= ffreq;
    Amp= fAmp;
    TimeInSec= fTimeInSec;
    iNumChannels = iChannels;
}

void c_sinewave::GetSineWave(float **foutsine,float fSamplingFreq)
{   int iNoSamples=TimeInSec * (fSamplingFreq);
    
    
    for (int j=0; j<iNumChannels; j++){
    for(int i=0;i<iNoSamples;i++)
    {
        foutsine[j][i] = (Amp * sin ( (2*PI*freq*i)/fSamplingFreq));
        //std::cout<< foutsine[i]<<std::endl;
    }
    }
   
}

#endif /* c_sinewave_h */
