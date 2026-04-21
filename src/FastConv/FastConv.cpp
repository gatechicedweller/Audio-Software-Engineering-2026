
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
//I have to make a copy of ImpulseResponse cuz it's a pointer, everything else is free

    m_pfImpulseResponse = new float[iLengthOfIr];
    std::copy(pfImpulseResponse, pfImpulseResponse + iLengthOfIr, m_pfImpulseResponse);

    m_pfOutputBuffer = new float[iBlockLength]; 

//find input buffer somewhere in here
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
    return Error_t::kNoError;
}

Error_t CFastConv::process (float* pfOutputBuffer, const float *pfInputBuffer, int iLengthOfBuffers )
{
    if (m_eCompMode == kTimeDomain)
    {
        // do time domain convolution here
        for (int i = 0; i< iLengthOfBuffers; i++)
        {
            pfOutputBuffer[i] = 0;
            for (int j = 0; j < iLengthOfIr; j++)
            {
                if (i-j >= 0)
                {
                    pfOutputBuffer[i] += m_pfImpulseResponse[iLengthOfIr-1-j] * m_pfInputBuffer[i-j];
                }
                
            }
        }
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


