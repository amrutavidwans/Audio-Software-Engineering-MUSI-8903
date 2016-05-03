
#include <iostream>
#include <ctime>

#include "MUSI8903Config.h"
#include "FastConv.h"

#include "AudioFileIf.h"
#include "FastConv.h"

using std::cout;
using std::endl;

// local function declarations
void    showClInfo ();

/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
    std::string             sInputFilePath,                 //!< file paths
                            sOutputFilePath,
                            sImpRespPath;

    static const int        kBlockSize          = 1024;

    clock_t                 time                = 0;

    float                   **ppfAudioData      = 0;
    float                   **ppfImpRespData    = 0;

    CAudioFileIf            *phAudioFile        = 0;
    
    CAudioFileIf            *phImpRespFile      = 0;

    std::fstream            hInputFile;
    std::fstream            hImpResp;
    std::fstream            hOutputFile;
    
    CAudioFileIf::FileSpec_t stFileSpec;
    CAudioFileIf::FileSpec_t stImpRespSpec;
    

    showClInfo ();

    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments
    if (argc == 3)
    {
        sInputFilePath  = argv[1];
        sImpRespPath    = argv[2];
        sOutputFilePath = sInputFilePath + "Conv.txt";
    }
    else
    {
        
    }

    //////////////////////////////////////////////////////////////////////////////
    // open the input wave file-
    CAudioFileIf::create(phAudioFile);
    phAudioFile->openFile(sInputFilePath, CAudioFileIf::kFileRead);
    if (!phAudioFile->isOpen())
    {
        cout << "Wave file open error!";
        return -1;
    }
    phAudioFile->getFileSpec(stFileSpec);
    
    //////////////////////////////////////////////////////////////////////////////
    //open the impulse response-
    CAudioFileIf::create(phImpRespFile);
    phImpRespFile->openFile(sImpRespPath, CAudioFileIf::kFileRead);
    if (!phImpRespFile->isOpen())
    {
        cout << "Wave file open error!";
        return -1;
    }
    phImpRespFile->getFileSpec(stImpRespSpec);
    
    //////////////////////////////////////////////////////////////////////////////
    // open the output text file
    hOutputFile.open (sOutputFilePath.c_str(), std::ios::out);
    if (!hOutputFile.is_open())
    {
        cout << "Text file open error!";
        return -1;
    }
    

    ////////////////////////////////////////////////////////////////////////////
    // object for fastconv
    float *pfIRresp = 0;
    long long iIRlen = 0;
    float *pfAudioPart = new float;
    float *pfOutputTmp = new float[kBlockSize];
    CFastConv *pCFastConv;
    pCFastConv = 0;
    CFastConv::createInstance(pCFastConv);
    
    //////////////////////////////////////////////////////////////////////////////
    // allocate memory for audio and then Imp Resp
    ppfAudioData            = new float* [stFileSpec.iNumChannels];
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        ppfAudioData[i] = new float [kBlockSize];

    ppfImpRespData           = new float* [stImpRespSpec.iNumChannels];
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        ppfImpRespData[i] = new float [kBlockSize];
    
    
    time                    = clock();
    
    //////////////////////////////////////////////////////////////////////////////
    // get audio data and write it to the output file

    while (!phAudioFile->isEof())
    {
        long long iNumFrames = kBlockSize;
        phAudioFile->readData(ppfAudioData, iNumFrames);
        pfAudioPart = ppfAudioData[0];

        pCFastConv->process(pfAudioPart, pfOutputTmp, iNumFrames);
        
        for (int i = 0; i < iNumFrames; i++)
        {
            hOutputFile << pfOutputTmp[i] << "\t";
            hOutputFile << endl;
        }
    }
    
    
    cout << "reading/writing done in: \t"    << (clock()-time)*1.F/CLOCKS_PER_SEC << " seconds." << endl;

    //////////////////////////////////////////////////////////////////////////////
    // clean-up
    CAudioFileIf::destroy(phAudioFile);
    CAudioFileIf::destroy(phImpRespFile);
    hOutputFile.close();

    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        delete [] ppfAudioData[i];
    delete [] ppfAudioData;
    ppfAudioData = 0;

    return 0;
    
}


void     showClInfo()
{
    cout << "GTCMT MUSI8903" << endl;
    //cout << "(c) 2016 by Alexander Lerch" << endl;
    cout  << endl;

    return;
}

