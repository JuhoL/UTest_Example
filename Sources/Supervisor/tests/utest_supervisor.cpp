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

//! @file    utest_supervisor.cpp
//! @author  Juho Lepistö juho.lepisto(a)gmail.com
//! @date    13 May 2020
//! 
//! @brief   These are example unit tests for utest_supervisor.c
//! 
//! These are unit tests for utest_supervisor.c utilizing Catch2 and FFF.

//-----------------------------------------------------------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------------------------------------------------------

#define CATCH_CONFIG_RUNNER
#include <catch_utils.hpp>
#include <fff.h>
DEFINE_FFF_GLOBALS;
#include "utest_helpers.hpp"

extern "C" {
#include "supervisor.h"
}

// Mocks
#include "adc_mock.h"
#include "gpio_mock.h"
#include "scheduler_mock.h"
#include "system_mock.h"

//-----------------------------------------------------------------------------------------------------------------------------
// Main
//-----------------------------------------------------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    UTestHelper::InitRandom();
    int result = Catch::Session().run(argc, argv);
    return result;
}

//-----------------------------------------------------------------------------------------------------------------------------
// Statics of UUT
//-----------------------------------------------------------------------------------------------------------------------------

extern "C" {

extern const GpioPin_t alarmPin;
extern bool isInitialised;
extern uint8_t samples;
extern uint16_t sampleSum;
extern uint16_t voltage;
extern bool uvIsActive;
extern bool ovIsActive;

extern void Supervisor_AdcCallback(uint16_t result);
extern void Supervisor_Task(void);
extern uint16_t Supervisor_AdcToVoltage(uint16_t adc);

}

//-----------------------------------------------------------------------------------------------------------------------------
// Test Variables
//-----------------------------------------------------------------------------------------------------------------------------

static AdcConfig_t adcConfig;
static GpioConfig_t gpioConfig;

//-----------------------------------------------------------------------------------------------------------------------------
// Custom Fake Prototypes
//-----------------------------------------------------------------------------------------------------------------------------

/// @brief A custom fake for HalAdc_SetConfiguration().
static void HalAdc_SetConfiguration_CustomFake(const AdcConfig_t* pConfig);

/// @brief A custom fake for HalGpio_SetConfiguration().
static void HalGpio_SetConfiguration_CustomFake(const GpioConfig_t* pConfig);

//-----------------------------------------------------------------------------------------------------------------------------
// Helper Functions
//-----------------------------------------------------------------------------------------------------------------------------

/// @brief This helper function sets the supervision voltage to a given value by calling Supervisor_AdcCallback() repeatedly.
/// @param voltage - An voltage value to set in 0.01 resolution.
static void Helper_SetVoltage(uint32_t voltage);

//-----------------------------------------------------------------------------------------------------------------------------
// Test Cases
//-----------------------------------------------------------------------------------------------------------------------------

SCENARIO ("Supervisor module is initialised", "[supervisor]")
{
    INIT_MOCKS();
    ADC_MOCK_RESET();
    GPIO_MOCK_RESET();
    
    MOCK_SET_CUSTOM_FAKE(HalAdc_SetConfiguration, HalAdc_SetConfiguration_CustomFake);
    MOCK_SET_CUSTOM_FAKE(HalGpio_SetConfiguration, HalGpio_SetConfiguration_CustomFake);

    GIVEN ("the module is not initialised")
    {
        isInitialised = false;

        WHEN ("the supervisor is initialised")
        {
            Supervisor_Init();

            THEN ("the initialised flag shall be set")
            {
                REQUIRE (isInitialised == true);

                AND_THEN ("the ADC shall be configured")
                {
                    REQUIRE (MOCK_NEXT_CALLED_FUNCTION_IS(HalAdc_SetConfiguration));
                    REQUIRE (MOCK_CALLS(HalAdc_SetConfiguration) == 1);

                    REQUIRE (adcConfig.channel == ADC1);
                    REQUIRE (adcConfig.resolution == ADC_RES_12_BIT);
                    REQUIRE (adcConfig.Callback == Supervisor_AdcCallback);

                    AND_THEN ("the alarm GPIO shall be configured")
                    {
                        REQUIRE (MOCK_NEXT_CALLED_FUNCTION_IS(HalGpio_SetConfiguration));
                        REQUIRE (MOCK_CALLS(HalGpio_SetConfiguration) == 1);

                        REQUIRE (gpioConfig.pin.port == portC);
                        REQUIRE (gpioConfig.pin.number == 5U);
                        REQUIRE (gpioConfig.mode == output);
                        REQUIRE (gpioConfig.isOpenDrain == true);
                        REQUIRE (gpioConfig.speed == low);
                        REQUIRE (gpioConfig.pull == floating);
                    }
                }
            }
        }
    }
}

SCENARIO ("Supervision is started", "[supervisor]")
{
    INIT_MOCKS();
    SCHEDULER_MOCK_RESET();

    GIVEN ("the module is initialised and there are some random measurement data")
    {
        isInitialised = true;
        samples = 123U;
        sampleSum = 123U;
        voltage = 123U;

        WHEN ("the supervision is started")
        {
            Error_t error = Supervisor_Start();

            THEN ("no errors shall occur")
            {
                REQUIRE (error == ERROR_OK);

                AND_THEN ("the supervision task shall be created")
                {
                    REQUIRE (MOCK_CALLS(Scheduler_CreateTask) == 1);
                    REQUIRE (MOCK_LAST_ARG(Scheduler_CreateTask, 0) == Supervisor_Task);
                    REQUIRE (MOCK_LAST_ARG(Scheduler_CreateTask, 1) == 100);
                    REQUIRE (MOCK_IS_CALLED_AT_POSITION(Scheduler_CreateTask, 0));

                    AND_THEN ("the measurement data shall be reset")
                    {
                        REQUIRE (samples == 0U);
                        REQUIRE (sampleSum == 0U);
                        REQUIRE (voltage == 0U);
                    }
                }
            }
        }
    }
}

SCENARIO ("Supervision start fails", "[supervisor][error_handling]")
{
    INIT_MOCKS();
    SCHEDULER_MOCK_RESET();

    GIVEN ("the module is initialised")
    {
        isInitialised = true;

        AND_GIVEN ("Scheduler_CreateTask() fails")
        {
            MOCK_SET_RETURN_VALUE(Scheduler_CreateTask, ERROR_NOT_ENOUGH_RESOURCES);

            WHEN ("the supervision is started")
            {
                Error_t error = Supervisor_Start();

                THEN ("the error shall propagate")
                {
                    REQUIRE (error == ERROR_NOT_ENOUGH_RESOURCES);
                }
            }
        }
    }

    GIVEN ("the module is not initialised")
    {
        isInitialised = false;

        WHEN ("the supervision is started")
        {
            Error_t error = Supervisor_Start();

            THEN ("an invalid action error shall occur")
            {
                REQUIRE (error == ERROR_INVALID_ACTION);
            }
        }
    }
}

SCENARIO ("Supervision is stopped", "[supervisor]")
{
    INIT_MOCKS();
    SCHEDULER_MOCK_RESET();

    GIVEN ("the module is initialised and some random measurement data")
    {
        isInitialised = true;
        samples = 123U;
        sampleSum = 123U;
        voltage = 123U;

        WHEN ("the supervision is stopped")
        {
            Error_t error = Supervisor_Stop();

            THEN ("no errors shall occur")
            {
                REQUIRE (error == ERROR_OK);

                AND_THEN ("the supervision task shall be deleted")
                {
                    REQUIRE (MOCK_CALLS(Scheduler_DeleteTask) == 1);
                    REQUIRE (MOCK_LAST_ARG(Scheduler_DeleteTask, 0) == Supervisor_Task);
                    REQUIRE (MOCK_IS_CALLED_AT_POSITION(Scheduler_DeleteTask, 0));

                    AND_THEN ("the measurement data shall be reset")
                    {
                        REQUIRE (samples == 0U);
                        REQUIRE (sampleSum == 0U);
                        REQUIRE (voltage == 0U);
                    }
                }
            }
        }
    }
}

SCENARIO ("Supervision stop fails", "[supervisor][error_handling]")
{
    INIT_MOCKS();
    SCHEDULER_MOCK_RESET();

    GIVEN ("the module is initialised")
    {
        isInitialised = true;

        AND_GIVEN ("Scheduler_DeleteTask() fails")
        {
            MOCK_SET_RETURN_VALUE(Scheduler_DeleteTask, ERROR_INVALID_ACTION);

            WHEN ("the supervision is stopped")
            {
                Error_t error = Supervisor_Stop();

                THEN ("the error shall propagate")
                {
                    REQUIRE (error == ERROR_INVALID_ACTION);
                }
            }
        }
    }

    GIVEN ("the module is not initialised")
    {
        isInitialised = false;

        WHEN ("the supervision is stopped")
        {
            Error_t error = Supervisor_Stop();

            THEN ("the error shall propagate")
            {
                REQUIRE (error == ERROR_INVALID_ACTION);
            }
        }
    }
}

SCENARIO ("Supervision voltage is read", "[supervisor]")
{
    GIVEN ("there is a random voltage detected")
    {
        voltage = (uint16_t)UTestHelper::GetRandomInt(5000, 20000);

        WHEN ("the voltage is read")
        {
            uint16_t readVoltage = Supervisor_GetVoltage();

            THEN ("the voltages shall match")
            {
                REQUIRE (readVoltage == voltage);
            }
        }
    }
}

SCENARIO ("Supervision task is called", "[supervisor]")
{
    INIT_MOCKS();
    ADC_MOCK_RESET();
    SYSTEM_MOCK_RESET();

    GIVEN ("no prerequisites")
    {
        WHEN ("the supervision task is called")
        {
            Supervisor_Task();

            THEN ("the ADC conversion shall be started")
            {
                REQUIRE (MOCK_CALLS(HalAdc_StartConversion) == 1);
                REQUIRE (MOCK_LAST_ARG(HalAdc_StartConversion, 0) == ADC1);
                REQUIRE (MOCK_IS_CALLED_AT_POSITION(HalAdc_StartConversion, 0));

                AND_THEN ("supervisor failure shall not be raised")
                {
                    REQUIRE (MOCK_CALLS(System_RaiseError) == 0);
                }
            }
        }
    }
}

SCENARIO ("Supervision task fails", "[supervisor][error_handling]")
{
    INIT_MOCKS();
    SYSTEM_MOCK_RESET();

    GIVEN ("HalAdc_StartConversion() fails")
    {
        MOCK_SET_RETURN_VALUE(HalAdc_StartConversion, ERROR_PERIPHERAL_FAILURE)

        WHEN ("the supervision task is called")
        {
            Supervisor_Task();

            THEN ("supervisor failure shall be raised")
            {
                REQUIRE (MOCK_CALLS(System_RaiseError) == 1);
                REQUIRE (MOCK_LAST_ARG(System_RaiseError, 0) == SUPERVISOR_FAILURE);
            }
        }
    }
}

SCENARIO ("ADC value is converted into voltage", "[supervisor]")
{
    GIVEN ("no prerequisites")
    {
        WHEN ("a ADC value 0x999 converted into voltage")
        {
            uint16_t result = Supervisor_AdcToVoltage(0x999);

            THEN ("the ADC value shall be correct")
            {
                REQUIRE (result == 1200U);
            }
        }

        WHEN ("a ADC value 0x805 converted into voltage")
        {
            uint16_t result = Supervisor_AdcToVoltage(0x805);

            THEN ("the ADC value shall be correct")
            {
                REQUIRE (result == 1003U);
            }
        }
    }
}

TEST_CASE ("Samples are filtered and voltage updated", "[supervisor]")
{
    // The supervision start from zero
    isInitialised = true;
    samples = 0U;
    sampleSum = 0U;
    voltage = 0U;

    // Prepare ADC sample array
    uint16_t samples[20] = {0x998, 0x99A, 0x996, 0x99C, 0x99E,
                            0x9A0, 0x9A2, 0x998, 0x990, 0x993,
                            0x98C, 0x98A, 0x988, 0x984, 0x987,
                            0x98C, 0x990, 0x996, 0x999, 0x99E};
    
    // When seven samples are read
    int i = 0;
    do
    {
        Supervisor_AdcCallback(samples[i]);
    } while (++i < 9);

    // No voltage value shall be set
    REQUIRE (voltage == 0U);

    // When eighth sample is read
    Supervisor_AdcCallback(samples[i]);
    ++i;

    // No voltage value shall be set according to the average of the first 10 samples.
    REQUIRE (voltage == 1200U);

    // When seven more samples are read
    do
    {
        Supervisor_AdcCallback(samples[i]);
    } while (++i < 19);

    // The voltage shall stay the same.
    REQUIRE (voltage == 1200U);

    // When 16th sample is read
    Supervisor_AdcCallback(samples[i]);

    // The voltage value shall be updated.
    REQUIRE (voltage == 1195U);
}

TEST_CASE ("Alarm GPIO pin is correct", "[supervisor]")
{
    REQUIRE (alarmPin.port == portC);
    REQUIRE (alarmPin.number == 5U);
}

TEST_CASE ("Undervoltage detection and recovery", "[supervisor]")
{
    INIT_MOCKS();
    SYSTEM_MOCK_RESET();
    GPIO_MOCK_RESET();

    // Initialise sampling and state
    samples = 0U;
    sampleSum = 0U;
    voltage = 0U;
    uvIsActive = false;
    ovIsActive = false;

    // Voltage reaches 10.50V
    Helper_SetVoltage(1050U);

    // No warnings shall trigger
    REQUIRE (MOCK_CALLS(System_RaiseWarning) == 0);
    REQUIRE (MOCK_CALLS(System_ClearWarning) == 0);
    REQUIRE (MOCK_CALLS(HalGpio_SetOutputState) == 0);

    // Voltage reaches 10.49V
    Helper_SetVoltage(1049U);

    // Undervoltage warning shall trigger
    REQUIRE (MOCK_CALLS(System_RaiseWarning) == 1);
    REQUIRE (MOCK_LAST_ARG(System_RaiseWarning, 0) == UNDERVOLTAGE_WARNING);
    REQUIRE (MOCK_CALLS(System_ClearWarning) == 0);
    REQUIRE (MOCK_CALLS(HalGpio_SetOutputState) == 1);
    REQUIRE (MOCK_LAST_ARG(HalGpio_SetOutputState, 0) == &alarmPin);
    REQUIRE (MOCK_LAST_ARG(HalGpio_SetOutputState, 1) == true);

    // Voltage rises to 10.99V
    Helper_SetVoltage(1099U);

    // There shall be no changes in warning flags.
    REQUIRE (MOCK_CALLS(System_RaiseWarning) == 1);
    REQUIRE (MOCK_CALLS(System_ClearWarning) == 0);
    REQUIRE (MOCK_CALLS(HalGpio_SetOutputState) == 1);

    // Voltage reaches 11.00V
    Helper_SetVoltage(1100U);

    // Undervoltage warning shall be cleared
    REQUIRE (MOCK_CALLS(System_RaiseWarning) == 1);
    REQUIRE (MOCK_CALLS(System_ClearWarning) == 1);
    REQUIRE (MOCK_LAST_ARG(System_ClearWarning, 0) == UNDERVOLTAGE_WARNING);
    REQUIRE (MOCK_CALLS(HalGpio_SetOutputState) == 2);
    REQUIRE (MOCK_LAST_ARG(HalGpio_SetOutputState, 0) == &alarmPin);
    REQUIRE (MOCK_LAST_ARG(HalGpio_SetOutputState, 1) == false);
}

TEST_CASE ("Overvoltage detection and recovery", "[supervisor]")
{
    INIT_MOCKS();
    SYSTEM_MOCK_RESET();
    GPIO_MOCK_RESET();

    // Initialise sampling and state
    samples = 0U;
    sampleSum = 0U;
    voltage = 0U;
    uvIsActive = false;
    ovIsActive = false;

    // Voltage reaches 13.50V
    Helper_SetVoltage(1350U);

    // No warnings shall trigger
    REQUIRE (MOCK_CALLS(System_RaiseWarning) == 0);
    REQUIRE (MOCK_CALLS(System_ClearWarning) == 0);
    REQUIRE (MOCK_CALLS(HalGpio_SetOutputState) == 0);

    // Voltage reaches 13.51V
    Helper_SetVoltage(1351U);

    // Overvoltage warning shall trigger
    REQUIRE (MOCK_CALLS(System_RaiseWarning) == 1);
    REQUIRE (MOCK_LAST_ARG(System_RaiseWarning, 0) == OVERVOLTAGE_WARNING);
    REQUIRE (MOCK_CALLS(System_ClearWarning) == 0);
    REQUIRE (MOCK_CALLS(HalGpio_SetOutputState) == 1);
    REQUIRE (MOCK_LAST_ARG(HalGpio_SetOutputState, 0) == &alarmPin);
    REQUIRE (MOCK_LAST_ARG(HalGpio_SetOutputState, 1) == true);

    // Voltage lowers to 13.01V
    Helper_SetVoltage(1301U);

    // There shall be no changes in warning flags.
    REQUIRE (MOCK_CALLS(System_RaiseWarning) == 1);
    REQUIRE (MOCK_CALLS(System_ClearWarning) == 0);
    REQUIRE (MOCK_CALLS(HalGpio_SetOutputState) == 1);

    // Voltage reaches 13.00V
    Helper_SetVoltage(1300U);

    // Overvoltage warning shall be cleared
    REQUIRE (MOCK_CALLS(System_RaiseWarning) == 1);
    REQUIRE (MOCK_CALLS(System_ClearWarning) == 1);
    REQUIRE (MOCK_LAST_ARG(System_ClearWarning, 0) == OVERVOLTAGE_WARNING);
    REQUIRE (MOCK_CALLS(HalGpio_SetOutputState) == 2);
    REQUIRE (MOCK_LAST_ARG(HalGpio_SetOutputState, 0) == &alarmPin);
    REQUIRE (MOCK_LAST_ARG(HalGpio_SetOutputState, 1) == false);
}

TEST_CASE ("Mixture of overvoltages and undervoltages", "[supervisor]")
{
    INIT_MOCKS();
    SYSTEM_MOCK_RESET();
    GPIO_MOCK_RESET();

    // Initialise sampling and state
    samples = 0U;
    sampleSum = 0U;
    voltage = 0U;
    uvIsActive = false;
    ovIsActive = false;

    // Voltage reaches 14.00V
    Helper_SetVoltage(1400U);

    // Overvoltage warning shall trigger
    REQUIRE (MOCK_CALLS(System_RaiseWarning) == 1);
    REQUIRE (MOCK_LAST_ARG(System_RaiseWarning, 0) == OVERVOLTAGE_WARNING);
    REQUIRE (MOCK_CALLS(System_ClearWarning) == 0);
    REQUIRE (MOCK_CALLS(HalGpio_SetOutputState) == 1);
    REQUIRE (MOCK_LAST_ARG(HalGpio_SetOutputState, 0) == &alarmPin);
    REQUIRE (MOCK_LAST_ARG(HalGpio_SetOutputState, 1) == true);

    // Voltage drops to 10.00V
    Helper_SetVoltage(1000U);

    // Overvoltage warning shall clear and undervoltage shall trigger
    REQUIRE (MOCK_CALLS(System_RaiseWarning) == 2);
    REQUIRE (MOCK_LAST_ARG(System_RaiseWarning, 0) == UNDERVOLTAGE_WARNING);
    REQUIRE (MOCK_CALLS(System_ClearWarning) == 1);
    REQUIRE (MOCK_LAST_ARG(System_ClearWarning, 0) == OVERVOLTAGE_WARNING);
    REQUIRE (MOCK_LAST_ARG(HalGpio_SetOutputState, 0) == &alarmPin);
    REQUIRE (MOCK_LAST_ARG(HalGpio_SetOutputState, 1) == true);

    // Voltage jumps back to 14.00V
    Helper_SetVoltage(1400U);

    // Overvoltage warning shall trigger and undervoltage shall clear
    REQUIRE (MOCK_CALLS(System_RaiseWarning) == 3);
    REQUIRE (MOCK_LAST_ARG(System_RaiseWarning, 0) == OVERVOLTAGE_WARNING);
    REQUIRE (MOCK_CALLS(System_ClearWarning) == 2);
    REQUIRE (MOCK_LAST_ARG(System_ClearWarning, 0) == UNDERVOLTAGE_WARNING);
    REQUIRE (MOCK_LAST_ARG(HalGpio_SetOutputState, 0) == &alarmPin);
    REQUIRE (MOCK_LAST_ARG(HalGpio_SetOutputState, 1) == true);
}

//-----------------------------------------------------------------------------------------------------------------------------
// Custom Fake Definitions
//-----------------------------------------------------------------------------------------------------------------------------

static void HalAdc_SetConfiguration_CustomFake(const AdcConfig_t* pConfig)
{
    adcConfig = *pConfig;
    return;
}

static void HalGpio_SetConfiguration_CustomFake(const GpioConfig_t* pConfig)
{
    gpioConfig = *pConfig;
    return;
}

//-----------------------------------------------------------------------------------------------------------------------------
// Helper Functions
//-----------------------------------------------------------------------------------------------------------------------------

static void Helper_SetVoltage(uint32_t voltage)
{
    uint16_t adc = (uint16_t)((voltage * 0xFFFUL + 1000UL) / 2000UL);
    for (int i = 0; i < 10; ++i)
    {
        Supervisor_AdcCallback(adc);
    }
    return;
}
