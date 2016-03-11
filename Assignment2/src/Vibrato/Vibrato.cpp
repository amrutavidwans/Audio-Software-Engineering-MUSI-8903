//
//  Vibrato.cpp
//  MUSI8903
//
//  Created by Amruta Vidwans on 2/5/16.
//
//

#include "Vibrato.h"
#include <iostream>
#include <stdio.h>
#include "Util.h"

// class constructor initializes the parameters as well

Vibrato::Vibrato(float fVParam[3], int UserNumChannels,float UserSamplingRate, int iMaxDelayInSec){
    //ModFreq = 5 * SamplingRate; // 5-14Hz range
    //Width = 0.01 * SamplingRate; //5 to 10ms
    
    iNumChannels = UserNumChannels;
    SamplingRate = UserSamplingRate;
    iFramesProcessed = 0;
    
    fVibParam[kModFreq] = fVParam[kModFreq] / SamplingRate;
    fVibParam[kWidth] = static_cast<int>(round(fVParam[kWidth] * SamplingRate));
    fVibParam[kDelay] = static_cast<int>(round(fVParam[kDelay] * SamplingRate));
    
    fVibParamRange[kModFreq][0] = -0.00001 / SamplingRate;
    fVibParamRange[kModFreq][1] = 14 / SamplingRate;
    fVibParamRange[kWidth][0] = -0.00001 * SamplingRate;     // WIdth is the modulation Amplitude
    fVibParamRange[kWidth][1] = (iMaxDelayInSec*SamplingRate/2);
    fVibParamRange[kDelay][0] = -0.00001 * SamplingRate; // must be atleast equal to the width
    fVibParamRange[kDelay][1] = (iMaxDelayInSec*SamplingRate/2);
    
    // check if parameters are in range
       if((fVibParamRange[kModFreq][0]> fVibParam[kModFreq] || fVibParam[kModFreq]>fVibParamRange[kModFreq][1]))
       {
           std::cout<< "Modulation frequency out of range. Enter values from 5-14Hz" << std::endl;
           std::cout << "Initializing to default value of 5Hz" <<std::endl;
           fVibParam[kModFreq]= 5/SamplingRate;
       }
    
    if ((fVibParamRange[kWidth][0]> fVibParam[kWidth] || fVibParam[kWidth]>fVibParamRange[kWidth][1])) {
        std::cout<< "Modulation width out of range. Enter values from 0.005 - 0.01 sec" << std::endl;
        std::cout << "Initializing to default value of 0.005sec" <<std::endl;
        fVibParam[kWidth]=0.005* SamplingRate;
    }
    
    if ((fVibParamRange[kDelay][0]> fVibParam[kDelay] || fVibParam[kDelay]>fVibParamRange[kDelay][1])) {
        std::cout<< "Delay out of range. It must be at least equal to modulation width" << std::endl;
        std::cout << "Initializing it to modulation Width" <<std::endl;
        fVibParam[kDelay]=fVibParam[kWidth];
    }
    
    
    if (fVibParam[kWidth]>fVibParam[kDelay]) {                            // width should not be greater than the delay
        std::cout <<"Width cannot be greater than Delay" << std::endl;
        std::cout <<"Interchanging Width and Delay" << std::endl;
        float temp = fVibParam[kDelay];
        fVibParam[kDelay]=fVibParam[kWidth];
        fVibParam[kWidth]=temp;
        
    }
    
    ppcLFO.setValsSineWave(fVParam[kModFreq], 1.0);
    
    int idx = CUtil::float2int<int>(fVibParam[kWidth] + fVibParam[kDelay] + 1);
    
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
            
            ppfRingBuff[i]->putPostInc(ppfInBuff[i][j]); // put new value in the write location of ring buffer and then increment write location
            ppfRingBuff[i]->getPostInc();  // increment read index of ring buffer
            
            double tapMod = 1 + fVibParam[kDelay] + (fVibParam[kWidth] * ppcLFO.GetSineWaveValue(iFramesProcessed/SamplingRate));
            
            int BuffWrtIdx = ppfRingBuff[i]->getWriteIdx();
            int BuffReadIdx = ppfRingBuff[i]->getReadIdx();
            
            double idx = BuffWrtIdx - tapMod - BuffReadIdx;
            double temp = ppfRingBuff[i]->getValuesAtNonIntLocations(idx);
            
            ppfOutbuff[i][j] =  temp;  // value to be accessed is
            
            //std::cout << ppfInBuff[i][j] << " " << temp << std::endl;
            
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





=======
>>>>>>> f65f7a8b673924d88b6e0b3f77b59323eaa4abdb
