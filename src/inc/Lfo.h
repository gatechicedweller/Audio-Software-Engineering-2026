#if !defined(__Lfo_hdr__)
#define __Lfo_hdr__

/*
 *
 * Design choices:
 * - Sins wave LFO with chill linear interpolation, computed at construction
 * - The fractional phase index steps like (tableLength * freqHz / sampleRateHz) per output sample, so changing frequency
 *   only requires updating a float
 * - Amplitude is in samples so the vibrato can directly use the output
 */

#define _USE_MATH_DEFINES
#include <cmath>
#include <cassert>

#include "RingBuffer.h"
#include "ErrorDef.h"

class CLfo
{
public:
    /*! Construct a wavetable LFO.
    \param fSampleRateInHz   host sample rate (must be > 0)
    \param iWavetableLength  number of samples in one period of the sine table (default 1024)
    */
    CLfo(float fSampleRateInHz, int iWavetableLength = 1024) :
        m_fSampleRateInHz(fSampleRateInHz),
        m_fFreqInHz(1.f),
        m_fAmplitude(1.f),
        m_fPhase(0.f),
        m_fPhaseIncr(0.f),
        m_iTableLen(iWavetableLength),
        m_pCWavetable(nullptr)
    {
        assert(fSampleRateInHz > 0.f);
        assert(iWavetableLength > 0);

        m_pCWavetable = new CRingBuffer<float>(m_iTableLen);

        // Fill table with one full period of a sine wave
        for (int k = 0; k < m_iTableLen; k++)
            m_pCWavetable->putPostInc(
                static_cast<float>(std::sin(2.0 * M_PI * k / m_iTableLen)));

        setFreqInHz(m_fFreqInHz);
    }

    ~CLfo()
    {
        delete m_pCWavetable;
        m_pCWavetable = nullptr;
    }

    /*! Set LFO frequency.
    \param fFreqInHz  modulation frequency in Hz (must be >= 0)
    \return Error_t
    */
    Error_t setFreqInHz(float fFreqInHz)
    {
        if (fFreqInHz < 0.f)
            return Error_t::kFunctionInvalidArgsError;
        m_fFreqInHz  = fFreqInHz;
        m_fPhaseIncr = static_cast<float>(m_iTableLen) * m_fFreqInHz / m_fSampleRateInHz;
        return Error_t::kNoError;
    }

    /*! Set LFO output amplitude (used as delay offset in samples by the vibrato).
    \param fAmplitudeInSamples  peak amplitude of the LFO output (must be >= 0)
    \return Error_t
    */
    Error_t setAmplitudeInSamples(float fAmplitudeInSamples)
    {
        if (fAmplitudeInSamples < 0.f)
            return Error_t::kFunctionInvalidArgsError;
        m_fAmplitude = fAmplitudeInSamples;
        return Error_t::kNoError;
    }

    /*! Return current amplitude in samples. */
    float getAmplitudeInSamples() const { return m_fAmplitude; }

    /*! Return current frequency in Hz. */
    float getFreqInHz() const { return m_fFreqInHz; }

    /*! Advance the LFO by one sample and return the current output value.
    \return float  output in the range [-amplitude, +amplitude]
    */
    float getNextSample()
    {
        // Fractional read from wavetable at current phase
        int   iPhaseInt  = static_cast<int>(m_fPhase);
        float fPhaseFrac = m_fPhase - static_cast<float>(iPhaseInt);
        m_pCWavetable->setReadIdx(iPhaseInt);
        float fOut = m_fAmplitude * m_pCWavetable->get(fPhaseFrac);

        // Advance phase
        m_fPhase += m_fPhaseIncr;
        if (m_fPhase >= static_cast<float>(m_iTableLen))
            m_fPhase -= static_cast<float>(m_iTableLen);

        return fOut;
    }

    /*! Reset phase to 0 (amplitude and frequency are preserved). */
    void reset()
    {
        m_fPhase = 0.f;
    }

private:
    CLfo();
    CLfo(const CLfo&);

    float m_fSampleRateInHz;
    float m_fFreqInHz;
    float m_fAmplitude;         //!< peak output amplitude in samples
    float m_fPhase;             //!< current fractional index into the wavetable [0, tableLen)
    float m_fPhaseIncr;         //!< phase advance per output sample
    int   m_iTableLen;
    CRingBuffer<float>* m_pCWavetable;
};

#endif // __Lfo_hdr__
