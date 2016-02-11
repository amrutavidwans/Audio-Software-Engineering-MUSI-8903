//
//  LFO.h
//  MUSI8903
//
//  Created by Amruta Vidwans on 2/6/16.
//
//

#ifndef LFO_h
#define LFO_h

#define PI 3.14159265

class LFO{
private:
    float freq;
    float Amp;
   // float phase;
public:

LFO()
{
    this->init();
}

//Initialising the LFO with Standard values freq=441 Hz and Amplitude=1 Hz

void init()
{
    freq=441;
    Amp=1;
   // phase = 0;
}

void setValsSineWave(float Userfreq, float UserAmp){
    freq = Userfreq;
    Amp = UserAmp;
}


float GetSineWaveValue(float TimeInSec)
{
    return (Amp * sin (2*PI*freq*TimeInSec));
}

float getFreq(){
    return freq;
}

float getAmp(){
    return Amp;
}
};

#endif /* LFO_h */
