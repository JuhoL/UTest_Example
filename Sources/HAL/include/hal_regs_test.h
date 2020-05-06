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

//! @file    hal_regs_test.h
//! @author  Juho Lepistö juho.lepisto(a)gmail.com
//! @date    13 Apr 2020
//! 
//! @brief   This is an example of a HAL register access interface for unit tests.

#ifndef HAL_REG_H
#define HAL_REG_H

//-----------------------------------------------------------------------------------------------------------------------------
// Include Dependencies
//-----------------------------------------------------------------------------------------------------------------------------

#include "types.h"

//-----------------------------------------------------------------------------------------------------------------------------
// Mock Functions
//-----------------------------------------------------------------------------------------------------------------------------

// When unit testing, replace the simple macros with mockable functions. See hal_mock.h for details.
uint32_t REG_READ_MOCK(uint32_t* pRegister_);
void REG_WRITE_MOCK(uint32_t* pRegister_, uint32_t value_);
void SET_BIT_MOCK(uint32_t* pRegister_, uint32_t bit_);
void CLEAR_BIT_MOCK(uint32_t* pRegister_, uint32_t bit_);
bool GET_BIT_MOCK(uint32_t* pRegister_, uint32_t bit_);
void SET_BITFIELD_MOCK(uint32_t* pRegister_, uint32_t position_, uint32_t mask_, uint32_t pattern_);
uint32_t GET_BITFIELD_MOCK(uint32_t* pRegister_, uint32_t position_, uint32_t mask_);

//-----------------------------------------------------------------------------------------------------------------------------
// Helper Macros
//-----------------------------------------------------------------------------------------------------------------------------

#define BIT_(position_)                                         (1UL << (position_))
#define REG_READ_(register_)                                    REG_READ_MOCK(&(register_))
#define REG_WRITE_(register_, value_)                           REG_WRITE_MOCK(&(register_), (value_))
#define SET_BIT_(register_, bit_)                               SET_BIT_MOCK(&(register_), (bit_))
#define CLEAR_BIT_(register_, bit_)                             CLEAR_BIT_MOCK(&(register_), (bit_))
#define GET_BIT_(register_, bit_)                               GET_BIT_MOCK(&(register_), (bit_))
#define SET_BITFIELD_(register_, position_, mask_, pattern_)    SET_BITFIELD_MOCK(&(register_), (position_), (mask_), (pattern_))
#define GET_BITFIELD_(register_, position_, mask_)              GET_BITFIELD_MOCK(&(register_), (position_), (mask_))

#endif // HAL_REG_H
