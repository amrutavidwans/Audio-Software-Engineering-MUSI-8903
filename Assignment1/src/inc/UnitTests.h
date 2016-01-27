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

/////////////////////////////////////////////////////////////////////////////
// Test 1 FIR filter output zero with match in feedforward delay line frequency

void FIRzeroOutputTest()
{
    // parameters
    static const int kBlockSize = 1024;
    c_sinewave  *pcsine = 0;
    int iSampleRateInHz = 44100;
    int iFsineInHz = 4410;
    int iNumChannels = 1;
    float SineAmp = 0.5;
    float SineDur = 1;
    int SamplesInSine = (iSampleRateInHz* SineDur);
    bool flag = 1;
    float g = -0.998;   // g = -1 makes a few samples in the signal to go haywire
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
    
    // debug: write the filtered sine wav, open in matlab and compare with original sine wave
    /*std::string sInputFile = "sinewave.txt";
    std::ofstream inputFile(sInputFile);
    if (iNumChannels>1){
        for (int j=0; j<SamplesInSine; j++) {
            inputFile << sineval[iNumChannels-2][j]<<"\t"<< sineval[iNumChannels-1][j] << std::endl;
        }
    }
    else{
        for (int j=0; j<SamplesInSine; j++) {
            inputFile << sineval[iNumChannels-1][j] << std::endl;
        }
        
    }
    inputFile.close();*/
    
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
    
    // debug: write the filtered sine wav, open in matlab and compare with original sine wave
    /*std::string sOutputFilter = "sinewave_filter.txt";
    std::ofstream outputFilter(sOutputFilter);
    if (iNumChannels>1){
        for (int j=0; j<SamplesInSine; j++) {
            outputFilter << OutputSig[iNumChannels-2][j]<<"\t"<< OutputSig[iNumChannels-1][j] << std::endl;
        }
    }
    else{
        for (int j=0; j<SamplesInSine; j++) {
            outputFilter << OutputSig[iNumChannels-1][j] << std::endl;
        }
        
    }
    outputFilter.close();*/
    
    
    //////////////////////////////////////////////////////////////////////////////
    // check for v low output compared to original sine wave
    float *SumOrigSine = 0;
    SumOrigSine = new float;
    pcsine->GetSineSum(sineval, SumOrigSine, iSampleRateInHz);
    
    float *SumFiltSine = 0;
    SumFiltSine = new float;
    pcsine->GetSineSum(OutputSig, SumFiltSine, iSampleRateInHz);
    
    float CompareVal = 0.003;
    float RatioOpIp=(*SumFiltSine)/(*SumOrigSine);
    
    if (RatioOpIp<CompareVal)
        flag=1;
    else flag =0;
    
    ///////////////////////////////////////////////////
    // output test result
    std::cout << "Test 1: FIR filter output zero with match in feedforward delay line frequency" << std::endl;
    if (flag) {
        std::cout << "Test 1: Passed !" << std::endl;
    }
    else
        std::cout << "Test 1: Failed !" << std::endl;
    std::cout<<std::endl;
    
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
    delete [] SumFiltSine;
    delete []SumOrigSine;
    
}

///////////////////////////////////////////////////////////////////////////////////
// Test 2 IIR: filter magnitude increase/decrease if input frequency matches feedback
void IIRfreqMatchTest(){
    
    // parameters
    static const int kBlockSize = 1024;
    c_sinewave  *pcsine = 0;
    int iSampleRateInHz = 44100;
    int iFsineInHz = 4410;
    int iNumChannels = 1;
    float SineAmp = 0.5;
    float SineDur = 1;
    int SamplesInSine = (iSampleRateInHz* SineDur);
    bool flag = 1;
    float g = -0.7;   // g = -1 makes a few samples in the signal to go haywire
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
    
    // debug: write the filtered sine wav, open in matlab and compare with original sine wave
     std::string sInputFile = "sinewave.txt";
     std::ofstream inputFile(sInputFile);
     if (iNumChannels>1){
     for (int j=0; j<SamplesInSine; j++) {
     inputFile << sineval[iNumChannels-2][j]<<"\t"<< sineval[iNumChannels-1][j] << std::endl;
     }
     }
     else{
     for (int j=0; j<SamplesInSine; j++) {
     inputFile << sineval[iNumChannels-1][j] << std::endl;
     }
     
     }
     inputFile.close();
    
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

    // IIR comb filtering
    objFilter->IIRCombFilt(sineval, OutputSig, iNumChannels, SamplesInSine);
    
    // debug: write the filtered sine wav, open in matlab and compare with original sine wave
    std::string sOutputFilter = "sinewave_filter.txt";
     std::ofstream outputFilter(sOutputFilter);
     if (iNumChannels>1){
     for (int j=0; j<SamplesInSine; j++) {
     outputFilter << OutputSig[iNumChannels-2][j]<<"\t"<< OutputSig[iNumChannels-1][j] << std::endl;
     }
     }
     else{
     for (int j=0; j<SamplesInSine; j++) {
     outputFilter << OutputSig[iNumChannels-1][j] << std::endl;
     }
     
     }
     outputFilter.close();
    
    //////////////////////////////////////////////////////////////////////////////
    // check for v low output compared to original sine wave
    float *SumOrigSine = 0;
    SumOrigSine = new float;
    pcsine->GetSineSum(sineval, SumOrigSine, iSampleRateInHz);
    
    float *SumFiltSine = 0;
    SumFiltSine = new float;
    pcsine->GetSineSum(OutputSig, SumFiltSine, iSampleRateInHz);
    
    float CompareVal = 0.5;
    float RatioOpIp=(*SumFiltSine)/(*SumOrigSine);
    
    if (RatioOpIp<CompareVal)
        flag=1;
    else flag =0;
    
    ///////////////////////////////////////////////////
    // output test result
    std::cout << "Test 2: IIR filter magnitude increase/decrease if input frequency matches feedback" << std::endl;
    if (flag) {
        std::cout << "Test 2: Passed !" << std::endl;
    }
    else
        std::cout << "Test 2: Failed !" << std::endl;
    std::cout<<std::endl;
    
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
    delete [] SumFiltSine;
    delete []SumOrigSine;
    
}


///////////////////////////////////////////////////////////////////////////////////

#endif /* UnitTests_h */