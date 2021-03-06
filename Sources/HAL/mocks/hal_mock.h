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

//! @file    hal_mock.h
//! @author  Juho Lepistö juho.lepisto(a)gmail.com
//! @date    18 Apr 2020
//! 
//! @brief   This is an example of an HAL module mocks.

#ifndef HAL_MOCK_H
#define HAL_MOCK_H

//-----------------------------------------------------------------------------------------------------------------------------
// Include Dependencies
//-----------------------------------------------------------------------------------------------------------------------------

#include "fff.h"

extern "C" {
#include "hal.h"
}

//-----------------------------------------------------------------------------------------------------------------------------
// Function Mocks
//-----------------------------------------------------------------------------------------------------------------------------

FAKE_VALUE_FUNC(uint32_t, REG_READ_MOCK, uint32_t*);
FAKE_VOID_FUNC(REG_WRITE_MOCK, uint32_t*, uint32_t);
FAKE_VOID_FUNC(SET_BIT_MOCK, uint32_t*, uint32_t);
FAKE_VOID_FUNC(CLEAR_BIT_MOCK, uint32_t*, uint32_t);
FAKE_VALUE_FUNC(bool, GET_BIT_MOCK, uint32_t*, uint32_t);
FAKE_VOID_FUNC(SET_BITFIELD_MOCK, uint32_t*, uint32_t, uint32_t, uint32_t);
FAKE_VALUE_FUNC(uint32_t, GET_BITFIELD_MOCK, uint32_t*, uint32_t, uint32_t);

//-----------------------------------------------------------------------------------------------------------------------------
// Helper Macros
//-----------------------------------------------------------------------------------------------------------------------------

#define HAL_MOCK_RESET() \
{ \
    RESET_FAKE(REG_READ_MOCK); \
    RESET_FAKE(REG_WRITE_MOCK); \
    RESET_FAKE(SET_BIT_MOCK); \
    RESET_FAKE(CLEAR_BIT_MOCK); \
    RESET_FAKE(GET_BIT_MOCK); \
    RESET_FAKE(SET_BITFIELD_MOCK); \
    RESET_FAKE(GET_BITFIELD_MOCK); \
}

#endif // HAL_MOCK_H
