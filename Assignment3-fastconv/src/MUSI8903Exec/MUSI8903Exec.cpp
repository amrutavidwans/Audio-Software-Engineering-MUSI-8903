
#include <iostream>
#include <ctime>

#include "MUSI8903Config.h"
#include "FastConv.h"

#include "AudioFileIf.h"

using std::cout;
using std::endl;

// local function declarations
void    showClInfo ();

/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
    std::string             sInputFilePath,                 //!< file paths
                            sOutputFilePath;

    static const int        kBlockSize          = 1024;

    clock_t                 time                = 0;

    float                   **ppfAudioData      = 0;

    CAudioFileIf            *phAudioFile        = 0;
    std::fstream            hOutputFile;
    CAudioFileIf::FileSpec_t stFileSpec;

    showClInfo ();

    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments
    if (argc < 2)
    {
        return -1;
    }
    else
    {
        sInputFilePath  = argv[1];
        sOutputFilePath = sInputFilePath + ".txt";
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
    // open the output text file
    hOutputFile.open (sOutputFilePath.c_str(), std::ios::out);
    if (!hOutputFile.is_open())
    {
        cout << "Text file open error!";
        return -1;
    }
    

    ////////////////////////////////////////////////////////////////////////////
    // object for fastconv
    float *pfIRresp = new float;
    int idelaySample = 5;
    int iIRlen = 3000;
    float *pfAudioPart = new float[kBlockSize];
    float *pfOutputTmp = new float[kBlockSize];
    CFastConv *pCFastConv;
    pCFastConv = 0;
    CFastConv::createInstance(pCFastConv);
    
    std::memset(pfIRresp, 0, iIRlen);
    pfIRresp[idelaySample]=1;
    
    pCFastConv->init(pfIRresp, iIRlen, kBlockSize);
    
    
    //////////////////////////////////////////////////////////////////////////////
    // allocate memory
    ppfAudioData            = new float* [stFileSpec.iNumChannels];
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        ppfAudioData[i] = new float [kBlockSize];

    time                    = clock();
    
    //////////////////////////////////////////////////////////////////////////////
    // get processed and write it to the output file
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
    
    int iLenFlush = iIRlen-1;
    float *pfOutputReverb = new float [iLenFlush];
    pCFastConv->flushBuffer(pfOutputReverb, iLenFlush);
    for (int i = 0; i < iLenFlush; i++)
    {
        hOutputFile << pfOutputReverb[i] << "\t";
        hOutputFile << endl;
    }
    
    cout << "reading/writing done in: \t"    << (clock()-time)*1.F/CLOCKS_PER_SEC << " seconds." << endl;

    //////////////////////////////////////////////////////////////////////////////
    // clean-up
    CAudioFileIf::destroy(phAudioFile);
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
    cout << "(c) 2016 by Alexander Lerch" << endl;
    cout  << endl;

    return;
}

