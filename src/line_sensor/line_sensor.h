/**
 * @file line_sensor.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef __LINE_SENSOR_ADC_H
#define __LINE_SENSOR_ADC_H

#include <stdint.h>
#include "i2c.h"
#include "stm32g0xx_hal_adc.h"
#include "stm32g0xx_hal_i2c.h"
#include "tcrt5000.h"

#ifndef LINE_SENSOR_N
#define LINE_SENSOR_N 6
#endif

/** Line sensor structure initialization macro */
#define LINE_SENSOR(_hi2c, _hadc, ...) (struct line_sensor) {\
  .tcrt = { __VA_ARGS__ },\
  .hi2c = _hi2c,\
  .hadc = _hadc,\
}\

/**
 * @brief Line Sensor Module IO Ports, Interfaces & Outputs.
 * 
 */
struct line_sensor {
    /** Each TCRT + ULN GPIO enable port/pin pairs 
      * Initialization should take place in rank order (see ADC init).
      * @note Reccomended order for 2.1a Target: S3,S2,S1,S6,S5,S4
      */
    struct tcrt_channel tcrt[LINE_SENSOR_N];
    uint32_t buff[LINE_SENSOR_N]; /**< DMA Sample buffer*/
    I2C_TypeDef* hi2c; /**< I2C peripheral */
    ADC_TypeDef* hadc; /**< ADC peripheral*/
    /** Processed line output */
    float pos;
};

void ls_init(struct line_sensor*, ADC_HandleTypeDef*);
void ls_start(void);
void ls_stop(void);
_Bool ls_ready_to_process(void);
void ls_resume(void);

void  HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef*);
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef*);
#endif
