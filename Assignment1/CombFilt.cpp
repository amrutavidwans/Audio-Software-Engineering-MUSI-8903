//
//  CombFilt.cpp
//  MUSI8903
//
//  Created by Milap Rane on 1/24/16.
//
//

#include <stdio.h>
#include "CombFilt.h"
#include "ErrorDef.h"


CombFilt::CombFilt ()
{
    // this never hurts
    this->reset ();
}

CombFilt::~CombFilt()
{
    this->reset ();
}


Error_t CombFilt::create (CombFilt *& pCombFilt)
{
    pCombFilt = new CombFilt ();
    
    if (!pCombFilt)
        return kUnknownError;
    
    
    return kNoError;
}

Error_t CombFilt::destroy (CombFilt*& pCombFilt)
{
    if (!pCombFilt)
        return kUnknownError;
    
    pCombFilt->reset ();
    
    delete pCombFilt;
    pCombFilt = 0;
    
    return kNoError;
    
}

void CombFilt::GetFiltVar(float g_ent, float tau_ent)
{
    g=g_ent;
    tau=tau_ent;
}




