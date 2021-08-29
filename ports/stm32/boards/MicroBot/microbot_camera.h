/**
  ******************************************************************************
  * @file    STM32H743_MICROBOT_camera.h
  * @author  MCD Application Team
  * @brief   This file contains the common defines and functions prototypes for
  *          the STM32H743_MICROBOT_camera.c driver.
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
#ifndef STM32H743_MICROBOT_CAMERA_H
#define STM32H743_MICROBOT_CAMERA_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h743_microbot_errno.h"
#include "stm32h743_microbot_conf.h"

#include "ov5640.h"

#include "camera.h"
#include "py/mpconfig.h"
#include "py/misc.h"
#include "py/mpstate.h"

typedef struct
{
  uint32_t Resolution;
  uint32_t PixelFormat;
  uint32_t LightMode;
  uint32_t ColorEffect;
  int32_t  Brightness;
  int32_t  Saturation;
  int32_t  Contrast;
  int32_t  HueDegree;
  uint32_t MirrorFlip;
  uint32_t Zoom;
  uint32_t NightMode;
  uint32_t IsMspCallbacksValid;
}CAMERA_Ctx_t;

typedef struct
{
  uint32_t Resolution;
  uint32_t LightMode;
  uint32_t ColorEffect;
  uint32_t Brightness;
  uint32_t Saturation;
  uint32_t Contrast;
  uint32_t HueDegree;
  uint32_t MirrorFlip;
  uint32_t Zoom;
  uint32_t NightMode;
} CAMERA_Capabilities_t;

/**
  * @}
  */

/** @defgroup STM32H743_MICROBOT_CAMERA_Exported_Constants CAMERA Exported Constants
  * @{
  */


#define CAMERA_MODE_CONTINUOUS         DCMI_MODE_CONTINUOUS
#define CAMERA_MODE_SNAPSHOT           DCMI_MODE_SNAPSHOT

/* Camera resolutions */
#define CAMERA_R160x120                 0U     /* QQVGA Resolution            */
#define CAMERA_R320x240                 1U     /* QVGA Resolution             */
#define CAMERA_R480x272                 2U     /* 480x272 Resolution          */
#define CAMERA_R640x480                 3U     /* VGA Resolution              */
#define CAMERA_R800x480                 4U     /* WVGA Resolution             */

/* Camera Pixel Format */
#define CAMERA_PF_RGB565                0U     /* Pixel Format RGB565         */
#define CAMERA_PF_RGB888                1U     /* Pixel Format RGB888         */
#define CAMERA_PF_YUV422                2U     /* Pixel Format YUV422         */

/* Brightness */
#define CAMERA_BRIGHTNESS_MIN          -4
#define CAMERA_BRIGHTNESS_MAX           4

/* Saturation */
#define CAMERA_SATURATION_MIN          -4
#define CAMERA_SATURATION_MAX           4

/* Contrast */
#define CAMERA_CONTRAST_MIN            -4
#define CAMERA_CONTRAST_MAX             4

/* Hue Control */
#define CAMERA_HUEDEGREE_MIN           -6
#define CAMERA_HUEDEGREE_MAX            5

/* Mirror/Flip */
#define CAMERA_MIRRORFLIP_NONE          0x00U   /* Set camera normal mode     */
#define CAMERA_MIRRORFLIP_FLIP          0x01U   /* Set camera flip config     */
#define CAMERA_MIRRORFLIP_MIRROR        0x02U   /* Set camera mirror config   */

/* Zoom */
#define CAMERA_ZOOM_x8                  0x00U   /* Set zoom to x8             */
#define CAMERA_ZOOM_x4                  0x11U   /* Set zoom to x4             */
#define CAMERA_ZOOM_x2                  0x22U   /* Set zoom to x2             */
#define CAMERA_ZOOM_x1                  0x44U   /* Set zoom to x1             */

/* Color Effect */
#define CAMERA_COLOR_EFFECT_NONE        0x00U   /* No effect                  */
#define CAMERA_COLOR_EFFECT_BLUE        0x01U   /* Blue effect                */
#define CAMERA_COLOR_EFFECT_RED         0x02U   /* Red effect                 */
#define CAMERA_COLOR_EFFECT_GREEN       0x04U   /* Green effect               */
#define CAMERA_COLOR_EFFECT_BW          0x08U   /* Black and White effect     */
#define CAMERA_COLOR_EFFECT_SEPIA       0x10U   /* Sepia effect               */
#define CAMERA_COLOR_EFFECT_NEGATIVE    0x20U   /* Negative effect            */

/* Light Mode */
#define CAMERA_LIGHT_AUTO               0x00U   /* Light Mode Auto            */
#define CAMERA_LIGHT_SUNNY              0x01U   /* Light Mode Sunny           */
#define CAMERA_LIGHT_OFFICE             0x02U   /* Light Mode Office          */
#define CAMERA_LIGHT_HOME               0x04U   /* Light Mode Home            */
#define CAMERA_LIGHT_CLOUDY             0x08U   /* Light Mode Claudy          */

/* Night Mode */
#define CAMERA_NIGHT_MODE_SET           0x00U   /* Disable night mode         */
#define CAMERA_NIGHT_MODE_RESET         0x01U   /* Enable night mode          */

#define CAMERA_OV5640_ADDRESS          0x78U

#define CAMERA_PWDN_PIN                           GPIO_PIN_0
#define CAMERA_PWDN_GPIO_PORT                     GPIOI
#define CAMERA_PWDN_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOI_CLK_ENABLE()
#define CAMERA_PWDN_GPIO_CLK_DISABLE()            __HAL_RCC_GPIOI_CLK_DISABLE()

#define CAMERA_RST_PIN                           GPIO_PIN_3
#define CAMERA_RST_GPIO_PORT                     GPIOE
#define CAMERA_RST_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOE_CLK_ENABLE()
#define CAMERA_RST_GPIO_CLK_DISABLE()            __HAL_RCC_GPIOE_CLK_DISABLE()

#define CAMERA_RST(x)  									        ((x) == 0 ? HAL_GPIO_WritePin(CAMERA_RST_GPIO_PORT, CAMERA_RST_PIN, GPIO_PIN_RESET) : HAL_GPIO_WritePin(CAMERA_RST_GPIO_PORT, CAMERA_RST_PIN, GPIO_PIN_SET))
#define CAMERA_PWDN(x)										      ((x) == 0 ? HAL_GPIO_WritePin(CAMERA_PWDN_GPIO_PORT, CAMERA_PWDN_PIN, GPIO_PIN_RESET) : HAL_GPIO_WritePin(CAMERA_PWDN_GPIO_PORT, CAMERA_PWDN_PIN, GPIO_PIN_SET))

#define CANVAS_WIDTH  320
#define CANVAS_HEIGHT  240
/**
  * @}
  */

#define I2C_TIMEOUT_MS                     100
/** @addtogroup STM32H743_MICROBOT_CAMERA_Exported_Variables
  * @{
  */
extern void                *Camera_CompObj;
extern DCMI_HandleTypeDef  hcamera_dcmi;
extern CAMERA_Ctx_t        Ov5640_Ctx;
/**
  * @}
  */

/** @addtogroup STM32H743_MICROBOT_CAMERA_Exported_Functions
  * @{
  */

int32_t BSP_CAMERA_Init(void);
int32_t BSP_CAMERA_GPIO_Init(void);
int32_t BSP_CAMERA_DeInit(void);
int32_t BSP_CAMERA_Start(uint32_t Mode, uint32_t FB_Address);
int32_t BSP_CAMERA_Stop(void);
int32_t BSP_CAMERA_Suspend(void);
int32_t BSP_CAMERA_Resume(void);
int32_t BSP_CAMERA_GetCapabilities(CAMERA_Capabilities_t *Capabilities);

int32_t BSP_CAMERA_SetResolution(uint32_t Resolution);
int32_t BSP_CAMERA_GetResolution(uint32_t *Resolution);

int32_t BSP_CAMERA_SetPixelFormat(uint32_t PixelFormat);
int32_t BSP_CAMERA_GetPixelFormat(uint32_t *PixelFormat);

int32_t BSP_CAMERA_SetLightMode(uint32_t LightMode);
int32_t BSP_CAMERA_GetLightMode(int32_t *LightMode);

int32_t BSP_CAMERA_SetColorEffect(uint32_t ColorEffect);
int32_t BSP_CAMERA_GetColorEffect(uint32_t *ColorEffect);

int32_t BSP_CAMERA_SetBrightness(int32_t Brightness);
int32_t BSP_CAMERA_GetBrightness(int32_t *Brightness);

int32_t BSP_CAMERA_SetSaturation(int32_t Saturation);
int32_t BSP_CAMERA_GetSaturation(int32_t *Saturation);

int32_t BSP_CAMERA_SetContrast(int32_t Contrast);
int32_t BSP_CAMERA_GetContrast(int32_t *Contrast);

int32_t BSP_CAMERA_SetHueDegree(int32_t HueDegree);
int32_t BSP_CAMERA_GetHueDegree(int32_t *HueDegree);

int32_t BSP_CAMERA_SetMirrorFlip(uint32_t MirrorFlip);
int32_t BSP_CAMERA_GetMirrorFlip(uint32_t *MirrorFlip);

int32_t BSP_CAMERA_SetZoom(uint32_t Zoom);
int32_t BSP_CAMERA_GetZoom(uint32_t *Zoom);

int32_t BSP_CAMERA_EnableNightMode(void);
int32_t BSP_CAMERA_DisableNightMode(void);

int32_t BSP_CAMERA_HwReset(void);
int32_t BSP_CAMERA_PwrDown(void);

/* These functions can be modified in case the current settings (e.g. DMA stream)
   need to be changed for specific application needs */
HAL_StatusTypeDef MX_DCMI_Init(DCMI_HandleTypeDef* hdcmi);
void BSP_CAMERA_BufferClear(void);
void BSP_CAMERA_BufferShow(void);
/**
  * @}
  */
extern const mp_obj_type_t camera_ov5640_type;
extern uint32_t ov5640_fb[320 * 240 /2];
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* STM32H743_MICROBOT_CAMERA_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
