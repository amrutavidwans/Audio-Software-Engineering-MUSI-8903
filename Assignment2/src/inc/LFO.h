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
    LFO();
    void init();
    float GetSineWaveValue(float TimeInSec);
    void setValsSineWave(float freq, float Amp);
    float getFreq();
    float getAmp();
};

LFO::LFO()
{
    this->init();
}

void LFO::init()
{
    freq=441;
    Amp=1;
   // phase = 0;
}

void LFO::setValsSineWave(float Userfreq, float UserAmp){
    freq = Userfreq;
    Amp = UserAmp;
}


float LFO::GetSineWaveValue(float TimeInSec)
{
    return (Amp * sin (2*PI*freq*TimeInSec));
}

float LFO::getFreq(){
    return freq;
}

float LFO::getAmp(){
    return Amp;
}


#endif /* LFO_h */
