//
//  Test_Vibrato.cpp
//  MUSI8903
//
//  Created by Milap Rane on 2/7/16.
//
//

#include "MUSI8903Config.h"

#ifdef WITH_TESTS
#include <cassert>
#include <cstdio>

#include "UnitTest++.h"

#include "Synthesis.h"
#include "Vector.h"
#include "CombFilterIf.h"

SUITE(CombFilter)
{
    struct VibratoData{
    float VibParam[3]  = {7,0.06,0.05};
       float m_ppfInputData(0),
        float m_ppfOutputData(0),
       float m_iDataLength(35131),
        m_fMaxDelayLength(3.F),
        m_iBlockLength(171),
        m_iNumChannels(3),
        m_fSampleRate(8000),
        
        
    
        
    };
}
#endif //WITH_TESTS
