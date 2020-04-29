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

//! @file    supervisor.h
//! @author  Juho Lepistö juho.lepisto(a)gmail.com
//! @date    13 Apr 2020
//! 
//! @brief   This is an example of a voltage supervisor module.
//! The module monitors voltage of 12V line and raises a system level warning flag and pulls an alarm line low
//! if the voltage is outside acceptable limits.

#ifndef SUPERVISOR_H
#define SUPERVISOR_H

//-----------------------------------------------------------------------------------------------------------------------------
// Include Dependencies
//-----------------------------------------------------------------------------------------------------------------------------

#include "types.h"

//-----------------------------------------------------------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------------------------------------------------------

/// @brief This function initialises the supervisor module.
void Supervisor_Init(void);

/// @brief This function starts the voltage supervision.
/// @return Returns a corresponding error code. See types.h.
Error_t Supervisor_Start(void);

/// @brief This function stops the voltage supervision.
/// @return Returns a corresponding error code. See types.h.
Error_t Supervisor_Stop(void);

/// @brief This function is used to read the latest voltage measurement.
/// @return Returns the lates voltage in resolution of 0.01.
uint16_t Supervisor_GetVoltage(void);

#endif // SUPERVISOR_H
