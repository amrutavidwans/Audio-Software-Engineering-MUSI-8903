
#include "Vector.h"
#include "Util.h"
#include "RingBuffer.h"

#include "FastConv.h"

CFastConv::CFastConv( void )
{
    reset();
}

CFastConv::~CFastConv( void )
{
    delete [] m_pfImpulseResponse;
    delete [] m_pCRingBuffCurr;
    delete [] m_pCRingBuffPrev;
    reset();
}


Error_t CFastConv::init(float *pfImpulseResponse, int iLengthOfIr, int iBlockLength /*= 8192*/)
{

    m_pfImpulseResponse = new float[iLengthOfIr];
    m_iLengthOfIr = iLengthOfIr;
    m_iBlockLength = iBlockLength;
    for (int i=0; i<m_iLengthOfIr; i++)
    {
        // store the inverted impulse response
        m_pfImpulseResponse[i] = pfImpulseResponse[m_iLengthOfIr - i -1];
    }
    m_pCRingBuffCurr = new CRingBuffer<float> ((m_iBlockLength+m_iLengthOfIr-1)*2);
    m_pCRingBuffCurr->setWriteIdx(m_iBlockLength+m_iLengthOfIr);
    m_pCRingBuffPrev = new CRingBuffer<float> ((m_iBlockLength+m_iLengthOfIr-1)*2);
    m_pCRingBuffPrev->setWriteIdx(m_iLengthOfIr+1);
    
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
    processTimeDomain ( pfInputBuffer, pfOutputBuffer, iLengthOfBuffers );

    return kNoError;
}


Error_t CFastConv::processTimeDomain (float *pfInputBuffer, float *pfOutputBuffer, int iLengthOfBuffers )
{
    // zeropadding of input signal
    float *pftempBuff= new float [iLengthOfBuffers+m_iLengthOfIr-1];
    for (int i=0;i<iLengthOfBuffers; i++)
    {
        pftempBuff[i]=pfInputBuffer[i];
    }
    for (int i=iLengthOfBuffers; i<iLengthOfBuffers+m_iLengthOfIr-1; i++)
    {
        pftempBuff[i]=0;
    }
    
    for (int i = 0; i< (iLengthOfBuffers+m_iLengthOfIr-1); i++)
    {
        float ftempVal = 0;
        for (int j=0; j<m_iLengthOfIr; j++)
        {
            ftempVal += m_pfImpulseResponse [j] * pftempBuff[i];
        }
        m_pCRingBuffCurr->putPostInc(ftempVal);
        m_pCRingBuffCurr->getPostInc();
        
    }
    
    // add the prev reverb tail with starting iLengthOfBuffers number of current output values
    int count =0;
    for (int i=0; i<iLengthOfBuffers; i++)
    {
        if (count < m_iLengthOfIr)
        {
            pfOutputBuffer[i]=m_pCRingBuffCurr->getPostInc()+m_pCRingBuffPrev->getPostInc();
            count +=1;
        }
        else
            pfOutputBuffer[i]=m_pCRingBuffCurr->getPostInc();
            
    }
    
    for (int i=0; i<m_iLengthOfIr; i++)
    {
        m_pCRingBuffPrev->putPostInc(m_pCRingBuffCurr->getPostInc());
    }
    
    delete [] pftempBuff;
    
    return kNoError;
}

