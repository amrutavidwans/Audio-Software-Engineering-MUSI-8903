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
    static void createInstance(CPan*& pCPan);
    static void destroyInstance(CPan*& pCPan);
    void resetInstance();
    void process(float **Inputbuf, float **Outputbuf, int iBlockLength);
    void setParam(int iSampFreq,int iNumberChannels);
    
protected:
    CPan();
    virtual ~CPan();
    
    
private:
    
    int iNumChannels;
    int iSampFreq;
    float fPan;
    
    
};

#endif /* Pan_hpp */
