
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <algorithm>
#include <vector>

#include "MUSI8903Config.h"
#include "MyProject.h"

#include "AudioFileIf.h"

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
    sInputFilePath = "/Users/milaprane/Desktop/sinewav4410.wav";//argv[0];
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
    // clean-up
    
    float dl[10]={1,2,3,4,5,6,7,8,9,10};
    std::rotate(&dl[0], &dl[9], &dl[10]);
    std::cout<<dl;
    
    
    ////////////////////////
    //FIR filter test code
    /*x=zeros(100,1);x(1)=1; % unit impulse signal of length 100
     g=0.5;
     Delayline=zeros(10,1);% memory allocation for length 10
     for n=1:length(x);
     y(n)=x(n)+g*Delayline(10);
     Delayline=[x(n);Delayline(1:10-1)];
     end*/
   float g = -1;
    //int LenDelayLine = 10;
    //float DelayLine[LenDelayLine]=0;
    float DelayLine[10]={};
    float **OutputSig = 0;
    
    OutputSig = new float*[fileSpecs.iNumChannels];
    for ( int i = 0; i<fileSpecs.iNumChannels; i++) {
        OutputSig[i] = new float[iInFileLength];
    }
    //**OutputSig={};
    
    for (int i=0; i < fileSpecs.iNumChannels;i++){
    for (int j=0; j<iInFileLength; j++) {
        OutputSig[i][j] = ppfAudioData[i][j] + g * DelayLine[9];
        std::rotate(&DelayLine[0], &DelayLine[9], &DelayLine[10]);
        DelayLine[0]=ppfAudioData[i][j];
        
    }
        memset(&DelayLine, 0, sizeof(DelayLine));
    }
    
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
    cout << "(c) 2016 by Alexander Lerch" << endl;
    cout  << endl;

    return;
}

