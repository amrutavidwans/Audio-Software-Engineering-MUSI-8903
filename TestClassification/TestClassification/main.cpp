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
    char input_file_name[1024];
    char model_file_name[1024] = "MyModelDoneHereRBF2" ;
    const char *error_msg;
    
    param.svm_type = EPSILON_SVR;
    param.eps = 1e-5;
    param.C = 1;
    param.kernel_type = LINEAR;
   // param.degree = 3;
    //    param.gamma = 0.5;
    //    param.coef0 = 0;
    //    param.nu = 0.5;
    //    param.cache_size = 100;
    // param.C = 1;
    //    param.eps = 1e-3;
    //    param.p = 0.1;
    //    param.shrinking = 1;
    //    param.probability = 1;
    //    param.nr_weight = 0;
    //    param.weight_label = NULL;
    //    param.weight = NULL;
    
    std::string Address = "/Users/Amruta/Documents/MS_GTCMT/Sem2/AudioSoftwareEngineering/Audio-Software-Engineering-MUSI-8903/TestClassification/";
    std::ifstream myfile1;
    myfile1.open("TrainNormFeat_small.txt");
    std::string line;
    
    float *m_pfTrainLabels;
    
    //Problem definition-------------------------------------------------------------
    prob.l = 20000;
    m_pfTrainLabels = new float [prob.l];
    
    //x values matrix of xor values
    float **matrix;
    matrix = new float *[prob.l]; //[prob.l][2];
    for (int j = 0; j< prob.l; j++) {
        matrix[j] = new float[2];
        }

    
    // give feature values to matrix to be passed for training
    for (int i = 0; i<prob.l; i++) {
        for (int j = 0; j<2; j++) {
            matrix[i][j]=0;
        }
    }
    
    
    
    if (myfile1.is_open())
    {
        int count=0;
        while ( getline (myfile1,line) )
        {
            //std::cout<<line<<std::endl;
            std::istringstream iss(line);
            std::vector<std::string> tokens;
            copy(std::istream_iterator<std::string>(iss),
                 std::istream_iterator<std::string>(),
                 std::back_inserter(tokens));
            
            
            matrix[count][0] = std::stof(tokens[0]);
            matrix[count][1] = std::stof(tokens[1]);
            //std::cout<<tokens[0]<<" "<<tokens[1]<<std::endl;
            //std::cout <<m_ppfInputFeat[0][count]<<" " <<m_ppfInputFeat[1][count] <<std::endl;
            count++;
        }
        
    }
    myfile1.close();
    
    svm_node** x = Malloc(svm_node*,prob.l);
    
    //Trying to assign from matrix to svm_node training examples
    for (int row = 0;row <prob.l; row++){
        svm_node* x_space = Malloc(svm_node,3);
        for (int col = 0;col < 2;col++){
            x_space[col].index = col;
            x_space[col].value = matrix[row][col];
        }
        x_space[2].index = -1;      //Each row of properties should be terminated with a -1 according to the readme
        x[row] = x_space;
    }
    
    prob.x = x;
    
    std::ifstream myfile;
    // read label file
    myfile.open("TrainLabelsMatlab_small.txt");
    
    if (myfile.is_open())
    {
        int count=0;
        while ( getline (myfile,line) )
        {
            m_pfTrainLabels[count]=std::stof(line,NULL);
            count++;
        }
    }
    myfile.close();
    
    //yvalues i.e. the train labels
    prob.y = Malloc(double,prob.l);
    for (int i = 0; i<prob.l; i++) {
        prob.y[i] = m_pfTrainLabels[i];
    }
    
    
    //Train model---------------------------------------------------------------------
    svm_model *model = svm_train(&prob,&param);
    //svm_model *model = svm_load_model(model_file_name);
    svm_save_model(model_file_name, model);
    
    //Test model----------------------------------------------------------------------
    svm_node* testnode = Malloc(svm_node,3);
    testnode[0].index = 0;
    testnode[0].value = 0.0076806;
    testnode[1].index = 1;
    testnode[1].value = 0.1249;
    testnode[2].index = -1;
    
    //This works correctly:
    double retval = svm_predict(model,testnode);
    
    
    printf("retval: %f\n",retval);
    printf("needed: %f\n",0.17323F);
    
    svm_destroy_param(&param);
    free(prob.y);
    free(prob.x);
    free(x_space);
    delete[] m_pfTrainLabels;
    for (int i=0; i<prob.l; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
    
    /*
    float **m_ppfInputFeat;
    float *m_pfOutputCheck;
    float *m_pfOutputHere;
    std::string Address = "";
    CNormalizeFeatures *m_pCNormFeat;
    
    char model_file_name[1024] = "MyModel" ;
    
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
    
    //Test model----------------------------------------------------------------------
    svm_node* testnode = Malloc(svm_node,3);
    
    float sum = 0;
    for (int i = 0; i<102; i++){
        
        testnode[0].index = 0;
        testnode[0].value = m_ppfInputFeat[0][i];
        testnode[1].index = 1;
        testnode[1].value = m_ppfInputFeat[1][i];
        testnode[3].index=-1;
        
        double retval = svm_predict(model,testnode);
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
    */
    
}
