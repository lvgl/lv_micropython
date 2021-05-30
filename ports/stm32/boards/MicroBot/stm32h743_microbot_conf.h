/**
  ******************************************************************************
  * @file    stm32h743_microbot_conf.h
  * @author  MCD Application Team
  * @brief   STM32H743_MICROBOT board configuration file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32H743_MICROBOT_CONF_H
#define STM32H743_MICROBOT_CONF_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* COM define */
#define USE_COM_LOG                         0U
#define USE_BSP_COM_FEATURE                 0U

/* POT define */
#define USE_BSP_POT_FEATURE                 1U

/* IO CLASS define */
#define USE_BSP_IO_CLASS                    0U

/* I2C BUS timing define */
#define I2C_VALID_TIMING_NBR                128U

/* Camera sensors defines */
#define USE_CAMERA_SENSOR_OV5640            1U
#define USE_CAMERA_SENSOR_S5K5CAG           0U

/* IRQ priorities */
#define BSP_SRAM_IT_PRIORITY                15U
#define BSP_SDRAM_IT_PRIORITY               15U
#define BSP_CAMERA_IT_PRIORITY              15U

#ifdef __cplusplus
}
#endif

#endif /* STM32H743_MICROBOT_CONF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
