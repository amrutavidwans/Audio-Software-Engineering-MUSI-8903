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
#include "LFO.h"
#include "Util.h"

// class constructor initializes the parameters as well

Vibrato::Vibrato(float fVParam[3], int UserNumChannels,float UserSamplingRate, int iMaxDelayInSec){
    //ModFreq = 5 * SamplingRate; // 5-14Hz range
    //Width = 0.01 * SamplingRate; //5 to 10ms
    fVibParam[kModFreq] = fVParam[kModFreq] / SamplingRate;
    fVibParam[kWidth] = fVParam[kWidth] * SamplingRate;
    fVibParam[kDelay] = fVParam[kDelay] * SamplingRate;
    
    iNumChannels = UserNumChannels;
    SamplingRate = UserSamplingRate;
    iFramesProcessed = 0;
    
    fVibParamRange[kModFreq][0] = 5 / SamplingRate;
    fVibParamRange[kModFreq][1] = 14 / SamplingRate;
    fVibParamRange[kWidth][0] = 0.005 * SamplingRate;
    fVibParamRange[kWidth][1] = 0.01 * SamplingRate;
    fVibParamRange[kDelay][0] = fVibParamRange[kWidth][0]; // must be atleast equal to the width
    fVibParamRange[kDelay][1] = iMaxDelayInSec - fVibParamRange[kWidth][0]; // must be less than max possible width specified in the input of the constructor
    
    // check if parameters are in range
       if(!(fVibParamRange[kModFreq][0]< fVParam[0]<=fVibParamRange[kModFreq][1])||!(fVibParamRange[kWidth][0]< fVParam[0]<=fVibParamRange[kWidth][1]))
       { fVibParam[kModFreq]= 5/SamplingRate;
           fVibParam[kWidth]=0.005* SamplingRate;
       }
    
    ppcLFO.setValsSineWave(fVParam[kModFreq], 1.0);
    
    int idx = CUtil::float2int<int>(fVibParam[kWidth] + fVibParam[kDelay]);
    
    ppfRingBuff = new CRingBuffer<float> *[iNumChannels];
    for (int i = 0; i<iNumChannels; i++){
        ppfRingBuff[i] = new CRingBuffer<float>(CUtil::float2int<int>(iMaxDelayInSec*SamplingRate));
        ppfRingBuff[i]->setWriteIdx(idx);
    }

}

// destructor
Vibrato::~Vibrato(){
    if (ppfRingBuff) {
        for (int i = 0; i<iNumChannels; i++)
            delete ppfRingBuff[i];
    }
    delete []ppfRingBuff;
}


// the actual vibrato process function
void Vibrato::process(float **ppfInBuff, float **ppfOutbuff, int iNumOfFrames){

    for (int j=0; j<iNumOfFrames; j++){
        
        for (int i=0; i<iNumChannels; i++){
            
            float tapMod = fVibParam[kDelay] + (fVibParam[kWidth] * ppcLFO.GetSineWaveValue(iFramesProcessed/SamplingRate));
            
            // delay line update
            
            ppfRingBuff[i]->putPostInc(ppfInBuff[i][j]); // put new value in the write location of ring buffer and then increment write location
            
            ppfRingBuff[i]->get();  // increment read index of ring buffer
            
            int BuffWrtIdx = ppfRingBuff[i]->getWriteIdx();
            
            ppfOutbuff[i][j] = ppfRingBuff[i]->getValuesAtNonIntLocations(BuffWrtIdx-tapMod);   // value to be accessed is
            
        }
        iFramesProcessed = iFramesProcessed +1;
    }
}

// set individual vibrato parameters
void Vibrato::setVibratoParam(VibratoParam eVibParam, float fParamVal){
    
    if (eVibParam == kWidth || eVibParam == kDelay){
        
        //What to do if there is change in Ring Buffer
        int iNumAdditionalTaps  = CUtil::float2int<int>(fParamVal - (fVibParam[kWidth]+fVibParam[kDelay]));
        if (iNumAdditionalTaps < 0)
        {
            for (int c = 0; c < iNumChannels; c++)
            {
                ppfRingBuff[c]->setWriteIdx(CUtil::float2int<int>(fParamVal) + ppfRingBuff[c]->getReadIdx());
            }
        }
        else
        {
            
            for (int c = 0; c < iNumChannels; c++)
            {
                for (int i = 0; i < iNumAdditionalTaps; i++)
                {
                    ppfRingBuff[c]->putPostInc(0.F);
                }
            }
        }

    }
    
    fVibParam[eVibParam] = fParamVal;
    
}

// grt individual vibrato parameters
float Vibrato::getVibratoParam(VibratoParam eVibParam) const{
    return fVibParam[eVibParam];
}





