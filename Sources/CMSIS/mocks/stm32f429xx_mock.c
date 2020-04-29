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

//! @file    stm32f429xx_mock.cpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    23 Jun 2019
//!
//! @brief   Mocks for STM32F429xx.
//! 
//! These is mock of STM32F429xx registers. The file is based on ST original MCU header stm32f429xx.h
    
//-----------------------------------------------------------------------------------------------------------------------------
// Include Files
//-----------------------------------------------------------------------------------------------------------------------------

#include <stm32f429xx_mock.h>

//-----------------------------------------------------------------------------------------------------------------------------
// Peripheral Mock Structs
//-----------------------------------------------------------------------------------------------------------------------------

TIM_TypeDef timers[HAL_TIMERS_MAX];
RTC_TypeDef rtc;
WWDG_TypeDef wwdg;
IWDG_TypeDef iwdg;
SPI_TypeDef spis[HAL_SPIS_MAX];
USART_TypeDef usarts[HAL_USARTS_MAX];
I2C_TypeDef i2cs[HAL_I2CS_MAX];
CAN_TypeDef cans[HAL_CANS_MAX];
PWR_TypeDef pwr;
DAC_TypeDef dac;
ADC_TypeDef adcs[HAL_ADCS_MAX];
ADC_Common_TypeDef adcCommon;
SDIO_TypeDef sdio;
SYSCFG_TypeDef sysCfg;
EXTI_TypeDef exti;
SAI_Block_TypeDef sais[HAL_SAIS_MAX];
LTDC_TypeDef ltdc;
LTDC_Layer_TypeDef ltdcLayers[HAL_LTDC_LAYERS_MAX];
GPIO_TypeDef gpios[HAL_GPIOS_MAX];
CRC_TypeDef crc;
RCC_TypeDef rcc;
FLASH_TypeDef flash;
DMA_TypeDef dmas[HAL_DMAS_MAX];
DMA_Stream_TypeDef dmaStreams[HAL_DMA_STREAMS_MAX];
ETH_TypeDef eth;
DMA2D_TypeDef dma2d;
DCMI_TypeDef dcmi;
RNG_TypeDef rng;
FMC_Bank1_TypeDef fmcBank1;
FMC_Bank1E_TypeDef fmcBank1E;
FMC_Bank2_3_TypeDef fmcBank2_3;
FMC_Bank4_TypeDef fmcBank4;
FMC_Bank5_6_TypeDef fmcBank5_6;
DBGMCU_TypeDef dbgMcu;
USB_OTG_GlobalTypeDef usbs[HAL_USBS_MAX];

//-----------------------------------------------------------------------------------------------------------------------------
// ARM Core Mock Structs
//-----------------------------------------------------------------------------------------------------------------------------

SCnSCB_Type scnScb;
SCB_Type scb;
SysTick_Type sysTick;
NVIC_Type nvic;
ITM_Type itm;
DWT_Type dwt;
TPI_Type tpi;
CoreDebug_Type coreDebug;
MPU_Type mpu;
FPU_Type fpu;
