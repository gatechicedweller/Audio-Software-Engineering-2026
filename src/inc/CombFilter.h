#if !defined(__CombFilter_hdr__)
#define __CombFilter_hdr__

#include "CombFilterIf.h"
#include "RingBuffer.h"

class FIR : public CCombFilterIf
{
public:
    static Error_t create(CCombFilterIf*& pCCombFilterIf);

    Error_t init(CombFilterType_t eFilterType, float fMaxDelayLengthInS, float fSampleRateInHz, int iNumChannels) override;
    Error_t reset() override;
    Error_t setParam(FilterParam_t eParam, float fParamValue) override;
    float   getParam(FilterParam_t eParam) const override;
    Error_t process(float** ppfInputBuffer, float** ppfOutputBuffer, int iNumberOfFrames) override;

private:
    FIR() = default;
    virtual ~FIR();

    CRingBuffer<float>** m_ppCDelayBuf = nullptr;
    float  m_fGain          = 0.f;
    float  m_fDelaySamples  = 0.f;
    float  m_fSampleRate    = 0.f;
    int    m_iNumChannels   = 0;
};

class IIR : public CCombFilterIf
{
public:
    static Error_t create(CCombFilterIf*& pCCombFilterIf);

    Error_t init(CombFilterType_t eFilterType, float fMaxDelayLengthInS, float fSampleRateInHz, int iNumChannels) override;
    Error_t reset() override;
    Error_t setParam(FilterParam_t eParam, float fParamValue) override;
    float   getParam(FilterParam_t eParam) const override;
    Error_t process(float** ppfInputBuffer, float** ppfOutputBuffer, int iNumberOfFrames) override;

private:
    IIR() = default;
    virtual ~IIR();

    CRingBuffer<float>** m_ppCDelayBuf = nullptr;
    float  m_fGain          = 0.f;
    float  m_fDelaySamples  = 0.f;
    float  m_fSampleRate    = 0.f;
    int    m_iNumChannels   = 0;
};

#endif // #if !defined(__CombFilter_hdr__)
