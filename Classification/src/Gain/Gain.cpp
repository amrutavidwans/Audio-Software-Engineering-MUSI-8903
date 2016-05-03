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
fGainIndB(0)
{
//Left empty
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

void CGain::resetInstance(){
    iNumChannels=0;
    iSampFreq=0;
    fGainIndB=0;

}

void CGain::setGain(float fSetGain){
    fGainIndB=valtodB(fSetGain);
}

float CGain::valtodB(float val){
    val = 20* log10f(val);

    if (val < -100)
        val = -100;

    return val;

}



void CGain::process(float **InputBuf, float **OutputBuf, int iBlockLength)
{
    for(int i=0;i<iNumChannels;i++)
    {
        for(int j=0;j<iBlockLength;j++)
        {
            OutputBuf[i][j]=dBtoval(fGainIndB)* InputBuf[i][j] ;
        
        }

    
    }
    
}

float CGain::dBtoval(float dB)
{  int val;
    val= val/20;
    val=10^val;
    return val;
}

float CGain::getGain()
{
    return fGainIndB;
}









