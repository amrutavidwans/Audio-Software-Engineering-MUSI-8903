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
        m_iIRlen(51),
        m_iBlockLen(512),
        m_iDataLen(10000),
        m_fSamplingRate(16000),
        m_iConvBlockLen(1024)
        
        {
            CFastConv::createInstance(m_pCFastConv);
            m_pfImpulseRespFCD = new float [m_iIRlen];
            m_pfInputData = new float [m_iDataLen];
            m_pfOutputData = new float [m_iIRlen+m_iDataLen-1];
            m_pfInputTmp = 0;
            m_pfOutputTmp = 0;
        }

        ~FastConvData() 
        {
            CFastConv::destroyInstance(m_pCFastConv);
            delete [] m_pfImpulseRespFCD;
            m_pfImpulseRespFCD = 0;
            delete [] m_pfInputData;
            m_pfInputData = 0;
            delete [] m_pfOutputData;
            m_pfOutputData = 0;
            
            //delete m_pfInputTmp;
            m_pfInputTmp = 0;
            //delete m_pfOutputTmp;
            m_pfOutputTmp = 0;
            m_iBlockLen = 0;
            m_iDataLen = 0;
            m_iIRlen = 0;
            m_fSamplingRate = 0;
            
        }

        void TestProcess()
        {
            //int iLenRemain = 0;
            int iNumFramesRemaining = m_iDataLen;
            while (iNumFramesRemaining > 0)
            {
                int iNumFrames = std::min(iNumFramesRemaining, m_iBlockLen);
                //if ((iNumFrames == iNumFramesRemaining) & (iNumFrames != m_iBlockLen))
                  //  iLenRemain = m_iBlockLen-iNumFrames+1;
                
                m_pfInputTmp    = &m_pfInputData[m_iDataLen - iNumFramesRemaining];
                m_pfOutputTmp   = &m_pfOutputData[m_iDataLen - iNumFramesRemaining];
                
                m_pCFastConv->process(m_pfInputTmp, m_pfOutputTmp, iNumFrames);
                
                iNumFramesRemaining -= iNumFrames;
            }
            int iLenFlush = m_iIRlen-1;
            m_pCFastConv->flushBuffer(&m_pfOutputData[m_iDataLen], iLenFlush);
        }

        void reset()
        {
            
            for (int i = 0; i < m_iDataLen; i++)
            {
                m_pfInputData[i]=0;
            }
            for (int i = 0; i < (m_iIRlen+m_iDataLen-1); i++)
            {
                m_pfOutputData[i]=0;
            }
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
        int m_iConvBlockLen;
        
    };


    TEST_FIXTURE(FastConvData, IrTest)
    {
        reset();
        std::memset(m_pfImpulseRespFCD, 0, sizeof(float)*m_iIRlen);
        int idelaySample = 5;
        m_pfImpulseRespFCD[idelaySample]=1;

        m_pCFastConv->init( m_pfImpulseRespFCD, m_iIRlen, m_iConvBlockLen);
        
        float fFreqInHz=50;
        CSynthesis::generateSine (m_pfInputData, fFreqInHz, m_fSamplingRate, m_iDataLen, 1.F, 0.F);

        TestProcess();
        
        /*
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
        */
        
        for (int i= 0; i < (m_iDataLen+m_iIRlen-1); i++)
        {
            if (i<idelaySample)
                CHECK_CLOSE(0.F, m_pfOutputData[i], 1e-3F);
            else if (i >= idelaySample & i<(m_iDataLen+idelaySample))
                CHECK_CLOSE(m_pfInputData[i-idelaySample], m_pfOutputData[i], 1e-3F);
            else
                CHECK_CLOSE(0.F, m_pfOutputData[i], 1e-3F);
        }
        //std::cout << "I was here 1"<< std::endl;
        
    }

    TEST_FIXTURE(FastConvData, InputBlockLengthTest)
    {
     
        //for an input signal of length 10000, run a similar test
       // with a succession of different input/output block sizes (1, 13, 1023, 2048,1,17, 5000, 1897)
        
        std::memset(m_pfImpulseRespFCD, 0, sizeof(float)*m_iIRlen);
        int idelaySample = 5;
        m_pfImpulseRespFCD[idelaySample]=1;

        m_iDataLen = 10000; // 51sec test signal
        float fFreqInHz=50;
        
        /*
        // BlockSize input 1
        m_iBlockLen = 1;
        
        m_pCFastConv->init( m_pfImpulseRespFCD, m_iIRlen, m_iBlockLen);
        CSynthesis::generateSine (m_pfInputData, fFreqInHz, m_fSamplingRate, m_iDataLen, 1.F, 0.F);
        
        TestProcess();
        
        for (int i= 0; i < (m_iDataLen+m_iIRlen-1); i++)
        {
            if (i<idelaySample)
                CHECK_CLOSE(0.F, m_pfOutputData[i], 1e-3F);
            else if (i >= idelaySample & i<(m_iDataLen+idelaySample))
                CHECK_CLOSE(m_pfInputData[i-idelaySample], m_pfOutputData[i], 1e-3F);
            else
                CHECK_CLOSE(0.F, m_pfOutputData[i], 1e-3F);
        }
       
        reset();
        ///////////////////////////////////////////////////////////////////////////////////////

        m_pCFastConv->reset();
        
        // BlockSize input 13
        m_iBlockLen = 13;
        
        m_pCFastConv->init( m_pfImpulseRespFCD, m_iIRlen, m_iBlockLen);
        CSynthesis::generateSine (m_pfInputData, fFreqInHz, m_fSamplingRate, m_iDataLen, 1.F, 0.F);
        TestProcess();
        
        for (int i= 0; i < (m_iDataLen+m_iIRlen-1); i++)
        {
            if (i<idelaySample)
                CHECK_CLOSE(0.F, m_pfOutputData[i], 1e-3F);
            else if (i >= idelaySample & i<(m_iDataLen+idelaySample))
                CHECK_CLOSE(m_pfInputData[i-idelaySample], m_pfOutputData[i], 1e-3F);
            else
                CHECK_CLOSE(0.F, m_pfOutputData[i], 1e-3F);
        }
         reset();
        */
        ///////////////////////////////////////////////////////////////////////////////////////
        
        m_pCFastConv->reset();
        
        // BlockSize input 17
        m_iBlockLen = 17;
        
        m_pCFastConv->init( m_pfImpulseRespFCD, m_iIRlen, m_iConvBlockLen);
        CSynthesis::generateSine (m_pfInputData, fFreqInHz, m_fSamplingRate, m_iDataLen, 1.F, 0.F);
        TestProcess();
        
        for (int i= 0; i < (m_iDataLen+m_iIRlen-1); i++)
        {
            if (i<idelaySample)
                CHECK_CLOSE(0.F, m_pfOutputData[i], 1e-3F);
            else if (i >= idelaySample & i<(m_iDataLen+idelaySample))
                CHECK_CLOSE(m_pfInputData[i-idelaySample], m_pfOutputData[i], 1e-3F);
            else
                CHECK_CLOSE(0.F, m_pfOutputData[i], 1e-3F);
        }
      
        reset();
        /*
        ///////////////////////////////////////////////////////////////////////////////////////
        m_pCFastConv->reset();
        
        // BlockSize input 1897
        m_iBlockLen = 1897;
        
        m_pCFastConv->init( m_pfImpulseRespFCD, m_iIRlen, m_iConvBlockLen);
        CSynthesis::generateSine (m_pfInputData, fFreqInHz, m_fSamplingRate, m_iDataLen, 1.F, 0.F);
        
        TestProcess();
        
        for (int i= 0; i < (m_iDataLen+m_iIRlen-1); i++)
        {
            if (i<idelaySample)
                CHECK_CLOSE(0.F, m_pfOutputData[i], 1e-3F);
            else if (i >= idelaySample & i<(m_iDataLen+idelaySample))
                CHECK_CLOSE(m_pfInputData[i-idelaySample], m_pfOutputData[i], 1e-3F);
            else
                CHECK_CLOSE(0.F, m_pfOutputData[i], 1e-3F);
        }
       
        reset();
         */
        ///////////////////////////////////////////////////////////////////////////////////////
        m_pCFastConv->reset();
        
        // BlockSize input 5000
        m_iBlockLen = 5000;
        
        m_pCFastConv->init( m_pfImpulseRespFCD, m_iIRlen, m_iBlockLen);
        CSynthesis::generateSine (m_pfInputData, fFreqInHz, m_fSamplingRate, m_iDataLen, 1.F, 0.F);
        
        TestProcess();
        
        for (int i= 0; i < (m_iDataLen+m_iIRlen-1); i++)
        {
            if (i<idelaySample)
                CHECK_CLOSE(0.F, m_pfOutputData[i], 1e-3F);
            else if (i >= idelaySample & i<(m_iDataLen+idelaySample))
                CHECK_CLOSE(m_pfInputData[i-idelaySample], m_pfOutputData[i], 1e-3F);
            else
                CHECK_CLOSE(0.F, m_pfOutputData[i], 1e-3F);
        }
        
        
        reset();
        ///////////////////////////////////////////////////////////////////////////////////////
        m_pCFastConv->reset();
        
        // BlockSize input 2048
        m_iBlockLen = 2048;
        
        m_pCFastConv->init( m_pfImpulseRespFCD, m_iIRlen, m_iBlockLen);
        
        CSynthesis::generateSine (m_pfInputData, fFreqInHz, m_fSamplingRate, m_iDataLen, 1.F, 0.F);
        
        TestProcess();
        
        for (int i= 0; i < (m_iDataLen+m_iIRlen-1); i++)
        {
            if (i<idelaySample)
                CHECK_CLOSE(0.F, m_pfOutputData[i], 1e-3F);
            else if (i >= idelaySample & i<(m_iDataLen+idelaySample))
                CHECK_CLOSE(m_pfInputData[i-idelaySample], m_pfOutputData[i], 1e-3F);
            else
                CHECK_CLOSE(0.F, m_pfOutputData[i], 1e-3F);
        }
        
        reset();
        ///////////////////////////////////////////////////////////////////////////////////////
        m_pCFastConv->reset();
        
        // BlockSize input 1023
        m_iBlockLen = 1023;
        
        m_pCFastConv->init( m_pfImpulseRespFCD, m_iIRlen, m_iBlockLen);
        
        CSynthesis::generateSine (m_pfInputData, fFreqInHz, m_fSamplingRate, m_iDataLen, 1.F, 0.F);
        
        TestProcess();
        
        for (int i= 0; i < (m_iDataLen+m_iIRlen-1); i++)
        {
            if (i<idelaySample)
                CHECK_CLOSE(0.F, m_pfOutputData[i], 1e-3F);
            else if (i >= idelaySample & i<(m_iDataLen+idelaySample))
                CHECK_CLOSE(m_pfInputData[i-idelaySample], m_pfOutputData[i], 1e-3F);
            else
                CHECK_CLOSE(0.F, m_pfOutputData[i], 1e-3F);
        }
      
        reset();
    
    }

}

#endif //WITH_TESTS