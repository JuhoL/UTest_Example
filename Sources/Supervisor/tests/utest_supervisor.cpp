//-----------------------------------------------------------------------------------------------------------------------------
// Copyright (c) 2019 Juho Lepistö
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

//! @file    utest_supervisor.cpp
//! @author  Juho Lepistö juho.lepisto(a)gmail.com
//! @date    13 May 2019
//! 
//! @brief   These are example unit tests for utest_supervisor.c
//! 
//! These are unit tests for utest_supervisor.c utilising Catch2 and FFF.

//-----------------------------------------------------------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------------------------------------------------------

#define CATCH_CONFIG_RUNNER
#include <catch_utils.hpp>
#include <fff.h>
DEFINE_FFF_GLOBALS;

extern "C" {
#include "supervisor.h"
}

//-----------------------------------------------------------------------------------------------------------------------------
// Main
//-----------------------------------------------------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    int result = Catch::Session().run(argc, argv);
    return result;
}

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Test Cases
//-----------------------------------------------------------------------------------------------------------------------------

SCENARIO ("System starts", "[feature_tag]")
{
    GIVEN ("system is starting")
    {
        WHEN ("main() is called")
        {
            THEN ("system init, pre-start config, scheduler start shall be called and main loop shall be entered")
            {
                REQUIRE (true);
            }
        }
    }
}
