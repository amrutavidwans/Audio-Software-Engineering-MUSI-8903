#if !defined(__RingBuffer_hdr__)
#define __RingBuffer_hdr__

#include <cassert>
#include <algorithm>
#include <cmath>

template <class T> 
class CRingBuffer
{
public:
    explicit CRingBuffer (int iBufferLengthInSamples) :
        m_iBuffLength(iBufferLengthInSamples),
        m_iReadIdx(0),                          
        m_iWriteIdx(0),
        m_ptBuff(0)
    {
        assert(iBufferLengthInSamples > 0);

        m_ptBuff        = new T [m_iBuffLength];
        reset();
    }

    virtual ~CRingBuffer ()
    {
        delete [] m_ptBuff;
        m_ptBuff    = 0;
    }

    /*! add a new value of type T to write index and increment write index
    \param T tNewValue the new value
    \return void
    */
    void putPostInc (T tNewValue)
    {
        put(tNewValue);
        incIdx(m_iWriteIdx);
    }

    /*! add new values of type T to write index and increment write index
    \param const T * ptNewBuff: new values
    \param int iLength: number of values
    \return void
    */
    void putPostInc (const T* ptNewBuff, int iLength)
    {
        put(ptNewBuff, iLength);
        incIdx(m_iWriteIdx, iLength);
    }

    /*! add a new value of type T to write index
    \param T tNewValue the new value
    \return void
    */
    void put(T tNewValue)
    {
        m_ptBuff[m_iWriteIdx]   = tNewValue;
    }

    /*! add new values of type T to write index
    \param const T * ptNewBuff: new values
    \param int iLength: number of values
    \return void
    */
    void put(const T* ptNewBuff, int iLength)
    {
        assert(iLength <= m_iBuffLength && iLength >= 0);

        // copy two parts: to the end of buffer and after wrap around
        int iNumValues2End      = std::min(iLength,m_iBuffLength - m_iWriteIdx);

        memcpy (&m_ptBuff[m_iWriteIdx], ptNewBuff, sizeof(T)*iNumValues2End);
        if ((iLength - iNumValues2End)>0)
            memcpy (m_ptBuff, &ptNewBuff[iNumValues2End], sizeof(T)*(iLength - iNumValues2End));
    }
    
    /*! return the value at the current read index and increment the read pointer
    \return float the value from the read index
    */
    T getPostInc ()
    {
        T tValue = get();
        incIdx(m_iReadIdx);
        return tValue;
    }

    /*! return the values starting at the current read index and increment the read pointer
    \param T * ptBuff: pointer to where the values will be written
    \param int iLength: number of values
    \return void
    */
    void getPostInc (T* ptBuff, int iLength)
    {
        get(ptBuff, iLength);
        incIdx(m_iReadIdx, iLength);
    }

    /*! return the value at the current read index
    \param int iOffset: read at offset from read index
    \return float the value from the read index
    */
    T get (int iOffset = 0) const
    {
        int iRead = m_iReadIdx + iOffset;
        while (iRead > m_iBuffLength-1)
            iRead  -= m_iBuffLength;
        while (iRead < 0)
            iRead  += m_iBuffLength;
        
        return m_ptBuff[iRead];
    }

    /*! return the values starting at the current read index
    \param T * ptBuffpointer to where the values will be written
    \param int iLength: number of values
    \return void
    */
    void get (T* ptBuff, int iLength) const
    {
        assert(iLength <= m_iBuffLength && iLength >= 0);

        // copy two parts: to the end of buffer and after wrap around
        int iNumValues2End      = std::min(iLength, m_iBuffLength - m_iReadIdx);

        memcpy (ptBuff, &m_ptBuff[m_iReadIdx], sizeof(T)*iNumValues2End);
        if ((iLength - iNumValues2End)>0)
            memcpy (&ptBuff[iNumValues2End], m_ptBuff, sizeof(T)*(iLength - iNumValues2End));
    }
    
    /*! return the value at the current read index
     \param int iOffset: read at offset from read index
     \return float the value from the read index
     \ this function was added to work with the fractional delay in the vibrato functionality
     */
    T getValuesAtNonIntLocations (double iOffset = 0) const
    {
        int iReadPrev = static_cast<int>(floor(iOffset));
        double frac = iOffset - iReadPrev;
        /*
        while (iReadPrev > m_iBuffLength-1)
            iReadPrev  -= m_iBuffLength;
        while (iReadPrev < 0)
            iReadPrev  += m_iBuffLength;
        
        
        while (iReadNxt > m_iBuffLength-1)
            iReadNxt  -= m_iBuffLength;
        while (iReadNxt < 0)
            iReadNxt  += m_iBuffLength;
        */
        return static_cast<T>(get(iReadPrev+1)*frac +  get(iReadPrev)*(1-frac));
    }
    
    /*! set buffer content and indices to 0
    \return void
    */
    void reset ()
    {
        memset (m_ptBuff, 0, sizeof(T)*m_iBuffLength);
        m_iReadIdx  = 0;
        m_iWriteIdx = 0;
    }

    /*! return the current index for writing/put
    \return int
    */
    int getWriteIdx () const
    {
        return m_iWriteIdx;
    }

    /*! move the write index to a new position
    \param int iNewWriteIdx: new position
    \return void
    */
    void setWriteIdx (int iNewWriteIdx)
    {
        incIdx(m_iWriteIdx, iNewWriteIdx - m_iWriteIdx);
    }

    /*! return the current index for reading/get
    \return int
    */
    int getReadIdx () const
    {
        return m_iReadIdx;
    }

    /*! move the read index to a new position
    \param int iNewReadIdx: new position
    \return void
    */
    void setReadIdx (int iNewReadIdx)
    {
        incIdx(m_iReadIdx, iNewReadIdx - m_iReadIdx);
    }

    /*! returns the number of values currently buffered (note: 0 could also mean the buffer is full!)
    \return int
    */
    int getNumValuesInBuffer () const
    {
        return (m_iWriteIdx - m_iReadIdx + m_iBuffLength)%m_iBuffLength;
    }

    /*! returns the length of the internal buffer
    \return int
    */
    int getLength () const
    {
        return m_iBuffLength;
    }
private:
    CRingBuffer ();
    CRingBuffer(const CRingBuffer& that);

    void incIdx (int &iIdx, int iOffset = 1)
    {
        while ((iIdx + iOffset) < 0)
        {
            // avoid negative buffer indices
            iOffset += m_iBuffLength;   
        }
        iIdx    = (iIdx + iOffset) % m_iBuffLength;
    };

    int m_iBuffLength,              //!< length of the internal buffer
        m_iReadIdx,                 //!< current read index
        m_iWriteIdx;                //!< current write index

    T   *m_ptBuff;                  //!< data buffer
};
#endif // __RingBuffer_hdr__
