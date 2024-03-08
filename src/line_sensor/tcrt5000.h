#include "stm32g030xx.h"
#include "stm32g0xx_hal.h"
#include "stm32g0xx_hal_gpio.h"

#ifndef TCRT5000_H
#define TCRT5000_H

#define SENSOR_TOGGLE(ch) HAL_GPIO_TogglePin(ch.en_port, ch.en_pin)
#define SENSOR_ENABLE(ch) HAL_GPIO_WritePin(ch.en_port, ch.en_pin, GPIO_PIN_SET)
#define SENSOR_DISABLE(ch) HAL_GPIO_WritePin(ch.en_port, ch.en_pin, GPIO_PIN_RESET)

struct tcrt_channel {
    GPIO_TypeDef* adc_port;
    uint16_t adc_pin;
    GPIO_TypeDef* en_port;
    uint16_t en_pin;
};

void tcrt_init(struct tcrt_channel *s, GPIO_TypeDef* adc_port, uint16_t adc_pin, GPIO_TypeDef* en_port, uint16_t en_pin);

#endif // TCRT5000_H
