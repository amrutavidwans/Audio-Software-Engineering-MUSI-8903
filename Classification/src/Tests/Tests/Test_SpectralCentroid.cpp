//
//  Test_SpectralCentroid.cpp
//  MUSI8903
//
//  Created by Amruta Vidwans on 5/1/16.
//
//

#include "MUSI8903Config.h"

#ifdef WITH_TESTS
#include <cassert>
#include <iostream>
#include "Util.h"

#include "UnitTest++.h"

#include "Synthesis.h"

#include "SpectralCentroid.h"



SUITE(SpectralCentroid)
{
    struct SpectralCentroidData
    {
        SpectralCentroidData():
        m_pCSpectralCentroid(0),
        m_pfInputData(0),
        m_pfOutputData(0),
        m_pfInputTmp(0),
        m_pfOutputTmp(0),
        m_iBlockLen(512),
        m_iDataLen(5120),
        m_fSamplingRate(16000)
        {
            CSpectralCentroid::createInstance(m_pCSpectralCentroid);
            m_pfInputData = new float [m_iDataLen];
            m_pfOutputData = new float [CUtil::float2int<int>(m_iDataLen/m_iBlockLen)];
            m_pfInputTmp = 0;
            m_pfOutputTmp = 0;
            m_pCSpectralCentroid->setParams(m_fSamplingRate, m_iBlockLen);
            
        }
        
        ~SpectralCentroidData()
        {
            
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
            m_fSamplingRate = 0;
            
        }
        
        void TestProcess()
        {
            //int iLenRemain = 0;
            int iNumFramesRemaining = m_iDataLen;
            int count=0;
            while (iNumFramesRemaining > 0)
            {
                int iNumFrames = std::min(iNumFramesRemaining, m_iBlockLen);
                //if ((iNumFrames == iNumFramesRemaining) & (iNumFrames != m_iBlockLen))
                //  iLenRemain = m_iBlockLen-iNumFrames+1;
                
                m_pfInputTmp    = &m_pfInputData[m_iDataLen - iNumFramesRemaining];
                m_pfOutputTmp   = &(m_pfOutputData[count]);
                *m_pfOutputTmp    =(m_pCSpectralCentroid->process(m_pfInputTmp));
                //std::cout << *m_pfOutputTmp <<std::endl;
                iNumFramesRemaining -= iNumFrames;
                count++;
            }
            
        }
        
        void reset()
        {
            
            for (int i = 0; i < m_iDataLen; i++)
            {
                m_pfInputData[i]=0;
                
            }
            for (int i = 0; i < CUtil::float2int<int>(m_iDataLen/m_iBlockLen); i++)
            {
                m_pfOutputData[i]=0;
            }
            m_pfInputTmp=0;
            m_pfOutputTmp=0;
        }
        
        CSpectralCentroid *m_pCSpectralCentroid;
        float *m_pfInputData;
        float *m_pfOutputData;
        float *m_pfInputTmp;
        float *m_pfOutputTmp;
        int m_iBlockLen;
        int m_iDataLen;
        float m_fSamplingRate;
        
    };




TEST_FIXTURE(SpectralCentroidData, ZeroInput)
{
    reset();
    std::memset(m_pfInputData, 0, sizeof(float)*m_iDataLen);
    
    TestProcess();
    CHECK_CLOSE(0.F, *m_pfOutputData,1e-3F);
    
    
    //std::cout << "I was here 1"<< std::endl;
}


TEST_FIXTURE(SpectralCentroidData, ImpInput)
    {
        reset();
        std::memset(m_pfInputData, 0.F, sizeof(float)*m_iDataLen);
        m_pfInputData[0]=1;
        TestProcess();
        CHECK_CLOSE(m_fSamplingRate/4.F, *m_pfOutputData,1e-3F);
        
        
        //std::cout << "I was here 1"<< std::endl;
    }
 

TEST_FIXTURE(SpectralCentroidData, SinInput)
    {
        reset();
        CSynthesis::generateSine(m_pfInputData, 330, m_fSamplingRate, m_iDataLen);
        TestProcess();
        CHECK_CLOSE(327.027653988775F, *m_pfOutputData ,2);
        
        
        //std::cout << "I was here 1"<< std::endl;
    }


}

















































#endif //WITH_TESTS