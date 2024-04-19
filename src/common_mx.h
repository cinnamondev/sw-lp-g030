/**
 * @file common_mx.h
 * @author cinnamondev, stm32cubemx
 * @brief hal init driver func
 * 
 */

#ifndef COMMON_MX_H
#define COMMON_MX_H

// Prototypes
void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_DMA_Init(void);
void MX_ADC1_Init(void);
void MX_I2C1_Init(void);
void MX_IWDG_Init(void);

#endif // COMMON_MX_H