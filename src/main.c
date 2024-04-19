/* USER CODE BEGIN Header */
/**
 * @file main.c
 * @author cinnamondev
 * @brief Line Sensor Module Firmware
 * @version rev2-tdb
 * 
 */

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "common_mx.h"
#include "stm32g0xx_hal.h"
#include "stm32g0xx_hal_adc_ex.h"
#include "stm32g0xx_hal_conf.h"
#include "line_sensor/line_sensor.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;
I2C_HandleTypeDef hi2c1;
IWDG_HandleTypeDef hiwdg;

/* USER CODE BEGIN PV */
static struct tcrt_channel sensors[6] = {
  S(3),
S(2),
S(1),
S(6),
S(5),
S(4),

};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_IWDG_Init();
  
  HAL_ADCEx_Calibration_Start(&hadc1);
  HAL_Delay(100);
  /* USER CODE BEGIN 2 */
  for (int i=0; i<6; i++) {
    SENSOR_ENABLE(sensors[i]);
  }

  ls_i2c_init(&hi2c1);
  ls_adc_init(&hadc1);
  ls_i2c_start();
  ls_adc_start();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    float* adcOutput = ls_adc_wait_for();
    uint32_t* tdbOutput = ls_adc_get_raw();
    ls_i2c_buff_update_blk(adcOutput);
    ls_i2c_tdb_buff_update_blk(tdbOutput);
    // if we arent getting to this point here...
    // then we arent getting ADC data which means something went wrong.
    HAL_IWDG_Refresh(&hiwdg);
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

void Error_Handler(void) {
  __disable_irq();
  while(1) {__NOP();}
}