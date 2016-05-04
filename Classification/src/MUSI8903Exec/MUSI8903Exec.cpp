
#include <iostream>
#include <ctime>

#include "MUSI8903Config.h"

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

#define Malloc(type,n) (type *)malloc((n)*sizeof(type))

using std::cout;
using std::endl;

// local function declarations
void    showClInfo ();

/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
    std::string             sInputFilePath,                 //!< file paths
                            sOutputFilePath,
                            sImpRespPath;

    static const int        kBlockSize          = 1024;

    clock_t                 time                = 0;

    float                   **ppfAudioData      = 0;
    float                   **ppfImpRespData    = 0;

    CAudioFileIf            *phAudioFile        = 0;
    
    CAudioFileIf            *phImpRespFile      = 0;

    std::fstream            hInputFile;
    std::fstream            hImpResp;
    std::fstream            hOutputFile;
    
    CAudioFileIf::FileSpec_t stFileSpec;
    CAudioFileIf::FileSpec_t stImpRespSpec;
    
    float **m_ppfInputFeat;
    float *m_pfOutputCheck;
    std::string Address = "../../src/Tests/Tests/";
    CNormalizeFeatures *m_pCNormFeat;
    
    char model_file_name[1024] = "../../src/MUSI8903Exec/MyModel" ;

    // feature file
    std::ifstream myfile;
    std::string line;
    
    CNormalizeFeatures::createInstance(m_pCNormFeat);
    myfile.open(Address+"TestOrigFeat.txt");
    
    m_ppfInputFeat = new float *[2];
    m_pfOutputCheck = new float [102];
    for (int i = 0; i<2; i++){
        m_ppfInputFeat[i] = new float[102];
        CVector::setZero(m_ppfInputFeat[i], 102);
    }
    CVector::setZero(m_pfOutputCheck, 102);
  
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
    
    
    //svm_model *model = svm_load_model(model_file_name);
    
    //Test model----------------------------------------------------------------------
    svm_node* testnode = Malloc(svm_node,3);
    testnode[0].index = 0;
    testnode[0].value = 0.6;
    testnode[1].index = 1;
    testnode[1].value = 0.7;
    testnode[2].index = -1;
    
    //This works correctly:
    //double retval = svm_predict(model,testnode);
    //printf("retval: %f\n",retval);
    
    
   // destroy
    CNormalizeFeatures::destroyInstance(m_pCNormFeat);
    for (int i = 0; i < 2; i++){
        delete[] m_ppfInputFeat[i];
    }
    delete[] m_ppfInputFeat;
    delete[] m_pfOutputCheck;

    
    return 0;
    
}


void     showClInfo()
{
    cout << "GTCMT MUSI8903" << endl;
    //cout << "(c) 2016 by Alexander Lerch" << endl;
    cout  << endl;

    return;
}

