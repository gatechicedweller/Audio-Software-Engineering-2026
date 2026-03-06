#include "CombFilterIf.h"
#include "Combfilter.h"
#include <iostream>

//SAO
static Error_t IIR::create (CCombFilterIf*& pCCombFilterIf) //takes in generic combfilter type
{

    IIR* pIIR = new IIR();
    pCCombFilterIf = pIIR; //takes in pCCombFilterIf, which is a pointer to a CCombFilterIf class, which is defined in the CombFilterIf.h files, turns it into a reference to a pointer, 
    return Error_t::kNoError;
    //the type of PIIR (pointer to an IIR) is different from the type of pCCombFilterIf (pointer to the class CCombFilterIf), is this interesting
    //because the Combfilter isn't a thing on its own, its either an IIR or an FIR, so the pointer to the IIR is the same as the pointer to the CCombFilter
    //polyymooorrppphiiissmsmmmsmssmm
}

//IRL
