
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <algorithm>


#include "MUSI8903Config.h"

#include "AudioFileIf.h"
#include "CombFilt.h"

using std::cout;
using std::endl;
using std::cin;

// local function declarations
void    showClInfo ();

/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
    std::string             sInputFilePath,                 //!< file paths
                            sOutputFilePath;

    long long               iInFileLength       = 0;        //!< length of input file

    clock_t                 time                = 0;

    float                   **ppfAudioData      = 0;

    CAudioFileIf            *phAudioFile        = 0;
    
    static const int        kBlockSize          = 1024;

    showClInfo ();

    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments
    sInputFilePath = "/Users/Amruta/Documents/MS GTCMT/Sem 2/Audio Software Engineering/sinewav4410.wav";//argv[0];
    //cout << sInputFilePath.length();
    sOutputFilePath = sInputFilePath + ".txt";
    

    //////////////////////////////////////////////////////////////////////////////
    // open the input wave file
    CAudioFileIf::create (phAudioFile);
    
    Error_t  errorMessages = phAudioFile->openFile (sInputFilePath, CAudioFileIf::kFileRead);
    if(errorMessages == kFileOpenError) {
        cout<<"File open error!\n";
    }
    CAudioFileIf::FileSpec_t fileSpecs;
    phAudioFile->getFileSpec(fileSpecs);
    cout << fileSpecs.iNumChannels<<endl;
    phAudioFile->getLength (iInFileLength);
    cout << iInFileLength<<endl;
    
    //////////////////////////////////////////////////////////////////////////////
    // allocate memory
    ppfAudioData = new float*[fileSpecs.iNumChannels];
    for ( int i = 0; i<fileSpecs.iNumChannels; i++) {
        ppfAudioData[i] = new float[kBlockSize];
    }
 
    //////////////////////////////////////////////////////////////////////////////
    // do processing
    cout << "Hello there!" << endl << endl;

    //////////////////////////////////////////////////////////////////////////////
    // Comb filter object created
    
    float g = -1;
    float ftau = 0.0022;  // in seconds
//    float DelayLine[1]={};
    float **OutputSig = 0;
    
    OutputSig = new float*[fileSpecs.iNumChannels];
    for ( int i = 0; i<fileSpecs.iNumChannels; i++) {
        OutputSig[i] = new float[kBlockSize];
    }
    
    
    // object for combfilter
    
    CombFilt *objFilter=0;
    CombFilt::create(objFilter);
    objFilter->GetFiltVar(g, ftau, fileSpecs.fSampleRateInHz);
    objFilter->createBuffer(fileSpecs.iNumChannels);
    int NumChannels= fileSpecs.iNumChannels;

    objFilter->clearBufer(fileSpecs.iNumChannels);
    
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
            objFilter->FIRCombFilt(ppfAudioData, OutputSig, fileSpecs.iNumChannels, iNumFrames);
        else
            objFilter->IIRCombFilt(ppfAudioData, OutputSig, fileSpecs.iNumChannels, iNumFrames);

    
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
    
    // write filtered audio to txt file
    
    outputFilter.close();
    
    
    ///////////////////////////////////////////////
    // clean-up
    CAudioFileIf::destroy(phAudioFile);
    for (int i=0;i<fileSpecs.iNumChannels;i++){
        delete [] ppfAudioData[i];
    }
    delete [] ppfAudioData;
    ppfAudioData = 0;
    
    objFilter->destroyBuffer(fileSpecs.iNumChannels);
    CombFilt::destroy(objFilter);
    for (int i=0; i<fileSpecs.iNumChannels;i++)
        delete [] OutputSig[i];
    delete [] OutputSig;
    OutputSig = 0;

    return 0;
    
}



void     showClInfo()
{
    cout << "GTCMT MUSI8903" << endl;
    //cout << "(c) 2016 by Alexander Lerch" << endl;
    cout  << endl;

    return;
}

