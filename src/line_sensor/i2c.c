/**
 * @file i2c.c
 * @author cinnamondev
 * @brief Line Sensor Module I2C
 */

#include "i2c.h"

#include <stdint.h>
#include <string.h>
#include "stm32g0xx.h"
#include "stm32g0xx_hal_i2c.h"

#ifndef LP_I2C_TX_BUF_SIZE
#define LP_I2C_TX_BUF_SIZE 6
#endif 
static void I2C_init(I2C_TypeDef* i2c);
static void I2C_Error_Handler(void);

static I2C_HandleTypeDef* hi2c;
static volatile uint8_t buf[sizeof(float)]; /**< Processed output buffer*/
static volatile _Bool tx_active = 0; /**< Marks if output buffer is used in tx operation (async work)*/

static uint8_t* buf_tdb = NULL; /**< TDB Demo Individual Sensor Raw Output Buffer */
static volatile _Bool tx_active_tdb = 0; /**< Marks if output tdb buffer is used in tx operation */
static volatile uint8_t latest_cmd; /** Recieve buffer (idk if we will use this in reality? maybe set delays for demo modes.)*/
static void (*writeResponse)(uint8_t) = NULL; /**< Write response implementation (TDBDemo)*/

/**
 * @brief Initialize I2C interface, set up buffers
 * 
 * @param instance 
 * @param _buf 
 * @return I2C_HandleTypeDef* 
 */
I2C_HandleTypeDef* lp_i2c_init(I2C_TypeDef* instance) {
  I2C_init(instance);
  return hi2c;  
}

/**
 * @brief Update buffer to new reported value
 * 
 * @param new 
 * @return _Bool 
 */
_Bool lp_i2c_update_buff(void* new) {
  if (tx_active) { return 0; }
  memcpy(new, buf, sizeof(float));
  return 1;
}

_Bool lp_i2c_update_tdb_buff(void* new) {
  if (tx_active_tdb) { return 0;}
  memcpy(new, buf_tdb, 6*sizeof(float));
  return 1;
}

/**
 * @brief Reports if the buffer is occupied (deny write)
 * 
 * @return _Bool 
 */
_Bool lp_i2c_buffer_occupied(void) {
  return tx_active;
}

/**
 * @brief Start I2C interface 
 * 
 */
void lp_i2c_start(void) {
    HAL_I2C_EnableListen_IT(hi2c);
}

/**
 * @brief Stop I2C interface
 * 
 */
void lp_i2c_stop(void) {
    HAL_I2C_DisableListen_IT(hi2c);
}

/**
 * @brief Callback is executed on byte recieve. Keeps I2C peripheral listening 
 * 
 * @param _hi2c 
 */
void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *_hi2c) {
  if (_hi2c->Instance != hi2c->Instance) { return; }
  HAL_I2C_EnableListen_IT(_hi2c); // continue listen;
}

/**
 * @brief I2C Transmit/Recieve Logic
 * 
 * @param _hi2c 
 * @param TransferDirection 
 * @param AddrMatchCode 
 */
void HAL_I2C_AddrCallback(I2C_HandleTypeDef *_hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode) {
  if (_hi2c->Instance != hi2c->Instance) { return; }
  if (TransferDirection == I2C_DIRECTION_TRANSMIT) {
    // Recieve Command Word
    HAL_I2C_Slave_Receive_IT(hi2c, &latest_cmd, 1); // recieve command byte
    // TDB Command Word
    if (latest_cmd == 0x42) { HAL_I2C_Slave_Transmit(_hi2c, buf_tdb, 6*sizeof(float), 0); }
  } else { // Send back latest sensor data
    tx_active = 1; // buffer is occupied - do not change data pls!
    HAL_I2C_Slave_Seq_Transmit_IT(hi2c, buf, 4, I2C_LAST_FRAME);
  } 
}

/**
 * @brief Marks tx buffer as unused, free for application usage. Execute
 * on transmit complete callback.
 * 
 * @param _hi2c 
 */
void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *_hi2c) {
  if (_hi2c->Instance != hi2c->Instance) { return; }
  tx_active = 0;
}

/**
 * @brief Configure I2C peripheral to accept 0x23 addr data.
 * 
 * @param instance I2C interface to use (i2c1 typ)
 */
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
    I2C_Error_Handler();
  }
  // Analog Filter
  if (HAL_I2CEx_ConfigAnalogFilter(hi2c, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    I2C_Error_Handler();
  }
  // Digital Filter
  if (HAL_I2CEx_ConfigDigitalFilter(hi2c, 0) != HAL_OK)
  {
  I2C_Error_Handler();
  }
}

/**
 * @brief I2C Error Handler
 */
static void I2C_Error_Handler(void) {
  while(1); // spin
}