
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

    showClInfo ();

    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments
    sInputFilePath = "/Users/Amruta/Documents/MS GTCMT/Sem 2/Audio Software Engineering/sinewav4410.wav";//argv[0];
    //cout << sInputFilePath.length();
    sOutputFilePath = sInputFilePath + ".txt";
    //for (int i=0; i<(sInputFilePath.length()); i++) {
    //    sOutputFilePath[i]=sInputFilePath[i];
    //}
    //sOutputFilePath = sOutputFilePath + ".txt";
    //std::cout << sInputFilePath;
    //std::cout << sOutputFilePath;

    //////////////////////////////////////////////////////////////////////////////
    // open the input wave file
    CAudioFileIf::create (phAudioFile);
    
    Error_t  errorMessages = phAudioFile->openFile (sInputFilePath, CAudioFileIf::kFileRead);//, psFileSpec_t1); //, *psFileSpec);
    if(errorMessages == kFileOpenError) {
        cout<<"File open error!\n";
    }
    CAudioFileIf::FileSpec_t fileSpecs;
    phAudioFile->getFileSpec(fileSpecs);
    cout << fileSpecs.iNumChannels<<endl;
    phAudioFile->getLength (iInFileLength);
    cout << iInFileLength;
    
    //////////////////////////////////////////////////////////////////////////////
    // allocate memory
    ppfAudioData = new float*[fileSpecs.iNumChannels];
    for ( int i = 0; i<fileSpecs.iNumChannels; i++) {
        ppfAudioData[i] = new float[iInFileLength];
    }
 
    phAudioFile->readData (ppfAudioData, iInFileLength);
    
    //print audio data
    /*for (int i = 0; i< fileSpecs.iNumChannels; i++) {
        for (int j=0; j<iInFileLength; j++) {
            cout<<ppfAudioData[i][j]<<endl;
        }
    }*/

    //////////////////////////////////////////////////////////////////////////////
    // get audio info and print it to stdout

    std::ofstream myfile(sOutputFilePath);
    
    int i= fileSpecs.iNumChannels;
    if (i>1){
    for (int j=0; j<iInFileLength; j++) {
            myfile << ppfAudioData[i-2][j]<<"\t"<< ppfAudioData[i-1][j] << endl;
        }
    }
    else{
        for (int j=0; j<iInFileLength; j++) {
            myfile << ppfAudioData[i-1][j] << endl;
        }
        
    }
    myfile.close();
    //////////////////////////////////////////////////////////////////////////////
    // do processing
    cout << "Hello there!" << endl << endl;

    //////////////////////////////////////////////////////////////////////////////
    // Comb filter object created
    
    ////////////////////////
    //FIR filter test code
    float g = -1;
    float ftau = 0.5;  // in seconds
//    float DelayLine[1]={};
    float **OutputSig = 0;
    
    OutputSig = new float*[fileSpecs.iNumChannels];
    for ( int i = 0; i<fileSpecs.iNumChannels; i++) {
        OutputSig[i] = new float[iInFileLength];
    }
    
    
    // object for combfilter
    
    CombFilt *objFilter=0;
    CombFilt::create(objFilter);
    objFilter->GetFiltVar(g, ftau, fileSpecs.fSampleRateInHz);
    
    // depending on the input call FIR or IIR
    if (strncmp(argv[3], "FIR", 3))
        objFilter->FIRCombFilt(ppfAudioData, OutputSig, fileSpecs.iNumChannels, iInFileLength);
    else
        objFilter->IIRCombFilt(ppfAudioData, OutputSig, fileSpecs.iNumChannels, iInFileLength);
    
    
    // write filtered audio to txt file
    std::string sOutputFilter = sInputFilePath + "_filter.txt";
    
    std::ofstream outputFilter(sOutputFilter);
    
    i= fileSpecs.iNumChannels;
    if (i>1){
        for (int j=0; j<iInFileLength; j++) {
            outputFilter << OutputSig[i-2][j]<<"\t"<< OutputSig[i-1][j] << endl;
        }
    }
    else{
        for (int j=0; j<iInFileLength; j++) {
            outputFilter << OutputSig[i-1][j] << endl;
        }
        
    }
    outputFilter.close();

    return 0;
    
}


void     showClInfo()
{
    cout << "GTCMT MUSI8903" << endl;
    //cout << "(c) 2016 by Alexander Lerch" << endl;
    cout  << endl;

    return;
}

