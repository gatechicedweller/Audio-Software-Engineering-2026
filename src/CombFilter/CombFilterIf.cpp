// standard headers

// project headers
#include "MUSI6106Config.h"

#include "ErrorDef.h"
#include "Util.h"

#include "CombFilterIf.h"
#include <iostream>
 

static const char*  kCMyProjectBuildDate = "3.6.2026";


CCombFilterIf::CCombFilterIf () :
    m_bIsInitialized(false),
    m_pCCombFilter(0),
    m_fSampleRate(0)
{
    // this should never hurt
    this->reset ();
}


CCombFilterIf::~CCombFilterIf ()
{
    this->reset ();
}
/*
const int  CCombFilterIf::getVersion (const Version_t eVersionIdx)
{
    int iVersion = 0;

    switch (eVersionIdx)
    {
        case kMajor:
            iVersion    = MUSI6106_VERSION_MAJOR;
            break;
        case kMinor:
            iVersion    = MUSI6106_VERSION_MINOR;
            break;
        case kPatch:
            iVersion    = MUSI6106_VERSION_PATCH;
            break;
        case kNumVersionInts:
            iVersion    = -1;
            break;
    }

    return iVersion;
}
    */
   /*
const char*  CCombFilterIf::getBuildDate ()
{
    return kCMyProjectBuildDate;
}
*/
//==================================================================

//all these errors: if you just try to create a comb filter, and not an FIR and not an IIR, we will kill ourselves. a comb filter on its own is not a thing.

Error_t CCombFilterIf::create (CCombFilterIf*& pCCombFilter)
{
    return Error_t::kNoError;
}

Error_t CCombFilterIf::destroy (CCombFilterIf*& pCCombFilter)
{
    return Error_t::kNoError;
}

Error_t CCombFilterIf::init (CombFilterType_t eFilterType, float fMaxDelayLengthInS, float fSampleRateInHz, int iNumChannels)
{
    return Error_t::kNoError;
}

Error_t CCombFilterIf::reset ()
{
    return Error_t::kNoError;
}

Error_t CCombFilterIf::process (float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames)
{
    return Error_t::kNoError;
}

Error_t CCombFilterIf::setParam (FilterParam_t eParam, float fParamValue)
{
    return Error_t::kNoError;
}

float CCombFilterIf::getParam (FilterParam_t eParam) const
{

    return 0;
}

