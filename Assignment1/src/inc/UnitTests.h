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
    objFilter->clearBufer(iNumChannels);
    
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
    //std::cout << *SumOrigSine <<std::endl;
    
    float *SumFiltSineIIR = 0;
    SumFiltSineIIR = new float;
    pcsine->GetSineSum(OutputSigIIR, SumFiltSineIIR, iSampleRateInHz);
    //std::cout<< *SumFiltSineIIR << std::endl;
    
    float *SumFiltSineFIR = 0;
    SumFiltSineFIR = new float;
    pcsine->GetSineSum(OutputSigFIR, SumFiltSineFIR, iSampleRateInHz);
    //std::cout << *SumFiltSineIIR << std::endl;
    
    ///////////////////////////////////////////////////
    // Test condition and output test result
    
    if ((*SumFiltSineIIR) == 0)
    {
        std::cout << "Test 3a: IIR filter: Zero output for zero input" << std::endl;
    
        std::cout << "Test 3a: Passed !" << std::endl;
    }
    else
        std::cout << "Test 3a: IIR Failed to give zero output !" << std::endl;
    
    
    if ((*SumFiltSineFIR) == 0)
    {
        std::cout << "Test 3b: FIR filter: Zero output for zero input" << std::endl;
        
        std::cout << "Test 3b: Passed !" << std::endl;
    }
    else
        std::cout << "Test 3b: FIR Failed to give zero output !" << std::endl;
    
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
    int iFsineInHz = 441;
    int iNumChannels = 1;
    float SineAmp = 0.5;
    float SineDur = 1;
    int SamplesInSine = (iSampleRateInHz* SineDur);
    //bool flag = 1;
    float g = -0.5;   // g = -1 makes a few samples in the signal to go haywire
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
    
    //////////////////////////////////////////////////////////////////////////////
    //// objects IIR
    // Output array for BlockSize=1024
    float **OutputSig1IIR = 0;
    OutputSig1IIR = new float*[iNumChannels];
    for ( int i = 0; i<iNumChannels; i++) {
        OutputSig1IIR[i] = new float[SamplesInSine];
    }
    
    //Output array for BlockSize=2048
    float **OutputSig2IIR = 0;
    OutputSig2IIR = new float*[iNumChannels];
    for ( int i = 0; i<iNumChannels; i++) {
        OutputSig2IIR[i] = new float[SamplesInSine];
    }
    
    //Output array for BlockSize=512
    float **OutputSigby2IIR = 0;
    OutputSigby2IIR = new float*[iNumChannels];
    for ( int i = 0; i<iNumChannels; i++) {
        OutputSigby2IIR[i] = new float[SamplesInSine];
    }
    
    ////////////////////FIR objects
    float **OutputSig1FIR = 0;
    OutputSig1FIR = new float*[iNumChannels];
    for ( int i = 0; i<iNumChannels; i++) {
        OutputSig1FIR[i] = new float[SamplesInSine];
    }
    
    //Output array for BlockSize=2048
    float **OutputSig2FIR = 0;
    OutputSig2FIR = new float*[iNumChannels];
    for ( int i = 0; i<iNumChannels; i++) {
        OutputSig2FIR[i] = new float[SamplesInSine];
    }
    
    //Output array for BlockSize=512
    float **OutputSigby2FIR = 0;
    OutputSigby2FIR = new float*[iNumChannels];
    for ( int i = 0; i<iNumChannels; i++) {
        OutputSigby2FIR[i] = new float[SamplesInSine];
    }
    
    //Output array temporary
    float **TempArr1 = 0;
    TempArr1 = new float*[iNumChannels];
    for ( int i = 0; i<iNumChannels; i++) {
        TempArr1[i] = new float[kBlockSize];
    }
    
    //Output array temporary
    float **TempArr2 = 0;
    TempArr2 = new float*[iNumChannels];
    for ( int i = 0; i<iNumChannels; i++) {
        TempArr2[i] = new float[kBlockSize2];
    }
    
    //Output array temporary
    float **TempArrby2 = 0;
    TempArrby2 = new float*[iNumChannels];
    for ( int i = 0; i<iNumChannels; i++) {
        TempArrby2[i] = new float[kBlockSizeby2];
    }

    //Output array temporary
    float **TempArrOP1 = 0;
    TempArrOP1 = new float*[iNumChannels];
    for ( int i = 0; i<iNumChannels; i++) {
        TempArrOP1[i] = new float[kBlockSize];
    }
    
    //Output array temporary
    float **TempArrOP2 = 0;
    TempArrOP2 = new float*[iNumChannels];
    for ( int i = 0; i<iNumChannels; i++) {
        TempArrOP2[i] = new float[kBlockSize2];
    }
    
    //Output array temporary
    float **TempArrbyOP2 = 0;
    TempArrbyOP2 = new float*[iNumChannels];
    for ( int i = 0; i<iNumChannels; i++) {
        TempArrbyOP2[i] = new float[kBlockSizeby2];
    }
    
    // object for combfilter
    CombFilt *objFilter=0;
    CombFilt::create(objFilter);
    
    objFilter->GetFiltVar(g, ftau, iSampleRateInHz);
    objFilter->createBuffer(iNumChannels);
    objFilter->clearBufer(iNumChannels);
    
    // FIR and IIR comb filtering for blocksize = 1024
    int chkTemp = SamplesInSine;
    int chk=0;
    while (chkTemp>0)
    {
        long long iNumFrames = kBlockSize;

        if (chkTemp < kBlockSize)
            iNumFrames = chkTemp;
        
        for (int i=0; i<iNumChannels; i++)
        {
            for (int j=0; j<iNumFrames; j++) {
                TempArr1[i][j]=sineval[i][iNumFrames*chk+j];
                //std::cout << TempArr1[i][j] << std::endl;
            }
        }
        
        objFilter->FIRCombFilt(TempArr1, TempArrOP1, iNumChannels, iNumFrames);
        
        for (int i=0; i<iNumChannels; i++)
        {
            for (int j=0; j<iNumFrames; j++) {
                OutputSig1FIR[i][iNumFrames*chk+j]=TempArrOP1[i][j];
                //std::cout << OutputSig1FIR[i][iNumFrames*chk+j] << std::endl;
            }
        }
        /*objFilter->IIRCombFilt(TempArr1, TempArrOP1, iNumChannels, iNumFrames);
        
        for (int i=0; i<iNumChannels; i++)
        {
            for (int j=0; j<iNumFrames; j++) {
                OutputSig1IIR[i][iNumFrames*chk+j]=TempArrOP1[i][j];
                //std::cout << OutputSig1IIR[i][iNumFrames*chk+j] << std::endl;
            }
        }*/
        
        
        chkTemp = chkTemp - kBlockSize;
        chk=chk+1;
    }
    objFilter->clearBufer(iNumChannels);
    
    
    chkTemp = SamplesInSine;
    chk=0;
    while (chkTemp>0)
    {
        long long iNumFrames = kBlockSize;
        
        if (chkTemp < kBlockSize)
            iNumFrames = chkTemp;
        
        for (int i=0; i<iNumChannels; i++)
        {
            for (int j=0; j<iNumFrames; j++) {
                TempArr1[i][j]=sineval[i][iNumFrames*chk+j];
                //std::cout << TempArr1[i][j] << std::endl;
            }
        }
        
         objFilter->IIRCombFilt(TempArr1, TempArrOP1, iNumChannels, iNumFrames);
         
         for (int i=0; i<iNumChannels; i++)
         {
         for (int j=0; j<iNumFrames; j++) {
         OutputSig1IIR[i][iNumFrames*chk+j]=TempArrOP1[i][j];
         //std::cout << OutputSig1IIR[i][iNumFrames*chk+j] << std::endl;
         }
         }
        
        chkTemp = chkTemp - kBlockSize;
        chk=chk+1;
    }
    objFilter->clearBufer(iNumChannels);
    
    // FIR and IIR comb filtering for blocksize = 2048

    chkTemp = SamplesInSine;
    chk=0;
    while (chkTemp>0)
    {
        long long iNumFrames = kBlockSize2;
        
        if (chkTemp < kBlockSize2)
            iNumFrames = chkTemp;
        
        for (int i=0; i<iNumChannels; i++)
        {
            for (int j=0; j<iNumFrames; j++)
                TempArr2[i][j]=sineval[i][iNumFrames*chk+j];
        }
        
        objFilter->FIRCombFilt(TempArr2, TempArrOP2, iNumChannels, iNumFrames);
        
        for (int i=0; i<iNumChannels; i++)
        {
            for (int j=0; j<iNumFrames; j++)
                OutputSig2FIR[i][iNumFrames*chk+j]=TempArrOP2[i][j];
        }
        
        /*objFilter->IIRCombFilt(TempArr2, TempArrOP2, iNumChannels, iNumFrames);
        
        for (int i=0; i<iNumChannels; i++)
        {
            for (int j=0; j<iNumFrames; j++)
                OutputSig2IIR[i][iNumFrames*chk+j]=TempArrOP2[i][j];
        }*/
        
        
        chkTemp = chkTemp - kBlockSize2;
        chk=chk+1;
    }
    objFilter->clearBufer(iNumChannels);
    /*
    for (int i = 0; i < SamplesInSine; i++) {
        std::cout << OutputSig1IIR[0][i] << " " << OutputSig2IIR[0][i] << std::endl;
    }
    */
    
    chkTemp = SamplesInSine;
    chk=0;
    while (chkTemp>0)
    {
        long long iNumFrames = kBlockSize2;
        
        if (chkTemp < kBlockSize2)
            iNumFrames = chkTemp;
        
        for (int i=0; i<iNumChannels; i++)
        {
            for (int j=0; j<iNumFrames; j++)
                TempArr2[i][j]=sineval[i][iNumFrames*chk+j];
        }
        
        
         objFilter->IIRCombFilt(TempArr2, TempArrOP2, iNumChannels, iNumFrames);
         
         for (int i=0; i<iNumChannels; i++)
         {
         for (int j=0; j<iNumFrames; j++)
         OutputSig2IIR[i][iNumFrames*chk+j]=TempArrOP2[i][j];
         }
        
        chkTemp = chkTemp - kBlockSize2;
        chk=chk+1;
    }
    objFilter->clearBufer(iNumChannels);
    
    // FIR and IIR comb filtering for blocksize = 512
    
    chkTemp = SamplesInSine;
    chk=0;
    while (chkTemp>0)
    {
        long long iNumFrames = kBlockSizeby2;
        
        if (chkTemp < kBlockSizeby2)
            iNumFrames = chkTemp;
        
        for (int i=0; i<iNumChannels; i++)
        {
            for (int j=0; j<iNumFrames; j++)
                TempArrby2[i][j]=sineval[i][iNumFrames*chk+j];
        }
        
        objFilter->FIRCombFilt(TempArrby2, TempArrbyOP2, iNumChannels, iNumFrames);
        
        for (int i=0; i<iNumChannels; i++)
        {
            for (int j=0; j<iNumFrames; j++)
                OutputSigby2FIR[i][iNumFrames*chk+j]=TempArrbyOP2[i][j];
        }
        
        /*objFilter->IIRCombFilt(TempArrby2, TempArrbyOP2, iNumChannels, iNumFrames);
        
        for (int i=0; i<iNumChannels; i++)
        {
            for (int j=0; j<iNumFrames; j++)
                OutputSigby2IIR[i][iNumFrames*chk+j]=TempArrbyOP2[i][j];
        }*/
        
        
        chkTemp = chkTemp - kBlockSizeby2;
        chk=chk+1;
    }
    
    objFilter->clearBufer(iNumChannels);
    
    chkTemp = SamplesInSine;
    chk=0;
    while (chkTemp>0)
    {
        long long iNumFrames = kBlockSizeby2;
        
        if (chkTemp < kBlockSizeby2)
            iNumFrames = chkTemp;
        
        for (int i=0; i<iNumChannels; i++)
        {
            for (int j=0; j<iNumFrames; j++)
                TempArrby2[i][j]=sineval[i][iNumFrames*chk+j];
        }
        
        objFilter->IIRCombFilt(TempArrby2, TempArrbyOP2, iNumChannels, iNumFrames);
         
         for (int i=0; i<iNumChannels; i++)
         {
         for (int j=0; j<iNumFrames; j++)
         OutputSigby2IIR[i][iNumFrames*chk+j]=TempArrbyOP2[i][j];
         }
        
        chkTemp = chkTemp - kBlockSizeby2;
        chk=chk+1;
    }
    
    objFilter->clearBufer(iNumChannels);
    
//    // debug: write the filtered sine wav, open in matlab and compare with original sine wave
//    std::string sOutputFilter = "sinewave_filter.txt";
//    std::string sOutputFilter2 = "sinewave_filter2.txt";
//    std::string sOutputFilterby2 = "sinewave_filterby2.txt";
//    std::ofstream outputFilter(sOutputFilter);
//    std::ofstream outputFilter2(sOutputFilter);
//    std::ofstream outputFilterby2(sOutputFilter);
    
    ///////////////////////////////////////////////////
    // output test result
    
    float *SumFiltSine1IIR = 0;
    SumFiltSine1IIR = new float;
    pcsine->GetSineSum(OutputSig1IIR, SumFiltSine1IIR, iSampleRateInHz);
    //std::cout<< *SumFiltSine1IIR << std::endl;
    
    float *SumFiltSine1FIR = 0;
    SumFiltSine1FIR = new float;
    pcsine->GetSineSum(OutputSig1FIR, SumFiltSine1FIR, iSampleRateInHz);
    //std::cout << *SumFiltSine1FIR << std::endl;
    
    float *SumFiltSine2IIR = 0;
    SumFiltSine2IIR = new float;
    pcsine->GetSineSum(OutputSig2IIR, SumFiltSine2IIR, iSampleRateInHz);
    //std::cout<< *SumFiltSine2IIR << std::endl;
    
    float *SumFiltSine2FIR = 0;
    SumFiltSine2FIR = new float;
    pcsine->GetSineSum(OutputSig2FIR, SumFiltSine2FIR, iSampleRateInHz);
    //std::cout << *SumFiltSine2FIR << std::endl;
    
    float *SumFiltSineby2IIR = 0;
    SumFiltSineby2IIR = new float;
    pcsine->GetSineSum(OutputSigby2IIR, SumFiltSineby2IIR, iSampleRateInHz);
    //std::cout<< *SumFiltSineby2IIR << std::endl;
    
    float *SumFiltSineby2FIR = 0;
    SumFiltSineby2FIR = new float;
    pcsine->GetSineSum(OutputSigby2FIR, SumFiltSineby2FIR, iSampleRateInHz);
    //std::cout << *SumFiltSineby2FIR << std::endl;
    
    ////////////print test results
    float thresh =400;
    std::cout << "Test 4a: IIR Filter: Different block length, same output" << std::endl;
    if (((*SumFiltSineby2IIR) - (*SumFiltSine2IIR))<thresh && ((*SumFiltSine2IIR) - (*SumFiltSine1IIR))< thresh) {
        std::cout << "Test 4a: Passed !" << std::endl;
    }
    else
        std::cout << "Test 4a: Failed !" << std::endl;
    
    std::cout << "Test 4b: FIR Filter: Different block length, same output" << std::endl;
    if (((*SumFiltSineby2FIR) - (*SumFiltSine2FIR))<thresh && ((*SumFiltSine2FIR) - (*SumFiltSine1FIR))< thresh) {
        std::cout << "Test 4b: Passed !" << std::endl;
    }
    else
        std::cout << "Test 4b: Failed !" << std::endl;
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
        delete [] OutputSig1IIR[i];
    delete [] OutputSig1IIR;
    OutputSig1IIR = 0;
    
    for (int i=0; i<iNumChannels;i++)
        delete [] OutputSig1FIR[i];
    delete [] OutputSig1FIR;
    OutputSig1FIR = 0;
    
    for (int i=0; i<iNumChannels;i++)
        delete [] OutputSig2IIR[i];
    delete [] OutputSig2IIR;
    OutputSig2IIR = 0;
    
    for (int i=0; i<iNumChannels;i++)
        delete [] OutputSig2FIR[i];
    delete [] OutputSig2FIR;
    OutputSig2FIR = 0;
    
    for (int i=0; i<iNumChannels;i++)
        delete [] OutputSigby2FIR[i];
    delete [] OutputSigby2FIR;
    OutputSigby2FIR = 0;
    
    for (int i=0; i<iNumChannels;i++)
        delete [] OutputSigby2IIR[i];
    delete [] OutputSigby2IIR;
    OutputSigby2IIR = 0;
    
    pcsine->destroy(pcsine);
    
    
}

void VaryingSamplingRateTest(){
    c_sinewave  *pcsine = 0;
    int iSampleRateInHz = 16000;
    int iFsineInHz = 1600;
    int iNumChannels = 1;
    float SineAmp = 0.5;
    float SineDur = 1;
    bool flag= 1;
  
    float gFIR = -0.9; // g = -1 makes a few samples in the signal to go haywire
    float gIIR = 0.75;
    float ftau = 0.000625;
    
    ///////////////////////////////////////////////////////////////////////////////////
    //////Testing with known sine wave
    pcsine->create(pcsine);
    pcsine->SetSineWavParam(iFsineInHz, SineAmp, SineDur, iNumChannels);
    float **sineval = 0;
    //float **sinevalby2=0;
    sineval = new float *[iNumChannels];
    for (int i=0; i<iNumChannels; i++){
        sineval[i] =new float[static_cast<int>(SineDur*iSampleRateInHz)];
    }
    pcsine->GetSineWave(sineval,iSampleRateInHz);

    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Output array for Sample Rate=44100,FIR
    float **OutputSig1 = 0;
    OutputSig1 = new float*[iNumChannels];
    for ( int i = 0; i<iNumChannels; i++) {
        OutputSig1[i] = new float[static_cast<int>(SineDur*iSampleRateInHz)];
    }
    
    //Output array for Sample Rate=22050,FIR
    float **OutputSig2 = 0;
    OutputSig2 = new float*[iNumChannels];
    for ( int i = 0; i<iNumChannels; i++) {
        OutputSig2[i] = new float[static_cast<int>(SineDur*iSampleRateInHz)];
    }

    
    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///Combfilter 1
    CombFilt *objFilter=0;
    CombFilt::create(objFilter);
    objFilter->GetFiltVar(gIIR, ftau, iSampleRateInHz);   // set filter values to those entered by user
    objFilter->createBuffer(iNumChannels);
    objFilter->clearBufer(iNumChannels);
    
    ///////////////////////////////////////////////////////////
    CombFilt *objFilter2=0;
    CombFilt::create(objFilter2);
    objFilter2->GetFiltVar(gFIR, ftau, iSampleRateInHz);   // set filter values to those entered by user
    objFilter2->createBuffer(iNumChannels);
    objFilter2->clearBufer(iNumChannels);
    
    
    /////////////////////////////////////////////////////////////////////////////////////////////////
    
    objFilter->IIRCombFilt(sineval, OutputSig1, iNumChannels,static_cast<int>(SineDur*iSampleRateInHz));
    objFilter->clearBufer(iNumChannels);
    objFilter2->FIRCombFilt(sineval, OutputSig2, iNumChannels,static_cast<int>(SineDur*iSampleRateInHz));
    objFilter2->clearBufer(iNumChannels);
    //////////////////////////////////////////////////////////////////////////////
    ////FIR STUFF
    // check for v low output compared to original sine wave
    float *SumOrigSine = 0;
    SumOrigSine = new float;
    pcsine->GetSineSum(sineval, SumOrigSine, iSampleRateInHz);
    
    float *SumFiltSine = 0;
    SumFiltSine = new float;
    pcsine->GetSineSum(OutputSig2, SumFiltSine, iSampleRateInHz);
    
    float CompareVal = 0.2;
    float RatioOpIp=(*SumFiltSine)/ (*SumOrigSine);
    
    if (RatioOpIp<CompareVal)
        flag=1;
    else flag =0;
    
    ///////////////////////////////////////////////////
    // output test result
    std::cout << "Test 5a: FIR filter output zero with match in feedforward delay line frequency for different frequency" << std::endl;
    if (flag) {
        std::cout << "Test 1: Passed !" << std::endl;
    }
    else
        std::cout << "Test 1: Failed !" << std::endl;
    std::cout<<std::endl;
    
    /////////////////////////////////////////////////
    /////IIR STUFF
    flag=1;
    float *SumOrigSine2 = 0;
    SumOrigSine2 = new float;
    pcsine->GetSineSum(sineval, SumOrigSine2, iSampleRateInHz);
    
    float *SumFiltSine2 = 0;
    SumFiltSine2 = new float;
    pcsine->GetSineSum(OutputSig1, SumFiltSine2, iSampleRateInHz);
    
    float CompareVal2 = 0.5;
    float RatioOpIp2=(*SumFiltSine2)/(*SumOrigSine2);
    
    if (RatioOpIp2>CompareVal2)
        flag=1;
    else flag =0;
    
    ///////////////////////////////////////////////////
    // output test result
    std::cout << "Test 5b: IIR filter magnitude increase/decrease if input frequency matches feedback for different frequency" << std::endl;
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
    
    for (int i=0; i<iNumChannels;i++)
        delete [] OutputSig2[i];
    delete [] OutputSig2;
    OutputSig2 = 0;
    
    pcsine->destroy(pcsine);
    delete [] SumFiltSine;
    delete []SumOrigSine;
    

    
    }

#endif /* UnitTests_h */