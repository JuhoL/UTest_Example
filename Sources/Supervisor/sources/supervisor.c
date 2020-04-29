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
//! The module monitors voltage of 12V line and raises a system level warning flag and pulls an alarm line low
//! if the voltage is outside acceptable limits.

//-----------------------------------------------------------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------------------------------------------------------

#include "supervisor.h"
#include "adc.h"
#include "scheduler.h"
#include "system.h"
#include "gpio.h"
#include "utils.h"

//-----------------------------------------------------------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------------------------------------------------------

#define SUPERVISOR_TASK_INTERVAL        100U    //!< A supervisor task interval in milliseconds.
#define SUPERVISOR_ADC_CHANNEL          ADC1    //!< A supervisor ADC channel.
#define SUPERVISOR_ALARM_PORT           portC   //!< A supervisor alarm GPIO port.
#define SUPERVISOR_ALARM_PIN_NUMBER     5U      //!< A supervisor alarm GPIO pin number.

#define SAMPLE_LIMIT                    10U     //<! Number of samples per average. Totals in 100ms * 8 = 800ms per average.
#define ADC_MAX                         0xFFFUL //<! Maximum ADC value.
#define SUPERVISOR_VOLTAGE_AT_MAX_ADC   2000UL  //<! Supervised voltage in 0.01 resolution at maximum ADC value.
#define SUPERVISOR_UV_LIMIT             1050U   //<! Voltage limit for undervoltage
#define SUPERVISOR_UV_RECOVERY_LIMIT    1100U   //<! Voltage limit for undervoltage recovery
#define SUPERVISOR_OV_LIMIT             1350U   //<! Voltage limit for overvoltage
#define SUPERVISOR_OV_RECOVERY_LIMIT    1300U   //<! Voltage limit for overvoltage recovery

//-----------------------------------------------------------------------------------------------------------------------------
// Static Variables
//-----------------------------------------------------------------------------------------------------------------------------

/// @brief Alarm GPIO pin.
staticv const GpioPin_t alarmPin = {.port = SUPERVISOR_ALARM_PORT, .number = SUPERVISOR_ALARM_PIN_NUMBER};

staticv bool isInitialised = false; //<! A flag indicating if the module has been initialised successfully.
staticv uint8_t samples = 0U;       //<! Number of samples in sampleSum.
staticv uint16_t sampleSum = 0U;    //<! Sum of samples.
staticv uint16_t voltage = 0U;      //<! Latest measured voltage.
staticv bool uvIsActive = false;    //<! A flag indicating if undervoltage is active.
staticv bool ovIsActive = false;    //<! A flag indicating if overvoltage is active.

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

/// @brief This function updates the warning flag statuses based on the latest measurement.
staticf void Supervisor_UpdateWarnings(void);

//-----------------------------------------------------------------------------------------------------------------------------
// Function Definitions
//-----------------------------------------------------------------------------------------------------------------------------

void Supervisor_Init(void)
{
    const AdcConfig_t adcConfig =
    {
        .channel = SUPERVISOR_ADC_CHANNEL,
        .resolution = ADC_RES_12_BIT,
        .Callback = Supervisor_AdcCallback
    };
    HalAdc_SetConfiguration(&adcConfig);

    const GpioConfig_t gpioConfig =
    {
        .pin = {.port = SUPERVISOR_ALARM_PORT, .number = SUPERVISOR_ALARM_PIN_NUMBER},
        .mode = output,
        .isOpenDrain = true,
        .speed = low,
        .pull = floating
    };
    HalGpio_SetConfiguration(&gpioConfig);

    isInitialised = true;
    return;
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
        voltage = Supervisor_AdcToVoltage(UTILS_DIVIDE_AND_ROUND(sampleSum, samples));
        sampleSum = 0U;
        samples = 0U;
        Supervisor_UpdateWarnings();
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
    return (uint16_t)UTILS_DIVIDE_AND_ROUND((uint32_t)adc * SUPERVISOR_VOLTAGE_AT_MAX_ADC, ADC_MAX);
}

staticf void Supervisor_UpdateWarnings(void)
{
    bool alarmChange = false;
    if (uvIsActive)
    {
        if (voltage >= SUPERVISOR_UV_RECOVERY_LIMIT)
        {
            System_ClearWarning(UNDERVOLTAGE_WARNING);
            uvIsActive = false;
            alarmChange = true;
        }
    }
    else
    {
        if (voltage < SUPERVISOR_UV_LIMIT)
        {
            System_RaiseWarning(UNDERVOLTAGE_WARNING);
            uvIsActive = true;
            alarmChange = true;
        }
    }

    if (ovIsActive)
    {
        if (voltage <= SUPERVISOR_OV_RECOVERY_LIMIT)
        {
            System_ClearWarning(OVERVOLTAGE_WARNING);
            ovIsActive = false;
            alarmChange = true;
        }
    }
    else
    {
        if (voltage > SUPERVISOR_OV_LIMIT)
        {
            System_RaiseWarning(OVERVOLTAGE_WARNING);
            ovIsActive = true;
            alarmChange = true;
        }
    }

    if (alarmChange)
    {
        HalGpio_SetOutputState(&alarmPin, (uvIsActive || ovIsActive));
    }
    return;
}
