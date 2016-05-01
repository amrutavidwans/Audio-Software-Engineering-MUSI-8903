
#define _USE_MATH_DEFINES
#include <cmath>

#include "Util.h"
#include "Vector.h"
#include "Fft.h"

#include "rvfft.h"

const float CFft::m_Pi  = static_cast<float>(M_PI);
const float CFft::m_Pi2 = static_cast<float>(M_PI_2);

CFft::CFft() :
    m_pfProcessBuff(0),
    m_pfWindowBuff(0),
    m_iDataLength(0),
    m_iFftLength(0),
    m_ePrePostWindowOpt(kNoWindow),
    m_bIsInitialized(false)
{
    reset ();
}

Error_t CFft::create( CFft*& pCFft )
{
    pCFft = new CFft ();

    if (!pCFft)
        return kMemError;

    return kNoError;
}

Error_t CFft::destroy( CFft*& pCFft )
{
    if (!pCFft)
        return kNoError;

    delete pCFft;
    pCFft   = 0;

    return kNoError;
}

Error_t CFft::init( int iBlockLength, int iZeroPadFactor, WindowFunction_t eWindow /*= kWindowHann*/, Windowing_t eWindowing /*= kPreWindow*/ )
{
    Error_t  rErr = kNoError;

    // sanity check
    if (!CUtil::isPowOf2(iBlockLength) || iZeroPadFactor <= 0 || !CUtil::isPowOf2(iBlockLength*iZeroPadFactor))
        return kFunctionInvalidArgsError;

    // clean up
    reset();

    m_iDataLength   = iBlockLength;
    m_iFftLength    = iBlockLength * iZeroPadFactor;

    m_ePrePostWindowOpt = eWindowing;

    rErr = allocMemory ();
    if (rErr)
        return rErr;

    rErr = computeWindow (eWindow);

    m_bIsInitialized    = true;

    return rErr;
}

Error_t CFft::reset()
{
    freeMemory();

    m_iDataLength       = 0;
    m_iFftLength        = 0;
    m_ePrePostWindowOpt = kNoWindow;
    
    m_bIsInitialized    = false;

    return kNoError;

}

Error_t CFft::overrideWindow( const float *pfNewWindow )
{
    if (!m_bIsInitialized)
        return kNotInitializedError;
    if (!pfNewWindow)
        return kFunctionInvalidArgsError;

    CVectorFloat::copy(m_pfWindowBuff, pfNewWindow, m_iDataLength);

    return kNoError;
}

Error_t CFft::getWindow( float *pfWindow ) const
{
    if (!m_bIsInitialized)
        return kNotInitializedError;
    if (!pfWindow)
        return kFunctionInvalidArgsError;

    CVectorFloat::copy(pfWindow, m_pfWindowBuff, m_iDataLength);

    return kNoError;
}

Error_t CFft::doFft( complex_t *pfSpectrum, const float *pfInput )
{
    if (!m_bIsInitialized)
        return kNotInitializedError;
    if (!pfInput || !pfSpectrum)
        return kFunctionInvalidArgsError;

    // copy data to internal buffer
    CVectorFloat::copy(m_pfProcessBuff, pfInput, m_iDataLength);
    CVectorFloat::setZero(&m_pfProcessBuff[m_iDataLength], m_iFftLength-m_iDataLength);

    // apply window function
    if (m_ePrePostWindowOpt & kPreWindow)
        CVectorFloat::mul_I(m_pfProcessBuff, m_pfWindowBuff, m_iDataLength);

    // compute fft
    LaszloFft::realfft_split(m_pfProcessBuff, m_iFftLength);

    // copy data to output buffer
    CVectorFloat::copy(pfSpectrum, m_pfProcessBuff, m_iFftLength);

    return kNoError;
}

Error_t CFft::doInvFft( float *pfOutput, const complex_t *pfSpectrum )
{
    if (!m_bIsInitialized)
        return kNotInitializedError;

    // copy data to internal buffer
    CVectorFloat::copy(m_pfProcessBuff, pfSpectrum, m_iFftLength);
    
    // compute ifft
    LaszloFft::irealfft_split(m_pfProcessBuff, m_iFftLength);

    // apply window function
    if (m_ePrePostWindowOpt & kPostWindow)
        CVectorFloat::mul_I(m_pfProcessBuff, m_pfWindowBuff, m_iDataLength);

    // copy data to output buffer
    CVectorFloat::copy(pfOutput, m_pfProcessBuff, m_iFftLength);

    return kNoError;
}

Error_t CFft::getMagnitude( float *pfMag, const complex_t *pfSpectrum ) const
{
    if (!m_bIsInitialized)
        return kNotInitializedError;

    // re(0),re(1),re(2),...,re(size/2),im(size/2-1),...,im(1)
    int iNyq        = m_iFftLength>>1;

    // no imaginary part at these bins
    pfMag[0]        = std::abs(pfSpectrum[0]);
    pfMag[iNyq]     = std::abs(pfSpectrum[iNyq]);

    for (int i = 1; i < iNyq; i++)
    {
        int iImagIdx    = m_iFftLength - i;
        pfMag[i]        = sqrtf(pfSpectrum[i]*pfSpectrum[i] + pfSpectrum[iImagIdx]*pfSpectrum[iImagIdx]);
    }
    return kNoError;
}

Error_t CFft::getPhase( float *pfPhase, const complex_t *pfSpectrum ) const
{
    if (!m_bIsInitialized)
        return kNotInitializedError;

    // re(0),re(1),re(2),...,re(size/2),im(size/2-1),...,im(1)
    int iNyq        = m_iFftLength>>1;

    pfPhase[0]      = m_Pi;
    pfPhase[iNyq]   = m_Pi;
    
    for (int i = 1; i < iNyq; i++)
    {
        int iImagIdx    = m_iFftLength - i;
        if (pfSpectrum[i] == .0F && pfSpectrum[iImagIdx] != .0F)
            pfPhase[i]   = m_Pi2;
        else
            pfPhase[i]   = atan2f (pfSpectrum[iImagIdx], pfSpectrum[i]);
    }
    return kNoError;
}

Error_t CFft::splitRealImag( float *pfReal, float *pfImag, const complex_t *pfSpectrum ) const
{
    if (!m_bIsInitialized)
        return kNotInitializedError;

    // re(0),re(1),re(2),...,re(size/2),im(size/2-1),...,im(1)
    int iNyq        = m_iFftLength>>1;

    CVectorFloat::copy(pfReal, pfSpectrum, iNyq+1);

    pfImag[0]       = 0;
    for (int i = 1, iImag = m_iFftLength-1; i < iNyq; i++, iImag--)
    {
        pfImag[i]   = pfSpectrum[iImag];
    }

    return kNoError;
}

Error_t CFft::mergeRealImag( complex_t *pfSpectrum, const float *pfReal, const float *pfImag ) const
{
    if (!m_bIsInitialized)
        return kNotInitializedError;

    assert(pfSpectrum || pfReal || pfImag);

    // re(0),re(1),re(2),...,re(size/2),im(size/2-1),...,im(1)
    int iNyq        = m_iFftLength>>1;

    CVectorFloat::copy(pfSpectrum, pfReal, iNyq+1);

    for (int i = 1, iImag = m_iFftLength-1; i < iNyq-1; i++, iImag--)
    {
        pfSpectrum[iImag]   = pfImag[i];
    }

    return kNoError;
}

Error_t CFft::conjCompSpectrum( complex_t *pfConjCompSpectrum, const complex_t *pfSpectrum ) const
{
    if (!m_bIsInitialized)
        return kNotInitializedError;

    assert(pfSpectrum || pfConjCompSpectrum);

    if (pfConjCompSpectrum != pfSpectrum)
        CVectorFloat::copy(pfConjCompSpectrum, pfSpectrum, m_iFftLength);

    CVectorFloat::mulC_I(&pfConjCompSpectrum[(m_iFftLength>>1)+1], -1.F, (m_iFftLength>>1)-1);

    return kNoError;
}

Error_t CFft::mulCompSpectrum( complex_t *pfSrcDest, const complex_t *pfSrc ) const
{
    if (!m_bIsInitialized)
        return kNotInitializedError;

    assert (pfSrcDest || pfSrc );

    // re(0),re(1),re(2),...,re(size/2),im(size/2-1),...,im(1)

    pfSrcDest[0]                *= pfSrc[0];
    pfSrcDest[m_iFftLength>>1]  *= pfSrc[m_iFftLength>>1];

    for (int i = 1; i < m_iFftLength>>1; i++)
    {
        float fTemp = pfSrcDest[i];

        // real part of multiplication
        pfSrcDest[i] *= pfSrc[i];
        pfSrcDest[i] -= pfSrcDest[m_iFftLength-i] * pfSrc[m_iFftLength-i];

        // imaginary part of multiplication
        pfSrcDest[m_iFftLength-i] *= pfSrc[i];
        pfSrcDest[m_iFftLength-i] += fTemp * pfSrc[m_iFftLength-i];
    }

    return kNoError;
}


float CFft::freq2bin( float fFreqInHz, float fSampleRateInHz ) const
{
    return fFreqInHz / fSampleRateInHz * m_iFftLength;
}

float CFft::bin2freq( int iBinIdx, float fSampleRateInHz ) const
{
    return iBinIdx * fSampleRateInHz / m_iFftLength;
}

Error_t CFft::allocMemory()
{

    m_pfProcessBuff = new float [m_iFftLength];
    m_pfWindowBuff  = new float [m_iDataLength];

    if (!m_pfProcessBuff || !m_pfWindowBuff)
        return kMemError;
    else
    {
        return kNoError;
    }
}

Error_t CFft::freeMemory()
{
    delete [] m_pfProcessBuff;
    delete [] m_pfWindowBuff;

    m_pfProcessBuff = 0;
    m_pfWindowBuff  = 0;

    return kNoError;
}

Error_t CFft::computeWindow( WindowFunction_t eWindow )
{
    int i;

    // note that these windows are periodic, not symmetric
    switch (eWindow)
    {
    case kWindowSine:
        {
            for (i = 0; i < m_iDataLength; i++)
            {
                m_pfWindowBuff[i]   = sinf((i * m_Pi) / (m_iDataLength+1));
            }
            break;
        }
    case kWindowHann:
        {
            for (i = 0; i < m_iDataLength; i++)
            {
                m_pfWindowBuff[i]   = .5F * (1.F - cosf((i * 2.F*m_Pi) / (m_iDataLength+1)));
            }
            break;
        }
    case kWindowHamming:
        {
            for (i = 0; i < m_iDataLength; i++)
            {
                m_pfWindowBuff[i]   = .54F - .46F * cosf((i * 2.F*m_Pi) / (m_iDataLength+1));
            }
            break;
        }
    case kNumWindows:
        {
            CVectorFloat::setZero(m_pfWindowBuff, m_iDataLength);
            return kFunctionInvalidArgsError;
        }
    }

    return kNoError;
}

int CFft::getLength( Length_t eLengthIdx )
{
    switch (eLengthIdx)
    {
    case kLengthFft:
        return m_iFftLength;
    case kLengthData:
        return m_iDataLength;
    case kLengthMagnitude:
    case kLengthPhase:
        return m_iFftLength/2+1;
    case kNumLengths:
        return -1;
    }
    return 0;
}
