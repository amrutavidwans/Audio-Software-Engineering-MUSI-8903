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

class Vibrato{
    CRingBuffer<<#class T#>>
public:
    void process(CRingBuffer<float> **tRingBuff,float **fOutbuff);
    void addParam(float Modfreq);
    void create();
    void destroy();
protected:
    

private:






};

#endif /* Vibrato_h */
