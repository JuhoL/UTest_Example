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

//! @file    hal.h
//! @author  Juho Lepistö juho.lepisto(a)gmail.com
//! @date    13 Apr 2020
//! 
//! @brief   This is an example of a HAL interface.

#ifndef HAL_H
#define HAL_H

//-----------------------------------------------------------------------------------------------------------------------------
// Include Dependencies
//-----------------------------------------------------------------------------------------------------------------------------

#include "types.h"
#ifdef UNIT_TEST
    #include "stm32f429xx_mock.h"
#else
    #include "stm32f4xx.h"
#endif

//-----------------------------------------------------------------------------------------------------------------------------
// Helper Macros
//-----------------------------------------------------------------------------------------------------------------------------

#define BIT(position_)                      (1UL << (position_))

#ifdef UNIT_TEST

// When unit testing, replace the simple macros with mockable functions. See hal_mock.h for details.
extern void BitMock_Set(uint32_t* pRegister_, uint32_t bit_);
extern void BitMock_Clear(uint32_t* pRegister_, uint32_t bit_);
extern bool BitMock_Get(uint32_t* pRegister_, uint32_t bit_);
extern void BitMock_SetBitfield(uint32_t* pRegister_, uint32_t position_, uint32_t mask_, uint32_t pattern_);
extern uint32_t BitMock_GetBitfield(uint32_t* pRegister_, uint32_t position_, uint32_t mask_);

#define SET_BIT(register_, bit_)                                BitMock_Set(&(register_), (bit_))
#define CLEAR_BIT(register_, bit_)                              BitMock_Clear(&(register_), (bit_))
#define GET_BIT(register_, bit_)                                BitMock_Get(&(register_), (bit_))
#define SET_BITFIELD(register_, position_, mask_, pattern_)     BitMock_SetBitfield(&(register_), (position_), (mask_), (pattern_))
#define GET_BITFIELD(register_, position_, mask_)               BitMock_GetBitfield(&(register_), (position_), (mask_))

#else

#define SET_BIT(register_, bit_)                                {(register_) |= BIT(bit_);}
#define CLEAR_BIT(register_, bit_)                              {(register_) &= ~(BIT(bit_));}
#define GET_BIT(register_, bit_)                                (((register_) & (BIT(bit_))) != 0UL)
#define SET_BITFIELD(register_, position_, mask_, pattern_)     {(register_) &= (mask_) << (position_); (register_) |= (pattern_) << (position_);}
#define GET_BITFIELD(register_, position_, mask_)               (((register_) << (position_)) & (mask_))

#endif

#endif // HAL_H
