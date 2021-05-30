Version 1.o:
The version includes the support of SDRAM and QFlash for MicroBot STM32H743 Board!
The update file list is in the following:
 1 board_init.c
 2 mpconfigboard.h
 3 pins.csv
 4 port/stm32/sdram.c: add the define for STM32H7

-----------------------------------------------------------------------------------
Version 2.0
增加LVGL的支持。
1 直接使用lv_micropython版本
2 将lv_bindings_micropython下载下来，copy到lv_micropython/lib目录
3 改lv_bindings目录下的drivers/stm32下面的液晶和触摸屏驱动
4 为了支持STM32H7xx，要修改port/stm32/i2c.c文件，增加对应的i2c控制部分代码
5 为了使用LVGL库，mpconfigboard.mk文件里增加：MICROPY_PY_LVGL = 1
6 在ports/stm32/boards/stm32h7xx_hal_conf_base.h文件中，增加：
     #include "stm32h7xx_hal_ltdc.h"
     #include "stm32h7xx_hal_ltdc_ex.h"
     #include "stm32h7xx_hal_dma2d.h"
     #define HAL_LTDC_MODULE_ENABLED
     #define HAL_DMA2D_MODULE_ENABLED
