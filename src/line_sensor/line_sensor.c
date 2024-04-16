/**
 * @file line_sensor.c
 * @author cinnamondev
 * @brief ADC interface code for line sensor 
 * 
 *
 */

#include "line_sensor.h"
#include "stm32g030xx.h"
#include "stm32g0xx.h"
#include "stm32g0xx_hal_adc.h"
#include "stm32g0xx_hal_adc_ex.h"
#include "stm32g0xx_hal_gpio.h"
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static void DMA_init(void); /**< Configure DMA peripheral*/
static void ADC_init(void); /**< Configure ADC peripheral*/
static void ADC_Error_Handler(void); /**< ADC peripheral init spinner*/
static ADC_HandleTypeDef* hadc; /**< ADC peripheral  instance*/
static struct line_sensor s; /**< Line sensor instance */
static int i = 0; /**< Counter used for GPIO switching*/

static _Bool running = false;
static _Bool ready_to_process = false;

float linePosition;

#define SENSOR_THRESHOLD 2000

void ls_init(struct line_sensor *sensor, ADC_HandleTypeDef* _hadc) {
  hadc = _hadc;
  s = *sensor;
  DMA_init();
  ADC_init(); 
  lp_i2c_init(s.hi2c);
}

void ls_start(void) { 
  running = true;
  // bootstrap & ensure safe starting position
  i=0;
  SENSOR_ENABLE(s.tcrt[i]);
  HAL_ADC_Start_DMA(hadc, s.buff, LINE_SENSOR_N); 
}
void ls_stop(void) { 
  running = false;
  HAL_ADC_Stop_DMA(hadc); 
}

_Bool ls_ready_to_process(void) {
  return ready_to_process;
}

void ls_resume(void) {
  HAL_ADC_Start(hadc);
}
static void ls_process(void) {
  
  for (int i=0; i<LINE_SENSOR_N; i++) {
    if(s.buff[i] < SENSOR_THRESHOLD) { s.buff[i] = 0; }
  }

  // continue the DMA cycle. will continue until stopped `ls_stop`.
  HAL_ADC_Start_DMA(hadc, s.buff, LINE_SENSOR_N);
}
/**
 * @brief ADC1 Initialization Function
 * @param None
 * @retval None
 */
static void ADC_init(void) {
  ADC_ChannelConfTypeDef sConfig = {0};
  // ADC Configuration
  hadc->Instance = s.hadc;
  hadc->Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc->Init.Resolution = ADC_RESOLUTION_12B;
  hadc->Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc->Init.ScanConvMode = ADC_SCAN_SEQ_FIXED;
  hadc->Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc->Init.LowPowerAutoWait = DISABLE;
  hadc->Init.LowPowerAutoPowerOff = DISABLE;
  hadc->Init.ContinuousConvMode = ENABLE;
  hadc->Init.NbrOfConversion = 1;
  hadc->Init.DiscontinuousConvMode = DISABLE;
  hadc->Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc->Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc->Init.DMAContinuousRequests = ENABLE;
  hadc->Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc->Init.SamplingTimeCommon1 = ADC_SAMPLETIME_3CYCLES_5;
  hadc->Init.OversamplingMode = DISABLE;
  hadc->Init.TriggerFrequencyMode = ADC_TRIGGER_FREQ_HIGH;
  if (HAL_ADC_Init(hadc) != HAL_OK) {
    ADC_Error_Handler();
  }

  // ADC Channel Enables
  sConfig.Channel = ADC_CHANNEL_5; // S3
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  if (HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK) {
    ADC_Error_Handler();
  }

  sConfig.Channel = ADC_CHANNEL_6; // S2
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  if (HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK) {
    ADC_Error_Handler();
  }

  sConfig.Channel = ADC_CHANNEL_7; // S1
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  if (HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK) {
    ADC_Error_Handler();
  }

  sConfig.Channel = ADC_CHANNEL_9; // S6
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  if (HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK) {
    ADC_Error_Handler();
  }

  sConfig.Channel = ADC_CHANNEL_15; // S5
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  if (HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK) {
    ADC_Error_Handler();
  }

  sConfig.Channel = ADC_CHANNEL_16; // S4
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  if (HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK) {
    ADC_Error_Handler();
  }

  HAL_ADCEx_Calibration_Start(hadc);
}

/**
 * Enable DMA controller clock
 */
static void DMA_init(void) {

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}

/**
 * @brief Callback executes on half complete transfer
 * 
 * @param _hadc 
 */
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *_hadc) {
  return; // we do NOTHING here!
}

/**
 * @brief Callback executes on end of sampling
 *
 * @param hadc ADC handler
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *_hadc) {
  SENSOR_DISABLE(s.tcrt[i]);
  i=0;
  HAL_ADC_Stop(_hadc);
}

/**
 * @brief Pre-emptively enable the next sensor once all samples from the
 * previous are gathered. (HAL End of Sampling Callback)
 * @param hadc ADC Handler
 */
void HAL_ADCEx_EndOfSamplingCallback(ADC_HandleTypeDef *_hadc) {
  // Enable next in sequence
  SENSOR_DISABLE(s.tcrt[i]);
  if(++i >= LINE_SENSOR_N) { i = 0; };
  SENSOR_ENABLE(s.tcrt[i]);
}

static void ADC_Error_Handler() {
  while (1)
    ; // spin
}
