//
//  UnitTests.h
//  MUSI8903
//
//  Created by Milap Rane on 1/26/16.
//
//

#ifndef UnitTests_h
#define UnitTests_h


#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <algorithm>


#include "MUSI8903Config.h"

#include "AudioFileIf.h"
#include "CombFilt.h"
#include "c_sinewave.h"

/*class UnitTests{

public:
    void FIRzeroOutputTest();
    void IIRfreqMatchTest();
    void InputBlockSizeTest();
    void zeroInputSignalTest();
    void VaryingSamplingRateTest();
    void VaryingNumChannelsTest();
};
 */


void FIRzeroOutputTest()
{
    static const int kBlockSize = 1024;
    c_sinewave  *pcsine = 0;
    int iSampleRateInHz = 44100;
    int iFsineInHz = 4410;
    int iNumChannels = 1;
    float SineAmp = 0.5;
    float SineDur = 2;
    int SamplesInSine = (iSampleRateInHz* SineDur);
    bool flag = 1;
    float g = -1;
    float ftau = 0.00022;
    
    
    ///////////////////////////////////////////////////////////////////////////////////
    //////Testing with known sine wave
    pcsine->create(pcsine);
    pcsine->SetSineWavParam(iFsineInHz, SineAmp, SineDur, iNumChannels);
    float **sineval = 0;
    sineval = new float *[iNumChannels];
    for (int i=0; i<iNumChannels; i++){
      sineval[i] =new float[SamplesInSine];
    }
    pcsine->GetSineWave(sineval,iSampleRateInHz);
    
    std::cout<< **sineval << std::endl;
    //////////////////////////////////////////////////////////////////////////////
    // Output array
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
    objFilter->clearBufer(iNumChannels);
    
    // FIR comb filtering
    objFilter->FIRCombFilt(sineval, OutputSig, iNumChannels, SamplesInSine);
    
    
    //////////////////////////////////////////////////////////////////////////////
    // check for zero output
    float *SumOrigSine = 0;
    SumOrigSine = new float;
    pcsine->GetSineSum(OutputSig, SumOrigSine, iSampleRateInHz);
    float CompareVal = 0.00001;
    
    if ((*SumOrigSine)<CompareVal)
        flag=1;
    else flag =0;
    
    ///////////////////////////////////////////////////
    // output test result
    if (flag) {
        std::cout << "Test zero output FIR passed !" << std::endl;
    }
    else
        std::cout << "Test zero output FIR failed !" << std::endl;
    
    //////////////////////////////////////////////////////////////////////////////
    // free the memory
    for (int i=0; i<iNumChannels;i++)
        delete [] sineval[i];
    delete [] sineval;
    sineval = 0;
    
    objFilter->destroyBuffer(iNumChannels);
    
    CombFilt::destroy(objFilter);
    for (int i=0; i<iNumChannels;i++)
        delete [] OutputSig[i];
    delete [] OutputSig;
    OutputSig = 0;
    
    pcsine->destroy(pcsine);
    
}






#endif /* UnitTests_h */