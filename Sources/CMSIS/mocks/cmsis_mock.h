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

//! @file    cmsis_mock.h
//! @author  Juho Lepistö juho.lepisto(a)gmail.com
//! @date    18 Apr 2020
//! 
//! @brief   This is an example of an CMSIS module mocks.

#ifndef CMSIS_MOCK_H
#define CMSIS_MOCK_H

//-----------------------------------------------------------------------------------------------------------------------------
// Include Dependencies
//-----------------------------------------------------------------------------------------------------------------------------

#include "fff.h"

extern "C" {
#include "core_cm4_mock.h"
}

//-----------------------------------------------------------------------------------------------------------------------------
// Function Mocks
//-----------------------------------------------------------------------------------------------------------------------------

FAKE_VOID_FUNC(NVIC_SetPriorityGrouping, uint32_t);
FAKE_VALUE_FUNC(uint32_t, NVIC_GetPriorityGrouping);
FAKE_VOID_FUNC(NVIC_EnableIRQ, IRQn_Type);
FAKE_VALUE_FUNC(uint32_t, NVIC_GetEnableIRQ, IRQn_Type);
FAKE_VOID_FUNC(NVIC_DisableIRQ, IRQn_Type);
FAKE_VALUE_FUNC(uint32_t, NVIC_GetDisableIRQ, IRQn_Type);
FAKE_VALUE_FUNC(uint32_t, NVIC_GetPendingIRQ, IRQn_Type);
FAKE_VOID_FUNC(NVIC_SetPendingIRQ, IRQn_Type);
FAKE_VOID_FUNC(NVIC_ClearPendingIRQ, IRQn_Type);
FAKE_VALUE_FUNC(uint32_t, NVIC_GetClearPendingIRQ, IRQn_Type);
FAKE_VALUE_FUNC(uint32_t, NVIC_GetActive, IRQn_Type);
FAKE_VOID_FUNC(NVIC_SetPriority, IRQn_Type, uint32_t);
FAKE_VALUE_FUNC(uint32_t, NVIC_GetPriority, IRQn_Type);
FAKE_VALUE_FUNC(uint32_t, NVIC_EncodePriority, uint32_t, uint32_t, uint32_t);
FAKE_VOID_FUNC(NVIC_DecodePriority, uint32_t, uint32_t, uint32_t*, uint32_t*);
FAKE_VOID_FUNC(NVIC_SetVector, IRQn_Type, uint32_t);
FAKE_VALUE_FUNC(uint32_t, NVIC_GetVector, IRQn_Type);
FAKE_VOID_FUNC(NVIC_SystemReset);
FAKE_VALUE_FUNC(uint32_t, SCB_GetFPUType);
FAKE_VALUE_FUNC(uint32_t, SysTick_Config, uint32_t);
FAKE_VALUE_FUNC(uint32_t, ITM_SendChar, uint32_t);
FAKE_VALUE_FUNC(int32_t, ITM_ReceiveChar);
FAKE_VALUE_FUNC(int32_t, ITM_CheckChar);

//-----------------------------------------------------------------------------------------------------------------------------
// Helper Macros
//-----------------------------------------------------------------------------------------------------------------------------

#define CMSIS_MOCK_RESET() \
{ \
    RESET_FAKE(NVIC_SetPriorityGrouping); \
    RESET_FAKE(NVIC_GetPriorityGrouping); \
    RESET_FAKE(NVIC_EnableIRQ); \
    RESET_FAKE(NVIC_GetEnableIRQ); \
    RESET_FAKE(NVIC_DisableIRQ); \
    RESET_FAKE(NVIC_GetDisableIRQ); \
    RESET_FAKE(NVIC_GetPendingIRQ); \
    RESET_FAKE(NVIC_SetPendingIRQ); \
    RESET_FAKE(NVIC_ClearPendingIRQ); \
    RESET_FAKE(NVIC_GetClearPendingIRQ); \
    RESET_FAKE(NVIC_GetActive); \
    RESET_FAKE(NVIC_SetPriority); \
    RESET_FAKE(NVIC_GetPriority); \
    RESET_FAKE(NVIC_EncodePriority); \
    RESET_FAKE(NVIC_DecodePriority); \
    RESET_FAKE(NVIC_SetVector); \
    RESET_FAKE(NVIC_GetVector); \
    RESET_FAKE(NVIC_SystemReset); \
    RESET_FAKE(SCB_GetFPUType); \
    RESET_FAKE(SysTick_Config); \
    RESET_FAKE(ITM_SendChar); \
    RESET_FAKE(ITM_ReceiveChar); \
    RESET_FAKE(ITM_CheckChar); \
}

#endif // CMSIS_MOCK_H
