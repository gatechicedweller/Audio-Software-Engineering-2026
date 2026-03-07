#include "CombFilterIf.h"
#include "CombFilter.h"
#include <iostream>

// ============================================================
// FIR
// ============================================================

Error_t FIR::create(CCombFilterIf*& pCCombFilterIf)
{
    FIR* pFIR = new FIR();
    pCCombFilterIf = pFIR;
    return Error_t::kNoError;
}

FIR::~FIR()
{
    reset();
}

Error_t FIR::init(CombFilterType_t eFilterType, float fMaxDelayLengthInS, float fSampleRateInHz, int iNumChannels)
{
    m_fSampleRate = fSampleRateInHz;
    m_iNumChannels = iNumChannels;
    m_fDelaySamples = fMaxDelayLengthInS * fSampleRateInHz;
    m_fGain = 0.f;

    //one buffer per channel
    m_ppCDelayBuf = new CRingBuffer<float>*[m_iNumChannels];
    for (int i = 0; i < m_iNumChannels; ++i)
    {
        m_ppCDelayBuf[i] = new CRingBuffer<float>(static_cast<int>(m_fDelaySamples) + 1);
        m_ppCDelayBuf[i]->setWriteIdx(static_cast<int>(m_fDelaySamples)); // pre-offset by however many samples
    }
    return Error_t::kNoError;
}

Error_t FIR::reset()
{
    if (m_ppCDelayBuf)
    {
        for (int i = 0; i < m_iNumChannels; ++i)
            delete m_ppCDelayBuf[i];
        delete[] m_ppCDelayBuf;
        m_ppCDelayBuf = nullptr;
    }
    m_fGain = 0.f;
    return Error_t::kNoError;
}

Error_t FIR::setParam(FilterParam_t eParam, float fParamValue)
{
    switch (eParam)
    {
    case FilterParam_t::kParamDelay:
        m_fDelaySamples = fParamValue * m_fSampleRate;
        break;
    case FilterParam_t::kParamGain:
        m_fGain = fParamValue;
        break;
    case FilterParam_t::kNumFilterParams:
        std::cout << "There are two params, kParamGain and kParamDelay." << std::endl;
        return Error_t::kFunctionInvalidArgsError;
    default:
        return Error_t::kFunctionInvalidArgsError;
    }
    return Error_t::kNoError;
}

float FIR::getParam(FilterParam_t eParam) const
{
    switch (eParam)
    {
    case FilterParam_t::kParamDelay:
        return m_fDelaySamples / m_fSampleRate;
    case FilterParam_t::kParamGain:
        return m_fGain;
    default:
        return 0.f;
    }
}

Error_t FIR::process(float** ppfInputBuffer, float** ppfOutputBuffer, int iNumberOfFrames)
{
    for (int i = 0; i < m_iNumChannels; ++i)
    {
        for (int j = 0; j < iNumberOfFrames; ++j)
        {
            float fDelayedSample = m_ppCDelayBuf[i]->getPostInc();
            float fInputSample = ppfInputBuffer[i][j];
            float fOutputSample = fInputSample + m_fGain * fDelayedSample;
            ppfOutputBuffer[i][j] = fOutputSample;
            m_ppCDelayBuf[i]->putPostInc(fInputSample);
        }
    }
    return Error_t::kNoError;
}

// ============================================================
// IIR
// ============================================================

Error_t IIR::create(CCombFilterIf*& pCCombFilterIf)
{
    IIR* pIIR = new IIR();
    pCCombFilterIf = pIIR;
    return Error_t::kNoError;
}

IIR::~IIR()
{
    reset();
}

Error_t IIR::init(CombFilterType_t eFilterType, float fMaxDelayLengthInS, float fSampleRateInHz, int iNumChannels)
{
    m_fSampleRate = fSampleRateInHz;
    m_iNumChannels = iNumChannels;
    m_fDelaySamples = fMaxDelayLengthInS * fSampleRateInHz;
    m_fGain = 0.f;

    m_ppCDelayBuf = new CRingBuffer<float>*[m_iNumChannels];
    for (int i = 0; i < m_iNumChannels; ++i)
    {
        m_ppCDelayBuf[i] = new CRingBuffer<float>(static_cast<int>(m_fDelaySamples) + 1);
        m_ppCDelayBuf[i]->setWriteIdx(static_cast<int>(m_fDelaySamples));
    }
    return Error_t::kNoError;
}

Error_t IIR::reset()
{
    if (m_ppCDelayBuf)
    {
        for (int i = 0; i < m_iNumChannels; ++i)
            delete m_ppCDelayBuf[i];
        delete[] m_ppCDelayBuf;
        m_ppCDelayBuf = nullptr;
    }
    m_fGain = 0.f;
    return Error_t::kNoError;
}

Error_t IIR::setParam(FilterParam_t eParam, float fParamValue)
{
    switch (eParam)
    {
    case FilterParam_t::kParamDelay:
        m_fDelaySamples = fParamValue * m_fSampleRate;
        break;
    case FilterParam_t::kParamGain:
        m_fGain = fParamValue;
        break;
    default:
        return Error_t::kFunctionInvalidArgsError;
    }
    return Error_t::kNoError;
}

float IIR::getParam(FilterParam_t eParam) const
{
    switch (eParam)
    {
    case FilterParam_t::kParamDelay:
        return m_fDelaySamples / m_fSampleRate;
    case FilterParam_t::kParamGain:
        return m_fGain;
    default:
        return 0.f;
    }
}

Error_t IIR::process(float** ppfInputBuffer, float** ppfOutputBuffer, int iNumberOfFrames)
{
    for (int i = 0; i < m_iNumChannels; ++i)
    {
        for (int j = 0; j < iNumberOfFrames; ++j)
        {
            float fDelayedSample = m_ppCDelayBuf[i]->getPostInc();
            float fOutputSample  = ppfInputBuffer[i][j] + m_fGain * fDelayedSample;
            ppfOutputBuffer[i][j] = fOutputSample;
            m_ppCDelayBuf[i]->putPostInc(fOutputSample); // feedback: write output, not input
        }
    }
    return Error_t::kNoError;
}