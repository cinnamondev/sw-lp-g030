/**
 * @file line_sensor_i2c.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-03-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "line_sensor_i2c.h"

#include <stdint.h>
#include "stm32g0xx.h"
#include "stm32g0xx_hal_i2c.h"

#ifndef LP_I2C_TX_BUF_SIZE
#define LP_I2C_TX_BUF_SIZE 6
#endif 
static void I2C_init(I2C_TypeDef* i2c);
static void Error_Handler(void);

static I2C_HandleTypeDef* hi2c;
static uint8_t* buf;
static volatile _Bool tx_active = 0;
static volatile uint8_t latest_cmd; /** Recieve buffer (idk if we will use this in reality? maybe set delays for demo modes.)*/

I2C_HandleTypeDef* lp_i2c_init(I2C_TypeDef* instance, void* _buf) {
  I2C_init(instance);
  buf = _buf;
  return hi2c;  
}
void lp_i2c_set_tx(void* _buf) {
    buf = _buf;
}

_Bool lp_i2c_buffer_occupied(void) {
  return tx_active;
}

void lp_i2c_start() {
    HAL_I2C_EnableListen_IT(hi2c);
}

void lp_i2c_stop() {
    HAL_I2C_DisableListen_IT(hi2c);
}

void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *_hi2c) {
  if (_hi2c->Instance != hi2c->Instance) { return; }
  HAL_I2C_EnableListen_IT(_hi2c); // continue listen;
}

void HAL_I2C_AddrCallback(I2C_HandleTypeDef *_hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode) {
  if (_hi2c->Instance != hi2c->Instance) { return; }
  if (TransferDirection == I2C_DIRECTION_TRANSMIT) {
    HAL_I2C_Slave_Receive_IT(hi2c, &latest_cmd, 1); // unsure what to do with this if at all :)
  } else { // Send back latest sensor data
    tx_active = 1;
    HAL_I2C_Slave_Seq_Transmit_IT(hi2c, buf, 4, I2C_LAST_FRAME);
  } 
}

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *_hi2c) {
  if (_hi2c->Instance != hi2c->Instance) { return; }
  tx_active = 0;
}

static void I2C_init(I2C_TypeDef* instance)
{
  hi2c->Instance = instance;
  hi2c->Init.Timing = 0x10707DBC;
  hi2c->Init.OwnAddress1 = 66;
  hi2c->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c->Init.OwnAddress2 = 0;
  hi2c->Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(hi2c) != HAL_OK)
  {
    Error_Handler();
  }
  // Analog Filter
  if (HAL_I2CEx_ConfigAnalogFilter(hi2c, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  // Digital Filter
  if (HAL_I2CEx_ConfigDigitalFilter(hi2c, 0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * @brief I2C Error Handler
 */
static void Error_Handler(void) {
  while(1); // spin
}