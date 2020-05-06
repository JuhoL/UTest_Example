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
    #include "hal_regs_test.h"
#else
    #include "stm32f4xx.h"
    #include "hal_regs.h"
#endif

//-----------------------------------------------------------------------------------------------------------------------------
// Helper Macros
//-----------------------------------------------------------------------------------------------------------------------------

/// @brief Gets a bit of a given position.
/// @param bit_ - A number of a bit in range of [0, 31].
#define BIT(position_)                                          BIT_(position_)

/// @brief This macro reads a given register.
/// @param register_ - A register to be read.
/// @return A 32-bit register value.
#define REG_READ(register_)                                     REG_READ_(register_)

/// @brief This macro writes into a given register.
/// @param register_ - A register where to write.
/// @param value_ - A 32-bit value to write.
#define REG_WRITE(register_, value_)                            REG_WRITE_((register_), (value_))

/// @brief This macro sets a bit in a given register.
/// @param register_ - A register to set a bit in.
/// @param bit_ - A number of a bit to be set in range of [0, 31].
#define SET_BIT(register_, bit_)                                SET_BIT_((register_), (bit_))

/// @brief This macro clears a bit from a given register.
/// @param register_ - A register to clear a bit from.
/// @param bit_ - A number of a bit to be cleared in range of [0, 31].
#define CLEAR_BIT(register_, bit_)                              CLEAR_BIT_((register_), (bit_))

/// @brief This macro reads a given bit from a given register.
/// @param register_ - A register to read a bit from.
/// @param bit_ - A number of a bit to be read in range of [0, 31].
/// @return Returns true or false depending on the bit state.
#define GET_BIT(register_, bit_)                                GET_BIT_((register_), (bit_))

/// @brief This macro sets a bitfield in a given register.
/// For example if we have a register full of ones and we call SET_BITFIELD(register, 5, 0x7, 0x2),
/// the register will change to 1111 1111 0101 1111, 0xFF5F.
/// @param register_ - A register to write.
/// @param position_ - A position of the lowest bit in the pattern in range of [0, 31], i.e. the shift of the mask_ and pattern_.
/// @param mask_ - A right-aligned mask to clear prior set.
/// @param pattern_ - A pattern to set after clear.
#define SET_BITFIELD(register_, position_, mask_, pattern_)     SET_BITFIELD_((register_), (position_), (mask_), (pattern_))

/// @brief This macro reads a bitfield from a given register.
/// For example if we have a register which value is 0xABCD and we call GET_BITFIELD(register, 5, 0x7),
/// the result will be 0x6.
/// @param register_ - A register to read from.
/// @param position_ - A position of the lowest bit in the pattern in range of [0, 31], i.e. the shift of the mask_.
/// @param mask_ - A mask of the bitfield.
/// @return A 32-bit bitfield.
#define GET_BITFIELD(register_, position_, mask_)               GET_BITFIELD_((register_), (position_), (mask_))

#endif // HAL_H
