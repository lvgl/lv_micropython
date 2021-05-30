/* Includes ------------------------------------------------------------------*/
#include "microbot_led.h"

void BSP_LED_GPIO_Init(void)
{
    GPIO_InitTypeDef gpio_init_structure;

    BSP_LED_BLUE_GPIO_CLK_ENABLE() ;
    BSP_LED_GREEN_GPIO_CLK_ENABLE() ;
    BSP_LED_RED_GPIO_CLK_ENABLE() ;

    gpio_init_structure.Pin = BSP_LED_BLUE_PIN;
    gpio_init_structure.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_structure.Pull = GPIO_PULLUP;
    gpio_init_structure.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(BSP_LED_BLUE_GPIO_PORT, &gpio_init_structure);

    gpio_init_structure.Pin = BSP_LED_GREEN_PIN;
    gpio_init_structure.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_structure.Pull = GPIO_PULLUP;
    gpio_init_structure.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(BSP_LED_GREEN_GPIO_PORT, &gpio_init_structure);

    gpio_init_structure.Pin = BSP_LED_RED_PIN;
    gpio_init_structure.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_structure.Pull = GPIO_PULLUP;
    gpio_init_structure.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(BSP_LED_RED_GPIO_PORT, &gpio_init_structure);

    BSP_LED_BLUE(0);
    BSP_LED_RED(0);
    BSP_LED_GREEN(0);
}

void BSP_LED_GPIO_DeInit(void)
{
    BSP_LED_BLUE_GPIO_CLK_DISABLE() ;
    BSP_LED_GREEN_GPIO_CLK_DISABLE() ;
    BSP_LED_RED_GPIO_CLK_DISABLE() ;
}
