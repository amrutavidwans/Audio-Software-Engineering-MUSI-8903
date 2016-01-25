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
{   float g,tau;
public:
    /*! version number */
    
    enum FilterType
    {
        FIR,
        IIR
    };
    
    void GetFiltVar(float,float);
    void FIRCombFilt(float audiodata);
    void IIRCombFilt(float audiodata);
    

    static Error_t create (CombFilt*& pCombFiltIf);
    static Error_t destroy (CombFilt*& pCombFiltIf);
    
    Error_t init ();
    Error_t reset ();
    
    //virtual Error_t process (float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames) = 0;
    
protected:
    CombFilt ();
    virtual ~CombFilt ();
};


#endif /* CombFilt_h */
