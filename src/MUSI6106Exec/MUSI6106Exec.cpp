
#include <iostream>
#include <ctime>
#include <cmath>
#include <string>

#include "MUSI6106Config.h"

#include "AudioFileIf.h"
#include "CombFilterIf.h"
#include "CombFilter.h"

using std::cout;
using std::endl;

// local function declarations
void    showClInfo ();
void test();

/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
    std::string             sInputFilePath,                 //!< file paths
                            sOutputFilePath;

    static const int        kBlockSize = 1024;

    clock_t                 time = 0;

    float                   **ppfAudioData = 0,
                            **ppfOutputData = 0;

    CAudioFileIf            *phAudioFile = 0;
    std::fstream            hOutputFile;
    CAudioFileIf::FileSpec_t stFileSpec;
    CCombFilterIf::CombFilterType_t eFilterType;
    float                   fDelay, fGain;

    // Initialize CombFilter

    showClInfo();

    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments
    // If no arguments, run tests
    if (argc < 5)
    {
        cout << "Missing arguments! -- Running Tests now" << endl;
        test();
        return -1;
    }
    // else check and assign filter parameters
    else
    {
        sInputFilePath = argv[1];
        sOutputFilePath = sInputFilePath + ".txt";
        
        eFilterType = static_cast<CCombFilterIf::CombFilterType_t>(std::stoi(argv[2])); //I don't understand this structure
        fDelay = std::stof(argv[3]);
        fGain = std::stof(argv[4]);
    }

    //////////////////////////////////////////////////////////////////////////////
    // open the input wave file
    CAudioFileIf::create(phAudioFile);
    phAudioFile->openFile(sInputFilePath, CAudioFileIf::kFileRead);
    if (!phAudioFile->isOpen())
    {
        cout << "Wave file open error!";
        CAudioFileIf::destroy(phAudioFile);
        return -1;
    }
    phAudioFile->getFileSpec(stFileSpec);

    //////////////////////////////////////////////////////////////////////////////
    // open the output text file
    hOutputFile.open(sOutputFilePath.c_str(), std::ios::out);
    if (!hOutputFile.is_open())
    {
        cout << "Text file open error!";
        CAudioFileIf::destroy(phAudioFile);
        return -1;
    }

    //////////////////////////////////////////////////////////////////////////////
    // allocate memory
    ppfAudioData = new float*[stFileSpec.iNumChannels];
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        ppfAudioData[i] = new float[kBlockSize];

    if (ppfAudioData == 0)
    {
        CAudioFileIf::destroy(phAudioFile);
        hOutputFile.close();
        return -1;
    }
    if (ppfAudioData[0] == 0)
    {
        CAudioFileIf::destroy(phAudioFile);
        hOutputFile.close();
        return -1;
    }

    ppfOutputData = new float*[stFileSpec.iNumChannels];
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        ppfOutputData[i] = new float[kBlockSize];

    if (ppfOutputData == 0)
    {
        CAudioFileIf::destroy(phAudioFile);
        hOutputFile.close();
        return -1;
    }
    if (ppfOutputData[0] == 0)
    {
        CAudioFileIf::destroy(phAudioFile);
        hOutputFile.close();
        return -1;
    }

    /////////////////////////////////////////////////////
    // Allocate memory for the CombFilter class and initialize CombFilter
    
    CCombFilterIf* pCombFilter = nullptr;
    if (eFilterType == CCombFilterIf::kCombFIR)
        FIR::create(pCombFilter);
    else
        IIR::create(pCombFilter);
    pCombFilter->init(eFilterType, fDelay, stFileSpec.fSampleRateInHz, stFileSpec.iNumChannels);
    pCombFilter->setParam(CCombFilterIf::kParamGain, fGain);
    pCombFilter->setParam(CCombFilterIf::kParamDelay, fDelay); //don't understand this structureb

    time = clock();

    //////////////////////////////////////////////////////////////////////////////
    // get audio data and write it to the output text file (one column per channel)
    // apply comb filter IN THIS LOOP
    while (!phAudioFile->isEof()) {
        // set block length variable
        long long iNumFrames = kBlockSize;

        // read data (iNumOfFrames might be updated!)
        phAudioFile->readData(ppfAudioData, iNumFrames);

        cout << "\r" << "reading and writing";

        //process method here
        pCombFilter->process(ppfAudioData, ppfOutputData, static_cast<int>(iNumFrames));

        // write
        for (int i = 0; i < iNumFrames; i++)
        {
            for (int c = 0; c < stFileSpec.iNumChannels; c++)
            {
                hOutputFile << ppfOutputData[c][i] << "\t";
            }
            hOutputFile << endl;
        }
    }

    cout << "\nreading/writing done in: \t" << (clock() - time) * 1.F / CLOCKS_PER_SEC << " seconds." << endl;

    //////////////////////////////////////////////////////////////////////////////
    // clean-up (close files and free memory)
    CAudioFileIf::destroy(phAudioFile);
    CCombFilterIf::destroy(pCombFilter);
    hOutputFile.close();

    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        delete[] ppfAudioData[i];
    delete[] ppfAudioData;
    ppfAudioData = 0;

    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        delete[] ppfOutputData[i];
    delete[] ppfOutputData;
    ppfOutputData = 0;

    // all done
    return 0;

}

// Write your tests here
void test()
{
    static const float kSampleRate = 8000.f;
    static const float kPi        = 3.14159265358979323846f;
    static const float kEpsilon   = 1e-4f;
    int iNumPassed = 0, iNumFailed = 0;

    auto printResult = [&](const std::string& sName, bool bPassed)
    {
        cout << (bPassed ? "[PASS] " : "[FAIL] ") << sName << "\n";
        if (bPassed) iNumPassed++; else iNumFailed++;
    };

    // helpers: allocate / free a [iChannels][iLen] buffer (zero-initialised)
    auto allocBuf = [](int iChannels, int iLen) -> float**
    {
        float** pp = new float*[iChannels];
        for (int c = 0; c < iChannels; ++c)
            pp[c] = new float[iLen]();
        return pp;
    };
    auto freeBuf = [](float** pp, int iChannels)
    {
        for (int c = 0; c < iChannels; ++c) delete[] pp[c];
        delete[] pp;
    };

    cout << "\n--- CombFilter Tests ---\n\n";

    // =========================================================
    // TEST 1  FIR: output is zero when input frequency matches a feedforward null
    //
    // FIR: y[n] = x[n] + g*x[n-D]
    // Null condition (|H(f)| = 0) for g = -1: f = k * fs/D
    // Setup: fs=8000, D=4 samples, g=-1 → null at 2000 Hz
    // After the initial D-sample transient the output must be ≈ 0.
    // =========================================================
    {
        const int   iD      = 4;
        const float fDelayS = iD / kSampleRate;
        const int   iN      = 128;

        CCombFilterIf* pFIR = nullptr;
        FIR::create(pFIR);
        pFIR->init(CCombFilterIf::kCombFIR, fDelayS, kSampleRate, 1);
        pFIR->setParam(CCombFilterIf::kParamGain, -1.f);

        float** ppIn  = allocBuf(1, iN);
        float** ppOut = allocBuf(1, iN);
        for (int n = 0; n < iN; ++n)
            ppIn[0][n] = sinf(2.f * kPi * (kSampleRate / iD) * n / kSampleRate);

        pFIR->process(ppIn, ppOut, iN);

        bool bPassed = true;
        for (int n = iD; n < iN; ++n)           // skip initial transient
            if (fabsf(ppOut[0][n]) > kEpsilon) { bPassed = false; break; }

        printResult("FIR: output is zero at null frequency", bPassed);

        CCombFilterIf::destroy(pFIR);
        freeBuf(ppIn, 1);
        freeBuf(ppOut, 1);
    }

    // =========================================================
    // TEST 2  IIR: magnitude is significantly increased at resonant frequency
    //
    // IIR: y[n] = x[n] + g*y[n-D]
    // Peaks at f = k * fs/D; steady-state amplitude → 1/(1-g)
    // Setup: fs=8000, D=4, g=0.9 → peak at 2000 Hz, gain = 1/(1-0.9) = 10
    // After 300 samples the max amplitude should exceed 5.
    // =========================================================
    {
        const int   iD      = 4;
        const float fDelayS = iD / kSampleRate;
        const float fGain   = 0.9f;
        const int   iN      = 300;

        CCombFilterIf* pIIR = nullptr;
        IIR::create(pIIR);
        pIIR->init(CCombFilterIf::kCombIIR, fDelayS, kSampleRate, 1);
        pIIR->setParam(CCombFilterIf::kParamGain, fGain);

        float** ppIn  = allocBuf(1, iN);
        float** ppOut = allocBuf(1, iN);
        for (int n = 0; n < iN; ++n)
            ppIn[0][n] = sinf(2.f * kPi * (kSampleRate / iD) * n / kSampleRate);

        pIIR->process(ppIn, ppOut, iN);

        float fMaxAmp = 0.f;
        for (int n = 0; n < iN; ++n)
            if (fabsf(ppOut[0][n]) > fMaxAmp) fMaxAmp = fabsf(ppOut[0][n]);

        printResult("IIR: magnitude amplified at resonant frequency (max > 5)", fMaxAmp > 5.f);

        CCombFilterIf::destroy(pIIR);
        freeBuf(ppIn, 1);
        freeBuf(ppOut, 1);
    }

    // =========================================================
    // TEST 3  FIR + IIR: result is identical regardless of block size
    //
    // Two identical filters process the same input:
    //   one sample at a time (block size 1)
    //   all at once        (block size N)
    // Every output sample must match within epsilon.
    // =========================================================
    for (int iType = 0; iType < 2; ++iType)
    {
        const int   iD      = 4;
        const float fDelayS = iD / kSampleRate;
        const int   iN      = 64;
        auto eType = (iType == 0) ? CCombFilterIf::kCombFIR : CCombFilterIf::kCombIIR;

        CCombFilterIf* pF1 = nullptr;
        CCombFilterIf* pF2 = nullptr;
        if (iType == 0) { FIR::create(pF1); FIR::create(pF2); }
        else            { IIR::create(pF1); IIR::create(pF2); }

        pF1->init(eType, fDelayS, kSampleRate, 1);
        pF2->init(eType, fDelayS, kSampleRate, 1);
        pF1->setParam(CCombFilterIf::kParamGain, 0.5f);
        pF2->setParam(CCombFilterIf::kParamGain, 0.5f);

        float** ppIn   = allocBuf(1, iN);
        float** ppOut1 = allocBuf(1, iN);
        float** ppOut2 = allocBuf(1, iN);
        for (int n = 0; n < iN; ++n)
            ppIn[0][n] = sinf(2.f * kPi * 500.f * n / kSampleRate);

        // block size 1
        for (int n = 0; n < iN; ++n)
        {
            float* pI = ppIn[0]   + n;
            float* pO = ppOut1[0] + n;
            pF1->process(&pI, &pO, 1);
        }
        // block size N
        pF2->process(ppIn, ppOut2, iN);

        bool bPassed = true;
        for (int n = 0; n < iN; ++n)
            if (fabsf(ppOut1[0][n] - ppOut2[0][n]) > kEpsilon) { bPassed = false; break; }

        std::string sName = std::string(iType == 0 ? "FIR" : "IIR") + ": block-size-1 matches block-size-N";
        printResult(sName, bPassed);

        CCombFilterIf::destroy(pF1);
        CCombFilterIf::destroy(pF2);
        freeBuf(ppIn,   1);
        freeBuf(ppOut1, 1);
        freeBuf(ppOut2, 1);
    }

    // =========================================================
    // TEST 4  FIR + IIR: all-zero input produces all-zero output
    // =========================================================
    for (int iType = 0; iType < 2; ++iType)
    {
        const float fDelayS = 4 / kSampleRate;
        const int   iN      = 64;
        auto eType = (iType == 0) ? CCombFilterIf::kCombFIR : CCombFilterIf::kCombIIR;

        CCombFilterIf* pF = nullptr;
        if (iType == 0) FIR::create(pF); else IIR::create(pF);
        pF->init(eType, fDelayS, kSampleRate, 1);
        pF->setParam(CCombFilterIf::kParamGain, 0.5f);

        float** ppIn  = allocBuf(1, iN);   // zero-initialised
        float** ppOut = allocBuf(1, iN);
        pF->process(ppIn, ppOut, iN);

        bool bPassed = true;
        for (int n = 0; n < iN; ++n)
            if (fabsf(ppOut[0][n]) > kEpsilon) { bPassed = false; break; }

        std::string sName = std::string(iType == 0 ? "FIR" : "IIR") + ": zero input produces zero output";
        printResult(sName, bPassed);

        CCombFilterIf::destroy(pF);
        freeBuf(ppIn,  1);
        freeBuf(ppOut, 1);
    }

    // =========================================================
    // TEST 5  FIR: impulse response has exactly two non-zero taps
    //
    // Input x = [1, 0, 0, ...].  Expected output:
    //   y[0] = 1,  y[D] = g,  y[n] = 0 everywhere else.
    // This directly verifies the FIR difference equation.
    // =========================================================
    {
        const int   iD      = 5;
        const float fDelayS = iD / kSampleRate;
        const float fGain   = 0.7f;
        const int   iN      = 32;

        CCombFilterIf* pFIR = nullptr;
        FIR::create(pFIR);
        pFIR->init(CCombFilterIf::kCombFIR, fDelayS, kSampleRate, 1);
        pFIR->setParam(CCombFilterIf::kParamGain, fGain);

        float** ppIn  = allocBuf(1, iN);   // all zeros
        float** ppOut = allocBuf(1, iN);
        ppIn[0][0] = 1.f;                  // impulse at n=0

        pFIR->process(ppIn, ppOut, iN);

        bool bPassed = (fabsf(ppOut[0][0]   - 1.f)   < kEpsilon) &&
                       (fabsf(ppOut[0][iD]  - fGain) < kEpsilon);
        for (int n = 1; n < iN && bPassed; ++n)
            if (n != iD && fabsf(ppOut[0][n]) > kEpsilon) bPassed = false;

        printResult("FIR: impulse response has exactly two non-zero taps (at 0 and D)", bPassed);

        CCombFilterIf::destroy(pFIR);
        freeBuf(ppIn,  1);
        freeBuf(ppOut, 1);
    }

    cout << "\n=== " << iNumPassed << "/" << (iNumPassed + iNumFailed) << " tests passed ===\n\n";
}

void     showClInfo()
{
    cout << "MUSI6106 Assignment Executable" << endl;
    cout << "(c) 2014-2026 by Alexander Lerch" << endl;
    cout  << endl;

    return;
}
