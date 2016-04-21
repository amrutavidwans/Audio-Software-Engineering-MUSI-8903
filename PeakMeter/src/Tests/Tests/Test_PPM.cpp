//
//  Test_PPM.cpp
//  MUSI8903
//
//  Created by Milap Rane on 4/20/16.
//
//


#include "MUSI8903Config.h"

#ifdef WITH_TESTS
#include <cassert>
#include <cstdio>

#include "UnitTest++.h"

#include "Synthesis.h"
#include "Vector.h"

#include "Vibrato.h"
#include "PeakMeter.h"

SUITE(PPM)
{
    struct PPM
    {
        PPM():
        m_fSamplingFreq(44100),
        m_iNumChannels(1),
        m_fAlphaRT(1.5),
        m_fAlphaAT(0.01),
        m_iBlockLength(1024),
        m_iHopLength(512),
        m_pfPreviousVPPM(0)
        
        {
            C_PPm= new CPeakMeter;
            m_ppfInputData = new float*[m_iNumChannels];
            m_pfOutputData = *new float*[m_iNumChannels];
            m_ppfInputTmp = new float*[m_iNumChannels];
            m_pfOutputTmp = *new float*[m_iNumChannels];
            for (int i = 0; i < m_iNumChannels; i++)
            {
                m_ppfInputData[i] = new float [m_iBlockLength];
                CSynthesis::generateSine(m_ppfInputData[i], 800, m_fSamplingFreq, m_iBlockLength, .6F, 0);
                m_pfOutputData[i] = *new float [m_iBlockLength];
                CVector::setZero(m_ppfInputData[i], m_iBlockLength);
            }
            
        }
        
        ~ PPM()
        {
            for (int i = 0; i < m_iNumChannels; i++)
            {
                delete [] m_ppfInputData[i];
            }
            delete [] m_pfOutputTmp;
            delete [] m_ppfInputTmp;
            delete [] m_ppfInputData;
            
            delete [] C_PPm;
        }
        
        void process ()
        {
            int iNumFramesRemaining = m_iBlockLength;
            while (iNumFramesRemaining > 0)
            {
                int iNumFrames = std::min(iNumFramesRemaining, m_iBlockLength);
                
                for (int c = 0; c < m_iNumChannels; c++)
                {
                    m_ppfInputTmp[c] = &m_ppfInputData[c][m_iBlockLength - iNumFramesRemaining];
                    m_pfOutputTmp[c] = m_pfOutputData[m_iBlockLength - iNumFramesRemaining];
                }
                C_PPm->process(m_ppfInputTmp,m_pfOutputTmp);
                
                iNumFramesRemaining -= iNumFrames;
            }
        }
        CPeakMeter *C_PPm;
        float **m_ppfInputData,
        *m_pfOutputData,
        **m_ppfInputTmp,
        *m_pfOutputTmp;
        float m_fSamplingFreq;
        int m_iNumChannels;
        float m_fAlphaRT;
        float m_fAlphaAT;
        int m_iBlockLength;
        int m_iHopLength;
        float *m_pfPreviousVPPM;
        
    };

    TEST_FIXTURE(PPM, ZeroInput)
    {
        for (int c = 0; c < m_iNumChannels; c++)
            CVector::setZero(m_ppfInputData[c], m_iBlockLength);
        
        process();
       
        for (int c = 0; c < m_iNumChannels; c++)
            CHECK_ARRAY_CLOSE(m_ppfInputData[c], &m_pfOutputData[c], m_iBlockLength, 1e-3F);
    }

    TEST_FIXTURE(PPM, DCInput)
    {
        for (int c = 0; c < m_iNumChannels; c++)
            CSynthesis::generateDc(m_ppfInputData[c], m_iBlockLength);
        process();
        
        for (int c = 0; c < m_iNumChannels; c++)
            CHECK_ARRAY_CLOSE(m_ppfInputData[c], &m_pfOutputData[c], m_iBlockLength, 1e-3F);
    }
    
    

    
}

#endif //WITH_TESTS