//
//  Pan.hpp
//  AudioMixer-Project
//
//  Created by Milap Rane on 5/1/16.
//
//

#ifndef Pan_hpp
#define Pan_hpp

class CPan
{
    
    
public:
    
    void setPan(float fPan);
    float getPan();
    void createInstance(CPan*& pCPan);
    void destroyInstance(CPan*& pCPan);
    void resetInstance();
    void process(float **Inputbuf, float **Outputbuf, int iBlockLength);
    
protected:
    
    
    
private:
    CPan();
    virtual ~CPan();
    int iNumChannels;
    int iSampFreq;
    float fPan;
    
    
};

#endif /* Pan_hpp */
