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
#include <iostream>
#include <fstream>


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
        f_iBlockLength(512),
        m_iDataLength(60*512)
        
        {
            CPeakMeter::createInstance(C_PPm);
            m_ppfInputData = new float*[m_iNumChannels];
            m_ppfOutputData = new float*[m_iNumChannels];
            m_ppfInputTmp = new float*[m_iNumChannels];
            m_pfOutputTmp = new float[m_iNumChannels];
            m_ppfOutputCheck = new float *[m_iNumChannels];
            
            C_PPm->initPeakMeter(m_fSamplingFreq, m_iNumChannels);
            for (int i = 0; i < m_iNumChannels; i++)
            {
                m_ppfInputData[i] = new float [m_iDataLength];
                m_ppfOutputData[i] = new float[60];
                m_ppfOutputCheck[i] = new float[60];
                CVector::setZero(m_ppfInputData[i], m_iDataLength);
                CVector::setZero(m_ppfOutputCheck[i], 60);
                CVector::setZero(m_ppfOutputData[i], 60);
            }
            
        }
        
        ~ PPM()
        {
            for (int i = 0; i < m_iNumChannels; i++)
            {
                delete [] m_ppfInputData[i];
                delete [] m_ppfOutputData[i];
                delete [] m_ppfOutputCheck[i];
            }
            delete [] m_pfOutputTmp;
            delete [] m_ppfInputTmp;
            delete [] m_ppfInputData;
            delete [] m_ppfOutputData;
            delete [] m_ppfOutputCheck;
            CPeakMeter::destroyInstance(C_PPm);
        }
        
        void process ()
        {
            int iNumFramesRemaining = m_iDataLength;
            int count =0;
            while (iNumFramesRemaining > 0)
            {
                int iNumFrames = std::min(iNumFramesRemaining, f_iBlockLength);
                
                for (int c = 0; c < m_iNumChannels; c++)
                {
                    m_ppfInputTmp[c]=  &m_ppfInputData[c][m_iDataLength-iNumFramesRemaining];
                }
                
                C_PPm->process(m_ppfInputTmp,iNumFrames,m_pfOutputTmp);
                for (int c =0; c < m_iNumChannels; c++){
                    m_ppfOutputData[c][count]=m_pfOutputTmp[c];
                    std::cout<<m_ppfOutputData[c][count]<<std::endl;
                }
                count = count +1;
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
        **m_ppfOutputData,
        **m_ppfInputTmp,
        **m_ppfOutputCheck,
        *m_pfOutputTmp;
        float m_fSamplingFreq;
        int m_iNumChannels;
        float m_fAlphaRT;
        float m_fAlphaAT;
        float *m_pfPreviousVPPM;
        int f_iBlockLength;
        int m_iDataLength;
        
    
        
    };

    TEST_FIXTURE(PPM, ZeroInput)
    {
        for (int c = 0; c < m_iNumChannels; c++)
            CVector::setZero(m_ppfOutputCheck[c], 4);
        
        process();
       //int iDelay = CUtil::float2int<int>(f_iBlockLength * m_fAlphaAT);
        for (int c = 0; c < m_iNumChannels; c++)
            CHECK_ARRAY_CLOSE(m_ppfOutputCheck[c], m_ppfOutputData[c],4, 1e-3F);
    }

   
    TEST_FIXTURE(PPM, DCInput)
    {
        int count=0;
        std::string line;
        
        for (int c = 0; c < m_iNumChannels; c++)
        {
            CSynthesis::generateDc(m_ppfInputData[c], m_iDataLength);
            CSynthesis::generateDc(m_ppfOutputCheck[c], 60);
        }
        
        process();
        //int iDelay = CUtil::float2int<int>(f_iBlockLength * m_fAlphaAT);
        
        std::ifstream myfile ("DCVPPM.txt");
        for (int i = 0; i<m_iNumChannels; i++){
            if (myfile.is_open())
            {
                while ( getline (myfile,line) )
                {
                    std::cout << m_ppfOutputCheck[i][count]<< '\n';
                    count++;
                }
                myfile.close();
            }
        }
        
        for (int c = 0; c < m_iNumChannels; c++)
        {
            CHECK_ARRAY_CLOSE(m_ppfOutputCheck[c], m_ppfOutputData[c], 60, 1e-3F);
        }
    }
    
    
    TEST_FIXTURE(PPM, AlphaATChange)
    {   int count=0;
        std::string line;
        
        for (int c = 0; c < m_iNumChannels; c++)
        {
            CSynthesis::generateDc(m_ppfInputData[c], m_iDataLength);
            for(int l=0;l<m_iDataLength;l++)
            {
                if(l >50 *f_iBlockLength &&  l<  m_iDataLength)
                    m_ppfInputData[c][l]=0;
            }
        }
        
        AlphaATChange(0.5);
        process();
        std::ifstream myfile ("AlphaRTVPPM.txt");
        for (int i = 0; i<m_iNumChannels; i++){
            if (myfile.is_open())
            {
                while (getline(myfile,line) )
                {
                    std::cout << m_ppfOutputCheck[i][count]<< '\n';
                    count++;
                }
                myfile.close();
            }
        }

       
        for (int c = 0; c < m_iNumChannels; c++)
            
            CHECK_ARRAY_CLOSE(m_ppfOutputCheck[c], m_ppfOutputData[c], 60, 1e-3F);
    }
    
    TEST_FIXTURE(PPM, AlphaRTChange)
    {
        
        for (int c = 0; c < m_iNumChannels; c++)
        {
            CVector::setZero(m_ppfInputData[c], m_iDataLength);
            m_ppfInputData[c][0]=1;
        }

        AlphaRTChange(0.75);
        process();
        for (int i = 0; i<m_iNumChannels; i++){
            m_ppfOutputCheck[i][0]=0.00497624;
            m_ppfOutputCheck[i][1]=0.00480961;
            m_ppfOutputCheck[i][2]=0.00464856;
            m_ppfOutputCheck[i][3]=0.00449290;
        }

        
        
        for (int c = 0; c < m_iNumChannels; c++)
            CHECK_ARRAY_CLOSE(m_ppfOutputCheck[c], m_ppfOutputData[c], 4, 1e-3F);
    }
 

    TEST_FIXTURE(PPM, SamplingFreqChange)
    {
        
        for (int c = 0; c < m_iNumChannels; c++)
            CSynthesis::generateSine(m_ppfInputData[c], 330, m_fSamplingFreq/2,  m_iDataLength);
        
        C_PPm->initPeakMeter(m_fSamplingFreq/2, m_iNumChannels);
        AlphaATChange(0.01);
        AlphaRTChange(1.5);
        process();
        for (int i = 0; i<m_iNumChannels; i++){
            
            m_ppfOutputCheck[i][0]=0.839382880455107;
            m_ppfOutputCheck[i][1]=0.928742407880281;
            m_ppfOutputCheck[i][2]=0.948680634173690;
            m_ppfOutputCheck[i][3]=0.955326789102863;
        }
        
        
        
        
        
        for (int c = 0; c < m_iNumChannels; c++)
            CHECK_ARRAY_CLOSE(m_ppfOutputCheck[c], m_ppfOutputData[c], 4, 1e-3F);
    }

   
    
    TEST_FIXTURE(PPM, DelayedImpulse)
    {
        for (int c = 0; c < m_iNumChannels; c++)
        {
            CVector::setZero(m_ppfInputData[c], m_iDataLength);
            m_ppfInputData[c][f_iBlockLength*2]=1;
        }
       
        
       
        AlphaATChange(0.01);
        AlphaRTChange(1.5);
        process();
        for (int i = 0; i<m_iNumChannels; i++){
            
            m_ppfOutputCheck[i][0]=0;
            m_ppfOutputCheck[i][1]=0;
            m_ppfOutputCheck[i][2]=0.00497623942273862;
            m_ppfOutputCheck[i][3]=0.00489222154028455;
        }
        //0	0	0.00497623942273862	0.00489222154028455
        
        
        
        
        for (int c = 0; c < m_iNumChannels; c++)
            CHECK_ARRAY_CLOSE(m_ppfOutputCheck[c], m_ppfOutputData[c], 4, 1e-3F);
    }

    
}

#endif //WITH_TESTS