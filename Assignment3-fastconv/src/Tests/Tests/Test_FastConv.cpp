#define _GLIBCXX_FULLY_DYNAMIC_STRING 1
#undef _GLIBCXX_DEBUG
#undef _GLIBCXX_DEBUG_PEDANTIC

#include "MUSI8903Config.h"

#ifdef WITH_TESTS
#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>

#include "UnitTest++.h"

#include "Vector.h"

#include "Synthesis.h"

#include "FastConv.h"

SUITE(FastConv)
{
    struct FastConvData
    {
        FastConvData():
        m_pCFastConv(0),
        m_pfInputData(0),
        m_pfOutputData(0),
        m_pfImpulseRespFCD(0),
        m_pfInputTmp(0),
        m_pfOutputTmp(0),
        m_iIRlen(171),
        m_iBlockLen(171),
        m_iDataLen(816000),
        m_fSamplingRate(16000)
        
        {
            CFastConv::createInstance(m_pCFastConv);
            m_pfImpulseRespFCD = new float [m_iIRlen];
            m_pfInputData = new float [m_iDataLen];
            m_pfOutputData = new float [m_iIRlen+m_iDataLen-1];
        }

        ~FastConvData() 
        {
            m_pCFastConv-> ~CFastConv() ;
            delete [] m_pfImpulseRespFCD;
            delete [] m_pfInputData;
            delete [] m_pfOutputData;
            delete [] m_pfInputTmp;
            delete [] m_pfOutputTmp;
            
        }

        void TestProcess()
        {
            int iNumFramesRemaining = m_iDataLen;
            while (iNumFramesRemaining > 0)
            {
                int iNumFrames = std::min(iNumFramesRemaining, m_iBlockLen);
                
                m_pfInputTmp    = &m_pfInputData[m_iDataLen - iNumFramesRemaining];
                m_pfOutputTmp   = &m_pfOutputData[m_iDataLen - iNumFramesRemaining];
                
                m_pCFastConv->process(m_pfInputTmp, m_pfOutputTmp, iNumFrames);
                
                iNumFramesRemaining -= iNumFrames;
            }
            int iLenRemain = m_iIRlen-1;
            m_pCFastConv->flushBuffer(&m_pfOutputData[m_iDataLen+1], iLenRemain);
        }

        CFastConv *m_pCFastConv;
        float *m_pfInputData;
        float *m_pfOutputData;
        float *m_pfImpulseRespFCD;
        float *m_pfInputTmp;
        float *m_pfOutputTmp;
        int m_iIRlen;
        int m_iBlockLen;
        int m_iDataLen;
        float m_fSamplingRate;
        
    };


    TEST_FIXTURE(FastConvData, IrTest)
    {
        std::memset(m_pfImpulseRespFCD, 0, m_iIRlen);
        int idelaySample = 5;
        m_pfImpulseRespFCD[idelaySample]=1;

        m_pCFastConv->init( m_pfImpulseRespFCD, m_iIRlen, m_iBlockLen);
        
        m_iDataLen = 51 * m_fSamplingRate; // 51sec test signal
        float fFreqInHz=50;
        CSynthesis::generateSine (m_pfInputData, fFreqInHz, m_fSamplingRate, m_iDataLen, 1.F, 0.F);

        TestProcess();
        
        std::fstream            hOutputFile;
        std::fstream            hInputFile;
        std::string sOutputFilePath = "Test1Output.txt";
        std::string sInputFilePath = "Test1Input.txt";
        
        hOutputFile.open (sOutputFilePath.c_str(), std::ios::out);
        hInputFile.open (sInputFilePath.c_str(), std::ios::out);
        
        for (int i=0; i<m_iDataLen; i++)
        {
            hInputFile<< m_pfInputData[i] << " ";
            //std::cout<< m_pfInputData[i] << std::endl;
            hInputFile<< std::endl;
        }
        hInputFile.close();
        
        for (int i =0; i< m_iDataLen+m_iIRlen-1;i++)
        {
            hOutputFile<< m_pfOutputData[i] << " ";
            //std::cout<< m_pfOutputData[i] << std::endl;
            hOutputFile<< std::endl;
        }
        
        hOutputFile.close();
        
        
        for (int i= 0; i < (m_iDataLen+m_iIRlen-1); i++)
        {
            if (i<idelaySample)
                CHECK_CLOSE(0.F, m_pfOutputData[i], 1e-3F);
            else if (i >= idelaySample & i<(m_iDataLen+idelaySample))
                CHECK_CLOSE(m_pfInputData[i-idelaySample], m_pfOutputData[i], 1e-3F);
            else
                CHECK_CLOSE(0.F, m_pfOutputData[i], 1e-3F);
        }
        
    }

    TEST_FIXTURE(FastConvData, InputBlockLengthTest)
    {
    }

}

#endif //WITH_TESTS