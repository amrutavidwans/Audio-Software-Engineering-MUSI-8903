//
//  Vibrato.cpp
//  MUSI8903
//
//  Created by Amruta Vidwans on 2/5/16.
//
//

#include "Vibrato.h"

#include <stdio.h>
#include "RingBuffer.h"
#include "c_sinewave.h"
#include "Util.h"


Vibrato::Vibrato(float fVParam[2], int UserNumChannels,float UserSamplingRate){
    //ModFreq = 5 * SamplingRate; // 5-14Hz range
    //Width = 0.01 * SamplingRate; //5 to 10ms
    fVibParam[kModFreq] = fVParam[kModFreq] / SamplingRate;
    fVibParam[kWidth] = fVParam[kWidth] * SamplingRate;
    
    iNumChannels = UserNumChannels;
    SamplingRate = UserSamplingRate;
    
    fVibParamRange[kModFreq][0] = 5 / SamplingRate;
    fVibParamRange[kModFreq][1] = 14 / SamplingRate;
    fVibParamRange[kWidth][0] = 0.005 * SamplingRate;
    fVibParamRange[kWidth][1] = 0.01 * SamplingRate;
    
    c_sinewave **ppcLFO;
    
    int idx = CUtil::float2int<int>(fVibParam[kWidth]);
    
    ppfRingBuff = new CRingBuffer<float> *[iNumChannels];
    for (int i = 0; i<iNumChannels; i++){
        ppfRingBuff[i] = new CRingBuffer<float>(idx);
    }
    
}

Vibrato::~Vibrato(){
    if (ppfRingBuff) {
        for (int i = 0; i<iNumChannels; i++)
            delete ppfRingBuff[i];
    }
    delete []ppfRingBuff;
}

void Vibrato::process(float **fInBuff,float **fOutbuff){
    
}


void Vibrato::setVibratoParam(VibratoParam eVibParam, float fParamVal){
    fVibParam[eVibParam] = fParamVal;
}

float Vibrato::getVibratoParam(VibratoParam eVibParam) const{
    return fVibParam[eVibParam];
}





