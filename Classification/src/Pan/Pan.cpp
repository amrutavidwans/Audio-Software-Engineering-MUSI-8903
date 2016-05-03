//
//  Pan.cpp
//  AudioMixer-Project
//
//  Created by Milap Rane on 5/1/16.
//
//

#include "Pan.h"
#include <math.h>

CPan::CPan():
iNumChannels(2),
iSampFreq(44100),
fPan(0)
{
    this->resetInstance();
}


CPan::~CPan()
{
    
    this->resetInstance();
}

void CPan::createInstance (CPan*& pCPan)
{
    pCPan = new CPan ();
}



void CPan::destroyInstance (CPan*& pCPan)
{
    
    delete pCPan;
    pCPan = 0;
    
}

void CPan::resetInstance()
{
    iNumChannels=0;
    iSampFreq=0;
    fPan=0;
    
}

void CPan::setPan(float fiPan)
{
   fPan=fiPan;
}

void CPan::setParam(int iSamplingFreq, int iNumberChannels){
    iNumChannels=iNumberChannels;
    iSampFreq=iSamplingFreq;
}


void CPan::process(float **InputBuf, float **OutputBuf, int iBlockLength)
{
    for(int i=0;i<iNumChannels;i++)
    {
        for(int j=0;j<iBlockLength;j++)
        {
            if (i==0)
                 OutputBuf[i][j]=cosf(M_PI*(fPan+1)/4)*InputBuf[i][j] ;
            else if (i==1)
                 OutputBuf[i][j]=sinf(M_PI*(fPan+1)/4)*InputBuf[i][j] ;
        
        }
        
        
    }
    
}

float CPan::getPan(){
    return fPan;

}
