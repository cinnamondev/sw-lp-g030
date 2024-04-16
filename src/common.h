/**
 * @file common.h
 * @author cinnamondev
 * @brief Common definitions & macros
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Macros --------------------------------------------------------------------*/

/** TCRT Channel Quick Create Macro (n = corresponding sensor number)*/
#define S(n) (struct tcrt_channel) {\
  .adc_port = S## n ##_GPIO_Port, \
  .adc_pin = S ## n ## _Pin, \
  .en_port = S ## n ## E_GPIO_Port, \
  .en_pin = S ## n ## E_Pin, \
} \

#define TARGET_LSA_21A_ORDER S(3),S(2),S(1),S(6),S(5),S(4)

/* Private defines -----------------------------------------------------------*/

// Build defines 
#define DEMOB /**< Undefine DEMOB to remove sensor debug output */

// GPIO Defines

#define S6E_Pin GPIO_PIN_14 /**< Sensor 6 Enable Pin */
#define S6E_GPIO_Port GPIOC /**< Sensor 6 Enable Port */
#define S5E_Pin GPIO_PIN_15 /**< Sensor 5 Enable Pin */
#define S5E_GPIO_Port GPIOC /**< Sensor 5 Enable Port */
#define S4E_Pin GPIO_PIN_0  /**< Sensor 4 Enable Pin */
#define S4E_GPIO_Port GPIOA /**< Sensor 4 Enable Port */
#define S3E_Pin GPIO_PIN_1 /**< Sensor 3 Enable Pin */
#define S3E_GPIO_Port GPIOA /**< Sensor 3 Enable Port */
#define S2E_Pin GPIO_PIN_2 /**< Sensor 2 Enable Pin */
#define S2E_GPIO_Port GPIOA /**< Sensor 2 Enable Port */
#define S1E_Pin GPIO_PIN_3 /**< Sensor 1 Enable Pin */
#define S1E_GPIO_Port GPIOA /**< Sensor 1 Enable Port */
#define INTX_Pin GPIO_PIN_4 /**< Host interrupt pin */
#define INTX_GPIO_Port GPIOA /**< Host interrupt port */
#define S3_Pin GPIO_PIN_5 /**< Sensor 3 Analog Pin */
#define S3_GPIO_Port GPIOA /**< Sensor 3 Analog Port */
#define S2_Pin GPIO_PIN_6 /**< Sensor 2 Analog Pin */
#define S2_GPIO_Port GPIOA /**< Sensor 2 Analog Port */
#define S1_Pin GPIO_PIN_7 /**< Senor 1 Analog Pin  */
#define S1_GPIO_Port GPIOA /**< Sensor 1 Analog Port */
#define S6_Pin GPIO_PIN_1 /**< Sensor 6 Analog Pin */
#define S6_GPIO_Port GPIOB /**< Sensor 6 Analog Port */
#define S5_Pin GPIO_PIN_11 /**< Sensor 5 Analog Pin */
#define S5_GPIO_Port GPIOA /**< Sensor 5 Analog Port */
#define S4_Pin GPIO_PIN_12 /**< Senor 4 Analog Pin */
#define S4_GPIO_Port GPIOA /**< Sensor 4 Analog Port */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */