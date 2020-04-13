//-----------------------------------------------------------------------------------------------------------------------------
// Copyright (c) 2018 Juho Lepistö
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

//! @file    catch_utils.hpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    12 Oct 2018
//!
//! @brief   Utility functions and macros for Catch2.

#ifndef CATCH_UTILS_HPP
#define CATCH_UTILS_HPP

//-----------------------------------------------------------------------------------------------------------------------------
// Include Dependencies
//-----------------------------------------------------------------------------------------------------------------------------

#include <catch.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// Typedefs, Structs, Enums and Constants
//-----------------------------------------------------------------------------------------------------------------------------

//! This macro is used to "prettify" FakeIt mock fake call.
#define CALL(mockClass, member)                                 Method(mockClass, member)

//! This macro is used to "prettify" FakeIt mock fake call with parameters.
#define PARAM_CALL(mockClass, member, ...)                      Method(mockClass, member).Using(__VA_ARGS__)

//! This macro is used to "prettify" FakeIt mock checks of members without parameters.
#define REQUIRE_CALLS(callCount, mockClass, member)             REQUIRE_NOTHROW (Verify(Method(mockClass, member)).Exactly(callCount))

//! This macro is used to "prettify" FakeIt mock checks of members with parameters.
#define REQUIRE_PARAM_CALLS(callCount, mockClass, member, ...)  REQUIRE_NOTHROW (Verify(Method(mockClass, member).Using(__VA_ARGS__)).Exactly(callCount))

//! This macro is used to "prettify" call order macros. It's basically just an alias for REQUIRE_NOTHROW.
#define REQUIRE_CALL_ORDER(calls)                               REQUIRE_NOTHROW (calls)

#define SET_RETURN(mock, function, returnValue)                 When(Method(mock, function)).AlwaysReturn(returnValue);

#define WITHIN_LIMITS(value, min, max)                          (((value) >= (min)) && ((value) <= (max)))

#define APPROX_EQUAL(value, expected, error)                    (((value) >= ((expected) - (error))) && ((value) <= ((expected) + (error))))

#endif // CATCH_UTILS_HPP
