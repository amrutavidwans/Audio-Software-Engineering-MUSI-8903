
#include "Vector.h"
#include "Util.h"
#include "RingBuffer.h"
#include <iostream>
#include "Fft.h"
#include <math.h>

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
    
    if (!CUtil::isPowOf2(iBlockLength))
        m_iNxtPow2BlkLen =CUtil::nextPowOf2(m_iBlockLength);  // for fft to be power of 2
    else
        m_iNxtPow2BlkLen = iBlockLength;
    
    m_iNumIrBlcks = ceil(m_iLengthOfIr / (double)m_iNxtPow2BlkLen);
    
    m_pfImpulseResponse = new float[m_iNxtPow2BlkLen];
    
    std::memset(m_pfImpulseResponse, 0, m_iNxtPow2BlkLen);
    
    for (int i=0; i<(m_iLengthOfIr); i++)
    {
        m_pfImpulseResponse[i] = pfImpulseResponse[i];
    }
    
    m_pCRingBuffCurr = new CRingBuffer<float> ((m_iLengthOfIr+m_iBlockLength-1)*4);
    //m_pCRingBuffCurr->setWriteIdx(1);
    m_pCRingBuffPrev = new CRingBuffer<float> ((m_iLengthOfIr+m_iBlockLength-1)*4);
    m_pCRingBuffPrev->setWriteIdx(m_iLengthOfIr-1);
    
    int iZPfactor = 1;
    CFft::create(m_pCFFT);
    m_pCFFT->init( m_iBlockLength, iZPfactor); //, 2, 1); 
     //, WindowFunction_t eWindow = kWindowHann, Windowing_t eWindowing = kPreWindow );
    
    m_pfIRfft = new float[m_iLengthOfIr+m_iBlockLength-1];
//    m_pCFFT->doFft( pfSpectrum, pfInputBuffer );
    
    return kNoError;
}


Error_t CFastConv::reset()
{
    m_iLengthOfIr = 0;
    m_iBlockLength = 0;
    m_iNumIrBlcks = 0;
    m_iNxtPow2BlkLen = 0;
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
    float *pftempBuff= new float [(2*m_iNxtPow2BlkLen)];
    std::memset(pftempBuff, 0, 2*m_iNxtPow2BlkLen);
    for (int i=0;i<(2*m_iNxtPow2BlkLen); i++)
    {
        if (i<iLengthOfBuffers)
            pftempBuff[i]=pfInputBuffer[i];
        else
            pftempBuff[i]=0;
       // std::cout << i<<" "<<pftempBuff[i] << std::endl;
    }
    
    std::cout<<"---------" << std::endl;
    
    float *pftempOutput=new float [m_iNxtPow2BlkLen+ m_iNxtPow2BlkLen*(m_iNumIrBlcks)];
    float *pfIRtemp = new float [2*m_iNxtPow2BlkLen];
    std::memset(pftempOutput, 0, 2*m_iNxtPow2BlkLen);
    std::memset(pfIRtemp, 0, 2*m_iNxtPow2BlkLen);
    
    for (int k =0; k<m_iNumIrBlcks; k++)
    {
        for (int i=0; i<(2*m_iNxtPow2BlkLen); i++)
        {   if (((k*m_iNxtPow2BlkLen)+i)<m_iLengthOfIr && i< m_iNxtPow2BlkLen)
                pfIRtemp[i]=m_pfImpulseResponse[(k*m_iNxtPow2BlkLen)+i];
            else
                pfIRtemp[i]=0;
            
            //std::cout<<pfIRtemp[i] << std::endl;
        }
        std::cout<<"---------" << std::endl;
        
    // convolution loop
        for (int i = 0; i< (2*m_iNxtPow2BlkLen)-1; i++)
        {
            float ftempVal = 0;
            for (int j=0; j<m_iNxtPow2BlkLen; j++)
            {
                if ((i-j)>=0)
                {
                    ftempVal += pfIRtemp[(i-j)] * pftempBuff[j];
                /*
                if (m_pfImpulseResponse[i-j]>0 && m_pfImpulseResponse[i-j]<1)
                    std::cout << m_pfImpulseResponse[i-j] <<std::endl;
                if (pftempBuff[j] > 1)
                    std::cout << pftempBuff[j] <<std::endl;
                 */
                }
                else
                    ftempVal += pfIRtemp[((i-j)%(2*m_iNxtPow2BlkLen))+(2*m_iNxtPow2BlkLen)] * pftempBuff[j];
            }
        
            pftempOutput[(k*m_iNxtPow2BlkLen)+i] += ftempVal;
            //std::cout << (k*m_iNxtPow2BlkLen)+i << " " << ftempVal << " " << pftempOutput[(k*m_iNxtPow2BlkLen)+i]<< std::endl;
            
         }
        
     }
    
    for (int iter=0; iter<(m_iLengthOfIr+iLengthOfBuffers-1); iter++)
    {
        m_pCRingBuffCurr->putPostInc(pftempOutput[iter]);
       // std::cout << pftempOutput[iter] << std::endl;
    }
    std::cout<<"---------" << std::endl;
     //std::cout << "-----" << std::endl;
    
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
    
    int iNewWrtPrev =(m_iLengthOfIr - iLengthOfBuffers-1);
    
    if (iLengthOfBuffers < m_iLengthOfIr) // if length of buffer is less than length of IR
    {
        for (int i=0; i<iNewWrtPrev; i++)
        {
            m_pCRingBuffPrev->putPostInc(m_pCRingBuffCurr->getPostInc()+m_pCRingBuffPrev->getPostInc());
        }
    }
    
    if (iNewWrtPrev < 0)
    {
        for (int i=0; i<(m_iLengthOfIr-1); i++)
        {
            m_pCRingBuffPrev->putPostInc(m_pCRingBuffCurr->getPostInc());
        }
    }
    else
    {
        for (int i=0; i<(m_iLengthOfIr-iNewWrtPrev-1); i++)
        {
            m_pCRingBuffPrev->putPostInc(m_pCRingBuffCurr->getPostInc());
        }
        
    }
    
    //delete [] pftempBuff;
    
    return kNoError;
}

Error_t CFastConv::flushBuffer(float *pfOutputBuffer, int iLengthOfBuffer)
{// user should provide iLengthOfBuffer atleast equal to the impulse response length as that much will remain in the end
    for (int i=0; i<iLengthOfBuffer; i++)
    {
        if (i<m_iLengthOfIr-1)
            pfOutputBuffer[i]= m_pCRingBuffPrev->getPostInc();
        else
            pfOutputBuffer[i]=0;
    }
    
    return kNoError;
}

