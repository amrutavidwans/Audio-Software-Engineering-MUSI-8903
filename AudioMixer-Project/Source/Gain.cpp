/*
  ==============================================================================

    Gain.cpp
    Created: 11 Mar 2016 12:20:55am
    Author:  Milap Rane

  ==============================================================================
*/

#include "Gain.h"

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

void CGain::setGain(float fGainIndB){
 

}











