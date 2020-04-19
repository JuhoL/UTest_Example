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

//! @file    utest_helpers.hpp
//! @author  Juho Lepistö juho.lepisto(a)gmail.com
//! @date    18 Apr 2020
//! 
//! @brief   These are helper functions for unit tests.

#ifndef UTEST_HELPERS_HPP
#define UTEST_HELPERS_HPP

//-----------------------------------------------------------------------------------------------------------------------------
// FFF Helper Macros
//----------------------------------------------------------------------------------------------------------------------------

/// @brief Initialises FFF framework.
#define INIT_MOCKS()                                                FFF_RESET_HISTORY()

/// @brief Gets a call count of given function.
/// @param function_ - A Function name.
#define MOCK_CALLS(function_)                                       (function_##_fake.call_count)

/// @brief Checks if a given function is called at position starting from index 0.
/// @param function_ - A Function name.
/// @param position_ - A chronological position of the call starting from 0.
#define MOCK_IS_CALLED_AT_POSITION(function_, position_)            (fff.call_history[(position_)] == (void*)(function_))

/// @brief Gives the last argument of a function.
/// @param function_ - A Function name.
/// @param arg_ - An argument index, where 0 is the first argument.
#define MOCK_LAST_ARG(function_, arg_)                              (function_##_fake.arg##arg_##_val)

/// @brief Sets a return value for a function.
/// @param function_ - A Function name.
/// @param return_ - The return value.
#define MOCK_SET_RETURN_VALUE(function_, return_)                   {function_##_fake.return_val = (return_);}

/// @brief Sets a return value sequence for a function.
/// @param function_ - A Function name.
/// @param array_ - An array of return values.
/// @param size_ - Number of return values in the sequence.
#define MOCK_SET_RETURN_VALUE_SEQUENCE(function_, array_, size_)    SET_RETURN_SEQ(function_, (array_), (size_))

/// @brief Sets a custom fake for a function.
/// @param function_ - A Function name.
/// @param fake_ - A function to be called in place of the actual function.
#define MOCK_SET_CUSTOM_FAKE(function_, fake_)                      {function_##_fake.custom_fake = (fake_);}

//-----------------------------------------------------------------------------------------------------------------------------
// Random Number Functions
//----------------------------------------------------------------------------------------------------------------------------

/// @brief Initialises the random number generator.
void UTestHelper_InitRandom(void);

/// @brief Gets a random integer
/// @param min - A minimum value included in the range.
/// @param max - A maximum value excluded from the range.
/// @return A random integer in range [min, max[
int UTestHelper_GetRandomInt(int min, int max);

#endif // UTEST_HELPERS_HPP
