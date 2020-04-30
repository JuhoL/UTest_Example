//-----------------------------------------------------------------------------------------------------------------------------
// Copyright (c) 2019 Juho Lepistö
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

//! @file    utest_gpio.cpp
//! @author  Juho Lepistö juho.lepisto(a)gmail.com
//! @date    13 May 2019
//! 
//! @brief   These are example unit tests for utest_gpio.c
//! 
//! These are unit tests for utest_gpio.c utilizing Catch2 and FFF.

//-----------------------------------------------------------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------------------------------------------------------

#define CATCH_CONFIG_RUNNER
#include <catch_utils.hpp>
#include <fff.h>
DEFINE_FFF_GLOBALS;
#include "utest_helpers.hpp"

extern "C" {
#include "gpio.h"
}

// Mocks
#include "hal_mock.h"
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
// Helper Functions and Macros
//-----------------------------------------------------------------------------------------------------------------------------

/// @brief This function randomises port and pin number of given pin struct.
/// @param pPin - Pointer to the pin struct to be randomised.
static void Helper_RandomisePin(GpioPin_t* const pPin);

/// @brief This function returns a corresponding GPIO register struct address of given port.
/// @param port - Port of the pin.
/// @return An address of corresponding GPIO register struct.
static GPIO_TypeDef* Helper_GetCorrespondingGpioStruct(GpioPort_t port);

/// @brief This function returns a corresponding clock enable bit of given port.
/// @param port - Port of the pin.
/// @return A corresponding clock enable bit.
static uint32_t Helper_GetCorrespondingClockEnableBit(GpioPort_t port);

//-----------------------------------------------------------------------------------------------------------------------------
// Test Cases
//-----------------------------------------------------------------------------------------------------------------------------

//------------------------------------
// HalGpio_GetConfiguration
//------------------------------------

SCENARIO ("GPIO configuration is get", "[hal][gpio]")
{
    INIT_MOCKS();
    SYSTEM_MOCK_RESET();
    HAL_MOCK_RESET();

    uint32_t aGetBitfieldReturns[] = {0x1, 0x2, 0x0, 0x8};
    MOCK_SET_RETURN_VALUE_SEQUENCE(BitMock_GetBitfield, aGetBitfieldReturns, ARRAY_LENGTH(aGetBitfieldReturns, uint32_t));

    bool aGetBitReturns[] = {true};
    MOCK_SET_RETURN_VALUE_SEQUENCE(BitMock_Get, aGetBitReturns, ARRAY_LENGTH(aGetBitReturns, bool));

    GIVEN ("a GPIO struct is created for a random port and pin")
    {
        GpioConfig_t gpio;
        Helper_RandomisePin(&gpio.pin);

        WHEN ("the configuration is get")
        {
            HalGpio_GetConfiguration(&gpio);

            THEN ("no errors shall occur")
            {
                REQUIRE (NO_ASSERT_ERRORS);

                AND_THEN ("the mode shall be read")
                {
                    GPIO_TypeDef* registers = Helper_GetCorrespondingGpioStruct(gpio.pin.port);

                    REQUIRE (MOCK_NEXT_CALLED_FUNCTION_IS(BitMock_GetBitfield));
                    REQUIRE (MOCK_ARG_HISTORY(BitMock_GetBitfield, 0, 0) == &registers->MODER);
                    REQUIRE (MOCK_ARG_HISTORY(BitMock_GetBitfield, 1, 0) == (gpio.pin.number * GPIO_MODER_MODER1_Pos));
                    REQUIRE (MOCK_ARG_HISTORY(BitMock_GetBitfield, 2, 0) == GPIO_MODER_MODER0_Msk);
                    REQUIRE (gpio.mode == output);

                    AND_THEN ("the open drain status shall be read")
                    {
                        REQUIRE (MOCK_NEXT_CALLED_FUNCTION_IS(BitMock_Get));
                        REQUIRE (MOCK_ARG_HISTORY(BitMock_Get, 0, 0) == &registers->OTYPER);
                        REQUIRE (MOCK_ARG_HISTORY(BitMock_Get, 1, 0) == gpio.pin.number);
                        REQUIRE (gpio.isOpenDrain == true);

                        AND_THEN ("the speed shall be read")
                        {
                            REQUIRE (MOCK_NEXT_CALLED_FUNCTION_IS(BitMock_GetBitfield));
                            REQUIRE (MOCK_ARG_HISTORY(BitMock_GetBitfield, 0, 1) == &registers->OSPEEDR);
                            REQUIRE (MOCK_ARG_HISTORY(BitMock_GetBitfield, 1, 1) == (gpio.pin.number * GPIO_OSPEEDR_OSPEED1_Pos));
                            REQUIRE (MOCK_ARG_HISTORY(BitMock_GetBitfield, 2, 1) == GPIO_OSPEEDR_OSPEED0_Msk);
                            REQUIRE (gpio.speed == high);

                            AND_THEN ("the pull-up status shall be read")
                            {
                                REQUIRE (MOCK_NEXT_CALLED_FUNCTION_IS(BitMock_GetBitfield));
                                REQUIRE (MOCK_ARG_HISTORY(BitMock_GetBitfield, 0, 2) == &registers->PUPDR);
                                REQUIRE (MOCK_ARG_HISTORY(BitMock_GetBitfield, 1, 2) == (gpio.pin.number * GPIO_PUPDR_PUPD1_Pos));
                                REQUIRE (MOCK_ARG_HISTORY(BitMock_GetBitfield, 2, 2) == GPIO_PUPDR_PUPD0_Msk);
                                REQUIRE (gpio.pull == floating);

                                AND_THEN ("the AF configuration shall be read")
                                {
                                    REQUIRE (MOCK_NEXT_CALLED_FUNCTION_IS(BitMock_GetBitfield));
                                    if (gpio.pin.number < 8)
                                    {
                                        REQUIRE (MOCK_ARG_HISTORY(BitMock_GetBitfield, 0, 3) == &registers->AFR[0]);
                                        REQUIRE (MOCK_ARG_HISTORY(BitMock_GetBitfield, 1, 3) == (gpio.pin.number * GPIO_AFRL_AFSEL1_Pos));
                                    }
                                    else
                                    {
                                        REQUIRE (MOCK_ARG_HISTORY(BitMock_GetBitfield, 0, 3) == &registers->AFR[1]);
                                        REQUIRE (MOCK_ARG_HISTORY(BitMock_GetBitfield, 1, 3) == ((gpio.pin.number - 8U) * GPIO_AFRH_AFSEL9_Pos));
                                    }
                                    REQUIRE (MOCK_ARG_HISTORY(BitMock_GetBitfield, 2, 3) == GPIO_AFRL_AFSEL0_Msk);
                                    REQUIRE (gpio.alternateFunction == af8);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

SCENARIO ("GPIO configuration is get erroneously", "[hal][gpio][error_handling]")
{
    INIT_MOCKS();
    SYSTEM_MOCK_RESET();

    GIVEN ("a GPIO struct is created for an invalid pin")
    {
        GpioConfig_t gpio;
        Helper_RandomisePin(&gpio.pin);
        gpio.pin.number = 16U;

        WHEN ("the configuration is get")
        {
            HalGpio_GetConfiguration(&gpio);

            THEN ("assert error shall occur")
            {
                REQUIRE (ASSERT_ERROR);
                REQUIRE (ASSERT_ERROR_TYPE_IS(HAL_GPIO_FAILURE));
            }
        }
    }

    GIVEN ("a GPIO struct is not created")
    {
        WHEN ("the configuration is get from null GPIO")
        {
            HalGpio_GetConfiguration(NULL);

            THEN ("assert error shall occur")
            {
                REQUIRE (ASSERT_ERROR);
                REQUIRE (ASSERT_ERROR_TYPE_IS(HAL_GPIO_FAILURE));
            }
        }
    }
}

//------------------------------------
// HalGpio_SetConfiguration
//------------------------------------

SCENARIO ("GPIO configuration is set", "[hal][gpio]")
{
    INIT_MOCKS();
    SYSTEM_MOCK_RESET();
    HAL_MOCK_RESET();

    GIVEN ("a GPIO struct is created with random configuration")
    {
        GpioConfig_t gpio;
        Helper_RandomisePin(&gpio.pin);
        gpio.mode = static_cast<GpioMode_t>(UTestHelper::GetRandomInt(0, 4));
        gpio.isOpenDrain = UTestHelper::GetRandomBool();
        gpio.speed = static_cast<GpioSpeed_t>(UTestHelper::GetRandomInt(0, 4));
        gpio.pull = static_cast<GpioPull_t>(UTestHelper::GetRandomInt(0, 3));
        gpio.alternateFunction = static_cast<GpioAf_t>(UTestHelper::GetRandomInt(0, 16));

        WHEN ("the configuration is set")
        {
            HalGpio_SetConfiguration(&gpio);

            THEN ("no errors shall occur")
            {
                REQUIRE (NO_ASSERT_ERRORS);

                AND_THEN ("the clock of the port shall be enabled")
                {
                    REQUIRE (MOCK_NEXT_CALLED_FUNCTION_IS(BitMock_Set));
                    REQUIRE (MOCK_ARG_HISTORY(BitMock_Set, 0, 0) == &RCC->AHB1ENR);
                    REQUIRE (MOCK_ARG_HISTORY(BitMock_Set, 1, 0) == Helper_GetCorrespondingClockEnableBit(gpio.pin.port));

                    AND_THEN ("the mode shall be set")
                    {
                        GPIO_TypeDef* registers = Helper_GetCorrespondingGpioStruct(gpio.pin.port);

                        REQUIRE (MOCK_NEXT_CALLED_FUNCTION_IS(BitMock_SetBitfield));
                        REQUIRE (MOCK_ARG_HISTORY(BitMock_SetBitfield, 0, 0) == &registers->MODER);
                        REQUIRE (MOCK_ARG_HISTORY(BitMock_SetBitfield, 1, 0) == (gpio.pin.number * GPIO_MODER_MODER1_Pos));
                        REQUIRE (MOCK_ARG_HISTORY(BitMock_SetBitfield, 2, 0) == GPIO_MODER_MODER0_Msk);
                        REQUIRE (MOCK_ARG_HISTORY(BitMock_SetBitfield, 3, 0) == static_cast<uint32_t>(gpio.mode));

                        AND_THEN ("the open drain status shall be set")
                        {
                            if (gpio.isOpenDrain)
                            {
                                REQUIRE (MOCK_NEXT_CALLED_FUNCTION_IS(BitMock_Set));
                                REQUIRE (MOCK_ARG_HISTORY(BitMock_Set, 0, 1) == &registers->OTYPER);
                                REQUIRE (MOCK_ARG_HISTORY(BitMock_Set, 1, 1) == gpio.pin.number);
                            }
                            else
                            {
                                REQUIRE (MOCK_NEXT_CALLED_FUNCTION_IS(BitMock_Clear));
                                REQUIRE (MOCK_ARG_HISTORY(BitMock_Clear, 0, 0) == &registers->OTYPER);
                                REQUIRE (MOCK_ARG_HISTORY(BitMock_Clear, 1, 0) == gpio.pin.number);
                            }

                            AND_THEN ("the speed shall be set")
                            {
                                REQUIRE (MOCK_NEXT_CALLED_FUNCTION_IS(BitMock_SetBitfield));
                                REQUIRE (MOCK_ARG_HISTORY(BitMock_SetBitfield, 0, 1) == &registers->OSPEEDR);
                                REQUIRE (MOCK_ARG_HISTORY(BitMock_SetBitfield, 1, 1) == (gpio.pin.number * GPIO_OSPEEDR_OSPEED1_Pos));
                                REQUIRE (MOCK_ARG_HISTORY(BitMock_SetBitfield, 2, 1) == GPIO_OSPEEDR_OSPEED0_Msk);
                                REQUIRE (MOCK_ARG_HISTORY(BitMock_SetBitfield, 3, 1) == static_cast<uint32_t>(gpio.speed));

                                AND_THEN ("the pull-up status shall be set")
                                {
                                    REQUIRE (MOCK_NEXT_CALLED_FUNCTION_IS(BitMock_SetBitfield));
                                    REQUIRE (MOCK_ARG_HISTORY(BitMock_SetBitfield, 0, 2) == &registers->PUPDR);
                                    REQUIRE (MOCK_ARG_HISTORY(BitMock_SetBitfield, 1, 2) == (gpio.pin.number * GPIO_PUPDR_PUPD1_Pos));
                                    REQUIRE (MOCK_ARG_HISTORY(BitMock_SetBitfield, 2, 2) == GPIO_PUPDR_PUPD0_Msk);
                                    REQUIRE (MOCK_ARG_HISTORY(BitMock_SetBitfield, 3, 2) == static_cast<uint32_t>(gpio.pull));

                                    AND_THEN ("the AF configuration shall be set")
                                    {
                                        REQUIRE (MOCK_NEXT_CALLED_FUNCTION_IS(BitMock_SetBitfield));
                                        if (gpio.pin.number < 8)
                                        {
                                            REQUIRE (MOCK_ARG_HISTORY(BitMock_SetBitfield, 0, 3) == &registers->AFR[0]);
                                            REQUIRE (MOCK_ARG_HISTORY(BitMock_SetBitfield, 1, 3) == (gpio.pin.number * GPIO_AFRL_AFSEL1_Pos));
                                        }
                                        else
                                        {
                                            REQUIRE (MOCK_ARG_HISTORY(BitMock_SetBitfield, 0, 3) == &registers->AFR[1]);
                                            REQUIRE (MOCK_ARG_HISTORY(BitMock_SetBitfield, 1, 3) == ((gpio.pin.number - 8U) * GPIO_AFRH_AFSEL9_Pos));
                                        }
                                        REQUIRE (MOCK_ARG_HISTORY(BitMock_SetBitfield, 2, 3) == GPIO_AFRL_AFSEL0_Msk);
                                        REQUIRE (MOCK_ARG_HISTORY(BitMock_SetBitfield, 3, 3) == static_cast<uint32_t>(gpio.alternateFunction));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

SCENARIO ("GPIO configuration is set erroneously", "[hal][gpio][error_handling]")
{
    INIT_MOCKS();
    SYSTEM_MOCK_RESET();

    GIVEN ("a GPIO struct is created for an invalid pin")
    {
        GpioConfig_t gpio;
        Helper_RandomisePin(&gpio.pin);
        gpio.pin.number = 16U;

        WHEN ("the configuration is set")
        {
            HalGpio_SetConfiguration(&gpio);

            THEN ("assert error shall occur")
            {
                REQUIRE (ASSERT_ERROR);
                REQUIRE (ASSERT_ERROR_TYPE_IS(HAL_GPIO_FAILURE));
            }
        }
    }

    GIVEN ("a GPIO struct is not created")
    {
        WHEN ("the configuration is set from null GPIO")
        {
            HalGpio_SetConfiguration(NULL);

            THEN ("assert error shall occur")
            {
                REQUIRE (ASSERT_ERROR);
                REQUIRE (ASSERT_ERROR_TYPE_IS(HAL_GPIO_FAILURE));
            }
        }
    }
}

//------------------------------------
// HalGpio_SetOutputState
//------------------------------------

SCENARIO ("GPIO output state is set", "[hal][gpio]")
{
    INIT_MOCKS();
    SYSTEM_MOCK_RESET();
    HAL_MOCK_RESET();

    GIVEN ("a GPIO pin struct is created with random pin")
    {
        GpioPin_t pin;
        Helper_RandomisePin(&pin);

        WHEN ("the output is set active")
        {
            HalGpio_SetOutputState(&pin, true);

            THEN ("no errors shall occur")
            {
                REQUIRE (NO_ASSERT_ERRORS);

                AND_THEN ("the correct output shall be set active")
                {
                    GPIO_TypeDef* registers = Helper_GetCorrespondingGpioStruct(pin.port);
                    
                    REQUIRE (MOCK_NEXT_CALLED_FUNCTION_IS(BitMock_Set));
                    REQUIRE (MOCK_ARG_HISTORY(BitMock_Set, 0, 0) == &registers->BSRR);
                    REQUIRE (MOCK_ARG_HISTORY(BitMock_Set, 1, 0) == static_cast<uint32_t>(pin.number));
                }
            }
        }

        WHEN ("the output is set inactive")
        {
            HalGpio_SetOutputState(&pin, false);

            THEN ("no errors shall occur")
            {
                REQUIRE (NO_ASSERT_ERRORS);

                AND_THEN ("the correct output shall be set inactive")
                {
                    GPIO_TypeDef* registers = Helper_GetCorrespondingGpioStruct(pin.port);
                    
                    REQUIRE (MOCK_NEXT_CALLED_FUNCTION_IS(BitMock_Set));
                    REQUIRE (MOCK_ARG_HISTORY(BitMock_Set, 0, 0) == &registers->BSRR);
                    REQUIRE (MOCK_ARG_HISTORY(BitMock_Set, 1, 0) == static_cast<uint32_t>(pin.number) + GPIO_BSRR_BR0_Pos);
                }
            }
        }
    }
}

SCENARIO ("GPIO output state is set erroneously", "[hal][gpio][error_handling]")
{
    INIT_MOCKS();
    SYSTEM_MOCK_RESET();

    GIVEN ("a GPIO pin struct is created for an invalid pin")
    {
        GpioPin_t pin;
        Helper_RandomisePin(&pin);
        pin.number = 16U;

        WHEN ("the output state is set")
        {
            bool state = UTestHelper::GetRandomBool();
            HalGpio_SetOutputState(&pin, state);

            THEN ("assert error shall occur")
            {
                REQUIRE (ASSERT_ERROR);
                REQUIRE (ASSERT_ERROR_TYPE_IS(HAL_GPIO_FAILURE));
            }
        }
    }

    GIVEN ("a GPIO pin struct is not created")
    {
        WHEN ("the output state of null pin is set")
        {
            bool state = UTestHelper::GetRandomBool();
            HalGpio_SetOutputState(NULL, state);

            THEN ("assert error shall occur")
            {
                REQUIRE (ASSERT_ERROR);
                REQUIRE (ASSERT_ERROR_TYPE_IS(HAL_GPIO_FAILURE));
            }
        }
    }
}

//------------------------------------
// HalGpio_GetInputState
//------------------------------------

SCENARIO ("GPIO input state is read", "[hal][gpio]")
{
    INIT_MOCKS();
    SYSTEM_MOCK_RESET();
    HAL_MOCK_RESET();

    bool randomState = UTestHelper::GetRandomBool();
    MOCK_SET_RETURN_VALUE(BitMock_Get, randomState);

    GIVEN ("a GPIO pin struct is created with random pin")
    {
        GpioPin_t pin;
        Helper_RandomisePin(&pin);

        WHEN ("the input state is read")
        {
            bool state = HalGpio_GetInputState(&pin);

            THEN ("no errors shall occur")
            {
                REQUIRE (NO_ASSERT_ERRORS);

                AND_THEN ("the correct output shall be read")
                {
                    GPIO_TypeDef* registers = Helper_GetCorrespondingGpioStruct(pin.port);
                    
                    REQUIRE (MOCK_NEXT_CALLED_FUNCTION_IS(BitMock_Get));
                    REQUIRE (MOCK_ARG_HISTORY(BitMock_Get, 0, 0) == &registers->IDR);
                    REQUIRE (MOCK_ARG_HISTORY(BitMock_Get, 1, 0) == static_cast<uint32_t>(pin.number));

                    REQUIRE (state == randomState);
                }
            }
        }
    }
}

SCENARIO ("GPIO input state is read erroneously", "[hal][gpio][error_handling]")
{
    INIT_MOCKS();
    SYSTEM_MOCK_RESET();

    GIVEN ("a GPIO pin struct is created for an invalid pin")
    {
        GpioPin_t pin;
        Helper_RandomisePin(&pin);
        pin.number = 16U;

        WHEN ("the input state is read")
        {
            bool state = HalGpio_GetInputState(&pin);

            THEN ("assert error shall occur")
            {
                REQUIRE (ASSERT_ERROR);
                REQUIRE (ASSERT_ERROR_TYPE_IS(HAL_GPIO_FAILURE));

                AND_THEN ("the state shall be false")
                {
                    REQUIRE (state == false);
                }
            }
        }
    }

    GIVEN ("a GPIO pin struct is not created")
    {
        WHEN ("the input state of null pin is read")
        {
            bool state = HalGpio_GetInputState(NULL);

            THEN ("assert error shall occur")
            {
                REQUIRE (ASSERT_ERROR);
                REQUIRE (ASSERT_ERROR_TYPE_IS(HAL_GPIO_FAILURE));

                AND_THEN ("the state shall be false")
                {
                    REQUIRE (state == false);
                }
            }
        }
    }
}

//------------------------------------
// HalGpio_GetOutputState
//------------------------------------

SCENARIO ("GPIO output state is read", "[hal][gpio]")
{
    INIT_MOCKS();
    SYSTEM_MOCK_RESET();
    HAL_MOCK_RESET();

    bool randomState = UTestHelper::GetRandomBool();
    MOCK_SET_RETURN_VALUE(BitMock_Get, randomState);

    GIVEN ("a GPIO pin struct is created with random pin")
    {
        GpioPin_t pin;
        Helper_RandomisePin(&pin);

        WHEN ("the output state is read")
        {
            bool state = HalGpio_GetOutputState(&pin);

            THEN ("no errors shall occur")
            {
                REQUIRE (NO_ASSERT_ERRORS);

                AND_THEN ("the correct output shall be read")
                {
                    GPIO_TypeDef* registers = Helper_GetCorrespondingGpioStruct(pin.port);
                    
                    REQUIRE (MOCK_NEXT_CALLED_FUNCTION_IS(BitMock_Get));
                    REQUIRE (MOCK_ARG_HISTORY(BitMock_Get, 0, 0) == &registers->ODR);
                    REQUIRE (MOCK_ARG_HISTORY(BitMock_Get, 1, 0) == static_cast<uint32_t>(pin.number));

                    REQUIRE (state == randomState);
                }
            }
        }
    }
}

SCENARIO ("GPIO output state is read erroneously", "[hal][gpio][error_handling]")
{
    INIT_MOCKS();
    SYSTEM_MOCK_RESET();

    GIVEN ("a GPIO pin struct is created for an invalid pin")
    {
        GpioPin_t pin;
        Helper_RandomisePin(&pin);
        pin.number = 16U;

        WHEN ("the output state is read")
        {
            bool state = HalGpio_GetOutputState(&pin);

            THEN ("assert error shall occur")
            {
                REQUIRE (ASSERT_ERROR);
                REQUIRE (ASSERT_ERROR_TYPE_IS(HAL_GPIO_FAILURE));

                AND_THEN ("the state shall be false")
                {
                    REQUIRE (state == false);
                }
            }
        }
    }

    GIVEN ("a GPIO pin struct is not created")
    {
        WHEN ("the output state of null pin is read")
        {
            bool state = HalGpio_GetOutputState(NULL);

            THEN ("assert error shall occur")
            {
                REQUIRE (ASSERT_ERROR);
                REQUIRE (ASSERT_ERROR_TYPE_IS(HAL_GPIO_FAILURE));

                AND_THEN ("the state shall be false")
                {
                    REQUIRE (state == false);
                }
            }
        }
    }
}

//-----------------------------------------------------------------------------------------------------------------------------
// Custom Fake Definitions
//-----------------------------------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------------------------------
// Helper Functions
//-----------------------------------------------------------------------------------------------------------------------------

static void Helper_RandomisePin(GpioPin_t* const pPin)
{
    pPin->port = static_cast<GpioPort_t>(UTestHelper::GetRandomInt(0, (int)portK + 1));
    pPin->number = static_cast<uint8_t>(UTestHelper::GetRandomInt(0, 16));
    return;
}

static GPIO_TypeDef* Helper_GetCorrespondingGpioStruct(GpioPort_t port)
{
    GPIO_TypeDef* pGpio;
    switch (port)
    {
        case portA: pGpio = GPIOA;  break;
        case portB: pGpio = GPIOB;  break;
        case portC: pGpio = GPIOC;  break;
        case portD: pGpio = GPIOD;  break;
        case portE: pGpio = GPIOE;  break;
        case portF: pGpio = GPIOF;  break;
        case portG: pGpio = GPIOG;  break;
        case portH: pGpio = GPIOH;  break;
        case portI: pGpio = GPIOI;  break;
        case portJ: pGpio = GPIOJ;  break;
        case portK: pGpio = GPIOK;  break;
        default:    pGpio = NULL;   break;
    }
    return pGpio;
}

static uint32_t Helper_GetCorrespondingClockEnableBit(GpioPort_t port)
{
    uint32_t bit;
    switch (port)
    {
        case portA: bit = RCC_AHB1ENR_GPIOAEN_Pos;  break;
        case portB: bit = RCC_AHB1ENR_GPIOBEN_Pos;  break;
        case portC: bit = RCC_AHB1ENR_GPIOCEN_Pos;  break;
        case portD: bit = RCC_AHB1ENR_GPIODEN_Pos;  break;
        case portE: bit = RCC_AHB1ENR_GPIOEEN_Pos;  break;
        case portF: bit = RCC_AHB1ENR_GPIOFEN_Pos;  break;
        case portG: bit = RCC_AHB1ENR_GPIOGEN_Pos;  break;
        case portH: bit = RCC_AHB1ENR_GPIOHEN_Pos;  break;
        case portI: bit = RCC_AHB1ENR_GPIOIEN_Pos;  break;
        case portJ: bit = RCC_AHB1ENR_GPIOJEN_Pos;  break;
        case portK: bit = RCC_AHB1ENR_GPIOKEN_Pos;  break;
        default:    bit = 0;                        break;
    }
    return bit;
}
