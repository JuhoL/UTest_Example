//-----------------------------------------------------------------------------------------------------------------------------
// Copyright (c) 2020 Juho Lepistö
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the "Software"), to deal in the Software without restriction, including without 
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so, subject to the following
// conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial
// portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------------------------------------------------------

//! @file    utest_helpers.cpp
//! @author  Juho Lepistö juho.lepisto(a)gmail.com
//! @date    18 Apr 2020
//! 
//! @brief   These are helper functions for unit tests.

//-----------------------------------------------------------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------------------------------------------------------

#include "utest_helpers.hpp"
#include <stdlib.h>
#include <time.h>

//-----------------------------------------------------------------------------------------------------------------------------
// Function Definitions
//-----------------------------------------------------------------------------------------------------------------------------

namespace UTestHelper
{

void InitRandom(void)
{
    srand(time(NULL));
    return;
}

int GetRandomInt(int min, int max)
{
    int range = max - min;
    int random = rand() % range;
    return random + min;
}

bool GetRandomBool(void)
{
    return ((rand() % 2) == 0);
}

} // namespace UTestHelper
