/**
 * @file tcrt5000.h
 * @author cinnamondev
 * @brief TCRT5000 Sensor Pinout descriptor & macros
 */

#ifndef TCRT5000_H
#define TCRT5000_H

#include "stm32g0xx.h"
#include "stm32g0xx_hal_gpio.h"

/** Toggle sensor on/off irrespective of state */
#define SENSOR_TOGGLE(ch) HAL_GPIO_TogglePin(ch.en_port, ch.en_pin)
/** Enable sensor irrespective of state */
#define SENSOR_ENABLE(ch) HAL_GPIO_WritePin(ch.en_port, ch.en_pin, GPIO_PIN_SET)
/** Disable sensor irrespective of state */
#define SENSOR_DISABLE(ch)                                                     \
  HAL_GPIO_WritePin(ch.en_port, ch.en_pin, GPIO_PIN_RESET)

/**
 * @brief TCRT Channel struct initialization macro
 * @note Relies on main.h sensor pin definitions (which shouldnt be changed
 * anyway)
 */
#define S(n) (struct tcrt_channel) {\
  .adc_port = S## n ##_GPIO_Port, \
  .adc_pin = S ## n ## _Pin, \
  .en_port = S ## n ## E_GPIO_Port, \
  .en_pin = S ## n ## E_Pin, \
} \


/**
 * @brief Generic TCRT5000 GPIO interface
 *
 * Use alongside macros defined in tcrt5000.h
 */
struct tcrt_channel {
  GPIO_TypeDef *adc_port;
  uint16_t adc_pin;
  GPIO_TypeDef *en_port;
  uint16_t en_pin;
};

/**
 * @brief Initialize generic tcrt5000 structure
 *
 * @param s Existing structure to populate
 * @param adc_port Port to use for analog pin
 * @param adc_pin Pin to use for analog pin
 * @param en_port Port to use for ENABLE pin (digital)
 * @param en_pin Pin to use for ENABLE pin (digital)
 */

void tcrt_init(struct tcrt_channel *s, GPIO_TypeDef *adc_port, uint16_t adc_pin,
               GPIO_TypeDef *en_port, uint16_t en_pin) {
  s->adc_port = adc_port;
  s->adc_pin = adc_pin;
  s->en_port = en_port;
  s->en_pin = en_pin;
}

#endif // TCRT5000_H
