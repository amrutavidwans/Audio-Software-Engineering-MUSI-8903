#include "MUSI8903Config.h"



#ifdef WITH_TESTS



#include <cassert>

#include <cstdio>

#include "UnitTest++.h"

#include "Synthesis.h"

#include "Vector.h"

#include "PeakMeter.h"


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
                    
                    //std::cout<<m_ppfOutputData[c][count]<<std::endl;
                    
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
        
        
        
        
        
        void resetValues(){
            
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
    
    {   resetValues();
        
        for (int c = 0; c < m_iNumChannels; c++)
            
            CVector::setZero(m_ppfOutputCheck[c], 4);
        
        
        
        process();
        
        //int iDelay = CUtil::float2int<int>(f_iBlockLength * m_fAlphaAT);
        
        for (int c = 0; c < m_iNumChannels; c++)
            
            CHECK_ARRAY_CLOSE(m_ppfOutputData[c], m_ppfOutputCheck[c],60, 1e-3F);
        
    }
    
    
    
    
    
    TEST_FIXTURE(PPM, DCInput)
    
    {
        
        resetValues();
        
        int count=0;
        
        std::string line;
        
        
        
        for (int c = 0; c < m_iNumChannels; c++)
            
        {
            
            CSynthesis::generateDc(m_ppfInputData[c], m_iDataLength);
            
            CVector::setZero(m_ppfOutputCheck[c], 60);
            
        }
        
        
        
        process();
        
        //int iDelay = CUtil::float2int<int>(f_iBlockLength * m_fAlphaAT);
        
        
        
        std::ifstream myfile;
        
        myfile.open("/Users/milaprane/Documents/Milap/Spring 2016/Audio-Software-Engineering-MUSI-8903/PeakMeter/src/Tests/Tests/DCVPPM.txt");
        
        for (int i = 0; i<m_iNumChannels; i++){
            
            if (myfile.is_open())
                
            {   count=0;
                
                while ( getline (myfile,line) )
                    
                {
                    
                    m_ppfOutputCheck[i][count]=std::stof(line,NULL);
                    
                    //std::cout <<m_ppfOutputCheck[i][count]<<" " <<m_ppfOutputData[i][count] <<std::endl;
                    
                    count++;
                    
                }
                
                
                
            }
            
        }
        
        
        
        myfile.close();
        
        for (int c = 0; c < m_iNumChannels; c++)
            
        {
            
            CHECK_ARRAY_CLOSE(m_ppfOutputCheck[c], m_ppfOutputData[c], 60, 1e-3F);
            
        }
        
    }
    
    
    
    
    
    
    
    TEST_FIXTURE(PPM, AlphaATChange)
    
    {
        
        resetValues();
        
        int count=0;
        
        std::string line;
        
        
        
        for (int c = 0; c < m_iNumChannels; c++)
            
        {
            
            CSynthesis::generateDc(m_ppfInputData[c], m_iDataLength);
            
            for(int l=0;l<m_iDataLength;l++)
                
            {
                
                if(l > 50*f_iBlockLength &&  l<  m_iDataLength)
                    
                    m_ppfInputData[c][l]=0;
                
            }
            
        }
        
        
        
        AlphaATChange(0.5);
        
        process();
        
        std::ifstream myfile;
        
        myfile.open("/Users/milaprane/Documents/Milap/Spring 2016/Audio-Software-Engineering-MUSI-8903/PeakMeter/src/Tests/Tests/AlphaATVPPM.txt");
        
        for (int i = 0; i<m_iNumChannels; i++){
            
            if (myfile.is_open())
                
            {
                
                while (getline(myfile,line) )
                    
                {
                    
                    m_ppfOutputCheck[i][count]=std::stof(line,NULL);
                    
                   // std::cout <<m_ppfOutputCheck[i][count] << " " <<m_ppfOutputData[i][count] << " " <<m_ppfInputData[i][count]<<std::endl;
                    
                    count++;
                    
                }
                
                
                
            }
            
        }
        
        myfile.close();
        
        
        
        for (int c = 0; c < m_iNumChannels; c++)
            
            
            
            CHECK_ARRAY_CLOSE(m_ppfOutputCheck[c], m_ppfOutputData[c], 60, 1e-3F);
        
    }
    
    
    
    TEST_FIXTURE(PPM, AlphaRTChange)
    
    {   int count=0;
        
        resetValues();
        
        std::string line;
        
        
        
        for (int c = 0; c < m_iNumChannels; c++)
            
        {
            
            CVector::setZero(m_ppfInputData[c], m_iDataLength);
            
            m_ppfInputData[c][0]=1;
            
        }
        
        
        
        AlphaRTChange(0.75);
        
        process();
        
        std::ifstream myfile;
        
        myfile.open("/Users/milaprane/Documents/Milap/Spring 2016/Audio-Software-Engineering-MUSI-8903/PeakMeter/src/Tests/Tests/AlphaRTVPPM.txt");
        
        for (int i = 0; i<m_iNumChannels; i++){
            
            if (myfile.is_open())
                
            {
                
                while (getline(myfile,line) )
                    
                {
                    
                    m_ppfOutputCheck[i][count]=std::stof(line,NULL);
                    
                    // std::cout <<m_ppfOutputCheck[i][count] << " " <<m_ppfOutputData[i][count] << " " <<m_ppfInputData[i][count]<<std::endl;
                    
                    count++;
                    
                }
                
                
                
            }
            
        }
        
        myfile.close();
        
        
        
        
        
        for (int c = 0; c < m_iNumChannels; c++)
            
            CHECK_ARRAY_CLOSE(m_ppfOutputCheck[c], m_ppfOutputData[c], 4, 1e-3F);
        
    }
    
    
    
    
    
    TEST_FIXTURE(PPM, SamplingFreqChange)
    
    {
        
        int count=0;
        
        resetValues();
        
        std::string line;
        
        
        
        for (int c = 0; c < m_iNumChannels; c++)
            
            CSynthesis::generateSine(m_ppfInputData[c], 330, m_fSamplingFreq/2,  m_iDataLength);
        
        
        
        C_PPm->initPeakMeter(m_fSamplingFreq/2, m_iNumChannels);
        
        AlphaATChange(0.01);
        
        AlphaRTChange(1.5);
        
        process();
        
        
        
        std::ifstream myfile;
        
        myfile.open("/Users/milaprane/Documents/Milap/Spring 2016/Audio-Software-Engineering-MUSI-8903/PeakMeter/src/Tests/Tests/AlphaSampChange.txt");
        
        for (int i = 0; i<m_iNumChannels; i++){
            
            if (myfile.is_open())
                
            {
                
                while (getline(myfile,line) )
                    
                {
                    
                    m_ppfOutputCheck[i][count]=std::stof(line,NULL);
                    
                   // std::cout <<m_ppfOutputCheck[i][count] << " " <<m_ppfOutputData[i][count] << " " <<m_ppfInputData[i][count]<<std::endl;
                    
                    count++;
                    
                }
                
                
                
            }
            
        }
        
        myfile.close();
        

        
        
        for (int c = 0; c < m_iNumChannels; c++)
            
            CHECK_ARRAY_CLOSE(m_ppfOutputCheck[c], m_ppfOutputData[c], 60, 1e-3F);
        
    }
    
    
    
    
    
    
    
    TEST_FIXTURE(PPM, DelayedImpulse)
    
    {
        
        int count=0;
        
        resetValues();
        
        std::string line;

        
        for (int c = 0; c < m_iNumChannels; c++)
            
        {
            
            CVector::setZero(m_ppfInputData[c], m_iDataLength);
            
            m_ppfInputData[c][f_iBlockLength*2]=1;
            
        }
        
        
        
        
        
        
        
        AlphaATChange(0.01);
        
        AlphaRTChange(1.5);
        
        process();
        
        
        std::ifstream myfile;
        
        myfile.open("/Users/milaprane/Documents/Milap/Spring 2016/Audio-Software-Engineering-MUSI-8903/PeakMeter/src/Tests/Tests/DelayedImpulse.txt");
        
        for (int i = 0; i<m_iNumChannels; i++){
            
            if (myfile.is_open())
                
            {
                
                while (getline(myfile,line) )
                    
                {
                    
                    m_ppfOutputCheck[i][count]=std::stof(line,NULL);
                    
                    //std::cout <<m_ppfOutputCheck[i][count] << " " <<m_ppfOutputData[i][count] << " " <<m_ppfInputData[i][count]<<std::endl;
                    
                    count++;
                    
                }
                
                
                
            }
            
        }
        
        myfile.close();
        
        
        
        
        
        
        
        for (int c = 0; c < m_iNumChannels; c++)
            
            CHECK_ARRAY_CLOSE(m_ppfOutputCheck[c], m_ppfOutputData[c], 60, 1e-3F);
        
    }
    
    
    
    
    
}



#endif //WITH_TESTS