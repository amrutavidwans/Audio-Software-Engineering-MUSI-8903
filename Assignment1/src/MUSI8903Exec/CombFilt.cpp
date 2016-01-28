//
//  CombFilt.cpp
//  MUSI8903
//
//  Created by Milap Rane on 1/24/16.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>
#include <iostream>
#include "CombFilt.h"
#include "ErrorDef.h"
#include "AudioFileIf.h"
//#include <algorithm>
#include "Util.h"
#include <new>


CombFilt::CombFilt ()
{
    // this never hurts
    this->init();
}

Error_t CombFilt::create (CombFilt *&pCombFilt)
{
    pCombFilt = new CombFilt;
    
    if (!pCombFilt)
        return kUnknownError;
    
    
    return kNoError;
}

Error_t CombFilt::destroy (CombFilt *&pCombFilt)
{
    if (!pCombFilt)
        return kUnknownError;
    
   // pCombFilt->reset ();
    
    delete pCombFilt;
    pCombFilt = 0;
    
    return kNoError;
    
}

Error_t CombFilt::init(){
    // allocate memory
    // initialize variables and buffers
    
    //pCombFilt = new CombFilt;
    g=0.5;     // gain from 0-1 for IIR filter
    tau = 0.5;  // tau in seconds
    return kNoError;
}


void CombFilt::GetFiltVar(float fg_ent, float ftau_ent,float fFs)
{
    g= fg_ent;
    float temp=ftau_ent*fFs;
    if (temp>=0.F)
        tau= static_cast<int>(temp+0.5F);
    else
        tau= static_cast<int>(temp-0.5F);
}

void CombFilt::createBuffer(int iNumChannels){
    
    ppfDelayLine= new float *[iNumChannels];
    for (int i=0; i< iNumChannels; i++) {
        ppfDelayLine[i]=new float[tau];
    }
    
}

void CombFilt::clearBufer(int iNumChannels){
    for (int i=0; i<iNumChannels; i++){
        for(int j=0; j<tau; j++){
            ppfDelayLine[i][j]=0;
        }
    }
    
}

void CombFilt::destroyBuffer(int iNumChannels){
    for (int i=0; i<iNumChannels; i++)
        delete [] ppfDelayLine[i];
    delete [] ppfDelayLine;
    ppfDelayLine = 0;
}

void CombFilt::FIRCombFilt(float **ppfAudioData,float **ppfFiltAudio,int iNumChannels ,int iInFileLength)
{
    for (int i=0; i < iNumChannels;i++){
        for (int j=0; j<iInFileLength; j++) {
            ppfFiltAudio[i][j] = ppfAudioData[i][j] + g * ppfDelayLine[i][tau-1];
            
            for (int k = tau-1; k>0; k--)
            {
                ppfDelayLine[i][k]=ppfDelayLine[i][k-1];
             }
                ppfDelayLine[i][0]=ppfAudioData[i][j];
            
            /*for (int kkt=0;kkt<tau; kkt++){
                std::cout<<ppfDelayLine[0][kkt]<<std::endl;
            }*/
            //std::cout<<std::endl;
            
        }
    }
    
    
}

void CombFilt::IIRCombFilt(float **ppfAudioData,float **ppfFiltAudio,int iNumChannels ,int iInFileLength)
{
    // handle range of values for g else filter will become unstable
    if (std::fabs(g)>=1)
    {
        std::cout<< "Absolute value of gain greater than or equal to 1 !" << std::endl;
        std::cout << "Using the default value of gain" << std::endl;
        g=0.5;
    }
    
    for (int i=0; i < iNumChannels;i++){
        for (int j=0; j<iInFileLength; j++) {
            ppfFiltAudio[i][j] = ppfAudioData[i][j] + g * (ppfDelayLine[i][tau-1]);
            for (int k = tau-1; k>0; k--)
            {
                ppfDelayLine[i][k]=ppfDelayLine[i][k-1];
            }
            ppfDelayLine[i][0]=ppfFiltAudio[i][j];
            
        }
    }
    
}





