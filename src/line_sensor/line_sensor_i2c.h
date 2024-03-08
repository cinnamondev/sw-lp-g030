/**
 * @file line_sensor_i2c.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef __LINE_SENSOR_I2C_H
#define __LINE_SENSOR_I2C_H

#include "stm32g0xx.h"
#include "stm32g0xx_hal_i2c.h"

/**
 * @brief Initializes the I2C instance and sets the TX buffer.
 * 
 * @param instance I2C Instance (I2X, for G030 board, use I2C1)
 * @param _buf Pointer to buffer to use. Define buffer size using macro
 * `LP_I2C_CONF_TX_BUF`
 *
 * @warning It is reccomended to check the result of `lp_i2c_buffer_occupied`
 * prior to writing to the buffer, as the I2C may be in the middle of a transfer.
 * @return I2C_HandleTypeDef* 
 */
I2C_HandleTypeDef* lp_i2c_init(I2C_TypeDef* instance, void* _buf);

/**
 * @brief Set the transmit buffer (where the resulting line position is stored)
 * 
 * @param buf Buffer (of size `LV_I2C_CONF_TX_BUF`)
 */
void lp_i2c_set_tx(void* buf);

/**
 * @brief Returns non-zero if the buffer is being used in a transfer currently.
 * 
 * @return _Bool Non-zero if occupied in transfer.
 */
_Bool lp_i2c_buffer_occupied(void);

/**
 * @brief Begin listening for matching I2C address.
 * 
 */
void lp_i2c_start(void);

/**
 * @brief Stop listening for matching I2C address.
 * 
 */
void lp_i2c_stop(void);


#endif // __LINE_SENSOR_I2C_H
