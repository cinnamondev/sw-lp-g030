/**
 * @file i2c.h
 * @author cinnamondev
 * @brief Line Sensor Module I2C Functionality
 * 
 */

#ifndef LS_I2C_H
#define LS_I2C_H

#include "stm32g0xx.h"
#include "stm32g0xx_hal_i2c.h"

void ls_i2c_init(I2C_HandleTypeDef* _hi2c);

void ls_i2c_start(void);

void ls_i2c_stop(void);

void ls_i2c_buff_update_blk(float* new);

void ls_i2c_tdb_buff_update_blk(uint32_t* new);

void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *_hi2c);

void HAL_I2C_AddrCallback(I2C_HandleTypeDef *_hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode);

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c);


#endif // LS_I2C_H
