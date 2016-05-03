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
#include <cstdio>

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
        m_iDataLen(10000),
        m_fSamplingRate(16000)
        {
            CSpectralCentroid::createInstance(m_pCSpectralCentroid);
            m_pfInputData = new float [m_iDataLen];
            m_pfOutputData = new float [m_iDataLen];
            m_pfInputTmp = 0;
            m_pfOutputTmp = 0;
            
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
            while (iNumFramesRemaining > 0)
            {
                int iNumFrames = std::min(iNumFramesRemaining, m_iBlockLen);
                //if ((iNumFrames == iNumFramesRemaining) & (iNumFrames != m_iBlockLen))
                //  iLenRemain = m_iBlockLen-iNumFrames+1;
                
                m_pfInputTmp    = &m_pfInputData[m_iDataLen - iNumFramesRemaining];
                m_pfOutputTmp   = &m_pfOutputData[m_iDataLen - iNumFramesRemaining];
                
                *m_pfOutputTmp    =(m_pCSpectralCentroid->process(m_pfInputTmp));
                
                iNumFramesRemaining -= iNumFrames;
            }
            
        }
        
        void reset()
        {
            
            for (int i = 0; i < m_iDataLen; i++)
            {
                m_pfInputData[i]=0;
            }
            for (int i = 0; i < (m_iDataLen); i++)
            {
                m_pfOutputData[i]=0;
            }
        }
        
        CSpectralCentroid *m_pCSpectralCentroid;
        float *m_pfInputData;
        float *m_pfOutputData;
        float *m_pfImpulseRespFCD;
        float *m_pfInputTmp;
        float *m_pfOutputTmp;
        int m_iBlockLen;
        int m_iDataLen;
        float m_fSamplingRate;
        
    };

}



















































#endif //WITH_TESTS