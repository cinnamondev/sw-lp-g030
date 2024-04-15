/**
 * @file main.c
 * @author cinnamondev
 * @brief Main program for Line Sensor Module
 * 
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "line_sensor/line_sensor.h"
#include "stm32g030xx.h"
#include "stm32g0xx_hal_adc_ex.h"
#include "stm32g0xx_hal_gpio.h"
#include "stm32g0xx_hal_i2c.h"

/* Private includes ----------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define DEMOB /**< Undefine DEMOB to remove sensor debug output */
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
static ADC_HandleTypeDef hadc1; /**< ADC peripheral*/
static DMA_HandleTypeDef hdma_adc1; /**< DMA peripheral (for ADC transfers)*/
static I2C_HandleTypeDef hi2c1; /**< I2C peripheral (acts as SLAVE). */
static struct line_sensor s; /**< Line sensor hardware*/

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void); /**< Configure system clocks (full speed sysclk) */
static void MX_GPIO_Init(void); /**< Initialize GPIO */

/* Private user code ---------------------------------------------------------*/

/**
  * @brief  The application entry point.
  * @retval int
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

  /* Main code */
  s = LINE_SENSOR(I2C1, ADC1, TARGET_LSA_21A_ORDER);


  /* Infinite loop */
  // Initial test state
  HAL_GPIO_WritePin(S1E_GPIO_Port, S1E_Pin, 1);
  HAL_GPIO_WritePin(S2E_GPIO_Port, S2E_Pin, 0);
  HAL_GPIO_WritePin(S3E_GPIO_Port, S3E_Pin, 1);
  HAL_GPIO_WritePin(S4E_GPIO_Port, S4E_Pin, 0);
  HAL_GPIO_WritePin(S5E_GPIO_Port, S5E_Pin, 1);
  float linePosition = 3.0f;
  while(1) {
    
  }
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
    Error_Handler();
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
    Error_Handler();
  }
  /** Initializes the peripherals clocks
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_ADC;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_SYSCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}



/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
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
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
