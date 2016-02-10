//
//  Test_Vibrato.cpp
//  MUSI8903
//
//  Created by Milap Rane on 2/7/16.
//
//

#include "MUSI8903Config.h"

#ifdef WITH_TESTS
#include <cassert>
#include <cstdio>
#include <iostream>
#include "UnitTest++.h"
#include "Synthesis.h"
#include "Vector.h"
#include "Vibrato.h"
//#include "Vibrato.cpp"

using namespace std;

SUITE(Vibrato_Test)
{
    struct VibratoData
    {
        VibratoData() :
        pVibrato(fVib, numChannels,sampleRate,blockLength),
        inputData(0),
        outputData(0),
        dataLength(35131),
        blockLength(171),
        numChannels(3),
        sampleRate(8000),
        fVib()
        {
            pVibrato= *new Vibrato::Vibrato(fVib, numChannels,sampleRate,blockLength);
            
            
            inputData = new float*[numChannels];
            outputData = new float*[numChannels];
            inputTmp = new float*[numChannels];
            outputTmp = new float*[numChannels];
            for (int i = 0;i < numChannels; i++)
            {
                inputData[i] = new float[dataLength];
                outputData[i] = new float[dataLength];
                CVectorFloat::setZero(inputData[i], dataLength);
                CVectorFloat::setZero(outputData[i], dataLength);
            }
        }
        
        ~VibratoData()
        {
            for (int i = 0;i < numChannels; i++)
            {
                delete[] inputData[i];
                delete[] outputData[i];
                //delete[] inputTmp[i];
                //delete[] outputTmp[i];
            }
            delete[] inputData;
            delete[] outputData;
            delete[] inputTmp;
            delete[] outputTmp;
            
        }
        
        void TestProcess()
        {
            int numFramesRemaining = dataLength;
            while (numFramesRemaining > 0)
            {
                int numFrames = std::min(numFramesRemaining, blockLength);
                for (int i = 0; i < numChannels; i++)
                {
                    inputTmp[i] = &inputData[i][dataLength - numFramesRemaining];
                    outputTmp[i] = &outputData[i][dataLength - numFramesRemaining];
                }
                pVibrato.process(inputTmp, outputTmp, numFrames);
                numFramesRemaining -= numFrames;
            }
        }
        
        Vibrato pVibrato;
        float   **inputData,
        **outputData,
        **inputTmp,
        **outputTmp;
        int     dataLength;
        //float   maxDelayLength;
        int     blockLength;
        int     numChannels;
        float   sampleRate;
        float   fVib[3]={5,0.05,0.05};
       
    };
    
    TEST_FIXTURE(VibratoData, MOD_AMP_ZERO)
    {     float fVibZeromodAmp[3]={5,0,0.05};
        //Set modulation amplitude to 0
        pVibrato= *new Vibrato::Vibrato(fVibZeromodAmp, numChannels,sampleRate,blockLength);
        
        //Generate arbitrary sine wave
        for (int c = 0; c < numChannels; c++)
            CSynthesis::generateSine(inputData[c], 50, sampleRate, dataLength, .8F, static_cast<float>(c*M_PI_2));
        
        TestProcess();
        
        for (int i = 0; i < numChannels; i++)
        {
            for (int j = fVib[2]*sampleRate; j < dataLength; j++)
            {
                CHECK_CLOSE(inputData[i][j-(int)(fVib[2]*sampleRate)], outputData[i][j], 1e-3F);
            }
        }
    }
    
    TEST_FIXTURE(VibratoData, INPUT_DC)
    {
        pVibrato= *new Vibrato::Vibrato(fVib, numChannels,sampleRate,blockLength);
        
        //Generate DC input
        float dc_value = 0.5;
        for (int i = 0; i < numChannels; i++)
        {
            for (int j = 0; j < dataLength; j++)
                inputData[i][j] = dc_value;
        }
        
        TestProcess();
        
        for (int i=0; i < numChannels; i++)
        {
            for (int j = (fVib[2] + 1)*sampleRate; j < dataLength; j++) {
                CHECK_EQUAL(dc_value, outputData[i][j]);
            }
        }
    }
    
    TEST_FIXTURE(VibratoData, VARYING_BLOCK_SIZE)
    {
        float **outputData2;
        pVibrato= *new Vibrato::Vibrato(fVib, numChannels,sampleRate,blockLength);
        
        //Generate arbitrary sine wave
        for (int c = 0; c < numChannels; c++)
            CSynthesis::generateSine(inputData[c], 50, sampleRate, dataLength, .8F, static_cast<float>(c*M_PI_2));
        
        TestProcess();

        outputData2 = new float*[numChannels];
        for (int i = 0; i < numChannels; i++)
        {
            outputData2[i] = new float[dataLength];
        }
        
        //Processing again with new block Length = 366;
        blockLength = 366;
        int numFramesRemaining = dataLength;
        while (numFramesRemaining > 0)
        {
            int numFrames = std::min(numFramesRemaining, blockLength);
            for (int i = 0; i < numChannels; i++)
            {
                inputTmp[i] = &inputData[i][dataLength - numFramesRemaining];
                outputTmp[i] = &outputData2[i][dataLength - numFramesRemaining];
            }
            pVibrato.process(inputTmp, outputTmp, numFrames);
            numFramesRemaining -= numFrames;
        }
        for (int i=0; i < numChannels; i++)
        {
            for (int j = 0; j < dataLength; j++)
            {
                CHECK_CLOSE(outputData2[i][j], outputData[i][j], 1e-3F);
            }
        }
        
        for (int i = 0;i < numChannels; i++)
        {
            delete[] outputData2[i];
        }
        delete[] outputData2;
    }
    
    TEST_FIXTURE(VibratoData, ZERO_INPUT)
    {
        pVibrato= *new Vibrato::Vibrato(fVib, numChannels,sampleRate,blockLength);
        
        //Generate zero input
        for (int i = 0; i < numChannels; i++)
        {
            for (int j = 0; j < dataLength; j++)
                inputData[i][j] = 0.F;
        }
        
        TestProcess();
        
        //Compare Output to zero
        for (int i = 0; i < numChannels; i++)
        {
            for (int j = (fVib[2] + 1)*sampleRate; j < dataLength; j++) {
                CHECK_EQUAL(0, outputData[i][j]);
            }
        }
    }
    
    TEST_FIXTURE(VibratoData, ZERO_MOD_FREQ)
    {   float fVibnew[3]= {0,0.05,0.05};        //Set modulation amplitude to 0
        
        pVibrato= *new  Vibrato::Vibrato(fVibnew, numChannels,sampleRate,blockLength);
        
        //Generate arbitrary sine wave
        for (int c = 0; c < numChannels; c++)
            CSynthesis::generateSine(inputData[c], 50, sampleRate, dataLength, .8F, static_cast<float>(c*M_PI_2));
        
        TestProcess();
        
        for (int i = 0; i < numChannels; i++)
        {
            for (int j = fVibnew[2]*sampleRate; j < dataLength; j++)
            {
                CHECK_CLOSE(inputData[i][j - (int)(fVibnew[2]*sampleRate)], outputData[i][j], 1e-3F);
            }
        }
    }
}

#endif //WITH_TESTS