#include "MUSI8903Config.h"

#ifdef WITH_TESTS
#include <cassert>
#include <cstdio>

#include "UnitTest++.h"

#include "Vector.h"

#include "FastConv.h"

SUITE(FastConv)
{
    struct FastConvData
    {
        FastConvData() 
        {
        }

        ~FastConvData() 
        {
        }


        CFastConv *m_pCFastConv;
    };


    TEST_FIXTURE(FastConvData, IrTest)
    {
    }

    TEST_FIXTURE(FastConvData, InputBlockLengthTest)
    {
    }

}

#endif //WITH_TESTS