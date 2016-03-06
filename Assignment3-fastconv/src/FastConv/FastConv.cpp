
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
    //delete [] m_pCRingBuffCurr;
    //delete [] m_pCRingBuffPrev;
    reset();
}

Error_t CFastConv::createInstance (CFastConv*& pCFastConv)
{
    pCFastConv = new CFastConv ();
    
    if (!pCFastConv)
        return kUnknownError;
    
    
    return kNoError;
}

Error_t CFastConv::destroyInstance (CFastConv*& pCFastConv)
{
    if (!pCFastConv)
        return kUnknownError;
    
    pCFastConv->reset();
    
    delete pCFastConv;
    pCFastConv = 0;
    
    return kNoError;
    
}

Error_t CFastConv::init(float *pfImpulseResponse, int iLengthOfIr, int iBlockLength /*= 8192*/)
{

    m_iLengthOfIr = iLengthOfIr;
    m_iBlockLength = iBlockLength;
    m_pfImpulseResponse = new float[iLengthOfIr];
    
    for (int i=0; i<(m_iLengthOfIr+m_iBlockLength-1); i++)
    {
        if (i<m_iLengthOfIr)
            m_pfImpulseResponse[i] = pfImpulseResponse[i];  //m_iLengthOfIr - i -1 //store the impulse response
        else
            m_pfImpulseResponse[i] = 0;
    }
    m_pCRingBuffCurr = new CRingBuffer<float> ((m_iBlockLength+m_iLengthOfIr-1)*2);
    //m_pCRingBuffCurr->setWriteIdx(1);
    m_pCRingBuffPrev = new CRingBuffer<float> ((m_iBlockLength+m_iLengthOfIr-1)*2);
    m_pCRingBuffPrev->setWriteIdx(m_iLengthOfIr-1);
    
    return kNoError;
}


Error_t CFastConv::reset()
{
    m_iLengthOfIr = 0;
    m_iBlockLength = 0;
    //delete m_pfImpulseResponse;
    m_pfImpulseResponse =0;
    //delete m_pCRingBuffPrev;
    m_pCRingBuffPrev =0;
    //delete m_pCRingBuffCurr;
    m_pCRingBuffCurr =0;
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
    for (int i=0;i<iLengthOfBuffers+m_iLengthOfIr-1; i++)
    {
        if (i<iLengthOfBuffers)
            pftempBuff[i]=pfInputBuffer[i];
        else
            pftempBuff[i]=0;
    }
    
    // convolution loop
    for (int i = 0; i< (iLengthOfBuffers+m_iLengthOfIr-1); i++)
    {
        float ftempVal = 0;
        for (int j=0; j<iLengthOfBuffers; j++)
        {
            if ((i-j)>=0)
            ftempVal += m_pfImpulseResponse [i-j] * pftempBuff[j];
        }
        m_pCRingBuffCurr->putPostInc(ftempVal);
        //m_pCRingBuffCurr->getPostInc();    // dummy call just to increment the read index
        
    }
    
    // add the prev reverb tail with starting iLengthOfBuffers number of current output values
    for (int i=0; i<iLengthOfBuffers; i++)
    {
        if (i < m_iLengthOfIr-1)
        {
            pfOutputBuffer[i]=m_pCRingBuffCurr->getPostInc()+m_pCRingBuffPrev->getPostInc();
        }
        else
            pfOutputBuffer[i]=m_pCRingBuffCurr->getPostInc();  // when length of buffer is greater than length of IR
            
    }
    
    if (iLengthOfBuffers < m_iLengthOfIr) // if length of buffer is less than length of IR
    {
        for (int i=0; i<(m_iLengthOfIr - iLengthOfBuffers-1); i++)
        {
            m_pCRingBuffPrev->putPostInc(m_pCRingBuffCurr->getPostInc()+m_pCRingBuffPrev->getPostInc());
        }
    }
    
    for (int i=0; i<m_iLengthOfIr-1; i++)
    {
        m_pCRingBuffPrev->putPostInc(m_pCRingBuffCurr->getPostInc());
    }
    
    delete [] pftempBuff;
    
    return kNoError;
}

Error_t CFastConv::flushBuffer(float *pfOutputBuffer, int iLengthOfBuffer)
{// user should provide iLengthOfBuffer atleast equal to the impulse response length as that much will remain in the end
    for (int i=0; i<iLengthOfBuffer; i++)
    {
        if (i<m_iLengthOfIr)
            pfOutputBuffer[i]= m_pCRingBuffPrev->getPostInc();
        else
            pfOutputBuffer[i]=0;
    }
    
    return kNoError;
}

