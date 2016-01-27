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
    //static const int kBlockSize = 1024;
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
        OutputSig[i] = new float[SamplesInSine];
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
    //static const int kBlockSize = 1024;
    c_sinewave  *pcsine = 0;
    int iSampleRateInHz = 44100;
    int iFsineInHz = 4410;
    int iNumChannels = 1;
    float SineAmp = 0.5;
    float SineDur = 1;
    int SamplesInSine = (iSampleRateInHz* SineDur);
    bool flag = 1;
    float g = 0.75;
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
        OutputSig[i] = new float[SamplesInSine];
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
    
    float CompareVal = 1;
    float RatioOpIp=(*SumFiltSine)/(*SumOrigSine);
    
    if (RatioOpIp>CompareVal)
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

void zeroInputSignalTest(){
    // parameters
    //static const int kBlockSize = 1024;
    c_sinewave  *pcsine = 0;
    int iSampleRateInHz = 44100;
    int iFsineInHz = 0;
    int iNumChannels = 1;
    float SineAmp = 0.5;
    float SineDur = 1;
    int SamplesInSine = (iSampleRateInHz* SineDur);
    //bool flag = 1;
    float g = -0.7;   // g = -1 makes a few samples in the signal to go haywire
    float ftau = 0.00022;
    
    // initialize a zero signal
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
    float **OutputSigIIR = 0;
    OutputSigIIR = new float*[iNumChannels];
    for ( int i = 0; i<iNumChannels; i++) {
        OutputSigIIR[i] = new float[SamplesInSine];
    }

    float **OutputSigFIR = 0;
    OutputSigFIR = new float*[iNumChannels];
    for ( int i = 0; i<iNumChannels; i++) {
        OutputSigFIR[i] = new float[SamplesInSine];
    }
    
    // object for combfilter
    CombFilt *objFilter=0;
    CombFilt::create(objFilter);
    objFilter->GetFiltVar(g, ftau, iSampleRateInHz);   // set filter values to those entered by user
    objFilter->createBuffer(iNumChannels);
    objFilter->clearBufer(iNumChannels);
    
    // IIR & FIR comb filtering
    objFilter->IIRCombFilt(sineval, OutputSigIIR, iNumChannels, SamplesInSine);
    objFilter->FIRCombFilt(sineval, OutputSigFIR, iNumChannels, SamplesInSine);
    
    // debug: write the filtered sine wav, open in matlab and compare with original sine wave
     std::string sOutputFilter = "sinewave_filter.txt";
     std::ofstream outputFilter(sOutputFilter);
     if (iNumChannels>1){
     for (int j=0; j<SamplesInSine; j++) {
     outputFilter << OutputSigIIR[iNumChannels-2][j]<<"\t"<< OutputSigIIR[iNumChannels-1][j] << std::endl;
     }
     }
     else{
     for (int j=0; j<SamplesInSine; j++) {
     outputFilter << OutputSigIIR[iNumChannels-1][j] << std::endl;
     }
     
     }
     outputFilter.close();
    
    //////////////////////////////////////////////////////////////////////////////
    // check for v low output compared to original sine wave
    float *SumOrigSine = 0;
    SumOrigSine = new float;
    pcsine->GetSineSum(sineval, SumOrigSine, iSampleRateInHz);
    std::cout << *SumOrigSine <<std::endl;
    
    float *SumFiltSineIIR = 0;
    SumFiltSineIIR = new float;
    pcsine->GetSineSum(OutputSigIIR, SumFiltSineIIR, iSampleRateInHz);
    std::cout<< *SumFiltSineIIR << std::endl;
    
    float *SumFiltSineFIR = 0;
    SumFiltSineFIR = new float;
    pcsine->GetSineSum(OutputSigFIR, SumFiltSineFIR, iSampleRateInHz);
    std::cout << *SumFiltSineIIR << std::endl;
    
    ///////////////////////////////////////////////////
    // Test condition and output test result
    
    if ((*SumFiltSineIIR) == 0)
    {
        std::cout << "Test 3: IIR filter: Zero output for zero input" << std::endl;
    
        std::cout << "Test 3: Passed !" << std::endl;
    }
    else
        std::cout << "Test 3: IIR Failed to give zero output !" << std::endl;
    
    
    if ((*SumFiltSineFIR) == 0)
    {
        std::cout << "Test 3: FIR filter: Zero output for zero input" << std::endl;
        
        std::cout << "Test 3: Passed !" << std::endl;
    }
    else
        std::cout << "Test 3: FIR Failed to give zero output !" << std::endl;
    
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
        delete [] OutputSigIIR[i];
    delete [] OutputSigIIR;
    OutputSigIIR = 0;
    
    CombFilt::destroy(objFilter);
    for (int i=0; i<iNumChannels;i++)
        delete [] OutputSigFIR[i];
    delete [] OutputSigFIR;
    OutputSigFIR = 0;
    
    
    pcsine->destroy(pcsine);
    delete [] SumFiltSineIIR;
    delete [] SumFiltSineFIR;
    delete []SumOrigSine;
    
    
}


///////////////////////////////////////////////////////////////////////////////////

void InputBlockSizeTest(){
    // parameters
    static const int kBlockSize = 1024;
    static const int kBlockSize2= 2048;
    static const int kBlockSizeby2 = 512;
    c_sinewave  *pcsine = 0;
    int iSampleRateInHz = 44100;
    int iFsineInHz = 4410;
    int iNumChannels = 1;
    float SineAmp = 0.5;
    float SineDur = 1;
    int SamplesInSine = (iSampleRateInHz* SineDur);
    bool flag = 1;
    float g = -1;   // g = -1 makes a few samples in the signal to go haywire
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
     inputFile.close();
     */
    //////////////////////////////////////////////////////////////////////////////
    // Output array for BlockSize=1024
    float **OutputSig1 = 0;
    OutputSig1 = new float*[iNumChannels];
    for ( int i = 0; i<iNumChannels; i++) {
        OutputSig1[i] = new float[kBlockSize];
    }
    
    //Output array for BlockSize=2048
    float **OutputSig2 = 0;
    OutputSig2 = new float*[iNumChannels];
    for ( int i = 0; i<iNumChannels; i++) {
        OutputSig2[i] = new float[kBlockSize2];
    }
    
    
    //Output array for BlockSize=512
    float **OutputSigby2 = 0;
    OutputSigby2 = new float*[iNumChannels];
    for ( int i = 0; i<iNumChannels; i++) {
        OutputSigby2[i] = new float[kBlockSizeby2];
    }
    
    
    // object for combfilter
    CombFilt *objFilter=0;
    CombFilt *objFilter2=0;
    CombFilt *objFilterby2=0;
    CombFilt::create(objFilter);
    CombFilt::create(objFilter2);
    CombFilt::create(objFilterby2);
    
    objFilter->GetFiltVar(g, ftau, iSampleRateInHz);
    objFilter->createBuffer(iNumChannels);
    objFilter->clearBufer(iNumChannels);
    
    objFilter2->GetFiltVar(g, ftau, iSampleRateInHz);
    objFilter2->createBuffer(iNumChannels);
    objFilter2->clearBufer(iNumChannels);
    
    objFilterby2->GetFiltVar(g, ftau, iSampleRateInHz);
    objFilterby2->createBuffer(iNumChannels);
    objFilterby2->clearBufer(iNumChannels);
    
    // IIR comb filtering for blocksize = 1024
    for (int i=0;i<SamplesInSine; i = i + kBlockSize )
    {
        long long iNumFrames = SamplesInSine/kBlockSize;
        
        if (iNumFrames < kBlockSize)
            std::cout << "Checking last frame" << std::endl;
        
        objFilter->IIRCombFilt(sineval, OutputSig1, iNumChannels, kBlockSize);
    }
    
    
    
    // IIR comb filtering for blocksize = 2048
    for (int i=0;i<SamplesInSine; i = i + kBlockSize2 )
    {
        long long iNumFrames = SamplesInSine/kBlockSize2;
        
        if (iNumFrames < kBlockSize2)
            std::cout << "Checking last frame" << std::endl;
        
        objFilter->IIRCombFilt(sineval, OutputSig2, iNumChannels, kBlockSize2);
    }
    
    // IIR comb filtering for blocksize = 512
    for (int i=0;i<SamplesInSine; i = i + kBlockSizeby2 )
    {
        long long iNumFrames = SamplesInSine/kBlockSizeby2;
        
        if (iNumFrames < kBlockSizeby2)
            std::cout << "Checking last frame" << std::endl;
        
        objFilter->IIRCombFilt(sineval, OutputSigby2, iNumChannels, kBlockSizeby2);
    }
    
    // debug: write the filtered sine wav, open in matlab and compare with original sine wave
    std::string sOutputFilter = "sinewave_filter.txt";
    std::string sOutputFilter2 = "sinewave_filter2.txt";
    std::string sOutputFilterby2 = "sinewave_filterby2.txt";
    std::ofstream outputFilter(sOutputFilter);
    std::ofstream outputFilter2(sOutputFilter);
    std::ofstream outputFilterby2(sOutputFilter);
    
    
    
    
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
        delete [] OutputSig1[i];
    delete [] OutputSig1;
    OutputSig1 = 0;
    
    pcsine->destroy(pcsine);
    
    
}


#endif /* UnitTests_h */