#include "stm32g030xx.h"
#include "stm32g0xx_hal.h"
#include "stm32g0xx_hal_gpio.h"

#ifndef TCRT5000_H
#define TCRT5000_H

/** Toggle sensor on/off irrespective of state */
#define SENSOR_TOGGLE(ch) HAL_GPIO_TogglePin(ch.en_port, ch.en_pin)
/** Enable sensor irrespective of state */
#define SENSOR_ENABLE(ch) HAL_GPIO_WritePin(ch.en_port, ch.en_pin, GPIO_PIN_SET)
/** Disable sensor irrespective of state */
#define SENSOR_DISABLE(ch) HAL_GPIO_WritePin(ch.en_port, ch.en_pin, GPIO_PIN_RESET)

/**
 * @brief Generic TCRT5000 GPIO interface
 * 
 * Use alongside macros defined in tcrt5000.h
 */
struct tcrt_channel {
    GPIO_TypeDef* adc_port;
    uint16_t adc_pin;
    GPIO_TypeDef* en_port;
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
void tcrt_init(struct tcrt_channel *s, GPIO_TypeDef* adc_port, uint16_t adc_pin, GPIO_TypeDef* en_port, uint16_t en_pin);

#endif // TCRT5000_H
