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

//! @file    scheduler.h
//! @author  Juho Lepistö juho.lepisto(a)gmail.com
//! @date    13 Apr 2020
//! 
//! @brief   This is an example of a scheduler module.

#ifndef SCHEDULER_H
#define SCHEDULER_H

//-----------------------------------------------------------------------------------------------------------------------------
// Include Dependencies
//-----------------------------------------------------------------------------------------------------------------------------

#include "types.h"

//-----------------------------------------------------------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------------------------------------------------------

/// @brief A function pointer type for tasks.
typedef void (*Task_t)(void);

//-----------------------------------------------------------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------------------------------------------------------

/// @brief This function creates a task for given function pointer.
/// @param Task - A task function.
/// @param interval - Task call interval in milliseconds.
/// @return Returns a corresponding error code. See types.h.
Error_t Scheduler_CreateTask(Task_t Task, uint16_t interval);

/// @brief This function deletes a task of given function pointer.
/// @param Task - A task function to be removed from execution.
/// @return Returns a corresponding error code. See types.h.
Error_t Scheduler_DeleteTask(Task_t Task);

#endif // SCHEDULER_H
