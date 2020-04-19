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

//! @file    system.h
//! @author  Juho Lepistö juho.lepisto(a)gmail.com
//! @date    18 Apr 2020
//! 
//! @brief   This is an example of a system module.

#ifndef SYSTEM_H
#define SYSTEM_H

//-----------------------------------------------------------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------------------------------------------------------

/// @brief A function pointer type for tasks.
typedef enum
{
    SUPERVISOR_FAILURE = 0
} SystemErrorFlag_t;

typedef enum
{
    UNDER_VOLTAGE_WARNING = 0,
    OVER_VOLTAGE_WARNING
} SystemWarningFlag_t;

//-----------------------------------------------------------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------------------------------------------------------

/// @brief This function raises a system error flag.
/// @param flag - Error flag to raise.
void System_RaiseError(SystemErrorFlag_t flag);

/// @brief This function clears a system error flag.
/// @param flag - Error flag to clear.
void System_ClearError(SystemErrorFlag_t flag);

/// @brief This function raises a system warning flag.
/// @param flag - Warning flag to raise.
void System_RaiseWarning(SystemWarningFlag_t flag);

/// @brief This function clears a system warning flag.
/// @param flag - Warning flag to clear.
void System_ClearWarning(SystemWarningFlag_t flag);

#endif // SYSTEM_H
