/* This file is part of the MicroPython project, http://micropython.org/
 * The MIT License (MIT)
 * Copyright (c) 2019 Damien P. George
 */
#ifndef MICROPY_INCLUDED_STM32H7XX_HAL_CONF_H
#define MICROPY_INCLUDED_STM32H7XX_HAL_CONF_H

// Oscillator values in Hz
#define HSE_VALUE (25000000)
#define LSE_VALUE (32768)
#define EXTERNAL_CLOCK_VALUE (12288000)

// Oscillator timeouts in ms
#define HSE_STARTUP_TIMEOUT (5000)
#define LSE_STARTUP_TIMEOUT (5000)

// Enable various HAL modules
#define HAL_ADC_MODULE_ENABLED
#define HAL_DAC_MODULE_ENABLED
#define HAL_LTDC_MODULE_ENABLED
#define HAL_DMA2D_MODULE_ENABLED
#define HAL_CORTEX_MODULE_ENABLED
#define HAL_CRC_MODULE_ENABLED
#define HAL_DMA_MODULE_ENABLED
#define HAL_FLASH_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED
#define HAL_HASH_MODULE_ENABLED
#define HAL_I2C_MODULE_ENABLED
#define HAL_I2S_MODULE_ENABLED
#define HAL_PWR_MODULE_ENABLED
#define HAL_RCC_MODULE_ENABLED
#define HAL_RTC_MODULE_ENABLED
#define HAL_SD_MODULE_ENABLED
#define HAL_SDRAM_MODULE_ENABLED
#define HAL_SPI_MODULE_ENABLED
#define HAL_TIM_MODULE_ENABLED
#define HAL_UART_MODULE_ENABLED
#define HAL_USART_MODULE_ENABLED
#define HAL_PCD_MODULE_ENABLED

// Oscillator values in Hz
#define CSI_VALUE (4000000)
#define HSI_VALUE (64000000)

// SysTick has the highest priority
#define TICK_INT_PRIORITY (0x00)

// Miscellaneous HAL settings
#define USE_RTOS            0
#define USE_SD_TRANSCEIVER  0
#define USE_SPI_CRC         1

// Include various HAL modules for convenience
#include "stm32h7xx_hal_dma.h"
#include "stm32h7xx_hal_mdma.h"
#include "stm32h7xx_hal_adc.h"
#include "stm32h7xx_hal_cortex.h"
#include "stm32h7xx_hal_crc.h"
#include "stm32h7xx_hal_dac.h"
#include "stm32h7xx_hal_dcmi.h"
#include "stm32h7xx_hal_fdcan.h"
#include "stm32h7xx_hal_flash.h"
#include "stm32h7xx_hal_gpio.h"
#include "stm32h7xx_hal_hash.h"
#include "stm32h7xx_hal_hcd.h"
#include "stm32h7xx_hal_i2c.h"
#include "stm32h7xx_hal_i2c_ex.h"
#include "stm32h7xx_hal_i2s.h"
#include "stm32h7xx_hal_iwdg.h"
#include "stm32h7xx_hal_pcd.h"
#include "stm32h7xx_hal_pwr.h"
#include "stm32h7xx_hal_rcc.h"
#include "stm32h7xx_hal_rtc.h"
#include "stm32h7xx_hal_sd.h"
#include "stm32h7xx_hal_sdram.h"
#include "stm32h7xx_hal_spi.h"
#include "stm32h7xx_hal_tim.h"
#include "stm32h7xx_hal_uart.h"
#include "stm32h7xx_hal_usart.h"
#include "stm32h7xx_hal_wwdg.h"
#include "stm32h7xx_hal_ltdc.h"
#include "stm32h7xx_hal_dma2d.h"
#include "stm32h7xx_ll_adc.h"
#include "stm32h7xx_ll_lpuart.h"
#include "stm32h7xx_ll_pwr.h"
#include "stm32h7xx_ll_rcc.h"
#include "stm32h7xx_ll_rtc.h"
#include "stm32h7xx_ll_usart.h"

// HAL parameter assertions are disabled
#define assert_param(expr) ((void)0)

// The STM32H7xx HAL defines LPUART1 AF macros without numbers.
#ifndef GPIO_AF3_LPUART1
#define GPIO_AF3_LPUART1 GPIO_AF3_LPUART
#define GPIO_AF8_LPUART1 GPIO_AF8_LPUART
#endif

extern LTDC_HandleTypeDef hltdc;

#endif // MICROPY_INCLUDED_STM32H7XX_HAL_CONF_H
