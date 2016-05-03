//
//  Test_NormalizeFeatures..cpp
//  MUSI8903
//
//  Created by Amruta Vidwans on 5/1/16.
//
//
#include "MUSI8903Config.h"
#include "UnitTest++.h"
#include "NormalizeFeatures.h"
#include <sstream>
#include <stdio.h>
#include <string>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <Vector>
#include "Vector.h"
#include <fstream>

SUITE(NormFeat)
{
    struct NormFeat
    {
        NormFeat()
        {
            CNormalizeFeatures::createInstance(m_pCNormFeat);
            m_ppfOutputCheck = new float *[2];
            m_ppfInputFeat = new float *[2];
            m_ppfNormOutput = new float *[2];
            for (int i = 0; i<2; i++){
                m_ppfOutputCheck[i] = new float[102];
                CVector::setZero(m_ppfOutputCheck[i], 102);
                
                m_ppfInputFeat[i] = new float[102];
                CVector::setZero(m_ppfInputFeat[i], 102);
                
                m_ppfNormOutput[i] = new float[102];
                CVector::setZero(m_ppfNormOutput[i], 102);
            }
        }
        
        ~NormFeat(){
            CNormalizeFeatures::destroyInstance(m_pCNormFeat);
            for (int i = 0; i < 2; i++){
                delete[] m_ppfOutputCheck[i];
                delete[] m_ppfInputFeat[i];
                delete[] m_ppfNormOutput[i];
            }
            delete[] m_ppfOutputCheck;
            delete[] m_ppfInputFeat;
            delete[] m_ppfNormOutput;
        }
        
        
        float **m_ppfOutputCheck;
        float **m_ppfInputFeat;
        float **m_ppfNormOutput;
        CNormalizeFeatures *m_pCNormFeat;
        std::string Address = "../../src/Tests/Tests/";
    };
    
    TEST_FIXTURE(NormFeat,Input)
    {
        // normalized feature file
        std::ifstream myfile;
        std::string line;
        myfile.open(Address+"TestNormFeat.txt");
        
        
        if (myfile.is_open())
        {
            int count=0;
            while ( getline (myfile,line) )
            {
                //std::cout<<line<<std::endl;
                std::istringstream iss(line);
                std::vector<std::string> tokens;
                copy(std::istream_iterator<std::string>(iss),
                     std::istream_iterator<std::string>(),
                     std::back_inserter(tokens));
                
                //m_ppfOutputCheck[i][count]=std::stof(line,NULL);
                
                m_ppfOutputCheck[0][count] = std::stof(tokens[0]);
                m_ppfOutputCheck[1][count] = std::stof(tokens[1]);
                //std::cout<<tokens[0]<<" "<<tokens[1]<<std::endl;
                //std::cout <<m_ppfOutputCheck[0][count]<<" " <<m_ppfOutputCheck[1][count] <<std::endl;
                count++;
            }
        }
        
        myfile.close();
        
        // feature file
        myfile.open(Address+"TestOrigFeat.txt");
        
        if (myfile.is_open())
        {
            int count=0;
            while ( getline (myfile,line) )
            {
                //std::cout<<line<<std::endl;
                std::istringstream iss(line);
                std::vector<std::string> tokens;
                copy(std::istream_iterator<std::string>(iss),
                     std::istream_iterator<std::string>(),
                     std::back_inserter(tokens));
                
                
                m_ppfInputFeat[0][count] = std::stof(tokens[0]);
                m_ppfInputFeat[1][count] = std::stof(tokens[1]);
                //std::cout<<tokens[0]<<" "<<tokens[1]<<std::endl;
                //std::cout <<m_ppfInputFeat[0][count]<<" " <<m_ppfInputFeat[1][count] <<std::endl;
                count++;
            }

        }
        
        myfile.close();

        
        for (int i = 0; i<102; i++){
            m_pCNormFeat->normalize(m_ppfInputFeat[0][i], CNormalizeFeatures::SelectFeat_t::kSpecCentroid);
            CHECK_CLOSE(m_ppfOutputCheck[0][i], m_ppfInputFeat[0][i], 1e-3F);
        }
        
        for (int i = 0; i<102; i++){
            m_pCNormFeat->normalize(m_ppfInputFeat[1][i], CNormalizeFeatures::SelectFeat_t::kTimeRMS);
            CHECK_CLOSE(m_ppfOutputCheck[1][i], m_ppfInputFeat[1][i], 1e-3F);
        }
        
        
        
    }
    
    
    
}
