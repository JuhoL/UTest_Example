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

//! @file    gpio.h
//! @author  Juho Lepistö juho.lepisto(a)gmail.com
//! @date    18 Apr 2020
//! 
//! @brief   This is an example of an GPIO module.

#ifndef GPIO_H
#define GPIO_H

//-----------------------------------------------------------------------------------------------------------------------------
// Include Dependencies
//-----------------------------------------------------------------------------------------------------------------------------

#include "types.h"

//-----------------------------------------------------------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------------------------------------------------------

typedef enum
{
    portA = 0,
    portB,
    portC,
    portD,
    portE,
    portF,
    portG,
    portH,
    portI,
    portJ,
    portK
} GpioPort_t;

typedef struct
{
    GpioPort_t port;
    uint8_t number;
} GpioPin_t;

/// @brief This is the GPIO mode enum.
typedef enum
{
    input = 0,  //!< The GPIO is an input.
    output,     //!< The GPIO is an output.
    alternate,  //!< The GPIO is in alternate function mode defined by AlternateFunction enum.
    analog      //!< The GPIO is in ADC/DAC mode.
} GpioMode_t;

/// @brief This is the GPIO output speed enum.
typedef enum
{
    low = 0,    //!< Low speed (power save)
    medium,     //!< Medium speed
    high,       //!< High speed
    veryHigh    //!< Very high speed
} GpioSpeed_t;

/// @brief This is the GPIO pull-up/pull-down enum.
typedef enum 
{
    floating = 0,   //!< The IO is floating.
    pullUp,         //!< The IO is pulled up.
    pullDown        //!< The IO is pulled down.
} GpioPull_t;

/// @brief This is the alternate function enum.
typedef enum 
{
    af0 = 0,
    af1,
    af2,
    af3,
    af4,
    af5,
    af6,
    af7,
    af8,
    af9,
    af10,
    af11,
    af12,
    af13,
    af14,
    af15
} GpioAf_t;

/// @brief This is the GPIO configuration struct.
typedef struct
{
    GpioPin_t pin;                  //!< GPIO pin to be configured.
    GpioMode_t mode;                //!< Pin mode.
    bool isOpenDrain;               //!< Set true to enable open drain output. No effect on inputs.
    GpioSpeed_t speed;              //!< Pin speed.
    GpioPull_t pull;                //!< Pin pull-up/pull-down configuration.
    GpioAf_t alternateFunction;     //!< Alternate function selector. No effect if alternate function mode is not enabled.
} GpioConfig_t;

//-----------------------------------------------------------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------------------------------------------------------

/// @brief This function gets the configuration of the given pin.
/// The pin is selected by configuring the pin in the configuration struct before passing it to this function. 
/// @param pGpio - A pointer to a GPIO configuration struct.
void HalGpio_GetConfiguration(GpioConfig_t* pGpio);

/// @brief This function sets the configuration of the given pin.
/// @param pGpio - A pointer to a GPIO configuration struct.
void HalGpio_SetConfiguration(const GpioConfig_t* pGpio);

/// @brief This function sets the output state of the given pin.
/// @param pPin - A pointer to the pin selector.
/// @param state - A new pin state.
void HalGpio_SetOutputState(const GpioPin_t* pPin, bool state);

/// @brief This function gets the input state of the given pin.
/// @param pPin - A pointer to the pin selector.
/// @return The state of the pin input.
bool HalGpio_GetInputState(const GpioPin_t* pPin);

/// @brief This function gets the output state of the given pin.
/// @param pPin - A pointer to the pin selector.
/// @return The state of the pin output.
bool HalGpio_GetOutputState(const GpioPin_t* pPin);

#endif // GPIO_H
