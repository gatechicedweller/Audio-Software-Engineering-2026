
#include "FastConv.h"

CFastConv::CFastConv( void ) //constructor
{

}

CFastConv::~CFastConv( void ) //destroyer
{
    reset();
}

Error_t CFastConv::init(float *pfImpulseResponse, int iLengthOfIr, int iBlockLength /*= 8192*/, ConvCompMode_t eCompMode /*= kFreqDomain*/)
{
//I have to make my own copies of these inputs

    m_pfImpulseResponse = new float[iLengthOfIr];
    std::copy(pfImpulseResponse, pfImpulseResponse + iLengthOfIr, m_pfImpulseResponse);

    //whole new buffer that holds the entire blocksize + ir length - 1
    m_pfConvolutionBuffer = new float[iBlockLength + iLengthOfIr - 1];

    // new output buffer of input length
    m_pfOutputBuffer = new float[iBlockLength]; 

    //ACTUAL PROCESSING SIZE
    m_iLengthOfIr = iLengthOfIr;

    //EXPECTED PROCESSING SIZE
    m_iBlockLength = iBlockLength;

    return Error_t::kNoError;
}

Error_t CFastConv::reset()
{
    if (m_pfImpulseResponse)
    {
        delete[] m_pfImpulseResponse;
        m_pfImpulseResponse = nullptr;
    }

    if (m_pfOutputBuffer)
    {
        delete[] m_pfOutputBuffer;
        m_pfOutputBuffer = nullptr;
    }

    if (m_pfConvolutionBuffer)
    {
        delete[] m_pfConvolutionBuffer;
        m_pfConvolutionBuffer = nullptr;
    }

    return Error_t::kNoError;
}

Error_t CFastConv::process (float* pfOutputBuffer, const float *pfInputBuffer, int iLengthOfBuffers )
{
     m_pfInputBuffer = new float[iLengthOfBuffers];
    std::copy(pfInputBuffer, pfInputBuffer + iLengthOfBuffers, m_pfInputBuffer);
    if (m_eCompMode == kTimeDomain)
    {
        // do time domain convolution here
        for (int i = 0; i< iLengthOfBuffers; i++)
        {
            m_pfConvolutionBuffer[i] = 0;
            for (int j = 0; j < m_iLengthOfIr; j++)
            {
                if (i-j >= 0)
                {
                    m_pfConvolutionBuffer[i] += m_pfImpulseResponse[m_iLengthOfIr-1-j] * m_pfInputBuffer[i-j];
                }
            }
        }

        std::copy(m_pfConvolutionBuffer, m_pfConvolutionBuffer + iLengthOfBuffers, pfOutputBuffer);

    }
    else
    {
        // do frequency domain convolution here
    }   
    return Error_t::kNoError;
}

Error_t CFastConv::flushBuffer(float* pfOutputBuffer)
{
    
    return Error_t::kNoError;
}


