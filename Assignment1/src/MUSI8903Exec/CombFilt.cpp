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
#include "Util.h"
#include <new>


CombFilt::CombFilt ()
{
    // this never hurts
    this->init();
}

/*CombFilt::~CombFilt()
{
    this->destroy();
}
*/

Error_t CombFilt::create (CombFilt *&pCombFilt)
{
    pCombFilt = new CombFilt;
    
    if (!pCombFilt)
        return kUnknownError;
    
    
    return kNoError;
}

Error_t CombFilt::destroy (CombFilt *&pCombFilt)
{
    if (!pCombFilt)
        return kUnknownError;
    
   // pCombFilt->reset ();
    
    delete pCombFilt;
    pCombFilt = 0;
    
    return kNoError;
    
}

Error_t CombFilt::init(){
    // allocate memory
    // initialize variables and buffers
    
    //pCombFilt = new CombFilt;
    g=0.5;     // gain from 0-1
    tau = 10;  // tau in seconds
    return kNoError;
}

/*Error_t CombFilt::reset (){
    // reset buffers and variables to default values
    
    Error_t eErr = closeFile ();
    if (eErr != kNoError)
        return eErr;
    
    if (bFreeMemory)
    {
        eErr = freeMemory ();
        if (eErr != kNoError)
            return eErr;
    }
    
    eErr = initDefaults ();
    if (eErr != kNoError)
        return eErr;
    
    return eErr;
    
    return kNoError;
}
*/

void CombFilt::GetFiltVar(float fg_ent, float ftau_ent,float fFs)
{
    g= fg_ent;
    float temp=ftau_ent*fFs;
    if (temp>=0.F)
        tau= static_cast<int>(temp+0.5F);
    else
        tau= static_cast<int>(temp-0.5F);
}

void CombFilt::createBuffer(int iNumChannels){
    
    ppfDelayLine= new float *[iNumChannels];
    for (int i=0; i< iNumChannels; i++) {
        ppfDelayLine[i]=new float[tau];
    }
    
}

void CombFilt::clearBufer(int iNumChannels){
    for (int i=0; i<iNumChannels; i++){
        for(int j=0; j<tau; j++){
            ppfDelayLine[i][j]=0;
        }
    }
    
}

void CombFilt::destroyBuffer(int iNumChannels){
    for (int i=0; i<iNumChannels; i++)
        delete [] ppfDelayLine[i];
    delete [] ppfDelayLine;
    ppfDelayLine = 0;
}

void CombFilt::FIRCombFilt(float **ppfAudioData,float **ppfFiltAudio,int iNumChannels ,int iInFileLength)
{
    // handle range of values for g
    
    for (int i=0; i < iNumChannels;i++){
        for (int j=0; j<iInFileLength; j++) {
            ppfFiltAudio[i][j] = ppfAudioData[i][j] + g * ppfDelayLine[i][tau-1];
            std::rotate(&ppfDelayLine[i][0], &ppfDelayLine[i][tau-1], &ppfDelayLine[i][tau]);
            ppfDelayLine[i][0]=ppfAudioData[i][j];
            
        }
        //memset(&pDelayLine, 0, sizeof(pDelayLine));
    }
    
    
}

void CombFilt::IIRCombFilt(float **ppfAudioData,float **ppfFiltAudio,int iNumChannels ,int iInFileLength)
{
    // handle range of values for g
    
    for (int i=0; i < iNumChannels;i++){
        for (int j=0; j<iInFileLength; j++) {
            ppfFiltAudio[i][j] = ppfAudioData[i][j] + g * (ppfDelayLine[i][tau-1]);
            std::rotate(&ppfDelayLine[i][0], &ppfDelayLine[i][tau-1], &ppfDelayLine[i][tau]);
            ppfDelayLine[i][0]=ppfFiltAudio[i][j];
            
        }
        //memset(&pDelayLine, 0, sizeof(pDelayLine));
    }
    
    
}





