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

class Vibrato{
    CRingBuffer<double> RingBuff;
    c_sinewave LFO;
    float ModFreq,Width,SamplingRate;   //Width in seconds,ModFreq in Hz
    
public:
    void process(float **fInBuff,float **fOutbuff);
    void addParam(float Modfreq,float Width,float SamplingRate);
    void create();
    void destroy();
protected:
    

private:
    Vibrato();
    ~Vibrato();
};

#endif /* Vibrato_h */
