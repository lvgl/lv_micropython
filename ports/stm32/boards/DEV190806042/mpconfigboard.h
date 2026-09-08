#define MICROPY_HW_BOARD_NAME       "DEV190806042"
#define MICROPY_HW_MCU_NAME         "STM32H743"

#define MICROPY_HW_ENABLE_RTC       (1)
#define MICROPY_HW_ENABLE_SERVO     (1)
#define MICROPY_HW_ENABLE_RNG       (1)
#define MICROPY_HW_ENABLE_HW_I2C    (1)
#define MICROPY_HW_ENABLE_ADC       (1)
#define MICROPY_HW_ENABLE_DAC       (1)
#define MICROPY_HW_ENABLE_USB       (1)
#define MICROPY_HW_USB_HID          (1)
#define MICROPY_HW_ENABLE_SDCARD    (0)
#define MICROPY_HW_HAS_FLASH        (1)
#define MICROPY_HW_ENABLE_STORAGE   (1)

#define MODULE_ST_LTDC_ENABLED      (1)
#define MODULE_TOUCH_I2C_ENABLED    (1)

// CAUTION: Do not enable! A bug here creates incorrect Option Bytes,
// potentially bricking the device.
#define MICROPY_HW_ENABLE_INTERNAL_FLASH_STORAGE (0)

#define MICROPY_HW_ENTER_BOOTLOADER_VIA_RESET   (0)
#define MICROPY_HW_SDCARD_MOUNT_AT_BOOT         (0)

#if defined(MODULE_ST_LTDC_ENABLED) && MODULE_ST_LTDC_ENABLED
extern void lv_deinit(void);
#define MICROPY_BOARD_START_SOFT_RESET(state) do { boardctrl_start_soft_reset(state); lv_deinit(); } while(0)
#endif

// The board has an 25MHz HSE, the following gives 480MHz CPU speed
#define MICROPY_HW_CLK_PLLM         (5)
#define MICROPY_HW_CLK_PLLN         (192)
#define MICROPY_HW_CLK_PLLP         (2)
#define MICROPY_HW_CLK_PLLQ         (2)
#define MICROPY_HW_CLK_PLLR         (2)
#define MICROPY_HW_CLK_PLLVCI       (RCC_PLL1VCIRANGE_2)
#define MICROPY_HW_CLK_PLLVCO       (RCC_PLL1VCOWIDE)
#define MICROPY_HW_CLK_PLLFRAC      (0)

// Bus clock divider values
#define MICROPY_HW_CLK_AHB_DIV      (RCC_HCLK_DIV2)
#define MICROPY_HW_CLK_APB1_DIV     (RCC_APB1_DIV2)
#define MICROPY_HW_CLK_APB2_DIV     (RCC_APB2_DIV2)
#define MICROPY_HW_CLK_APB3_DIV     (RCC_APB3_DIV2)
#define MICROPY_HW_CLK_APB4_DIV     (RCC_APB4_DIV2)

// 4 wait states
#define MICROPY_HW_FLASH_LATENCY    FLASH_LATENCY_4

// UART config
#define MICROPY_HW_UART1_TX         (pin_A9)
#define MICROPY_HW_UART1_RX         (pin_A10)
#define MICROPY_HW_UART_REPL        PYB_UART_1
#define MICROPY_HW_UART_REPL_BAUD   115200

// FLASH QSPI config - W25Q256
#define MICROPY_HW_QSPIFLASH_SIZE_BITS_LOG2  (28)
#define MICROPY_HW_QSPIFLASH_CS              (pin_G6)
#define MICROPY_HW_QSPIFLASH_SCK             (pin_F10)
#define MICROPY_HW_QSPIFLASH_IO0             (pin_F8)
#define MICROPY_HW_QSPIFLASH_IO1             (pin_F9)
#define MICROPY_HW_QSPIFLASH_IO2             (pin_F7)
#define MICROPY_HW_QSPIFLASH_IO3             (pin_F6)

// Block device config for SPI flash
extern const struct _mp_spiflash_config_t spiflash_config;
extern struct _spi_bdev_t spi_bdev;
#define MICROPY_HW_SPIFLASH_ENABLE_CACHE    (1)
#define MICROPY_HW_BDEV_SPIFLASH            (&spi_bdev)
#define MICROPY_HW_BDEV_SPIFLASH_CONFIG     (&spiflash_config)
#define MICROPY_HW_SPIFLASH_SIZE_BITS       (256 * 1024 * 1024)
#define MICROPY_HW_BDEV_SPIFLASH_SIZE_BYTES (MICROPY_HW_SPIFLASH_SIZE_BITS / 8)
#define MICROPY_HW_QSPI_PRESCALER           (2)
#define MICROPY_HW_BDEV_SPIFLASH_EXTENDED   (&spi_bdev)

// USB config
// Enable the HSI48 oscillator and route it to USB
#define MICROPY_HW_RCC_HSI48_STATE      (RCC_HSI48_ON)
#define MICROPY_HW_RCC_USB_CLKSOURCE    (RCC_USBCLKSOURCE_HSI48)

#define MICROPY_HW_USB_FS           (1)
#define MICROPY_HW_USB_HS           (0)
#define MICROPY_HW_USB_HS_IN_FS     (0)
#define MICROPY_HW_USB_MAIN_DEV     (USB_PHY_FS_ID)
#define MICROPY_HW_USB_CDC_NUM      (2)
#define MICROPY_HW_USB_MSC          (1)

#define MICROPY_HW_USB_PRODUCT_FS_STRING          "STM32H7 Virtual Comm Port in FS Mode"
#define MICROPY_HW_USB_INTERFACE_FS_STRING        "STM32H7 Interface"
#define MICROPY_HW_USB_MSC_INQUIRY_PRODUCT_STRING "STM32H7 Flash   "

#define MICROPY_HW_LED1             (pin_B0)
#define MICROPY_HW_LED_ON(pin)      (mp_hal_pin_low(pin))
#define MICROPY_HW_LED_OFF(pin)     (mp_hal_pin_high(pin))

#define MICROPY_HW_USRSW_PIN        (pin_A0)
#define MICROPY_HW_HAS_SWITCH       (1)
// USRSW is pulled low. Pressing the button makes the input go high.
#define MICROPY_HW_USRSW_PULL       (GPIO_NOPULL)
#define MICROPY_HW_USRSW_EXTI_MODE  (GPIO_MODE_IT_RISING)
#define MICROPY_HW_USRSW_PRESSED    (1)

// I2C buses
#define MICROPY_HW_I2C4_SCL         (pin_D12)
#define MICROPY_HW_I2C4_SDA         (pin_D13)

// SDRAM
#define MICROPY_HW_SDRAM_SIZE               (32 * 1024 * 1024)
#define MICROPY_HW_SDRAM_STARTUP_TEST       (1)
#define MICROPY_HW_SDRAM_TEST_FAIL_ON_ERROR (1)
#define MICROPY_HEAP_START  ((sdram_valid) ? sdram_start() : &_heap_start)
#define MICROPY_HEAP_END    ((sdram_valid) ? sdram_end() : &_heap_end)

// Timing configuration for SDRAM @120Mhz
#define MICROPY_HW_SDRAM_TIMING_TMRD        (2)
#define MICROPY_HW_SDRAM_TIMING_TXSR        (9)
#define MICROPY_HW_SDRAM_TIMING_TRAS        (6)
#define MICROPY_HW_SDRAM_TIMING_TRC         (8)
#define MICROPY_HW_SDRAM_TIMING_TWR         (2)
#define MICROPY_HW_SDRAM_TIMING_TRP         (3)
#define MICROPY_HW_SDRAM_TIMING_TRCD        (3)
#define MICROPY_HW_SDRAM_REFRESH_RATE       (64)
// Required by upstream micropython SDRAM driver;
// harmless on lv_micropython until sync.
#define MICROPY_HW_SDRAM_FREQUENCY_KHZ      (120000) // 120 MHz
#define MICROPY_HW_SDRAM_REFRESH_CYCLES     8192

#define MICROPY_HW_SDRAM_BURST_LENGTH       8
#define MICROPY_HW_SDRAM_CAS_LATENCY        2
#define MICROPY_HW_SDRAM_COLUMN_BITS_NUM    9
#define MICROPY_HW_SDRAM_ROW_BITS_NUM       12
#define MICROPY_HW_SDRAM_MEM_BUS_WIDTH      32
#define MICROPY_HW_SDRAM_INTERN_BANKS_NUM   4
#define MICROPY_HW_SDRAM_CLOCK_PERIOD       2
#define MICROPY_HW_SDRAM_RPIPE_DELAY        1
#define MICROPY_HW_SDRAM_RBURST             (1)
#define MICROPY_HW_SDRAM_WRITE_PROTECTION   (0)
#define MICROPY_HW_SDRAM_AUTOREFRESH_NUM    (4)

#define MICROPY_HW_FMC_SDNE0        (pin_C2)
#define MICROPY_HW_FMC_SDCKE0       (pin_C3)
#define MICROPY_HW_FMC_SDNWE        (pin_C0)
#define MICROPY_HW_FMC_SDNRAS       (pin_F11)
#define MICROPY_HW_FMC_SDNCAS       (pin_G15)
#define MICROPY_HW_FMC_SDCLK        (pin_G8)
#define MICROPY_HW_FMC_BA0          (pin_G4)
#define MICROPY_HW_FMC_BA1          (pin_G5)
#define MICROPY_HW_FMC_NBL0         (pin_E0)
#define MICROPY_HW_FMC_NBL1         (pin_E1)
#define MICROPY_HW_FMC_NBL2         (pin_I4)
#define MICROPY_HW_FMC_NBL3         (pin_I5)
#define MICROPY_HW_FMC_A0           (pin_F0)
#define MICROPY_HW_FMC_A1           (pin_F1)
#define MICROPY_HW_FMC_A2           (pin_F2)
#define MICROPY_HW_FMC_A3           (pin_F3)
#define MICROPY_HW_FMC_A4           (pin_F4)
#define MICROPY_HW_FMC_A5           (pin_F5)
#define MICROPY_HW_FMC_A6           (pin_F12)
#define MICROPY_HW_FMC_A7           (pin_F13)
#define MICROPY_HW_FMC_A8           (pin_F14)
#define MICROPY_HW_FMC_A9           (pin_F15)
#define MICROPY_HW_FMC_A10          (pin_G0)
#define MICROPY_HW_FMC_A11          (pin_G1)
#define MICROPY_HW_FMC_D0           (pin_D14)
#define MICROPY_HW_FMC_D1           (pin_D15)
#define MICROPY_HW_FMC_D2           (pin_D0)
#define MICROPY_HW_FMC_D3           (pin_D1)
#define MICROPY_HW_FMC_D4           (pin_E7)
#define MICROPY_HW_FMC_D5           (pin_E8)
#define MICROPY_HW_FMC_D6           (pin_E9)
#define MICROPY_HW_FMC_D7           (pin_E10)
#define MICROPY_HW_FMC_D8           (pin_E11)
#define MICROPY_HW_FMC_D9           (pin_E12)
#define MICROPY_HW_FMC_D10          (pin_E13)
#define MICROPY_HW_FMC_D11          (pin_E14)
#define MICROPY_HW_FMC_D12          (pin_E15)
#define MICROPY_HW_FMC_D13          (pin_D8)
#define MICROPY_HW_FMC_D14          (pin_D9)
#define MICROPY_HW_FMC_D15          (pin_D10)
#define MICROPY_HW_FMC_D16          (pin_H8)
#define MICROPY_HW_FMC_D17          (pin_H9)
#define MICROPY_HW_FMC_D18          (pin_H10)
#define MICROPY_HW_FMC_D19          (pin_H11)
#define MICROPY_HW_FMC_D20          (pin_H12)
#define MICROPY_HW_FMC_D21          (pin_H13)
#define MICROPY_HW_FMC_D22          (pin_H14)
#define MICROPY_HW_FMC_D23          (pin_H15)
#define MICROPY_HW_FMC_D24          (pin_I0)
#define MICROPY_HW_FMC_D25          (pin_I1)
#define MICROPY_HW_FMC_D26          (pin_I2)
#define MICROPY_HW_FMC_D27          (pin_I3)
#define MICROPY_HW_FMC_D28          (pin_I6)
#define MICROPY_HW_FMC_D29          (pin_I7)
#define MICROPY_HW_FMC_D30          (pin_I9)
#define MICROPY_HW_FMC_D31          (pin_I10)
