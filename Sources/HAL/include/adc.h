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

//! @file    adc.h
//! @author  Juho Lepistö juho.lepisto(a)gmail.com
//! @date    18 Apr 2020
//! 
//! @brief   This is an simplified example of an ADC module.

#ifndef ADC_H
#define ADC_H

//-----------------------------------------------------------------------------------------------------------------------------
// Include Dependencies
//-----------------------------------------------------------------------------------------------------------------------------

#include "types.h"

//-----------------------------------------------------------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------------------------------------------------------

/// @brief A function pointer type for ADC callbacks.
/// Parameter is the ADC result.
typedef void (*AdcCallback_t)(uint16_t);

/// @brief An ADC channel enum
typedef enum
{
    ADC0 = 0,   //!< ADC channel 0
    ADC1,       //!< ADC channel 1
    ADC2        //!< ADC channel 2
} AdcChannel_t;

/// @brief ADC channel resolution
typedef enum
{
    ADC_RES_12_BIT = 0, //!< 12-bit resolution
    ADC_RES_10_BIT,     //!< 10-bit resolution
    ADC_RES_8_BIT       //!< 8-bit resolution
} AdcResolution_t;

/// @brief ADC configuration struct
typedef struct
{
    AdcChannel_t channel;       //!< A channel selector
    AdcResolution_t resolution; //!< A channel resolution
    AdcCallback_t Callback;     //!< A callback for passing results.
} AdcConfig_t;

//-----------------------------------------------------------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------------------------------------------------------

/// @brief This function initialises an ADC channel based on the configuration struct.
/// @param pConfig - A pointer to the configuration struct.
/// @return Returns a corresponding error code. See types.h.
Error_t HalAdc_Init(const AdcConfig_t* pConfig);

/// @brief This function starts a conversion of a given channel.
/// @param channel - A channel to convert.
/// @return Returns a corresponding error code. See types.h.
Error_t HalAdc_StartConversion(AdcChannel_t channel);

#endif // ADC_H
