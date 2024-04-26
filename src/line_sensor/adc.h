/**
 * @file adc.h
 * @author cinnamondev
 * @brief Line Sensor ADC functions
 * 
 */

#ifndef LS_ADC_H
#define LS_ADC_H
/**
 * @file adc.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-04-19
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <stddef.h>
#include <stdint.h>

#include "stm32g0xx.h"
#include "stm32g0xx_hal_adc.h"

void ls_adc_init(ADC_HandleTypeDef* _hadc);
void ls_adc_start(void);
void ls_adc_stop(void);
float ls_adc_wait_for(void);
uint32_t* ls_adc_get_raw(void);
void ls_adc_resume(void);
int ls_adc_running(void);
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *_hadc);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *_hadc);
#endif // LS_ADC_H