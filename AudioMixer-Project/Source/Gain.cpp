/*
  ==============================================================================

    Gain.cpp
    Created: 11 Mar 2016 12:20:55am
    Author:  Milap Rane

  ==============================================================================
*/

#include "Gain.h"
#include <math.h>

CGain::CGain():
iNumChannels(2), 
iSampFreq(44100),
fGain(0)
{
    this->resetInstance();
}


CGain::~CGain()
{

    this->resetInstance();
}

void CGain::createInstance (CGain*& pCCgain)
{
 pCCgain = new CGain ();
}



void CGain::destroyInstance (CGain*& pCCGain)
{
    
    delete pCCGain;
    pCCGain = 0;
    
}

void CGain::resetInstance()
{
    iNumChannels=0;
    iSampFreq=0;
    fGain=0;

}

void CGain::setGain(float fSetGain)
{
    fGain=fSetGain;
}

float CGain::valtodB(float val)
{
    val = 20* log10f(val);

    if (val < -100)
        val = -100;

    return val;

}

void CGain::setParam(int iSamplingFreq, int iNumberChannels)
{
    iNumChannels=iNumberChannels;
    iSampFreq=iSamplingFreq;
}




void CGain::process(float **InputBuf, float **OutputBuf, int iBlockLength)
{
    for(int i=0;i<iNumChannels;i++)
    {
        for(int j=0;j<iBlockLength;j++)
        {
            OutputBuf[i][j]= fGain * InputBuf[i][j] ;
            if(OutputBuf[i][j]>=1.F)
                OutputBuf[i][j]=1.F;
        }

    
    }
    
}

float CGain::dBtoval(float dB)
{   float val=0.F;
    val=dB;
    val= val/20;
    val=powf(10,val);
    return val;
}

float CGain::getGain()
{
    return fGain;
}









