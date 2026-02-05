
#ifndef MUSI6106_RINGBUFFER_H
#define MUSI6106_RINGBUFFER_H



#include <cassert>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>

// TODO: Clear out each of the cout and implement intended functionality

template <class T>
class CRingBuffer
{
public:
    explicit CRingBuffer(int iBufferLengthInSamples) :
    m_iBuffLength(iBufferLengthInSamples),
    m_iReadIdx(0),
    m_iWriteIdx(0),
    m_ptBuff(0)

    {
        assert(iBufferLengthInSamples > 0);
        std::cout <<"constructor allocations not implemented" << std::endl;
        // allocate and init
        // First step: allocate and initialize m_ptBuff in the constructor.
        // Nothing else will work until you do.
    }

    // explicitly deleting copy constructor
    CRingBuffer(const CRingBuffer&) = delete;
    CRingBuffer& operator=(const CRingBuffer&) = delete;

    virtual ~CRingBuffer()
    {
        delete[] m_ptBuff;
        m_ptBuff = 0;

    }
    /*! add a new value of type T to write index and increment write index
       \param tNewValue the new value
       \return void
       */
    void putPostInc (T tNewValue)
    {
        std::cout <<"putPostInc() not implemented" << std::endl;
    }

    /*! add a new value of type T to write index
    */
    void put(T tNewValue)
    {
        std::cout <<"put() not implemented" << std::endl;
    }

    /*! return the value at the current read index and increment the read pointer
    \return float the value from the read index
    */
    T getPostInc()
    {
        std::cout <<"getPostInc() not implemented" << std::endl;
        return static_cast<T>(-1);
    }

    /*! return the value at the current read index
    \return float the value from the read index
    */
    T get() const
    {
        std::cout <<"get() not implemented" << std::endl;
        return static_cast<T>(-1);
    }

    /*! set buffer content and indices to 0
    \return void
    */
    void reset()
    {

        std::cout <<"reset() not implemented" << std::endl;

    }

    /*! return the current index for writing/put
    \return int
    */
    int getWriteIdx() const
    {
        std::cout <<"getWriteInx() not implemented" << std::endl;
        return -1;
    }

    /*! move the write index to a new position
    */
    void setWriteIdx(int iNewWriteIdx)
    {
        std::cout <<"setWriteIdx() not implemented" << std::endl;

    }

    /*! return the current index for reading/get
    \return int
    */
    int getReadIdx() const
    {
        std::cout <<"getReadIdx() not implemented" << std::endl;
        return -1;
    }

    /*! move the read index to a new position
    */
    void setReadIdx(int iNewReadIdx)
    {
        std::cout <<"setReadIdx() implemented" << std::endl;
    }

    /*! returns the number of values currently buffered (note: 0 could also mean the buffer is full!)
    \return int
    */
    int getNumValuesInBuffer() const
    {
        // HINT:
        // How can the distance between read and write indices
        // tell you how many samples are buffered?
        std::cout <<"getNumValuesInBuffer() not implemented" << std::endl;
        return -1;
    }

    /*! returns the length of the internal buffer
    \return int
    */
    int getLength() const
    {
        std::cout <<"getLength() not implemented" << std::endl;
        return -1;
    }
private:
    // We make this default constructor private because this class cannot function without a buffer length.
    CRingBuffer();

    /*! Implement the coreRingBuffer mechanics here
    // Input reference to write or read index and specified offset count, ensure it wraps around the buffer legnth
    \return void
    */
    void incIdx(int& iIdx, int iOffset = 1) {

    }
    int m_iBuffLength,      //!< length of the internal buffer
       m_iReadIdx,         //!< current read index
       m_iWriteIdx;        //!< current write index

    // internal pointer to the buffer memory, why is it private? (rationalize)
    T* m_ptBuff;            //!< data buffer











};
#endif //
