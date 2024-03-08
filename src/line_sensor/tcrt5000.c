#include "tcrt5000.h"
#include "stm32g030xx.h"
#include "stm32g0xx_hal.h"
#include "stm32g0xx_hal_gpio.h"

void tcrt_init(struct tcrt_channel *s, GPIO_TypeDef* adc_port, uint16_t adc_pin, GPIO_TypeDef* en_port, uint16_t en_pin) {
  s->adc_port = adc_port;
  s->adc_pin = adc_pin;
  s->en_port = en_port;
  s->en_pin = en_pin;
}
