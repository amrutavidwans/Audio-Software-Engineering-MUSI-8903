
#include <iostream>
#include <ctime>
#include <fstream>
#include "MUSI8903Config.h"
#include "AudioFileIf.h"
#include "Vibrato.h"


using std::cout;
using std::endl;

// local function declarations
void    showClInfo ();

/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
    std::string             sInputFilePath, sOutputFilePath;
    static const int        kBlockSize          = 1024;
    clock_t                 time                = 0;
    float                   **ppfAudioData      = 0;
    float                   **ppfVibratoAudio   = 0;
    CAudioFileIf            *phAudioFile        = 0;
    std::fstream            hOutputFile;
    CAudioFileIf::FileSpec_t  stFileSpec;
    float                   kVibParamEnt[3];
    double                     iInFileLength;
    showClInfo ();
    
//////////////////////////////////////////////////////////////////////////////
// parse command line arguments
    
    if (argc ==5)
    {
        sInputFilePath  = argv[1];
        sOutputFilePath = sInputFilePath + ".txt";
        kVibParamEnt[0]= atof(argv[2]);
        kVibParamEnt[1]= atof(argv[3]);
        kVibParamEnt[2]= atof(argv[4]);
    }
    else if (argc == 4)
    {
        sInputFilePath  = argv[1];
        sOutputFilePath = sInputFilePath + ".txt";
        kVibParamEnt[0]= atof(argv[2]);
        kVibParamEnt[1] = atof(argv[3]);
        cout << "Taking default delay value" << endl;
        kVibParamEnt[2]= 0.05;
    }
    
    else if (argc == 3)
    {
        sInputFilePath  = argv[1];
        sOutputFilePath = sInputFilePath + ".txt";
        kVibParamEnt[0] = atof(argv[2]);
        cout << "Taking default value for width" << endl;
        kVibParamEnt[1] = 0.5;
        cout << "Taking default value for delay" << endl;
        kVibParamEnt[2]= 0.05;
    }
    else if (argc == 2)
    {
        sInputFilePath  = argv[1];
        sOutputFilePath = sInputFilePath + ".txt";
        cout << "Taking default value of mod. freq" << endl;
        kVibParamEnt[0]=7;
        cout << "Taking default value for width" << endl;
        kVibParamEnt[1]=0.5;
        cout << "Taking default value for delay" << endl;
        kVibParamEnt[2]=0.05;
    }
    
    if (argc < 2)
    {
        cout << "Give file name and parameters" << endl;
        return -1;
        
    }
    

//////////////////////////////////////////////////////////////////////////////////////////////////////
// open the input wave file
CAudioFileIf::create(phAudioFile);
phAudioFile ->openFile(sInputFilePath, CAudioFileIf::kFileRead);
if (!phAudioFile->isOpen())
{
    cout << "Wave file open error!";
    return -1;
    }
    phAudioFile->getFileSpec(stFileSpec);
    phAudioFile->getLength(iInFileLength);
    
    
//////////////////////////////////////////////////////////////////////////////
// allocate memory
    ppfAudioData            = new float* [stFileSpec.iNumChannels];
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
    ppfAudioData[i] = new float [kBlockSize];
    
    time                    = clock();
    
//////////////////////////////////////////////////////////////////////////////
//Vibrato object instantiation
   
    int NumChannels= stFileSpec.iNumChannels;
    float SampleRate= stFileSpec.fSampleRateInHz;
    Vibrato *Vibr = new Vibrato::Vibrato(kVibParamEnt,NumChannels,SampleRate,kBlockSize);
    
    ppfVibratoAudio            = new float* [stFileSpec.iNumChannels];
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        ppfVibratoAudio[i] = new float [kBlockSize];

//////////////////////////////////////////////////////////////////////////////
    // open the output text file
    
    hOutputFile.open (sOutputFilePath.c_str(), std::ios::out);
    if (!hOutputFile.is_open())
    {
        cout << "Text file open error!";
        return -1;
    }

   
/////////////////////////////////////////////////////////////////////////////////
// get audio data and write it to the output file
    while (!phAudioFile->isEof())
    {
        long long iNumFrames = kBlockSize;
        phAudioFile->readData(ppfAudioData, iNumFrames);
        
        for (int i = 0; i < iNumFrames; i++)
        {
            for (int c = 0; c < stFileSpec.iNumChannels; c++)
            {
                Vibr->process(ppfAudioData, ppfVibratoAudio, iNumFrames);
                //std::cout<<ppfVibratoAudio[c][i]<<std::endl;
                hOutputFile<< ppfVibratoAudio[c][i] << " ";
            }

            hOutputFile<< endl;
        }
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
    
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        delete [] ppfVibratoAudio[i];
    delete [] ppfVibratoAudio;
    ppfVibratoAudio = 0;
    
    Vibr->~Vibrato();
    return 0;
    
    }
    
    
    void     showClInfo()
    {
        cout << "GTCMT MUSI8903" << endl;
        cout << "(c) 2016 by Alexander Lerch" << endl;
        cout  << endl;
        
        return;
    }
    
