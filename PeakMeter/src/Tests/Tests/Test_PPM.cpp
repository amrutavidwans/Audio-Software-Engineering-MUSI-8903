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
#include "PeakMeter.h"
#include <math.h>

SUITE(PPM)
{
    struct PPM
    {
        PPM():
        m_fSamplingFreq(44100),
        m_iNumChannels(1),
        m_fAlphaRT(1.5),
        m_fAlphaAT(0.01),
        m_pfPreviousVPPM(0),
        f_iBlockLength(512)
        
        {
            C_PPm= new CPeakMeter;
            m_ppfInputData = new float*[m_iNumChannels];
            m_pfOutputData = new float*[m_iNumChannels];
            m_ppfInputTmp = new float*[m_iNumChannels];
            m_pfOutputTmp = new float*[m_iNumChannels];
            C_PPm->initPeakMeter(m_fSamplingFreq, m_iNumChannels);
            for (int i = 0; i < m_iNumChannels; i++)
            {
                m_ppfInputData[i] = new float [f_iBlockLength];
                CSynthesis::generateSine(m_ppfInputData[i], 330, m_fSamplingFreq, .6F, 0);
                m_pfOutputData[i] = new float[f_iBlockLength];
                CVector::setZero(m_ppfInputData[i], f_iBlockLength);
            }
            
        }
        
        ~ PPM()
        {
            for (int i = 0; i < m_iNumChannels; i++)
            {
                delete [] m_ppfInputData[i];
                delete [] m_pfOutputData[i];
                delete [] m_ppfInputTmp[i];
                delete [] m_pfOutputTmp[i];
                
            }
            delete [] m_pfOutputTmp;
            delete [] m_ppfInputTmp;
            delete [] m_ppfInputData;
            
            delete [] C_PPm;
        }
        
        void process ()
        {
            int iNumFramesRemaining = f_iBlockLength;
            while (iNumFramesRemaining > 0)
            {
                int iNumFrames = std::min(iNumFramesRemaining, f_iBlockLength);
                
                for (int c = 0; c < m_iNumChannels; c++)
                {
                    m_ppfInputTmp[c]= &m_ppfInputData[c][f_iBlockLength - iNumFramesRemaining];
                    m_pfOutputTmp[c] = &m_pfOutputData[c][f_iBlockLength-iNumFramesRemaining];
                }
                C_PPm->process(m_ppfInputTmp,f_iBlockLength,m_pfOutputTmp);
                
                iNumFramesRemaining -= iNumFrames;
            }
        }
        
        void AlphaATChange(float fNewAT){
            C_PPm->setAlphaAT(fNewAT);
        }
        
        void AlphaRTChange(float fNewRT){
            C_PPm->setAlphaRT(fNewRT);
        }
        
        CPeakMeter *C_PPm;
        float **m_ppfInputData,
        **m_pfOutputData,
        **m_ppfInputTmp,
        **m_pfOutputTmp;
        float m_fSamplingFreq;
        int m_iNumChannels;
        float m_fAlphaRT;
        float m_fAlphaAT;
        float *m_pfPreviousVPPM;
        int f_iBlockLength;
    
        
    };

    TEST_FIXTURE(PPM, ZeroInput)
    {
        for (int c = 0; c < m_iNumChannels; c++)
            CVector::setZero(m_ppfInputData[c], f_iBlockLength);
        
        process();
        int iDelay = CUtil::float2int<int>(f_iBlockLength * m_fAlphaAT);
        for (int c = 0; c < m_iNumChannels; c++)
            CHECK_ARRAY_CLOSE(m_ppfInputData[c], m_pfOutputData[c],f_iBlockLength-iDelay, 1e-3F);
    }

   /*
    TEST_FIXTURE(PPM, DCInput)
    {
        for (int c = 0; c < m_iNumChannels; c++)
            CSynthesis::generateDc(m_ppfInputData[c], f_iBlockLength);
        process();
        int iDelay = CUtil::float2int<int>(f_iBlockLength * m_fAlphaAT);
        
        for (int c = 0; c < m_iNumChannels; c++)
            CHECK_ARRAY_CLOSE(m_ppfInputData[c], *m_pfOutputData, iDelay, 1e-3F);
    }
    
    TEST_FIXTURE(PPM, AlphaATChange)
    {
        for (int c = 0; c < m_iNumChannels; c++)
            CSynthesis::generateDc(m_ppfInputData[c], f_iBlockLength);
        
        AlphaATChange(0.05);
        process();
        
        int iDelay = CUtil::float2int<int>(f_iBlockLength * m_fAlphaAT);
        for (int c = 0; c < m_iNumChannels; c++)
            CHECK_ARRAY_CLOSE(m_ppfInputData[c], m_pfOutputData[c], iDelay, 1e-3F);
    }
    
    TEST_FIXTURE(PPM, AlphaRTChange)
    {
        for (int c = 0; c < m_iNumChannels; c++)
            CSynthesis::generateDc(m_ppfInputData[c], f_iBlockLength);
        AlphaRTChange(2);
        process();
        int iDelay = CUtil::float2int<int>(f_iBlockLength * m_fAlphaRT);
        for (int c = 0; c < m_iNumChannels; c++)
            CHECK_ARRAY_CLOSE(m_ppfInputData[c], *m_pfOutputData, iDelay, 1e-3F);
    }


    */

    
}

#endif //WITH_TESTS