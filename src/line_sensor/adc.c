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

#include "adc.h"
#include "stm32g0xx.h"
#include "stm32g0xx_hal_adc.h"

struct arr_range {
  size_t min;
  size_t max;
} RANGE_DEF = {
  .min = 0xFFFF,
  .max = 0xFFFF,
};

static ADC_HandleTypeDef* hadc;
static volatile int buffer_ready = 0; /**< Acts as a buffer lock */
static int adc_running = 0; /**< Indicates if ADC needs resuming or not*/
static volatile uint32_t raw_buff[6]; /**< DMA buffer */
static volatile float calculated_position; /**< Final processed position*/

/**
 * @brief Update the calculated line position using the latest buffer sample.
 * 
 */
static void process_samples(void);
/**
 * @brief Finds a cluster of elements in an array that are above a threshold.
 * 
 * @param buffer Array to search
 * @param size Size of array
 * @param thres Threshold value
 * @return struct arr_range Range structure that stores the indicies that are
 * consecutively above a threshold.
 */
static struct arr_range find_next_thres_cluster(uint32_t* buffer, size_t size, uint32_t thres);
static size_t find_max(uint32_t* buffer, size_t size);
static void rearrange_samples();

/** @brief Set ADC peripheral used */
void ls_adc_init(ADC_HandleTypeDef* _hadc) {
    hadc = _hadc;
}

/** @brief Start constant ADC conversions via DMA */
void ls_adc_start(void) {
    HAL_ADC_Start_DMA(hadc, raw_buff, 6);
}

/** @brief Stop constant ADC conversions */
void ls_adc_stop(void) {
    HAL_ADC_Stop_DMA(hadc);
}

/**
 * @brief Lock until a fully processed line position is available.
 * 
 * @return float* Line position
 */
float ls_adc_wait_for(void) {
    while (!buffer_ready) {  __NOP(); } // spinny until we have new data
    buffer_ready = 0;
    return calculated_position;
}

/**
 * @brief Get the raw buffer from the ADC
 * @note Does not spin lock, you should wait for a processed result first to
 * garuntee that the data in this buffer is all from the same sample period.
 * 
 * @return uint32_t* Pointer to the internal raw data buffer (size = 6)
 */
uint32_t* ls_adc_get_raw(void) {
    return raw_buff;
}

/**
 * @brief Resume conversions after 
 * 
 */
void ls_adc_resume(void) {
    buffer_ready = 0;
    HAL_ADC_Start(hadc);
}

/**
 * @brief Indicates if the ADC is running
 * 
 * @return int Boolean
 */
int ls_adc_running(void) {
    return adc_running;
}

/**
 * @brief Rearranges the raw buffer values to be correctly ordered, as the DMA
 * will recieve them in a different order than how we want to process them.
 */
static void rearrange_samples() {
    // because we are intaking from adc in ADC rank order... we have to swap certain
    // elements to put it in OUR order. See difference in main c variable order,
    // 3 2 1 6 5 4 is rank order when real order is 1 2 3 4 5 6
    uint32_t tmp; 
    tmp = raw_buff[2];   // swap 3 and 1
    raw_buff[2] = raw_buff[0];
    raw_buff[0] = tmp;

    tmp = raw_buff[5]; // swap 6 and 4
    raw_buff[5] = raw_buff[3];
    raw_buff[3] = tmp;
}
static void process_samples() {
    rearrange_samples(); // ensure real order
    struct arr_range rangeFPass = find_next_thres_cluster(raw_buff, 6, 40000);

    if (rangeFPass.min == 0xFFFF) {
      // uhoh.. we arent on the line / line break ?
      calculated_position = -1;
    }
    size_t range = rangeFPass.max - rangeFPass.min;
    if (range > 3) {
      // strange edge case, we should never see this ever happen! probably a turn...
      // the behaviour will be to report the most extreme from centre point.
      // (with a preference to the right side if it cannot decide)
      _Bool closeToEdgeL = (rangeFPass.min <= 2);
      _Bool closeToEdgeR = (rangeFPass.max >= 5);
      if (closeToEdgeR) {
        calculated_position = rangeFPass.max;
      } else if (closeToEdgeL) {
        calculated_position = rangeFPass.min;
      } else {
        calculated_position = rangeFPass.max;
      }
    } else if (range == 3) {
      // find max within bounds
      size_t max = find_max(&raw_buff[rangeFPass.min], 6-rangeFPass.min);

    } else if (range == 2) {
      // Find "where" the mean lies between the two (express mean as a percentage increase of min then apply to indicies)
      // note: looks bad.
      
      float mean = (raw_buff[rangeFPass.min] + raw_buff[rangeFPass.max])/2.00;

      float decrease = 100.0f-((1.0f*raw_buff[rangeFPass.max]) - mean)/(1.0f*raw_buff[rangeFPass.max]);

      uint32_t max = (raw_buff[rangeFPass.min] > raw_buff[rangeFPass.max]) ? rangeFPass.min : rangeFPass.max;
      _Bool minLarger = (raw_buff[rangeFPass.min] > raw_buff[rangeFPass.max]);
      calculated_position = (raw_buff[rangeFPass.min] > raw_buff[rangeFPass.max]) ?
        rangeFPass.min + (range*decrease) :
        rangeFPass.max - (range*decrease); 
    } else {
      calculated_position = rangeFPass.min;
    }
}

static struct arr_range find_next_thres_cluster(uint32_t* buffer, size_t size, uint32_t thres) {
  struct arr_range range;
  for (size_t i=0; i<size; i++) {
    if (buffer[i] >= thres) {
      range.min = i;
      break;
    }
  }
  if (range.min == 0xFFFF) { return range; }
  for (size_t i=range.min+1; i<size; i++) {
    if (buffer[i] <= thres) {
      range.max = i-1;
      break;
    }
  }
  return range;
}

static size_t find_max(uint32_t* buffer, size_t size) {
  size_t i_m = 0;
  for (int i=0; i<size;i++) {
    if (buffer[i] > buffer[i_m]) {
      i_m = i;
    }
  }
  return i_m;
}

/** @brief required */
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *_hadc) {
    // hmm...
}

/**
 * @brief Required callback. Interrupts when DMA has completed transfer of
 * the latest conversions. Stops the ADC from performing more conversions
 * until the buffer is free again.
 * 
 * @param _hadc 
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *_hadc) {
    HAL_ADC_Stop(hadc);
    buffer_ready = 1;
}