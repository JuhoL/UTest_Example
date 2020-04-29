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

//! @file    pGpio->c
//! @author  Juho Lepistö juho.lepisto(a)gmail.com
//! @date    13 Apr 2020
//! 
//! @brief   This is an example of a GPIO HAL module.

//-----------------------------------------------------------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------------------------------------------------------

#include "gpio.h"
#include "hal.h"
#include "utils.h"

//-----------------------------------------------------------------------------------------------------------------------------
// Defines and Macros
//-----------------------------------------------------------------------------------------------------------------------------

#define BITS_IN_MODE                    (GPIO_MODER_MODER1_Pos)
#define MODE_MASK                       (GPIO_MODER_MODER0_Msk)
#define BITS_IN_SPEED                   (GPIO_OSPEEDR_OSPEED1_Pos)
#define SPEED_MASK                      (GPIO_OSPEEDR_OSPEED0_Msk)
#define BITS_IN_PULL                    (GPIO_PUPDR_PUPD1_Pos)
#define PULL_MASK                       (GPIO_PUPDR_PUPD0_Msk)
#define BITS_IN_AF                      (GPIO_AFRL_AFSEL1_Pos)
#define AF_MASK                         (GPIO_AFRL_AFSEL0_Msk)
#define BIT_CLEAR_OFFSET                (GPIO_BSRR_BR0_Pos)
#define MAX_PINS                        16U
#define AF_LOW_REGISTER_LIMIT           8U

#define GPIO(pin_)                      apGpios[(pin_)->port]
#define MODE_POSITION(pin_)             ((uint32_t)(pin_)->number * BITS_IN_MODE)
#define SPEED_POSITION(pin_)            ((pin_)->number * BITS_IN_SPEED)
#define PULL_POSITION(pin_)             ((pin_)->number * BITS_IN_PULL)
#define AFRL_POSITION(pin_)             ((pin_)->number * BITS_IN_AF)
#define AFRH_POSITION(pin_)             (((pin_)->number - AF_LOW_REGISTER_LIMIT) * BITS_IN_AF)

//-----------------------------------------------------------------------------------------------------------------------------
// Static Variables
//-----------------------------------------------------------------------------------------------------------------------------

/// @brief A helper array for choosing the correct GPIO register struct using GpioPort_t enum.
staticv GPIO_TypeDef* const apGpios[] =
{
    GPIOA,
    GPIOB,
    GPIOC,
    GPIOD,
    GPIOE,
    GPIOF,
    GPIOG,
    GPIOH,
    GPIOI,
    GPIOJ,
    GPIOK
};

/// @brief A helper array for choosing the correct clock enalble bit using GpioPort_t enum.
staticv const uint32_t clockEnableBits[] =
{
    RCC_AHB1ENR_GPIOAEN_Pos,
    RCC_AHB1ENR_GPIOBEN_Pos,
    RCC_AHB1ENR_GPIOCEN_Pos,
    RCC_AHB1ENR_GPIODEN_Pos,
    RCC_AHB1ENR_GPIOEEN_Pos,
    RCC_AHB1ENR_GPIOFEN_Pos,
    RCC_AHB1ENR_GPIOGEN_Pos,
    RCC_AHB1ENR_GPIOHEN_Pos,
    RCC_AHB1ENR_GPIOIEN_Pos,
    RCC_AHB1ENR_GPIOJEN_Pos,
    RCC_AHB1ENR_GPIOKEN_Pos
};

//-----------------------------------------------------------------------------------------------------------------------------
// Static Function Prototypes
//-----------------------------------------------------------------------------------------------------------------------------

/// @brief This function gets the mode of the given pin.
/// @param pin - A reference to the pin selector.
/// @return The mode of the pin.
staticf GpioMode_t HalGpio_GetMode(const GpioPin_t* pPin);

/// @brief This function sets the mode of the given pin.
/// @param pin - A reference to the pin selector.
/// @param mode - New GPIO mode.
staticf void HalGpio_SetMode(const GpioPin_t* pPin, GpioMode_t mode);

/// @brief This function gets the open drain configuration of the given pin.
/// @param pin - A reference to the pin selector.
/// @return The open drain configuration of the pin.
staticf bool HalGpio_IsOpenDrain(const GpioPin_t* pPin);

/// @brief This function sets the open drain configuration of the given pin.
/// @param pin - A reference to the pin selector.
/// @param isOpenDrain - New open drain configuration.
staticf void HalGpio_SetOpenDrain(const GpioPin_t* pPin, bool isOpenDrain);

/// @brief This function gets the speed of the given pin.
/// @param pin - A reference to the pin selector.
/// @return The speed of the pin.
staticf GpioSpeed_t HalGpio_GetSpeed(const GpioPin_t* pPin);

/// @brief This function sets the speed of the given pin.
/// @param pin - A reference to the pin selector.
/// @param speed - New GPIO speed.
staticf void HalGpio_SetSpeed(const GpioPin_t* pPin, GpioSpeed_t speed);

/// @brief This function gets the pull-up/pull-down configuration of the given pin.
/// @param pin - A reference to the pin selector.
/// @return The pull-up/pull-down configuration of the pin.
staticf GpioPull_t HalGpio_GetPull(const GpioPin_t* pPin);

/// @brief This function sets the pull-up/pull-down configuration of the given pin.
/// @param pin - A reference to the pin selector.
/// @param pull - New GPIO pull-up/pull-down configuration.
staticf void HalGpio_SetPull(const GpioPin_t* pPin, GpioPull_t pull);

/// @brief This function gets the alternate function configuration of the given pin.
/// @param pin - A reference to the pin selector.
/// @return The alternate function configuration of the pin.
staticf GpioAf_t HalGpio_GetAlternateFunction(const GpioPin_t* pPin);

/// @brief This function sets the alternate function configuration of the given pin.
/// @param pin - A reference to the pin selector.
/// @param alternateFunction - New GPIO alternate function configuration.
staticf void HalGpio_SetAlternateFunction(const GpioPin_t* pPin, GpioAf_t alternateFunction);

/// @brief This function enables the clock for given port.
/// @param port - The port which clock will be enabled.
staticf void HalGpio_EnablePortClock(GpioPort_t port);

//-----------------------------------------------------------------------------------------------------------------------------
// Function Definitions
//-----------------------------------------------------------------------------------------------------------------------------

void HalGpio_GetConfiguration(GpioConfig_t* pGpio)
{
    UTILS_ASSERT_VOID((pGpio != NULL), HAL_GPIO_FAILURE);
    UTILS_ASSERT_VOID((pGpio->pin.number < MAX_PINS), HAL_GPIO_FAILURE);

    pGpio->mode = HalGpio_GetMode(&pGpio->pin);
    pGpio->isOpenDrain = HalGpio_IsOpenDrain(&pGpio->pin);
    pGpio->speed = HalGpio_GetSpeed(&pGpio->pin);
    pGpio->pull = HalGpio_GetPull(&pGpio->pin);
    pGpio->alternateFunction = HalGpio_GetAlternateFunction(&pGpio->pin);
    return;
}

void HalGpio_SetConfiguration(const GpioConfig_t* pGpio)
{
    UTILS_ASSERT_VOID((pGpio != NULL), HAL_GPIO_FAILURE);
    UTILS_ASSERT_VOID((pGpio->pin.number < MAX_PINS), HAL_GPIO_FAILURE);

    HalGpio_EnablePortClock(pGpio->pin.port);
    HalGpio_SetMode(&pGpio->pin, pGpio->mode);
    HalGpio_SetOpenDrain(&pGpio->pin, pGpio->isOpenDrain);
    HalGpio_SetSpeed(&pGpio->pin, pGpio->speed);
    HalGpio_SetPull(&pGpio->pin, pGpio->pull);
    HalGpio_SetAlternateFunction(&pGpio->pin, pGpio->alternateFunction);
    return;
}

void HalGpio_SetOutputState(const GpioPin_t* pPin, bool state)
{
    UTILS_ASSERT_VOID((pPin != NULL), HAL_GPIO_FAILURE);
    UTILS_ASSERT_VOID((pPin->number < MAX_PINS), HAL_GPIO_FAILURE);

    // See STM32F429ZI datasheet chapter 8.4.7.
    if (state == true)
    {
        SET_BIT(GPIO(pPin)->BSRR, pPin->number);
    }
    else
    {
        SET_BIT(GPIO(pPin)->BSRR, pPin->number + BIT_CLEAR_OFFSET);
    }
    return;
}

bool HalGpio_GetInputState(const GpioPin_t* pPin)
{
    UTILS_ASSERT((pPin != NULL), HAL_GPIO_FAILURE, false);
    UTILS_ASSERT((pPin->number < MAX_PINS), HAL_GPIO_FAILURE, false);
    return GET_BIT(GPIO(pPin)->IDR, pPin->number);
}

bool HalGpio_GetOutputState(const GpioPin_t* pPin)
{
    UTILS_ASSERT((pPin != NULL), HAL_GPIO_FAILURE, false);
    UTILS_ASSERT((pPin->number < MAX_PINS), HAL_GPIO_FAILURE, false);
    return GET_BIT(GPIO(pPin)->ODR, pPin->number);
}

//-----------------------------------------------------------------------------------------------------------------------------
// Static Function Definitions
//-----------------------------------------------------------------------------------------------------------------------------

staticf GpioMode_t HalGpio_GetMode(const GpioPin_t* pPin)
{
    // See STM32F429ZI datasheet chapter 8.4.1.
    return (GpioMode_t)GET_BITFIELD(GPIO(pPin)->MODER, MODE_POSITION(pPin), MODE_MASK);
}

staticf void HalGpio_SetMode(const GpioPin_t* pPin, GpioMode_t mode)
{
    // See STM32F429ZI datasheet chapter 8.4.1.
    SET_BITFIELD(GPIO(pPin)->MODER, MODE_POSITION(pPin), MODE_MASK, (uint32_t)mode);
    return;
}

staticf bool HalGpio_IsOpenDrain(const GpioPin_t* pPin)
{
    return GET_BIT(GPIO(pPin)->OTYPER, pPin->number);
}

staticf void HalGpio_SetOpenDrain(const GpioPin_t* pPin, bool isOpenDrain)
{
    if (isOpenDrain)
    {
        SET_BIT(GPIO(pPin)->OTYPER, pPin->number);
    }
    else
    {
        CLEAR_BIT(GPIO(pPin)->OTYPER, pPin->number);
    }
    return;
}

staticf GpioSpeed_t HalGpio_GetSpeed(const GpioPin_t* pPin)
{
    // See STM32F429ZI datasheet chapter 8.4.3.
    return (GpioSpeed_t)GET_BITFIELD(GPIO(pPin)->OSPEEDR, SPEED_POSITION(pPin), SPEED_MASK);
}

staticf void HalGpio_SetSpeed(const GpioPin_t* pPin, GpioSpeed_t speed)
{
    // See STM32F429ZI datasheet chapter 8.4.3.
    SET_BITFIELD(GPIO(pPin)->OSPEEDR, SPEED_POSITION(pPin), SPEED_MASK, (uint32_t)speed);
    return;
}

staticf GpioPull_t HalGpio_GetPull(const GpioPin_t* pPin)
{
    // See STM32F429ZI datasheet chapter 8.4.4.
    return (GpioPull_t)GET_BITFIELD(GPIO(pPin)->PUPDR, PULL_POSITION(pPin), PULL_MASK);
}

staticf void HalGpio_SetPull(const GpioPin_t* pPin, GpioPull_t pull)
{
    // See STM32F429ZI datasheet chapter 8.4.4.
    SET_BITFIELD(GPIO(pPin)->PUPDR, PULL_POSITION(pPin), PULL_MASK, (uint32_t)pull);
    return;
}

staticf GpioAf_t HalGpio_GetAlternateFunction(const GpioPin_t* pPin)
{
    uint32_t af;
    if (pPin->number < AF_LOW_REGISTER_LIMIT)
    {
        af = GET_BITFIELD(GPIO(pPin)->AFR[0], AFRL_POSITION(pPin), AF_MASK);
    }
    else
    {
        af = GET_BITFIELD(GPIO(pPin)->AFR[1], AFRH_POSITION(pPin), AF_MASK);
    }
    return (GpioAf_t)af;
}

staticf void HalGpio_SetAlternateFunction(const GpioPin_t* pPin, GpioAf_t alternateFunction)
{
    if (pPin->number < AF_LOW_REGISTER_LIMIT)
    {
        SET_BITFIELD(GPIO(pPin)->AFR[0], AFRL_POSITION(pPin), AF_MASK, (uint32_t)alternateFunction);
    }
    else
    {
        SET_BITFIELD(GPIO(pPin)->AFR[1], AFRH_POSITION(pPin), AF_MASK, (uint32_t)alternateFunction);
    }
    return;
}

staticf void HalGpio_EnablePortClock(GpioPort_t port)
{
    SET_BIT(RCC->AHB1ENR, clockEnableBits[port]);
    return;
}
