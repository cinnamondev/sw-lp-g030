/* USER CODE BEGIN Header */
/**
 * @file main.h
 * @author cinnamondev
 * @brief Common definitions (Pin labels)
 * 
 */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void); /**< HAL error handler*/

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define S6E_Pin GPIO_PIN_14
#define S6E_GPIO_Port GPIOC
#define S5E_Pin GPIO_PIN_15
#define S5E_GPIO_Port GPIOC
#define S4E_Pin GPIO_PIN_0
#define S4E_GPIO_Port GPIOA
#define S3E_Pin GPIO_PIN_1
#define S3E_GPIO_Port GPIOA
#define S2E_Pin GPIO_PIN_2
#define S2E_GPIO_Port GPIOA
#define S1E_Pin GPIO_PIN_3
#define S1E_GPIO_Port GPIOA
#define INTX_Pin GPIO_PIN_4
#define INTX_GPIO_Port GPIOA
#define S3_Pin GPIO_PIN_5
#define S3_GPIO_Port GPIOA
#define S2_Pin GPIO_PIN_6
#define S2_GPIO_Port GPIOA
#define S1_Pin GPIO_PIN_7
#define S1_GPIO_Port GPIOA
#define S6_Pin GPIO_PIN_1
#define S6_GPIO_Port GPIOB
#define S5_Pin GPIO_PIN_11
#define S5_GPIO_Port GPIOA
#define S4_Pin GPIO_PIN_12
#define S4_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif // MAIN_H

