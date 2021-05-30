/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32H743_MICROBOT_LEDs_H
#define STM32H743_MICROBOT_LEDs_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

#define BSP_LED_BLUE_PIN                           GPIO_PIN_4
#define BSP_LED_BLUE_GPIO_PORT                     GPIOD
#define BSP_LED_BLUE_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOD_CLK_ENABLE()
#define BSP_LED_BLUE_GPIO_CLK_DISABLE()            __HAL_RCC_GPIOD_CLK_DISABLE()

#define BSP_LED_GREEN_PIN                           GPIO_PIN_5
#define BSP_LED_GREEN_GPIO_PORT                     GPIOB
#define BSP_LED_GREEN_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOB_CLK_ENABLE()
#define BSP_LED_GREEN_GPIO_CLK_DISABLE()            __HAL_RCC_GPIOB_CLK_DISABLE()

#define BSP_LED_RED_PIN                           GPIO_PIN_13
#define BSP_LED_RED_GPIO_PORT                     GPIOC
#define BSP_LED_RED_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOC_CLK_ENABLE()
#define BSP_LED_RED_GPIO_CLK_DISABLE()            __HAL_RCC_GPIOC_CLK_DISABLE()

#define BSP_LED_BLUE(x)  				        ((x) == 0 ? HAL_GPIO_WritePin(BSP_LED_BLUE_GPIO_PORT, BSP_LED_BLUE_PIN, GPIO_PIN_SET) : HAL_GPIO_WritePin(BSP_LED_BLUE_GPIO_PORT, BSP_LED_BLUE_PIN, GPIO_PIN_RESET))
#define BSP_LED_GREEN(x)  				        ((x) == 0 ? HAL_GPIO_WritePin(BSP_LED_GREEN_GPIO_PORT, BSP_LED_GREEN_PIN, GPIO_PIN_SET) : HAL_GPIO_WritePin(BSP_LED_GREEN_GPIO_PORT, BSP_LED_GREEN_PIN, GPIO_PIN_RESET))
#define BSP_LED_RED(x)  				        ((x) == 0 ? HAL_GPIO_WritePin(BSP_LED_RED_GPIO_PORT, BSP_LED_RED_PIN, GPIO_PIN_SET) : HAL_GPIO_WritePin(BSP_LED_RED_GPIO_PORT, BSP_LED_RED_PIN, GPIO_PIN_RESET))

void BSP_LED_GPIO_Init(void);
void BSP_LED_GPIO_DeInit(void);

#ifdef __cplusplus
}
#endif

#endif /* STM32H743_MICROBOT_LEDs_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/