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
#include "AudioFileIf.h"
#include <algorithm>


//CombFilt::CombFilt ()
//{
    // this never hurts
    //this->reset ();
//}

//CombFilt::~CombFilt()
//{
  //  this->reset ();
//}


Error_t CombFilt::create (CombFilt *&pCombFilt)
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
    
   // pCombFilt->reset ();
    
    delete pCombFilt;
    pCombFilt = 0;
    
    return kNoError;
    
}

void CombFilt::GetFiltVar(float fg_ent, float ftau_ent,float fFs)
{
    g= fg_ent;
    tau = static_cast<int>(ftau_ent*fFs);
}

void CombFilt::FIRCombFilt(float **ppfAudioData,float **ppfFiltAudio,int iNumChannels ,int iInFileLength)
{   float *pDelayLine=0;
    
    pDelayLine= *new float *[tau];
    
    for (int i=0; i < iNumChannels;i++){
        for (int j=0; j<iInFileLength; j++) {
            ppfFiltAudio[i][j] = ppfAudioData[i][j] + g * pDelayLine[9];
            std::rotate(&pDelayLine[0], &pDelayLine[9], &pDelayLine[10]);
            pDelayLine[0]=ppfAudioData[i][j];
            
        }
        memset(&pDelayLine, 0, sizeof(pDelayLine));
    }
    
    
}

void CombFilt::IIRCombFilt(float **ppfAudioData,float **ppfFiltAudio,int iNumChannels ,int iInFileLength)
{   float *pDelayLine=0;
    
    pDelayLine= *new float *[tau];
    
    for (int i=0; i < iNumChannels;i++){
        for (int j=0; j<iInFileLength; j++) {
            ppfFiltAudio[i][j] = ppfAudioData[i][j] + g * pDelayLine[9];
            std::rotate(&pDelayLine[0], &pDelayLine[9], &pDelayLine[10]);
            pDelayLine[0]=ppfFiltAudio[i][j];
            
        }
        memset(&pDelayLine, 0, sizeof(pDelayLine));
    }
    
    
}





