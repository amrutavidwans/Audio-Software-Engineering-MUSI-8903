//
//  main.cpp
//  TestClassification
//
//  Created by Amruta Vidwans on 5/3/16.
//  Copyright © 2016 Amruta Vidwans. All rights reserved.
//

#include <iostream>
#include "AudioFileIf.h"
#include "NormalizeFeatures.h"
#include "SpectralCentroid.h"
#include "TimeRMS.h"
#include "svm.h"
#include "Vector.h"
#include <Vector>
#include <stdio.h>
#include <string>
#include <iterator>
#include <sstream>
#include <fstream>

#define Malloc(type,n) (type *)malloc((n)*sizeof(type))
struct svm_parameter param;
struct svm_problem prob;
//struct svm_model *model;
struct svm_node *x_space;

int main(int argc, const char * argv[]) {
    
    float **m_ppfInputFeat;
    float *m_pfOutputCheck;
    float *m_pfOutputHere;
    std::string Address = "";
    CNormalizeFeatures *m_pCNormFeat;
    
    char model_file_name[1024] = "MyModelDoneHereRBF2" ;
    //char model_file_name2[1024] = "MyModelResave" ;
    
    // feature file
    std::ifstream myfile;
    std::string line;
    
    CNormalizeFeatures::createInstance(m_pCNormFeat);
    myfile.open(Address+"TestOrigFeat.txt");
    
    m_ppfInputFeat = new float *[2];
    for (int i = 0; i<2; i++){
        m_ppfInputFeat[i] = new float[102];
        CVector::setZero(m_ppfInputFeat[i], 102);
    }
    m_pfOutputCheck = new float [102];
    CVector::setZero(m_pfOutputCheck, 102);
    m_pfOutputHere = new float [102];
    CVector::setZero(m_pfOutputHere, 102);

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
    
    myfile.open(Address+"CurrentPredictionMatlab.txt");
    
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
            
            m_pfOutputCheck[count]=std::stof(line,NULL);
            
            //m_pfOutputCheck[count] = std::stof(tokens);
            //std::cout<<tokens[0]<<" "<<tokens[1]<<std::endl;
            //std::cout <<m_ppfOutputCheck[0][count]<<" " <<m_ppfOutputCheck[1][count] <<std::endl;
            count++;
        }
    }
    myfile.close();
    
    // normalize features
    for (int i = 0; i<102; i++){
        m_pCNormFeat->normalize(m_ppfInputFeat[0][i], CNormalizeFeatures::SelectFeat_t::kSpecCentroid);
    }
    
    for (int i = 0; i<102; i++){
        m_pCNormFeat->normalize(m_ppfInputFeat[1][i], CNormalizeFeatures::SelectFeat_t::kTimeRMS);
    }
    
    
    svm_model *model = svm_load_model(model_file_name);
    //svm_save_model(model_file_name2, model);
    
    //Test model----------------------------------------------------------------------
    svm_node* testnode = Malloc(svm_node,3);
    
    float sum = 0;
    double retval = 0.0;
    for (int i = 0; i<102; i++){
        
        testnode[0].index = 0;
        testnode[0].value = m_ppfInputFeat[0][i];
        std::cout<<testnode[0].value<<" "<<std::endl;
        testnode[1].index = 1;
        testnode[1].value = m_ppfInputFeat[1][i];
        std::cout<<testnode[1].value<<" "<<std::endl;
        testnode[3].index=-1;
        std::cout<<"Hi"<<std::endl;
        retval = svm_predict(model,testnode);
        printf("MODEL OP: %f\t",retval);
        
        m_pfOutputHere[i] = retval;
        sum += m_pfOutputHere[i]-m_pfOutputCheck[i];
        printf("Matlab OP: %f\n",m_pfOutputCheck[i]);
        
    }
    
    printf("\n");
    printf("Final Sum %f\n",sum);
    
    
   // testnode[0].index = 0;
   // testnode[0].value = 0.6;
   // testnode[1].index = 1;
   // testnode[1].value = 0.7;
   // testnode[2].index = -1;
    
    
    
    // destroy
    CNormalizeFeatures::destroyInstance(m_pCNormFeat);
    for (int i = 0; i < 2; i++){
        delete[] m_ppfInputFeat[i];
    }
    delete[] m_ppfInputFeat;
    delete[] m_pfOutputCheck;
    
    
}
