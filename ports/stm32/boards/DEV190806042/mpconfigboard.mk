
# MCU settings
MCU_SERIES = h7
CMSIS_MCU = STM32H743xx
MICROPY_FLOAT_IMPL = double
AF_FILE = boards/stm32h743_af.csv
TOP_DIR := $(abspath ../..)

# Enable LVGL modules
LV_CONF_PATH = ${TOP_DIR}/user_modules/lv_binding_micropython/driver/stm32/st_ltdc/lv_conf.h
USER_C_MODULES = ${TOP_DIR}/user_modules/lv_binding_micropython/micropython.mk
USER_C_MODULES += ${TOP_DIR}/user_modules/lv_binding_micropython/driver/stm32/st_ltdc

ifeq ($(USE_MBOOT),1)
# When using Mboot everything goes after the bootloader
LD_FILES = boards/FK743M5-XIH6/stm32h743.ld
TEXT0_ADDR = 0x08020000
else
# When not using Mboot everything goes at the start of flash
LD_FILES = boards/FK743M5-XIH6/stm32h743.ld
TEXT0_ADDR = 0x08000000
endif

# MicroPython settings
MICROPY_PY_LWIP = 0
MICROPY_PY_SSL = 0
MICROPY_SSL_MBEDTLS = 0
MICROPY_VFS_LFS2 = 1
MICROPY_VFS_FAT = 1
MICROPY_HW_ENABLE_ISR_UART_FLASH_FUNCS_IN_RAM = 1

FROZEN_MANIFEST ?= $(BOARD_DIR)/manifest.py
FROZEN_MANIFEST += ${TOP_DIR}/user_modules/lv_binding_micropython/driver/stm32/st_ltdc/manifest.py

HAL_SRC_C += $(addprefix $(STM32LIB_HAL_BASE)/Src/stm32$(MCU_SERIES)xx_,\
	hal_ltdc.c \
	hal_ltdc_ex.c \
	hal_dma2d.c \
	)
