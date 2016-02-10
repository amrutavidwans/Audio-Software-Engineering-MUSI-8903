//
//  Vibrato.h
//  MUSI8903
//
//  Created by Amruta Vidwans on 2/5/16.
//
//
//This is the class for LFO for vibrato

#ifndef Vibrato_h
#define Vibrato_h

#include <stdio.h>
#include "RingBuffer.h"
#include "LFO.h"
#include "ErrorDef.h"

class Vibrato{
private:
    CRingBuffer<float> **ppfRingBuff;
    LFO ppcLFO;
  
    enum VibratoParam {
        kModFreq,
        kWidth,
        kDelay,
        kNumParams
    };
    
    float fVibParam[kNumParams];//ModFreq,Width,
    float fVibParamRange[kNumParams][3];
    float SamplingRate;
    int iNumChannels;   //Width in seconds,ModFreq in Hz
    int iFramesProcessed;
    
public:
    void process(float **ppfInBuff,float **ppfOutbuff, int iNumOfFrames);
    
    void setVibratoParam(VibratoParam eVibParam, float fParamVal);
    float getVibratoParam(VibratoParam eVibParam) const;
    
    Vibrato(float fVParam[3], int UserNumChannels,float UserSamplingRate, int iMaxDelayInSec);
    virtual ~Vibrato();
};

#endif /* Vibrato_h */
