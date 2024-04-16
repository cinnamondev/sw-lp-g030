/**
 * @file main.c
 * @author cinnamondev
 * @brief Main program for Line Sensor Module
 * 
 */

/* Includes ------------------------------------------------------------------*/
#include "common.h"
#include "line_sensor/line_sensor.h"
#include "stm32g030xx.h"
#include "stm32g0xx_hal.h"
#include "stm32g0xx_hal_adc_ex.h"
#include "stm32g0xx_hal_gpio.h"
#include "stm32g0xx_hal_i2c.h"
#include "stm32g0xx_hal_iwdg.h"
#include <math.h>
#include <stdint.h>

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

struct arr_range {
  size_t min;
  size_t max;
} RANGE_DEF = {
  .min = 0xFFFF,
  .max = 0xFFFF,
};

/* Private define ------------------------------------------------------------*/

#define THRESHOLD 2000
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static ADC_HandleTypeDef hadc1; /**< ADC peripheral*/
static DMA_HandleTypeDef hdma_adc1; /**< DMA peripheral (for ADC transfers)*/
static I2C_HandleTypeDef hi2c1; /**< I2C peripheral (acts as SLAVE). */
static IWDG_HandleTypeDef hiwdg; /**< Independent Watchdog Peripheral*/
static struct line_sensor s; /**< Line sensor hardware*/

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void); /**< Configure system clocks (full speed sysclk) */
static void MX_GPIO_Init(void); /**< Initialize GPIO */
static void MX_IWDG_Init(void); /**< Independent Watchdog Init*/
static void MX_Error_Handler(void); /**< MX spin peripheral init fail general. */
static struct arr_range find_next_thres_cluster(uint32_t* buffer, size_t size, uint32_t thres);
static size_t find_max(uint32_t*, size_t);
/* Private user code ---------------------------------------------------------*/

/**
  * @brief  The application entry point.
  * @retval int error code
  */
int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  lp_i2c_init(I2C1);


  HAL_ADCEx_Calibration_Start(&hadc1);
  HAL_Delay(100); // Wait a while and hopefully the ADC is all sorted out.

  /* Main code */
  s = LINE_SENSOR(I2C1, ADC1, TARGET_LSA_21A_ORDER);

  HAL_GPIO_WritePin(S1E_GPIO_Port, S1E_Pin, 1);
  HAL_GPIO_WritePin(S2E_GPIO_Port, S2E_Pin, 0);
  HAL_GPIO_WritePin(S3E_GPIO_Port, S3E_Pin, 1);
  HAL_GPIO_WritePin(S4E_GPIO_Port, S4E_Pin, 0);
  HAL_GPIO_WritePin(S5E_GPIO_Port, S5E_Pin, 1);
  float linePosition = 3.0f;
  lp_i2c_start();
  while(1) {
    float output;
    if(!ls_ready_to_process()) {continue;} // exit condition
    // oh boy processing time!

    // mark consecutive elements in array above threshold (lower upper range)
    struct arr_range rangeFPass = find_next_thres_cluster(s.buff, LINE_SENSOR_N, THRESHOLD);
    struct arr_range rangeSPass = find_next_thres_cluster(s.buff + rangeFPass.max, LINE_SENSOR_N-rangeFPass.max, THRESHOLD);
    if (rangeSPass.min != 0xFFFF) {
      // we have a problem...
    } 
    if (rangeFPass.min == 0xFFFF) {
      // uhoh.. we arent on the line / line break ?


    }
    size_t range = rangeFPass.max - rangeFPass.min;
    if (range > 3) {
      // strange edge case, we should never see this ever happen! probably a turn...
      // the behaviour will be to report the most extreme from centre point.
      // (with a preference to the right side if it cannot decide)
      output = (rangeFPass.min
    } else if (range == 3) {
      // find max within bounds
      size_t max = find_max(&s.buff[rangeFPass.min], LINE_SENSOR_N-rangeFPass.min);

    } else if (range == 2) {
      // Find "where" the mean lies between the two (express mean as a percentage increase of min then apply to indicies)
      float mean = (s.buff[rangeFPass.min] + s.buff[rangeFPass.max])/2.00;
      float decrease = 100.0f-((1.0f*s.buff[rangeFPass.max]) - mean)/(1.0f*s.buff[rangeFPass.max]);
      uint32_t max = (s.buff[rangeFPass.min] > s.buff[rangeFPass.max]) ? rangeFPass.min : rangeFPass.max;
      _Bool minLarger = (s.buff[rangeFPass.min] > s.buff[rangeFPass.max]);
      output = (s.buff[rangeFPass.min] > s.buff[rangeFPass.max]) ?
        rangeFPass.min + (range*decrease) :
        rangeFPass.max - (range*decrease); 
    } else {
      output = rangeFPass.min;
    }
    // if consecutive > 1
    // mix using values to determine LP

    // if consecutive = 1
    // LP = line number (i)

    // dump to I2C

    // if we arent getting to this point here...
    // then we arent getting ADC data which means something went wrong.
    HAL_IWDG_Refresh(&hiwdg);
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
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 8;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    MX_Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    MX_Error_Handler();
  }
  /** Initializes the peripherals clocks
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_ADC;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_SYSCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    MX_Error_Handler();
  }
}



/**
  * @brief GPIO Peripheral Init
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, S6E_Pin|S5E_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, S4E_Pin|S3E_Pin|S2E_Pin|S1E_Pin
                          |INTX_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : S6E_Pin S5E_Pin */
  GPIO_InitStruct.Pin = S6E_Pin|S5E_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : S4E_Pin S3E_Pin S2E_Pin S1E_Pin
                           INTX_Pin */
  GPIO_InitStruct.Pin = S4E_Pin|S3E_Pin|S2E_Pin|S1E_Pin
                          |INTX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/**
 * @brief IWDG Peripheral Init
 */
static void MX_IWDG_Init(void)
{
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_4;
  hiwdg.Init.Window = 4095;
  hiwdg.Init.Reload = 4095;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
  {
    MX_Error_Handler();
  }
  /* USER CODE BEGIN IWDG_Init 2 */

  /* USER CODE END IWDG_Init 2 */

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
static void MX_Error_Handler(void)
{
  __disable_irq();
  while (1) {

  }
}
