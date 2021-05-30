/**
  ******************************************************************************
  * @file    STM32H743_MICROBOT_camera.c
  * @author  MCD Application Team
  * @brief   This file includes the driver for Camera modules mounted on
  *          STM32H743_MICROBOT board.
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

/* File Info: ------------------------------------------------------------------
                                   User NOTES
1. How to use this driver:
--------------------------
   - This driver is used to drive the camera.
   - The S5K5CAG or OV5640 component driver MUST be included with this driver.

2. Driver description:
---------------------
     o Initialize the camera instance using the BSP_CAMERA_Init() function with the required
       Resolution and Pixel format where:
       - Instance: Is the physical camera interface and is always 0 on this board.
       - Resolution: The camera resolution
       - PixelFormat: The camera Pixel format

     o DeInitialize the camera instance using the BSP_CAMERA_Init() . This
       function will firstly stop the camera to insure the data transfer complete.
       Then de-initializes the dcmi peripheral.

     o Get the camera instance capabilities using the BSP_CAMERA_GetCapabilities().
       This function must be called after the BSP_CAMERA_Init() to get the right
       sensor capabilities

     o Start the camera using the CAMERA_Start() function by specifying the capture Mode:
       - CAMERA_MODE_CONTINUOUS: For continuous capture
       - CAMERA_MODE_SNAPSHOT  : For on shot capture

     o Suspend, resume or stop the camera capture using the following functions:
      - BSP_CAMERA_Suspend()
      - BSP_CAMERA_Resume()
      - BSP_CAMERA_Stop()

     o Call BSP_CAMERA_SetResolution()/BSP_CAMERA_GetResolution() to set/get the camera resolution
       Resolution: - CAMERA_R160x120
                   - CAMERA_R320x240
                   - CAMERA_R480x272
                   - CAMERA_R640x480
                   - CAMERA_R800x480

     o Call BSP_CAMERA_SetPixelFormat()/BSP_CAMERA_GetPixelFormat() to set/get the camera pixel format
       PixelFormat: - CAMERA_PF_RGB565
                    - CAMERA_PF_RGB888
                    - CAMERA_PF_YUV422

     o Call BSP_CAMERA_SetLightMode()/BSP_CAMERA_GetLightMode() to set/get the camera light mode
       LightMode: - CAMERA_LIGHT_AUTO
                  - CAMERA_LIGHT_SUNNY
                  - CAMERA_LIGHT_OFFICE
                  - CAMERA_LIGHT_HOME
                  - CAMERA_LIGHT_CLOUDY

     o Call BSP_CAMERA_SetColorEffect()/BSP_CAMERA_GetColorEffect() to set/get the camera color effects
       Effect: - CAMERA_COLOR_EFFECT_NONE
               - CAMERA_COLOR_EFFECT_BLUE
               - CAMERA_COLOR_EFFECT_RED
               - CAMERA_COLOR_EFFECT_GREEN
               - CAMERA_COLOR_EFFECT_BW
               - CAMERA_COLOR_EFFECT_SEPIA
               - CAMERA_COLOR_EFFECT_NEGATIVE

     o Call BSP_CAMERA_SetBrightness()/BSP_CAMERA_GetBrightness() to set/get the camera Brightness
       Brightness is value between -4(Level 4 negative) and 4(Level 4 positive).

     o Call BSP_CAMERA_SetSaturation()/BSP_CAMERA_GetSaturation() to set/get the camera Saturation
       Saturation is value between -4(Level 4 negative) and 4(Level 4 positive).

     o Call BSP_CAMERA_SetContrast()/BSP_CAMERA_GetContrast() to set/get the camera Contrast
       Contrast is value between -4(Level 4 negative) and 4(Level 4 positive).

     o Call BSP_CAMERA_SetHueDegree()/BSP_CAMERA_GetHueDegree() to set/get the camera Hue Degree
       HueDegree is value between -4(180 degree negative) and 4(150 degree positive).

     o Call BSP_CAMERA_SetMirrorFlip()/BSP_CAMERA_GetMirrorFlip() to set/get the camera mirror and flip
       MirrorFlip could be any combination of: - CAMERA_MIRRORFLIP_NONE
                                               - CAMERA_MIRRORFLIP_FLIP
                                               - CAMERA_MIRRORFLIP_MIRROR
       Note that This feature is only supported with S5K5CAG sensor.

     o Call BSP_CAMERA_SetZoom()/BSP_CAMERA_GetZoom() to set/get the camera zooming
       Zoom is supported only with S5K5CAG sensor could be any value of:
       - CAMERA_ZOOM_x8 for CAMERA_R160x120 resolution only
       - CAMERA_ZOOM_x4 For all resolutions except CAMERA_R640x480 and CAMERA_R800x480
       - CAMERA_ZOOM_x2 For all resolutions except CAMERA_R800x480
       - CAMERA_ZOOM_x1 For all resolutions

     o Call BSP_CAMERA_EnableNightMode() to enable night mode. This feature is only supported
       with S5K5CAG sensor

     o Call BSP_CAMERA_DisableNightMode() to disable night mode. This feature is only supported
       with S5K5CAG sensor

    o Call BSP_CAMERA_RegisterDefaultMspCallbacks() to register Msp default callbacks
    o Call BSP_CAMERA_RegisterMspCallbacks() to register application Msp callbacks.

    o Error, line event, vsync event and frame event are handled through dedicated weak
      callbacks that can be override at application level: BSP_CAMERA_LineEventCallback(),
      BSP_CAMERA_FrameEventCallback(), BSP_CAMERA_VsyncEventCallback(), BSP_CAMERA_ErrorCallback()

  Known Limitations:
  ------------------
   1- CAMERA_PF_RGB888 resolution is not supported with S5K5CAG sensor.
   2- The following feature are only supported through S5K5CAG sensor:
      o LightMode setting
      o Saturation setting
      o HueDegree setting
      o Mirror/Flip setting
      o Zoom setting
      o NightMode enable/disable
------------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "py/obj.h"
#include "py/runtime.h"
#include "py/mphal.h"

#include "lib/oofatfs/ff.h"
#include "extmod/vfs.h"
#include "extmod/vfs_fat.h"

#include "py/objstr.h"
#include "py/objlist.h"
#include "py/stream.h"
#include "pin.h"
#include "pin_static_af.h"

#include "stdint.h"
#include "stdio.h"
#include "microbot_camera.h"
#include "stm32h743_microbot_bus.h"
#include "microbot_led.h"

//#include "i2c.h"

/** @addtogroup BSP
  * @{
  */
#define OV5640_DEBUG    1

/** @addtogroup STM32H743_MICROBOT
  * @{
  */

/** @addtogroup STM32H743_MICROBOT_CAMERA
  * @{
  */
/** @defgroup STM32H743_MICROBOT_CAMERA_Exported_Variables CAMERA Exported Variables
  * @{
  */
void                *Camera_CompObj = NULL;
DCMI_HandleTypeDef  hcamera_dcmi;
DMA_HandleTypeDef hdma_handler;
CAMERA_Ctx_t        Ov5640_Ctx;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


/**
  * @}
  */

/** @defgroup STM32H743_MICROBOT_CAMERA_Private_Variables CAMERA Private Variables
  * @{
  */
static CAMERA_Drv_t *Camera_Drv = NULL;
static CAMERA_Capabilities_t *Camera_Cap;
/**
  * @}
  */

/** @defgroup STM32H743_MICROBOT_CAMERA_Private_FunctionsPrototypes CAMERA Private Functions Prototypes
  * @{
  */
//static int32_t GetSize(uint32_t Resolution, uint32_t PixelFormat);
//static void DCMI_MspInit(DCMI_HandleTypeDef *hdcmi);
static void DCMI_MspDeInit(DCMI_HandleTypeDef *hdcmi);
static int32_t OV5640_Probe(uint32_t Resolution, uint32_t PixelFormat);

/**
  * @}
  */




int32_t BSP_CAMERA_GPIO_Init(void)
{
  GPIO_InitTypeDef gpio_init_structure;

  CAMERA_RST_GPIO_CLK_ENABLE();
  CAMERA_PWDN_GPIO_CLK_ENABLE();

  /* Enable GPIO clocks */
    /*Configure GPIO PI0, PE3 pins as PWDN, RESET pins */
  gpio_init_structure.Pin = CAMERA_PWDN_PIN;
  gpio_init_structure.Mode = GPIO_MODE_OUTPUT_PP;
  gpio_init_structure.Pull = GPIO_NOPULL;
  gpio_init_structure.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(CAMERA_PWDN_GPIO_PORT, &gpio_init_structure);

  gpio_init_structure.Pin = CAMERA_RST_PIN;
  gpio_init_structure.Mode = GPIO_MODE_OUTPUT_PP;
  gpio_init_structure.Pull = GPIO_NOPULL;
  gpio_init_structure.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(CAMERA_RST_GPIO_PORT, &gpio_init_structure);

	//Camera Module OV5640 Initlizing
  CAMERA_RST(0);	
	HAL_Delay(20); 
	CAMERA_PWDN(0);	
	HAL_Delay(5);  
	CAMERA_RST(1);			 
	HAL_Delay(20);

  return 0;

}

/** @defgroup STM32H743_MICROBOT_CAMERA_Exported_Functions CAMERA Exported Functions
  * @{
  */

/**
  * @brief  Initializes the camera.
  * @param  Instance    Camera instance.
  * @param  Resolution  Camera sensor requested resolution (x, y) : standard resolution
  *         naming QQVGA, QVGA, VGA ...
  * @param  PixelFormat Capture pixel format
  * @retval BSP status
  */
int32_t BSP_CAMERA_Init(void)
{
  int32_t ret;

  //Camera Module Power&Reset Init...
  BSP_CAMERA_GPIO_Init();
  
  if(MX_DCMI_Init(&hcamera_dcmi) != HAL_OK)
  {
      ret = BSP_ERROR_PERIPH_FAILURE;
      return ret;
  }

  //DCMI_MspInit(&hcamera_dcmi);

  Ov5640_Ctx.Resolution = CAMERA_R320x240;
  Ov5640_Ctx.PixelFormat = CAMERA_PF_RGB565;

  ret = OV5640_Probe(Ov5640_Ctx.Resolution, Ov5640_Ctx.PixelFormat);
 
   BSP_LED_GREEN(1);

  return ret;
}

/**
  * @brief  DeInitializes the camera.
  * @param  Instance Camera instance.
  * @retval BSP status
  */
int32_t BSP_CAMERA_DeInit(void)
{
  int32_t ret;


    hcamera_dcmi.Instance = DCMI;

    /* First stop the camera to insure all data are transfered */
    if(BSP_CAMERA_Stop() != BSP_ERROR_NONE)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else if(HAL_DCMI_DeInit(&hcamera_dcmi) != HAL_OK)
    {
      ret = BSP_ERROR_PERIPH_FAILURE;
    }
    else
    {
      DCMI_MspDeInit(&hcamera_dcmi);

      /* Set Camera in Power Down */
      if(BSP_CAMERA_PwrDown() != BSP_ERROR_NONE)
      {
        ret = BSP_ERROR_BUS_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }

  /* Return BSP status */
  return ret;
}


/**
  * @brief  Initializes the DCMI peripheral
  * @param  hdcmi  DCMI handle
  * @note   Being __weak it can be overwritten by the application
  * @retval HAL status
  */
HAL_StatusTypeDef MX_DCMI_Init(DCMI_HandleTypeDef* hdcmi)
{
  /*** Configures the DCMI to interface with the camera module ***/
  /* DCMI configuration */
  hdcmi->Instance              = DCMI;
  hdcmi->Init.CaptureRate      = DCMI_CR_ALL_FRAME;
  hdcmi->Init.HSPolarity       = DCMI_HSPOLARITY_LOW;
  hdcmi->Init.SynchroMode      = DCMI_SYNCHRO_HARDWARE;
  hdcmi->Init.VSPolarity       = DCMI_VSPOLARITY_HIGH;
  hdcmi->Init.ExtendedDataMode = DCMI_EXTEND_DATA_8B;
  hdcmi->Init.PCKPolarity      = DCMI_PCKPOLARITY_RISING;

  if(HAL_DCMI_Init(hdcmi) != HAL_OK)
  {
    return HAL_ERROR;
  }
  return HAL_OK;
}


/**
  * @brief  Starts the camera capture in continuous mode.
  * @param  Instance Camera instance.
  * @param  pBff     pointer to the camera output buffer
  * @param  Mode CAMERA_MODE_CONTINUOUS or CAMERA_MODE_SNAPSHOT
  * @retval BSP status
  */
int32_t BSP_CAMERA_Start(uint32_t Mode, uint32_t FB_Address)
{
  int32_t ret = 0;

  ret = HAL_DCMI_Start_DMA(&hcamera_dcmi, Mode, (uint32_t)FB_Address, 320*240/2);

  /* Return DCMI status */
  return ret;
}

void DCMI_IRQHandler(void)
{
    BSP_LED_GREEN(1);

    HAL_DCMI_IRQHandler(&hcamera_dcmi);
}

//捕获到一帧图像处理函数
void HAL_DCMI_VsyncEventCallback(DCMI_HandleTypeDef *hdcmi)
{

  UNUSED(hdcmi);

  BSP_LED_RED(1);
  BSP_CAMERA_Suspend();

  BSP_CAMERA_Start(DCMI_MODE_CONTINUOUS, (uint32_t)ov5640_fb); 
  //LCD_LL_Convert_RGB565ToARGB8888((uint32_t *)cam_fb, (uint32_t*)(hLtdcHandler.LayerCfg[ActiveLayer].FBStartAdress), 320, 240);
  BSP_CAMERA_Resume();

}
/*
void lv_ex_canvas_1(void)
{
    lv_draw_rect_dsc_t rect_dsc;
    lv_draw_rect_dsc_init(&rect_dsc);
    rect_dsc.radius = 10;
    rect_dsc.bg_opa = LV_OPA_COVER;
    rect_dsc.bg_grad_dir = LV_GRAD_DIR_HOR;
    rect_dsc.bg_color = LV_COLOR_RED;
    rect_dsc.bg_grad_color = LV_COLOR_BLUE;
    rect_dsc.border_width = 2;
    rect_dsc.border_opa = LV_OPA_90;
    rect_dsc.border_color = LV_COLOR_WHITE;
    rect_dsc.shadow_width = 5;
    rect_dsc.shadow_ofs_x = 5;
    rect_dsc.shadow_ofs_y = 5;

    lv_draw_label_dsc_t label_dsc;
    lv_draw_label_dsc_init(&label_dsc);
    label_dsc.color = LV_COLOR_YELLOW;

    static lv_color_t cbuf[LV_CANVAS_BUF_SIZE_TRUE_COLOR(CANVAS_WIDTH, CANVAS_HEIGHT)];

    lv_obj_t * canvas = lv_canvas_create(lv_scr_act(), NULL);
    lv_canvas_set_buffer(canvas, cbuf, CANVAS_WIDTH, CANVAS_HEIGHT, LV_IMG_CF_TRUE_COLOR);
    lv_obj_align(canvas, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_canvas_fill_bg(canvas, LV_COLOR_SILVER, LV_OPA_COVER);

    lv_canvas_draw_rect(canvas, 70, 60, 100, 70, &rect_dsc);

    lv_canvas_draw_text(canvas, 40, 20, 100, &label_dsc, "Some text on text canvas", LV_LABEL_ALIGN_LEFT);

    // Test the rotation. It requires an other buffer where the orignal image is stored.
    // So copy the current image to buffer and rotate it to the canvas 
    static lv_color_t cbuf_tmp[CANVAS_WIDTH * CANVAS_HEIGHT];
    memcpy(cbuf_tmp, cbuf, sizeof(cbuf_tmp));
    lv_img_dsc_t img;
    img.data = (void *)cbuf_tmp;
    img.header.cf = LV_IMG_CF_TRUE_COLOR;
    img.header.w = CANVAS_WIDTH;
    img.header.h = CANVAS_HEIGHT;

    lv_canvas_fill_bg(canvas, LV_COLOR_SILVER, LV_OPA_COVER);
    lv_canvas_transform(canvas, &img, 30, LV_IMG_ZOOM_NONE, 0, 0, CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, true);
}
*/
/**
  * @brief  Stop the CAMERA capture
  * @param  Instance Camera instance.
  * @retval BSP status
  */
int32_t BSP_CAMERA_Stop(void)
{
  int32_t ret;


  if(HAL_DCMI_Stop(&hcamera_dcmi) != HAL_OK)
  {
    ret = BSP_ERROR_PERIPH_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief Suspend the CAMERA capture
  * @param  Instance Camera instance.
  */
int32_t BSP_CAMERA_Suspend(void)
{
  int32_t ret;


  if(HAL_DCMI_Suspend(&hcamera_dcmi) != HAL_OK)
  {
    return BSP_ERROR_PERIPH_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief Resume the CAMERA capture
  * @param  Instance Camera instance.
  */
int32_t BSP_CAMERA_Resume(void)
{
  int32_t ret;


  if(HAL_DCMI_Resume(&hcamera_dcmi) != HAL_OK)
  {
    ret = BSP_ERROR_PERIPH_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Get the Camera Capabilities.
  * @param  Instance  Camera instance.
  * @param  Capabilities  pointer to camera Capabilities
  * @note   This function should be called after the init. This to get Capabilities
  *         from the right camera sensor(S5K5CAG or OV5640)
  * @retval Component status
  */
int32_t BSP_CAMERA_GetCapabilities(CAMERA_Capabilities_t *Capabilities)
{
  int32_t ret;


  if(Camera_Drv->GetCapabilities(Camera_CompObj, Capabilities) < 0)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Set the camera pixel format.
  * @param  Instance  Camera instance.
  * @param  PixelFormat pixel format to be configured
  * @retval BSP status
  */
int32_t BSP_CAMERA_SetPixelFormat(uint32_t PixelFormat)
{
  int32_t ret;


  if(Camera_Drv->SetPixelFormat(Camera_CompObj, PixelFormat) < 0)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    Ov5640_Ctx.PixelFormat = PixelFormat;
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Get the camera pixel format.
  * @param  Instance  Camera instance.
  * @param  PixelFormat pixel format to be returned
  * @retval BSP status
  */
int32_t BSP_CAMERA_GetPixelFormat(uint32_t *PixelFormat)
{
  int32_t ret;


    *PixelFormat = Ov5640_Ctx.PixelFormat;
    ret = BSP_ERROR_NONE;

  /* Return BSP status */
  return ret;
}


/**
  * @brief  Set the camera Resolution.
  * @param  Instance  Camera instance.
  * @param  Resolution Resolution to be configured
  * @retval BSP status
  */
int32_t BSP_CAMERA_SetResolution(uint32_t Resolution)
{
  int32_t ret;

  if(Camera_Cap->Resolution == 0U)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if(Camera_Drv->SetResolution(Camera_CompObj, Resolution) < 0)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    Ov5640_Ctx.Resolution = Resolution;
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Get the camera Resolution.
  * @param  Instance  Camera instance.
  * @param  Resolution Resolution to be returned
  * @retval BSP status
  */
int32_t BSP_CAMERA_GetResolution(uint32_t *Resolution)
{
  int32_t ret;

  if(Camera_Cap->Resolution == 0U)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    *Resolution = Ov5640_Ctx.Resolution;
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Set the camera Light Mode.
  * @param  Instance  Camera instance.
  * @param  LightMode Light Mode to be configured
  * @retval BSP status
  */
int32_t BSP_CAMERA_SetLightMode(uint32_t LightMode)
{
  int32_t ret;

  if(Camera_Cap->LightMode == 0U)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if(Camera_Drv->SetLightMode(Camera_CompObj, LightMode) < 0)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    Ov5640_Ctx.LightMode = LightMode;
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Get the camera Light Mode.
  * @param  Instance  Camera instance.
  * @param  LightMode Light Mode to be returned
  * @retval BSP status
  */
int32_t BSP_CAMERA_GetLightMode(int32_t *LightMode)
{
  int32_t ret;

  if(Camera_Cap->LightMode == 0U)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    *LightMode = Ov5640_Ctx.LightMode;
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Set the camera Special Effect.
  * @param  Instance Camera instance.
  * @param  ColorEffect Effect to be configured
  * @retval BSP status
  */
int32_t BSP_CAMERA_SetColorEffect(uint32_t ColorEffect)
{
  int32_t ret;

  if(Camera_Cap->ColorEffect == 0U)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if(Camera_Drv->SetColorEffect(Camera_CompObj, ColorEffect) < 0)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    Ov5640_Ctx.ColorEffect = ColorEffect;
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Get the camera Special Effect.
  * @param  Instance Camera instance.
  * @param  ColorEffect Effect to be returned
  * @retval BSP status
  */
int32_t BSP_CAMERA_GetColorEffect(uint32_t *ColorEffect)
{
  int32_t ret;
  
  if(Camera_Cap->ColorEffect == 0U)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    *ColorEffect = Ov5640_Ctx.ColorEffect;
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Set the camera Brightness Level.
  * @param  Instance   Camera instance.
  * @param  Brightness Brightness Level
  * @retval BSP status
  */
int32_t BSP_CAMERA_SetBrightness(int32_t Brightness)
{
  int32_t ret;

  if((Brightness < CAMERA_BRIGHTNESS_MIN) && (Brightness > CAMERA_BRIGHTNESS_MAX))
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if(Camera_Cap->Brightness == 0U)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if(Camera_Drv->SetBrightness(Camera_CompObj, Brightness) < 0)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    Ov5640_Ctx.Brightness = Brightness;
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Get the camera Brightness Level.
  * @param  Instance Camera instance.
  * @param  Brightness  Brightness Level
  * @retval BSP status
  */
int32_t BSP_CAMERA_GetBrightness(int32_t *Brightness)
{
  int32_t ret;

  if(Camera_Cap->Brightness == 0U)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    *Brightness = Ov5640_Ctx.Brightness;
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Set the camera Saturation Level.
  * @param  Instance    Camera instance.
  * @param  Saturation  Saturation Level
  * @retval BSP status
  */
int32_t BSP_CAMERA_SetSaturation(int32_t Saturation)
{
  int32_t ret;

  if( (Saturation < CAMERA_SATURATION_MIN) && (Saturation > CAMERA_SATURATION_MAX))
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if(Camera_Cap->Saturation == 0U)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if(Camera_Drv->SetSaturation(Camera_CompObj, Saturation)  < 0)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    Ov5640_Ctx.Saturation = Saturation;
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Get the camera Saturation Level.
  * @param  Instance    Camera instance.
  * @param  Saturation  Saturation Level
  * @retval BSP status
  */
int32_t BSP_CAMERA_GetSaturation(int32_t *Saturation)
{
  int32_t ret;

  if(Camera_Cap->Saturation == 0U)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    *Saturation = Ov5640_Ctx.Saturation;
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Set the camera Contrast Level.
  * @param  Instance Camera instance.
  * @param  Contrast Contrast Level
  * @retval BSP status
  */
int32_t BSP_CAMERA_SetContrast(int32_t Contrast)
{
  int32_t ret;

  if( (Contrast < CAMERA_CONTRAST_MIN) && (Contrast > CAMERA_CONTRAST_MAX))
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if(Camera_Cap->Contrast == 0U)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if(Camera_Drv->SetContrast(Camera_CompObj, Contrast)  < 0)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    Ov5640_Ctx.Contrast = Contrast;
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Get the camera Contrast Level.
  * @param  Instance Camera instance.
  * @param  Contrast Contrast Level
  * @retval BSP status
  */
int32_t BSP_CAMERA_GetContrast(int32_t *Contrast)
{
  int32_t ret;

  if(Camera_Cap->Contrast == 0U)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    *Contrast = Ov5640_Ctx.Contrast;
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Set the camera Hue Degree.
  * @param  Instance   Camera instance.
  * @param  HueDegree  Hue Degree
  * @retval BSP status
  */
int32_t BSP_CAMERA_SetHueDegree(int32_t HueDegree)
{
  int32_t ret;

  if( (HueDegree < CAMERA_HUEDEGREE_MIN) && (HueDegree > CAMERA_HUEDEGREE_MAX))
  {
    ret = BSP_ERROR_WRONG_PARAM;
  }
  else if(Camera_Cap->HueDegree == 0U)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if(Camera_Drv->SetHueDegree(Camera_CompObj, HueDegree) < 0)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    Ov5640_Ctx.HueDegree = HueDegree;
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Get the camera Hue Degree.
  * @param  Instance   Camera instance.
  * @param  HueDegree  Hue Degree
  * @retval BSP status
  */
int32_t BSP_CAMERA_GetHueDegree(int32_t *HueDegree)
{
  int32_t ret;

  if(Camera_Cap->HueDegree == 0U)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    *HueDegree = Ov5640_Ctx.HueDegree;
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Set the camera Mirror/Flip.
  * @param  Instance  Camera instance.
  * @param  MirrorFlip CAMERA_MIRRORFLIP_NONE or any combination of
  *                    CAMERA_MIRRORFLIP_FLIP and CAMERA_MIRRORFLIP_MIRROR
  * @retval BSP status
  */
int32_t BSP_CAMERA_SetMirrorFlip(uint32_t MirrorFlip)
{
  int32_t ret;

  if(Camera_Cap->MirrorFlip == 0U)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if(Camera_Drv->MirrorFlipConfig(Camera_CompObj, MirrorFlip)  < 0)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    Ov5640_Ctx.MirrorFlip = MirrorFlip;
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Get the camera Mirror/Flip.
  * @param  Instance   Camera instance.
  * @param  MirrorFlip Mirror/Flip config
  * @retval BSP status
  */
int32_t BSP_CAMERA_GetMirrorFlip(uint32_t *MirrorFlip)
{
  int32_t ret;

  if(Camera_Cap->MirrorFlip == 0U)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    *MirrorFlip = Ov5640_Ctx.MirrorFlip;
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Set the camera zoom
  * @param  Instance Camera instance.
  * @param  Zoom     Zoom to be configured
  * @retval BSP status
  */
int32_t BSP_CAMERA_SetZoom(uint32_t Zoom)
{
  int32_t ret;

  if(Camera_Cap->Zoom == 0U)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if(Camera_Drv->ZoomConfig(Camera_CompObj, Zoom) < 0)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    Ov5640_Ctx.Zoom = Zoom;
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Get the camera zoom
  * @param  Instance Camera instance.
  * @param  Zoom     Zoom to be returned
  * @retval BSP status
  */
int32_t BSP_CAMERA_GetZoom(uint32_t *Zoom)
{
  int32_t ret;

  if(Camera_Cap->Zoom == 0U)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else
  {
    *Zoom = Ov5640_Ctx.Zoom;
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Enable the camera night mode
  * @param  Instance Camera instance.
  * @retval BSP status
  */
int32_t BSP_CAMERA_EnableNightMode(void)
{
  int32_t ret;

  if(Camera_Cap->NightMode == 0U)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if(Camera_Drv->NightModeConfig(Camera_CompObj, CAMERA_NIGHT_MODE_SET) < 0)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  Disable the camera night mode
  * @param  Instance Camera instance.
  * @retval BSP status
  */
int32_t BSP_CAMERA_DisableNightMode(void)
{
  int32_t ret;

  if(Camera_Cap->NightMode == 0U)
  {
    ret = BSP_ERROR_FEATURE_NOT_SUPPORTED;
  }
  else if(Camera_Drv->NightModeConfig(Camera_CompObj, CAMERA_NIGHT_MODE_RESET) < 0)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    ret = BSP_ERROR_NONE;
  }

  /* Return BSP status */
  return ret;
}

/**
  * @brief  CAMERA hardware reset
  * @param  Instance Camera instance.
  * @retval BSP status
  */
int32_t BSP_CAMERA_HwReset(void)
{
  int32_t ret = BSP_ERROR_NONE;
 
 
  CAMERA_RST(0);
  return ret;
}

/**
  * @brief  CAMERA power down
  * @param  Instance Camera instance.
  * @retval BSP status
  */
int32_t BSP_CAMERA_PwrDown(void)
{
  int32_t ret = BSP_ERROR_NONE;
 
 
  CAMERA_PWDN(1);
  
  return ret;
}

/**
  * @}
  */

/** @defgroup STM32H743_MICROBOT_CAMERA_Private_Functions CAMERA Private Functions
  * @{
  */


/**
  * @brief  DeInitializes the DCMI MSP.
  * @param  hdcmi  DCMI handle
  * @retval None
  */
static void DCMI_MspDeInit(DCMI_HandleTypeDef *hdcmi)
{
  GPIO_InitTypeDef gpio_init_structure;

  /* Disable NVIC  for DCMI transfer complete interrupt */
  HAL_NVIC_DisableIRQ(DCMI_IRQn);

  /* Disable NVIC for DMA2 transfer complete interrupt */
  HAL_NVIC_DisableIRQ(DMA2_Stream1_IRQn);

  /* Configure the DMA stream */
  (void)HAL_DMA_DeInit(hdcmi->DMA_Handle);

  /* DeInit DCMI GPIOs */
  gpio_init_structure.Pin       = GPIO_PIN_6 | GPIO_PIN_4;
  HAL_GPIO_DeInit(GPIOA, gpio_init_structure.Pin);

  gpio_init_structure.Pin       = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_9;
  HAL_GPIO_DeInit(GPIOB, gpio_init_structure.Pin);

  gpio_init_structure.Pin       = GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_14;
  HAL_GPIO_DeInit(GPIOH, gpio_init_structure.Pin);

  gpio_init_structure.Pin       = GPIO_PIN_6;
  HAL_GPIO_DeInit(GPIOI, gpio_init_structure.Pin);

  gpio_init_structure.Pin       = GPIO_PIN_11;
  HAL_GPIO_DeInit(GPIOG, gpio_init_structure.Pin);

  /* Disable DCMI clock */
  __HAL_RCC_DCMI_CLK_DISABLE();
}

void HAL_DCMI_MspInit(DCMI_HandleTypeDef* dcmiHandle)
{
  BSP_LED_RED(1);

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(dcmiHandle->Instance==DCMI)
  {
  /* USER CODE BEGIN DCMI_MspInit 0 */

  /* USER CODE END DCMI_MspInit 0 */
    /* DCMI clock enable */
    __HAL_RCC_DCMI_CLK_ENABLE();
    __HAL_RCC_DMA2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE();
    /**DCMI GPIO Configuration
    PA4     ------> DCMI_HSYNC
    PA6     ------> DCMI_PIXCLK
    PH9     ------> DCMI_D0
    PH10     ------> DCMI_D1
    PH11     ------> DCMI_D2
    PH14     ------> DCMI_D4
    PB6     ------> DCMI_D5 //
    PG11     ------> DCMI_D3
    PB7     ------> DCMI_VSYNC
    PB9     ------> DCMI_D7
    PI6     ------> DCMI_D6
    */
    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);
/*
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
*/
    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
    HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

    /* DCMI DMA Init */
    /* DCMI Init */
    hdma_handler.Instance = DMA2_Stream1;
    hdma_handler.Init.Request = DMA_REQUEST_DCMI;
    hdma_handler.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_handler.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_handler.Init.MemInc = DMA_MINC_ENABLE;
    hdma_handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_handler.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_handler.Init.Mode = DMA_CIRCULAR;;
    hdma_handler.Init.Priority = DMA_PRIORITY_HIGH;;
		hdma_handler.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
		hdma_handler.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
		hdma_handler.Init.MemBurst = DMA_MBURST_INC4;
		hdma_handler.Init.PeriphBurst = DMA_PBURST_SINGLE;  
    
    HAL_DMA_Init(&hdma_handler);

    __HAL_LINKDMA(dcmiHandle,DMA_Handle,hdma_handler);

    /* DCMI interrupt Init */
    HAL_NVIC_SetPriority(DCMI_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DCMI_IRQn);
    HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
  /* USER CODE BEGIN DCMI_MspInit 1 */
    //BSP_CAMERA_Start(DCMI_MODE_CONTINUOUS, (uint32_t)dcmi_fb);
		//HAL_DCMI_Start_DMA(&hdcmi, DCMI_MODE_CONTINUOUS, (uint32_t)dcmi_data_buf, OV5640_XSize*OV5640_YSize/2);
  /* USER CODE END DCMI_MspInit 1 */
  }
}

/**
  * @brief  Register Bus IOs if component ID is OK
  * @retval error status
  */
static int32_t OV5640_Probe(uint32_t Resolution, uint32_t PixelFormat)
{
  int32_t ret = 0;
  OV5640_IO_t              IOCtx;
  uint32_t                  id;
  static OV5640_Object_t   OV5640Obj;

  IOCtx.Address     = CAMERA_OV5640_ADDRESS;
  IOCtx.Init        = BSP_I2C4_Init;
  IOCtx.DeInit      = BSP_I2C4_DeInit;
  IOCtx.ReadReg     = BSP_I2C4_ReadReg16;
  IOCtx.WriteReg    = BSP_I2C4_WriteReg16;
  IOCtx.GetTick     = BSP_GetTick;

  id = ret;

  if(OV5640_RegisterBusIO (&OV5640Obj, &IOCtx) != OV5640_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else if(OV5640_ReadID(&OV5640Obj, &id) != OV5640_OK)
  {
    ret = BSP_ERROR_COMPONENT_FAILURE;
  }
  else
  {
    if(id != OV5640_ID)
    {
      ret = BSP_ERROR_UNKNOWN_COMPONENT;
    }
    else
    {
      Camera_Drv = (CAMERA_Drv_t *) &OV5640_CAMERA_Driver;
      Camera_CompObj = &OV5640Obj;
      if(Camera_Drv->Init(Camera_CompObj, Resolution, PixelFormat) != OV5640_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
	  else if(Camera_Drv->GetCapabilities(Camera_CompObj, Camera_Cap) != OV5640_OK)
      {
        ret = BSP_ERROR_COMPONENT_FAILURE;
      }
      else
      {
        ret = BSP_ERROR_NONE;
      }
    }
  }


  return id;
}

/**
  * @}
  */



/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
