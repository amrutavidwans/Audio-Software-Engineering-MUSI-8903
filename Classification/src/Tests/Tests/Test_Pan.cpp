#include "MUSI8903Config.h"

#ifdef WITH_TESTS
#include <cassert>
#include <iostream>
#include "Util.h"

#include "UnitTest++.h"

#include "Synthesis.h"
#include "Vector.h"

#include "Pan.h"



SUITE(Pan)
{
    struct PanData
    {
        PanData():
        m_pCPan(0),
        m_ppfInputData(0),
        m_ppfOutputData(0),
        m_ppfInputTmp(0),
        m_ppfOutputTmp(0),
        m_iBlockLen(512),
        m_iDataLen(5120),
        m_iSamplingRate(16000),
        m_iNumChannels(2)
        {
            CPan::createInstance(m_pCPan);
            m_ppfInputData  = new float*[m_iNumChannels];
            m_ppfOutputData = new float*[m_iNumChannels];
            m_ppfInputTmp   = new float*[m_iNumChannels];
            m_ppfOutputTmp  = new float*[m_iNumChannels];
            m_pCPan->setParam(m_iSamplingRate, m_iNumChannels);
            for (int i = 0; i < m_iNumChannels; i++)
            {
                m_ppfInputData[i]   = new float [m_iDataLen];
                CVectorFloat::setZero(m_ppfInputData[i], m_iDataLen);
                m_ppfOutputData[i]  = new float [m_iDataLen];
                CVectorFloat::setZero(m_ppfOutputData[i], m_iDataLen);            }
            

            
        }
        
        ~PanData()
        {
            
            for (int i = 0; i < m_iNumChannels; i++)
            {
                delete [] m_ppfOutputData[i];
                delete [] m_ppfInputData[i];
            }
            delete [] m_ppfOutputTmp;
            delete [] m_ppfInputTmp;
            delete [] m_ppfOutputData;
            delete [] m_ppfInputData;
            
            CPan::destroyInstance(m_pCPan);
           
            
        }
        
        void TestProcess()
        {
            
            int iNumFramesRemaining = m_iDataLen;
            while (iNumFramesRemaining > 0)
            {
                int iNumFrames = std::min(iNumFramesRemaining, m_iBlockLen);
                
                for (int c = 0; c < m_iNumChannels; c++)
                {
                    m_ppfInputTmp[c]    = &m_ppfInputData[c][m_iDataLen - iNumFramesRemaining];
                    m_ppfOutputTmp[c]   = &m_ppfOutputData[c][m_iDataLen - iNumFramesRemaining];
                }
                m_pCPan->process(m_ppfInputTmp, m_ppfOutputTmp, iNumFrames);
                
                iNumFramesRemaining -= iNumFrames;
            }
            
        }
        
        void SetPan(float pan)
        {
            m_pCPan->setPan(pan);
        }
        
        
        CPan *m_pCPan;
        float **m_ppfInputData;
        float **m_ppfOutputData;
        float **m_ppfInputTmp;
        float **m_ppfOutputTmp;
        int m_iBlockLen;
        int m_iDataLen;
        int m_iSamplingRate;
        int m_iNumChannels;
        
    };
    
    
    
    
    TEST_FIXTURE(PanData, HardRight)
    {
        for (int c = 0; c < m_iNumChannels; c++)
            CSynthesis::generateSine (m_ppfInputData[c], 330, m_iSamplingRate, m_iDataLen, .8F, static_cast<float>(c*M_PI_2));
        SetPan(1);
        TestProcess();
        for (int c = 0; c < m_iNumChannels; c++)
           {
            for (int i = 0; i < m_iDataLen; i++)
              {
                if(c==0)
                CHECK_CLOSE(0.F, m_ppfOutputData[c][i], 1e-3F);
                if (c==1)
                CHECK_CLOSE(m_ppfInputData[c][i], m_ppfOutputData[c][i], 1e-3F);

              }
           }
                
        
        //std::cout << "I was here 1"<< std::endl;
    }
    
    TEST_FIXTURE(PanData, HardLeft)
    {
        for (int c = 0; c < m_iNumChannels; c++)
            CSynthesis::generateSine (m_ppfInputData[c], 330, m_iSamplingRate, m_iDataLen, .8F, static_cast<float>(c*M_PI_2));
        SetPan(-1);
        TestProcess();
        for (int c = 0; c < m_iNumChannels; c++)
        {
            for (int i = 0; i < m_iDataLen; i++)
            {
                if(c==0)
                    CHECK_CLOSE(m_ppfInputData[c][i], m_ppfOutputData[c][i], 1e-3F);
                if (c==1)
                    CHECK_CLOSE(0.F, m_ppfOutputData[c][i], 1e-3F);
                
            }
        }
    }
        
    TEST_FIXTURE(PanData, Center)
        {
            for (int c = 0; c < m_iNumChannels; c++)
                CSynthesis::generateSine (m_ppfInputData[c], 330, m_iSamplingRate, m_iDataLen, .8F, static_cast<float>(c*M_PI_2));
            SetPan(0);
            TestProcess();
            for (int c = 0; c < m_iNumChannels; c++)
            {
                for (int i = 0; i < m_iDataLen; i++)
                {
                    if(c==0)
                        CHECK_CLOSE(0.707F *m_ppfInputData[c][i], m_ppfOutputData[c][i], 1e-3F);
                    if (c==1)
                        CHECK_CLOSE(0.707F *m_ppfInputData[c][i],  m_ppfOutputData[c][i], 1e-3F);
                    
                }
            }

        
    }

    
    
   }
#endif //WITH_TESTS
