
#include <iostream>
#include <math.h>

#include "Vector.h"
#include "Util.h"
#include "FastConv.h"

CFastConv::CFastConv( void )
{
    
    reset();
}

CFastConv::~CFastConv( void )
{
    delete[] m_pfImpulseResponse;
    delete m_pCRingBuffCurr;
    delete m_pCRingBuffPrev;
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
    
    delete pCFastConv;
    pCFastConv->reset();
    
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
    
    m_iNumIrBlcks = ceil(m_iLengthOfIr / (double)m_iNxtPow2BlkLen); // number of blocks of IR
    
    m_pfImpulseResponse = new float[m_iNxtPow2BlkLen];
    
    std::memset(m_pfImpulseResponse, 0, sizeof(float)*m_iNxtPow2BlkLen);
    
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
    m_pCFFT->init( (2*m_iNxtPow2BlkLen), iZPfactor, CFft::WindowFunction_t::kWindowHann, CFft::Windowing_t::kNoWindow); //, 2, 1);
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
    m_pfImpulseResponse =0;
    m_pCRingBuffPrev=0;
    m_pCRingBuffCurr=0;
    m_pCFFT = 0;
    m_pfIRfft =0;
    return kNoError;
}


Error_t CFastConv::process (float *pfInputBuffer, float *pfOutputBuffer, int iLengthOfBuffers )
{
    //processTimeDomain ( pfInputBuffer, pfOutputBuffer, iLengthOfBuffers );

    // zeropadding of input signal
    float *pftempBuff= new float [(2*m_iNxtPow2BlkLen)];
    std::memset(pftempBuff, 0, sizeof(float)*2*m_iNxtPow2BlkLen);
    for (int i=0;i<(2*m_iNxtPow2BlkLen); i++)
    {
        if (i<iLengthOfBuffers)
            pftempBuff[i]=pfInputBuffer[i]*2*m_iNxtPow2BlkLen;
        else
            pftempBuff[i]=0;
         //std::cout << i<<" "<<pftempBuff[i] << std::endl;
    }
    
    //std::cout<<"---------" << std::endl;
    
    float *pftempOutput=new float [m_iNxtPow2BlkLen+ m_iNxtPow2BlkLen*(m_iNumIrBlcks)];
    float *pfIRtemp = new float [2*m_iNxtPow2BlkLen];
    std::memset(pftempOutput, 0, sizeof(float)*2*m_iNxtPow2BlkLen);
    std::memset(pfIRtemp, 0, sizeof(float)*2*m_iNxtPow2BlkLen);
    
    for (int k =0; k<m_iNumIrBlcks; k++)
    {
        for (int i=0; i<(2*m_iNxtPow2BlkLen); i++)
        {   if (((k*m_iNxtPow2BlkLen)+i)<m_iLengthOfIr && i< m_iNxtPow2BlkLen)
            pfIRtemp[i]=m_pfImpulseResponse[(k*m_iNxtPow2BlkLen)+i];
        else
            pfIRtemp[i]=0;
            
            //std::cout<<pfIRtemp[i] << std::endl;
        }
        //std::cout<<"---------" << std::endl;
        
        
        //pftempOutput array can be fed to fft and output can be obtained
        float *pfSpectrumInput= new float [2*m_iNxtPow2BlkLen];
        std::memset(pfSpectrumInput, 0, sizeof(float)*2*m_iNxtPow2BlkLen);
        m_pCFFT->doFft( pfSpectrumInput, pftempBuff);
        
        float *pfSpectrumIR= new float [2*m_iNxtPow2BlkLen];
        std::memset(pfSpectrumIR, 0, sizeof(float)*2*m_iNxtPow2BlkLen);
        m_pCFFT->doFft( pfSpectrumIR, pfIRtemp);
        
        m_pCFFT->mulCompSpectrum(pfSpectrumInput, pfSpectrumIR);
        
        float *pfIFFTtemp= new float [2*m_iNxtPow2BlkLen];
        std::memset(pfIFFTtemp, 0, sizeof(float)*2*m_iNxtPow2BlkLen);
        m_pCFFT->doInvFft (pfIFFTtemp, pfSpectrumInput);
        
        for (int i = 0; i< (2*m_iNxtPow2BlkLen)-1; i++)
        {
            pftempOutput[(k*m_iNxtPow2BlkLen)+i] += pfIFFTtemp[i];
            //std::cout << (k*m_iNxtPow2BlkLen)+i << " " << pfSpectrumInput[i] << " " << pfSpectrumIR[i] << " " << pfIFFTtemp[i] << " " << pftempOutput[(k*m_iNxtPow2BlkLen)+i]<< std::endl;
        }
        
        
    }
    // update the Current buffer with the blocked convolution output
    for (int iter=0; iter<(m_iLengthOfIr+iLengthOfBuffers-1); iter++)
    {
        m_pCRingBuffCurr->putPostInc(pftempOutput[iter]);
        // std::cout << pftempOutput[iter] << std::endl;
    }
    //std::cout<<"---------" << std::endl;
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
    // the reverb tail stored in RingBuffPrev will be either LengthOfIr-1 or less than that depending on the Buffer length provided by the user
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

    delete[] pftempBuff;
    pftempBuff = 0;
    
    return kNoError;
}


Error_t CFastConv::processTimeDomain (float *pfInputBuffer, float *pfOutputBuffer, int iLengthOfBuffers )
{
    // zeropadding of input signal
    float *pftempBuff= new float [(2*m_iNxtPow2BlkLen)];
    std::memset(pftempBuff, 0, sizeof(float)*2*m_iNxtPow2BlkLen);
    for (int i=0;i<(2*m_iNxtPow2BlkLen); i++)
    {
        if (i<iLengthOfBuffers)
            pftempBuff[i]=pfInputBuffer[i];
        else
            pftempBuff[i]=0;
       std::cout << i<<" "<<pftempBuff[i] << std::endl;
    }
    
    std::cout<<"---------" << std::endl;
    
    float *pftempOutput=new float [m_iNxtPow2BlkLen+ m_iNxtPow2BlkLen*(m_iNumIrBlcks)];
    float *pfIRtemp = new float [2*m_iNxtPow2BlkLen];
    std::memset(pftempOutput, 0, sizeof(float)*2*m_iNxtPow2BlkLen);
    std::memset(pfIRtemp, 0, sizeof(float)*2*m_iNxtPow2BlkLen);
    
    for (int k =0; k<m_iNumIrBlcks; k++)
    {
        for (int i=0; i<(2*m_iNxtPow2BlkLen); i++)
        {   if (((k*m_iNxtPow2BlkLen)+i)<m_iLengthOfIr && i< m_iNxtPow2BlkLen)
                pfIRtemp[i]=m_pfImpulseResponse[(k*m_iNxtPow2BlkLen)+i];
            else
                pfIRtemp[i]=0;
            
            //std::cout<<pfIRtemp[i] << std::endl;
        }
       
        
    // convolution loop
        for (int i = 0; i< (2*m_iNxtPow2BlkLen)-1; i++)
        {
            float ftempVal = 0;
            for (int j=0; j<m_iNxtPow2BlkLen; j++)
            {
                if ((j-i)>=0)
                {
                    ftempVal += pfIRtemp[j-i] * pftempBuff[j];
                   // std::cout << pfIRtemp[j]<< " "<<pftempBuff[j-i]<<" "<< ftempVal <<std::endl;
                /*
                if (pfIRtemp[i-j]>0 && pfIRtemp[i-j]<1)
                    std::cout << pfIRtemp[i-j] <<std::endl;
                if (pftempBuff[j] > 1)
                    std::cout << pftempBuff[j] <<std::endl;
                 */
                }
                else
                    ftempVal += pfIRtemp[(i-j)%(2*m_iNxtPow2BlkLen)+(2*m_iNxtPow2BlkLen)]* pftempBuff[j];
               // std::cout << pfIRtemp[j]<< " "<<pftempBuff[(i-j)%(2*m_iNxtPow2BlkLen)+(2*m_iNxtPow2BlkLen)]<<" "<< ftempVal <<std::endl;
            }
        
            pftempOutput[i] += ftempVal;
           // std::cout << pfIRtemp[(k*m_iNxtPow2BlkLen)+i] << " " << ftempVal << " " << pftempOutput[(k*m_iNxtPow2BlkLen)+i]<< std::endl;
            //std::cout<<"---------" << std::endl;
         }
        
     }
    
    for (int iter=0; iter<(m_iLengthOfIr+iLengthOfBuffers-1); iter++)
    {
        m_pCRingBuffCurr->putPostInc(pftempOutput[iter]);
       // std::cout << pftempOutput[iter] << std::endl;
    }
    //std::cout<<"---------" << std::endl;
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
    
    delete[] pftempBuff;
    pftempBuff = 0;
    
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
        //std::cout << pfOutputBuffer[i]<<std::endl;
    }
    
    return kNoError;
}

