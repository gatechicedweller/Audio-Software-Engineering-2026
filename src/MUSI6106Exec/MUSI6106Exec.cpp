
#include <ctime>

#include "MUSI6106Config.h"

#include <iostream>
#include "RingBuffer.h"

using std::cout;
using std::endl;

// local function declarations
void    showClInfo ();

/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
    showClInfo();

    CRingBuffer<float>* pCRingBuff = 0;
    static const int kBlockSize = 17;
    pCRingBuff = new CRingBuffer<float>(kBlockSize);

    // above can be replaced by: auto* pCRingBuff = new CRingBuffer<float>(kBlockSize);

    for (int i = 0; i < 5; i++)
    {
        pCRingBuff->putPostInc(1.F*i);
    }

    float sample = 0.0F;
    for (int i = 5; i < 30; i++)
    {
        pCRingBuff->getNumValuesInBuffer(); // should be five
        sample = pCRingBuff->getPostInc(); // should be i-5
        pCRingBuff->putPostInc(1.F*i);
    }

    cout << sample << endl;
    // all done
    delete pCRingBuff;
    pCRingBuff = nullptr;

    return 0;

}


void     showClInfo()
{
    cout << "MUSI6106 Assignment Executable" << endl;
    cout << "(c) 2014-2026 by Alexander Lerch" << endl;
    cout  << endl;

    return;
}
