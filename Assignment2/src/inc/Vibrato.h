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
#include "c_sinewave.h"
#include "ErrorDef.h"

class Vibrato{
private:
    CRingBuffer<float> **ppfRingBuff;
    c_sinewave **ppcLFO;
    
    enum VibratoParam {
        kModFreq,
        kWidth,
        kNumParams
    };
    
    float fVibParam[kNumParams];//ModFreq,Width,
    float fVibParamRange[kNumParams][2];
    float SamplingRate;
    int iNumChannels;   //Width in seconds,ModFreq in Hz
    
public:
    void process(float **fInBuff,float **fOutbuff);
    
    void setVibratoParam(VibratoParam eVibParam, float fParamVal);
    float getVibratoParam(VibratoParam eVibParam) const;
    
    
protected:
    Vibrato(float fVParam[2], int UserNumChannels,float UserSamplingRate);
    virtual ~Vibrato();
};

#endif /* Vibrato_h */
