//
//  UnitTests.h
//  MUSI8903
//
//  Created by Milap Rane on 1/26/16.
//
//

#ifndef UnitTests_h
#define UnitTests_h
#endif /* UnitTests_h */

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <algorithm>


#include "MUSI8903Config.h"

#include "AudioFileIf.h"
#include "CombFilt.h"
#include "c_sinewave.h"



static const int        kBlockSize          = 1024;
c_sinewave  *pcsine             = 0   ;
int iSampleRateInHz = 44100;
int iFsineInHz = 4410;
int iNumChannels =2;


///////////////////////////////////////////////////////////////////////////////////
//////Testing with known sine wave

pcsine->create(pcsine);
pcsine->SetSineWavParam(iFsineInHz, 0.5, 2);
float *sineval =new float[static_cast<int>(fileSpecs.fSampleRateInHz* 2)];
pcsine->GetSineWave(sineval,fileSpecs.fSampleRateInHz);
for (int i=0; i<(fileSpecs.fSampleRateInHz* 2); i++) {
    cout<< sineval[i] << endl;
    }
    
    pcsine->destroy(pcsine);


//////////////////////////////////////////////////////////////////////////////
// Comb filter object created
float **OutputSig = 0;

OutputSig = new float*[iNumChannels];
for ( int i = 0; i<iNumChannels; i++) {
    OutputSig[i] = new float[kBlockSize];
}


// object for combfilter

CombFilt *objFilter=0;
CombFilt::create(objFilter);
objFilter->GetFiltVar(g, ftau, iSampleRateInHz);   // set filter values to those entered by user
objFilter->createBuffer(iNumChannels);
int NumChannels= fileSpecs.iNumChannels;

objFilter->clearBufer(iNumChannels);

std::string sOutputFilter = sInputFilePath + "_filter.txt";
std::ofstream outputFilter(sOutputFilter);


// depending on the input call FIR or IIR
while (!phAudioFile->isEof())
{
    long long iNumFrames = kBlockSize;
    phAudioFile->readData(ppfAudioData, iNumFrames);
    if (iNumFrames < kBlockSize)
        cout << "Checking last frame" << endl;
        
        if (strncmp(argv[3], "FIR", 3))
        objFilter->FIRCombFilt(ppfAudioData, OutputSig, iNumChannels, iNumFrames);
        else
        objFilter->IIRCombFilt(ppfAudioData, OutputSig, iNumChannels, iNumFrames);
        
        // write filtered audio to txt file
        if (NumChannels>1){
            for (int j=0; j<iNumFrames; j++) {
                outputFilter << OutputSig[NumChannels-2][j]<<"\t"<< OutputSig[NumChannels-1][j] << endl;
            }
        }
        else{
            for (int j=0; j<iNumFrames; j++) {
                outputFilter << OutputSig[NumChannels-1][j] << endl;
            }
            
        }
        
    
}
        
        //close file after writing is done
        outputFilter.close();