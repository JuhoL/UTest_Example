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

//! @file    utils.h
//! @author  Juho Lepistö juho.lepisto(a)gmail.com
//! @date    28 Apr 2020
//! 
//! @brief   These are utility macros.

#ifndef UTILS_H
#define UTILS_H

//-----------------------------------------------------------------------------------------------------------------------------
// Include Dependencies
//-----------------------------------------------------------------------------------------------------------------------------

#include "system.h"

//-----------------------------------------------------------------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------------------------------------------------------------

#ifdef RELEASE
#define UTILS_ASSERT_VOID(_condition, _error)
#define UTILS_ASSERT(_condition, _error, returnValue_)
#else
#define UTILS_ASSERT_VOID(_condition, _error)               {if (!(_condition)) {System_RaiseError(_error); return;}}
#define UTILS_ASSERT(_condition, _error, returnValue_)      {if (!(_condition)) {System_RaiseError(_error); return (returnValue_);}}
#endif

/// @brief This macro is used to get array length.
#define UTILS_ARRAY_LENGTH(array_, type_)     (sizeof(array_)/sizeof(type_))

/// @brief A simple helper macro for dividing and rounding integers.
#define UTILS_DIVIDE_AND_ROUND(a, b)          (((a) + ((b) >> 1))/(b))

#endif // UTILS_H
