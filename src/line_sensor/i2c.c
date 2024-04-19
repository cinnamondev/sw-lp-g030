#include "i2c.h"
#include "stm32g0xx.h"
#include "stm32g0xx_hal_def.h"
#include "stm32g0xx_hal_i2c.h"
#include <stdint.h>
#include <string.h>

#define WAIT_BUFFER_FREE() while(outputBufferTxActive) { __NOP(); }
#define WAIT_TDB_BUFFER_FREE() while(tdbOutputBufferTxActive) { __NOP(); }

static I2C_HandleTypeDef* hi2c;
static volatile uint8_t commandWord;
static volatile float outputBuffer = -1;
static volatile int outputBufferTxActive = 0;
static volatile uint32_t tdbOutputBuffer[6] = {0,0,0,0,0,0}; 
static volatile int tdbOutputBufferTxActive = 0;

void ls_i2c_init(I2C_HandleTypeDef* _hi2c) {
    hi2c = _hi2c;
}

void ls_i2c_start(void) {
    HAL_I2C_EnableListen_IT(hi2c);
}

void ls_i2c_stop(void) {
    WAIT_BUFFER_FREE()
    HAL_I2C_DisableListen_IT(hi2c);
}

void ls_i2c_buff_update_blk(float* new) {
    WAIT_BUFFER_FREE();
    memcpy(&outputBuffer, new, sizeof(outputBuffer));
}

void ls_i2c_tdb_buff_update_blk(uint32_t* new) {
    WAIT_TDB_BUFFER_FREE();
    memcpy(tdbOutputBuffer, new, sizeof(tdbOutputBuffer));

}

void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *_hi2c) {
    HAL_I2C_EnableListen_IT(_hi2c);
}

void HAL_I2C_AddrCallback(I2C_HandleTypeDef *_hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode) {
    if (TransferDirection == I2C_DIRECTION_TRANSMIT) {
        // Master wants to send data... we recieve a command word.
        HAL_I2C_Slave_Receive(hi2c, &commandWord, 1, HAL_MAX_DELAY);
    } else {
        // Master wants to recieve data...
        if (commandWord == 0x42) {
            tdbOutputBufferTxActive = 1;
            HAL_I2C_Slave_Transmit_IT(hi2c, (uint8_t*)tdbOutputBuffer, sizeof(tdbOutputBuffer));
            commandWord = 0x00; // reset command word (exec once)
        } else {
            // Standard behaviour
            outputBufferTxActive = 1;
            HAL_I2C_Slave_Transmit_IT(hi2c, (uint8_t*)&outputBuffer, 4);
        }
    }
}

void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c) {
    if(outputBufferTxActive) { outputBufferTxActive = 0; }
    if(tdbOutputBufferTxActive) { tdbOutputBufferTxActive = 0;}
}

