
#include "Vector.h"
#include "Util.h"

#include "FastConv.h"

CFastConv::CFastConv( void )
{
    
    reset();
}

CFastConv::~CFastConv( void )
{
    delete [] m_pfImpulseResponse;
    reset();
}


Error_t CFastConv::init(float *pfImpulseResponse, int iLengthOfIr, int iBlockLength /*= 8192*/)
{

    m_pfImpulseResponse = new float[iLengthOfIr];
    m_iLengthOfIr = iLengthOfIr;
    m_iBlockLength = iBlockLength;
    for (int i=0; i<m_iLengthOfIr; i++)
    {
        m_pfImpulseResponse[i] = pfImpulseResponse[i];
    }
    
    return kNoError;
}


Error_t CFastConv::reset()
{
    m_iLengthOfIr = 0;
    m_iLengthOfIr = 0;
    return kNoError;
}


Error_t CFastConv::process (float *pfInputBuffer, float *pfOutputBuffer, int iLengthOfBuffers )
{
    

    return kNoError;
}


Error_t CFastConv::processTimeDomain (float *pfInputBuffer, float *pfOutputBuffer, int iLengthOfBuffers )
{
    for (int i = 0; i< iLengthOfBuffers; i++)
    {
        
    }
    return kNoError;
}

