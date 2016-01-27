//
//  CombFilt.h
//  MUSI8903
//
//  Created by Milap Rane on 1/24/16.
//
//

#ifndef CombFilt_h
#define CombFilt_h
#include "ErrorDef.h"

class CombFilt
{   float g;   ///
    int tau;
    float **ppfDelayLine;
public:
    /*! version number */
    
//    enum FilterType
//    {
//        FIR,
//        IIR
//    };
    
    void GetFiltVar(float fg_ent,float ftau_ent,float fFs);
    void FIRCombFilt(float **ppfAudioData,float **ppfFiltAudio,int iNumChannels ,int iInFileLength);
    void IIRCombFilt(float **ppfAudioData,float **ppfFiltAudio,int iNumChannels ,int iInFileLength);
    void createBuffer(int iNumChannels);
    void clearBufer(int iNumChannels);
    void destroyBuffer(int iNumChannels);

    static Error_t create (CombFilt *&pCombFiltIf);
    static Error_t destroy (CombFilt *&pCombFiltIf);
    
    Error_t init ();
    //Error_t reset ();
    
    //virtual Error_t process (float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames) = 0;
    
protected:
    CombFilt ();
    //virtual ~CombFilt ();
};


#endif /* CombFilt_h */
