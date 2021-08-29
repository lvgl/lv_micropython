#define MICROPY_HW_BOARD_NAME           "MicroBot Mainboard"        //开发板名子自己随意
#define MICROPY_HW_MCU_NAME             "STM32H743"
#define MICROPY_HW_FLASH_FS_LABEL	    "MicroBot"

#define MICROPY_PY_STM                  (1)

#define MICROPY_HW_ENABLE_RTC           (1)
#define MICROPY_HW_ENABLE_RNG           (1)
#define MICROPY_HW_ENABLE_TIMER         (1)
#define MICROPY_HW_ENABLE_ADC           (1)
#define MICROPY_HW_ENABLE_DAC           (0)
#define MICROPY_HW_ENABLE_USB           (1)
#define MICROPY_HW_ENABLE_SDCARD        (1)
#define MICROPY_HW_ENABLE_ADC		    (1)
#define MICROPY_HW_ENABLE_DCMI          (1)
#define MICROPY_HW_HAS_SWITCH           (0)
#define MICROPY_HW_HAS_FLASH            (1)
#define MICROPY_FATFS_EXFAT             (1)

//LVGL define
//#define MICROPY_PY_LVGL                 (1)

#define MICROPY_BOARD_EARLY_INIT        board_early_init
void board_early_init(void);

// The board has an 25MHz HSE, the following gives 480MHz CPU speed
#define MICROPY_HW_CLK_PLLM             (5) //配置成480M主频
#define MICROPY_HW_CLK_PLLN             (192)
#define MICROPY_HW_CLK_PLLP             (2)
#define MICROPY_HW_CLK_PLLQ             (4)
#define MICROPY_HW_CLK_PLLR             (2)

// The USB clock is set using PLL3 ，USB的主频必须是48M
#define MICROPY_HW_CLK_PLL3M            (5)
#define MICROPY_HW_CLK_PLL3N            (48)
#define MICROPY_HW_CLK_PLL3P            (2)
#define MICROPY_HW_CLK_PLL3Q            (5)
#define MICROPY_HW_CLK_PLL3R            (2)

// The pyboard has a 32kHz crystal for the RTC
#define MICROPY_HW_RTC_USE_LSE      	 (1)
#define MICROPY_HW_RTC_USE_US       	 (0)
#define MICROPY_HW_RTC_USE_CALOUT   	 (1)
// 4 wait states
#define MICROPY_HW_FLASH_LATENCY        FLASH_LATENCY_4

//===================================================================================
//@LS.org MicroBot define
//------------------------------------------------------------------------------------
#define MICROPY_HW_BOARD_MICROBOT 	(1) //MicroBot Mainboard

//------------------------------------------------------------------------------------
#define MICROPY_HW_HAS_CAMERA           (1)
//===================================================================================

//===================================================================================

#define MICROPY_PY_THREAD_GIL		(1)
#define MICROPY_PY_THREAD		(1)

#define MICROPY_HW_UART_REPL           PYB_UART_3
#define MICROPY_HW_UART_REPL_BAUD      115200

// UART config
#define MICROPY_HW_UART6_TX            (pin_G14)
#define MICROPY_HW_UART6_RX            (pin_C7)
#define MICROPY_HW_UART7_TX            (pin_F7)
#define MICROPY_HW_UART7_RX            (pin_F6)

// I2C busses
#define MICROPY_HW_I2C3_SCL            (pin_H7)
#define MICROPY_HW_I2C3_SDA            (pin_H8)
#define MICROPY_HW_I2C4_SCL            (pin_D12)
#define MICROPY_HW_I2C4_SDA            (pin_H12)

// LEDs
#define MICROPY_HW_LED1                (pin_C13) // green
#define MICROPY_HW_LED2                (pin_B5) // blue
#define MICROPY_HW_LED3                (pin_D4) // blue
#define MICROPY_HW_LED_OFF(pin)        (mp_hal_pin_high(pin))
#define MICROPY_HW_LED_ON(pin)         (mp_hal_pin_low(pin))

// USB config
#define MICROPY_HW_USB_FS              (1)
#define MICROPY_HW_USB_VBUS_DETECT_PIN (pin_A9)
#define MICROPY_HW_USB_OTG_ID_PIN      (pin_A10)

// SD card detect switch
#define MICROPY_HW_SDMMC1_CK           (pin_C12)
#define MICROPY_HW_SDMMC1_CMD          (pin_D2)
#define MICROPY_HW_SDMMC1_D0           (pin_C8)
#define MICROPY_HW_SDMMC1_D1           (pin_C9)
#define MICROPY_HW_SDMMC1_D2           (pin_C10)
#define MICROPY_HW_SDMMC1_D3           (pin_C11)
//#define MICROPY_HW_SDCARD_DETECT_PIN (pin_G2)
//#define MICROPY_HW_SDCARD_DETECT_PULL (GPIO_PULLUP)
//#define MICROPY_HW_SDCARD_DETECT_PRESENT (GPIO_PIN_RESET)

// QSPI Flash 32MBits
#define MICROPY_HW_SPIFLASH_SIZE_BITS  (32 * 1024 * 1024)
#define MICROPY_HW_QSPIFLASH_SIZE_BITS_LOG2 (28)
#define MICROPY_HW_QSPIFLASH_CS        (pin_B10)
#define MICROPY_HW_QSPIFLASH_SCK       (pin_B2)
#define MICROPY_HW_QSPIFLASH_IO0       (pin_F8)
#define MICROPY_HW_QSPIFLASH_IO1       (pin_F9)
#define MICROPY_HW_QSPIFLASH_IO2       (pin_E2)
#define MICROPY_HW_QSPIFLASH_IO3       (pin_D13)

// SPI flash, block device config (when used as the filesystem)
extern const struct _mp_spiflash_config_t spiflash_config;
extern struct _spi_bdev_t               spi_bdev;
#if !USE_QSPI_XIP
#define MICROPY_HW_ENABLE_INTERNAL_FLASH_STORAGE (0)
#define MICROPY_HW_SPIFLASH_ENABLE_CACHE (1)
#define MICROPY_HW_BDEV_IOCTL(op, arg) ( \
    (op) == BDEV_IOCTL_NUM_BLOCKS ? ((1 << MICROPY_HW_QSPIFLASH_SIZE_BITS_LOG2) / 8 / FLASH_BLOCK_SIZE) : \
    (op) == BDEV_IOCTL_INIT ? spi_bdev_ioctl(&spi_bdev, (op), (uint32_t)&spiflash_config) : \
    spi_bdev_ioctl(&spi_bdev, (op), (arg)) \
)
#define MICROPY_HW_BDEV_READBLOCKS(dest, bl, n) spi_bdev_readblocks(&spi_bdev, (dest), (bl), (n))
#define MICROPY_HW_BDEV_WRITEBLOCKS(src, bl, n) spi_bdev_writeblocks(&spi_bdev, (src), (bl), (n))
#define MICROPY_HW_BDEV_SPIFLASH_EXTENDED (&spi_bdev) // for extended block protocol
#endif

// Use external SDRAM
#define MICROPY_HW_SDRAM_SIZE           (32 * 1024 * 1024)
#define MICROPY_HW_SDRAM_STARTUP_TEST   (1)
#define MICROPY_HEAP_START              sdram_start()
#define MICROPY_HEAP_END                sdram_end()
// Timing configuration for 240MHz/2=120MHz (8.3ns)
#define MICROPY_HW_SDRAM_CLOCK_PERIOD   2
#define MICROPY_HW_SDRAM_CAS_LATENCY    2
#define MICROPY_HW_SDRAM_FREQUENCY      (120000) // 120 MHz
#define MICROPY_HW_SDRAM_TIMING_TMRD    (2)
#define MICROPY_HW_SDRAM_TIMING_TXSR    (8)
#define MICROPY_HW_SDRAM_TIMING_TRAS    (6)
#define MICROPY_HW_SDRAM_TIMING_TRC     (6)
#define MICROPY_HW_SDRAM_TIMING_TWR     (2)
#define MICROPY_HW_SDRAM_TIMING_TRP     (2)
#define MICROPY_HW_SDRAM_TIMING_TRCD    (2)

// 16-bit SDRAM
#define MICROPY_HW_SDRAM_ROW_BITS_NUM   13
#define MICROPY_HW_SDRAM_MEM_BUS_WIDTH  16
#define MICROPY_HW_SDRAM_REFRESH_CYCLES 8192
#define MICROPY_HW_SDRAM_COLUMN_BITS_NUM 9
#define MICROPY_HW_SDRAM_INTERN_BANKS_NUM 4
#define MICROPY_HW_SDRAM_RPIPE_DELAY    1
#define MICROPY_HW_SDRAM_RBURST         (1)
#define MICROPY_HW_SDRAM_WRITE_PROTECTION (0)
#define MICROPY_HW_SDRAM_AUTOREFRESH_NUM (8)
#define MICROPY_HW_SDRAM_BURST_LENGTH   1
#define MICROPY_HW_SDRAM_REFRESH_RATE   (64) // ms

#define MICROPY_HW_FMC_SDCKE0           (pin_C3)
#define MICROPY_HW_FMC_SDNE0            (pin_C2)
#define MICROPY_HW_FMC_SDCLK            (pin_G8)
#define MICROPY_HW_FMC_SDNCAS           (pin_G15)
#define MICROPY_HW_FMC_SDNRAS           (pin_F11)
#define MICROPY_HW_FMC_SDNWE            (pin_A7)
#define MICROPY_HW_FMC_BA0              (pin_G4)
#define MICROPY_HW_FMC_BA1              (pin_G5)
#define MICROPY_HW_FMC_NBL0             (pin_E0)
#define MICROPY_HW_FMC_NBL1             (pin_E1)
#define MICROPY_HW_FMC_A0               (pin_F0)
#define MICROPY_HW_FMC_A1               (pin_F1)
#define MICROPY_HW_FMC_A2               (pin_F2)
#define MICROPY_HW_FMC_A3               (pin_F3)
#define MICROPY_HW_FMC_A4               (pin_F4)
#define MICROPY_HW_FMC_A5               (pin_F5)
#define MICROPY_HW_FMC_A6               (pin_F12)
#define MICROPY_HW_FMC_A7               (pin_F13)
#define MICROPY_HW_FMC_A8               (pin_F14)
#define MICROPY_HW_FMC_A9               (pin_F15)
#define MICROPY_HW_FMC_A10              (pin_G0)
#define MICROPY_HW_FMC_A11              (pin_G1)
#define MICROPY_HW_FMC_A12              (pin_G2)
#define MICROPY_HW_FMC_D0               (pin_D14)
#define MICROPY_HW_FMC_D1               (pin_D15)
#define MICROPY_HW_FMC_D2               (pin_D0)
#define MICROPY_HW_FMC_D3               (pin_D1)
#define MICROPY_HW_FMC_D4               (pin_E7)
#define MICROPY_HW_FMC_D5               (pin_E8)
#define MICROPY_HW_FMC_D6               (pin_E9)
#define MICROPY_HW_FMC_D7               (pin_E10)
#define MICROPY_HW_FMC_D8               (pin_E11)
#define MICROPY_HW_FMC_D9               (pin_E12)
#define MICROPY_HW_FMC_D10              (pin_E13)
#define MICROPY_HW_FMC_D11              (pin_E14)
#define MICROPY_HW_FMC_D12              (pin_E15)
#define MICROPY_HW_FMC_D13              (pin_D8)
#define MICROPY_HW_FMC_D14              (pin_D9)
#define MICROPY_HW_FMC_D15              (pin_D10)

/*
#if MICROPY_HW_RGBLCD
#define MICROPY_HW_LCD_BL               (pin_C7)
#define MICROPY_HW_LCD_VSYNC   	        (pin_I9)
#define MICROPY_HW_LCD_HSYNC            (pin_I10)
#define MICROPY_HW_LCD_DOTCLK           (pin_G7)
#define MICROPY_HW_LCD_DE               (pin_F10)
#define MICROPY_HW_LCD_RESET            (pin_I1)
#define MICROPY_HW_LCD_CS               (pin_H2)
#define MICROPY_HW_LCD_SCK              (pin_H3)
#define MICROPY_HW_LCD_MOSI             (pin_I5)
#define MICROPY_HW_LCD_MISO             (pin_B4)
#define MICROPY_HW_LCD_R0               (pin_G13)
#define MICROPY_HW_LCD_R1               (pin_A2)
#define MICROPY_HW_LCD_R2               (pin_A1)
#define MICROPY_HW_LCD_R3               (pin_B0)
#define MICROPY_HW_LCD_R4               (pin_A5)
#define MICROPY_HW_LCD_R5               (pin_C0)
#define MICROPY_HW_LCD_R6               (pin_B1)
#define MICROPY_HW_LCD_R7               (pin_G6)
#define MICROPY_HW_LCD_G0               (pin_E5)
#define MICROPY_HW_LCD_G1               (pin_E6)
#define MICROPY_HW_LCD_G2               (pin_H13)
#define MICROPY_HW_LCD_G3               (pin_G10)
#define MICROPY_HW_LCD_G4               (pin_H15)
#define MICROPY_HW_LCD_G5               (pin_H4)
#define MICROPY_HW_LCD_G6               (pin_I11)
#define MICROPY_HW_LCD_G7               (pin_I2)
#define MICROPY_HW_LCD_B0               (pin_G14)
#define MICROPY_HW_LCD_B1               (pin_G12)
#define MICROPY_HW_LCD_B2               (pin_D6)
#define MICROPY_HW_LCD_B3               (pin_A8)
#define MICROPY_HW_LCD_B4               (pin_I4)
#define MICROPY_HW_LCD_B5               (pin_A3)
#define MICROPY_HW_LCD_B6               (pin_B8)
#define MICROPY_HW_LCD_B7               (pin_I7)
#endif
*/
