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

//! @file    supervisor.c
//! @author  Juho Lepistö juho.lepisto(a)gmail.com
//! @date    13 Apr 2020
//! 
//! @brief   This is an example of a voltage supervisor module.

//-----------------------------------------------------------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------------------------------------------------------

#include "supervisor.h"
#include "adc.h"
#include "scheduler.h"
#include "system.h"

//-----------------------------------------------------------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------------------------------------------------------

#define SUPERVISOR_TASK_INTERVAL        100U    //!< A supervisor task interval in milliseconds.
#define SUPERVISOR_ADC_CHANNEL          ADC1    //!< A supervisor ADC channel.
#define SAMPLE_LIMIT                    10U     //<! Number of samples per average. Totals in 100ms * 8 = 800ms per average.
#define ADC_MAX                         0xFFFUL //<! Maximum ADC value.
#define SUPERVISOR_VOLTAGE_AT_MAX_ADC   2000UL  //<! Supervised voltage in 0.01 resolution at maximum ADC value.

#define DIVIDE_AND_ROUND(a, b)          (((a) + ((b) >> 1))/(b))

//-----------------------------------------------------------------------------------------------------------------------------
// Static Variables
//-----------------------------------------------------------------------------------------------------------------------------

staticv bool isInitialised = false;
staticv uint8_t samples = 0U;
staticv uint16_t sampleSum = 0U;
staticv uint16_t voltage = 0U;

//-----------------------------------------------------------------------------------------------------------------------------
// Static Function Prototypes
//-----------------------------------------------------------------------------------------------------------------------------

/// @brief A callback for ADC measurement.
/// @param result - 12-bit ADC result.
staticf void Supervisor_AdcCallback(uint16_t result);

/// @brief A supervisor task that triggers a ADC conversion.
staticf void Supervisor_Task(void);

/// @brief This function converts a given 12-bit ADC value into voltage.
/// @param adc - 12-bit ADC value.
/// @return Returns supervides voltage in resolution of 0.01.
staticf uint16_t Supervisor_AdcToVoltage(uint16_t adc);

//-----------------------------------------------------------------------------------------------------------------------------
// Function Definitions
//-----------------------------------------------------------------------------------------------------------------------------

Error_t Supervisor_Init(void)
{
    Error_t error;
    AdcConfig_t config =
    {
        .channel = SUPERVISOR_ADC_CHANNEL,
        .resolution = ADC_RES_12_BIT,
        .Callback = Supervisor_AdcCallback
    };
    error = HalAdc_Init(&config);
    if (error == ERROR_OK)
    {
        isInitialised = true;
    }
    return error;
}

Error_t Supervisor_Start(void)
{
    Error_t error;
    if (isInitialised)
    {
        samples = 0U;
        sampleSum = 0U;
        voltage = 0U;
        error = Scheduler_CreateTask(Supervisor_Task, SUPERVISOR_TASK_INTERVAL);
    }
    else
    {
        error = ERROR_INVALID_ACTION;
    }
    return error;
}

Error_t Supervisor_Stop(void)
{
    Error_t error;
    if (isInitialised)
    {
        error = Scheduler_DeleteTask(Supervisor_Task);
        samples = 0U;
        sampleSum = 0U;
        voltage = 0U;
    }
    else
    {
        error = ERROR_INVALID_ACTION;
    }
    return error;
}

uint16_t Supervisor_GetVoltage(void)
{
    return voltage;
}

//-----------------------------------------------------------------------------------------------------------------------------
// Static Function Definitions
//-----------------------------------------------------------------------------------------------------------------------------

staticf void Supervisor_AdcCallback(uint16_t result)
{
    sampleSum += result;
    if (++samples >= SAMPLE_LIMIT)
    {
        voltage = Supervisor_AdcToVoltage(DIVIDE_AND_ROUND(sampleSum, samples));
        sampleSum = 0U;
        samples = 0U;
    }
    return;
}

staticf void Supervisor_Task(void)
{
    Error_t error = HalAdc_StartConversion(SUPERVISOR_ADC_CHANNEL);
    if (error != ERROR_OK)
    {
        System_RaiseError(SUPERVISOR_FAILURE);
    }
    return;
}

staticf uint16_t Supervisor_AdcToVoltage(uint16_t adc)
{
    return (uint16_t)DIVIDE_AND_ROUND((uint32_t)adc * SUPERVISOR_VOLTAGE_AT_MAX_ADC, ADC_MAX);
}
