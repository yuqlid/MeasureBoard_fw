/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "can.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include "MeasurementBoard_v1.h"
#include "CLI-commands.h"
#include "CLI-commands_bq34Z100-G1.h"
#include "CLI-commands_MA7xx.h"
#include "scramble_tasks.h"
#include "eeprom_CLI-commands.h"
#include "CLI-commands_bq769x0.h"
#include "CLI-commands_bq78350.h"
#include "CLI-commands_Scramble.h"

#include "usb_cdc.h"
//#include "uart_util_hal.h"
#include "AMT23.h"
#include "uart_util_hal.h"
#include "IncEncoder.h"

#include "FreeRTOS_CLI.h"
//#include "UART-interrupt-driven-command-console.h"

#include <stdio.h>
#include <stdint.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define FILTER_DEEP_SHIFT 4
#define FILTER_DEEP       (1<<FILTER_DEEP_SHIFT)

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

void __io_putchar(uint8_t ch) {
  HAL_UART_Transmit(&huart1, &ch, 1, 100);
}
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
int32_t MechSpeed_RPM = 0;
int32_t MechSpeed_filterd_RPM = 0;
uint16_t index_array = 0;                           /*!<  Speed filter variable */
int32_t speed_tmp_array[FILTER_DEEP];               /*!<  Speed filter variable */

int32_t speed_sum_sp_filt = 0;

uint8_t  array_completed = false;

//CAN_HandleTypeDef     CanHandle;
CAN_TxHeaderTypeDef   TxHeader;
CAN_RxHeaderTypeDef   RxHeader;
uint8_t               TxData[8];
uint8_t               RxData[8];
uint32_t              TxMailbox;
CAN_FilterTypeDef  sFilterConfig;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */
extern void vUARTCommandConsoleStart( void );
extern void vRegisterSampleCLICommands( void );
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/*
void __io_putchar(uint8_t ch) {
  HAL_UART_Transmit(&huart1, &ch, 1, 1);
}
*/
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
  MX_CAN_Init();
  MX_SPI1_Init();
  MX_I2C1_SMBUS_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_TIM17_Init();
  MX_TIM16_Init();
  /* USER CODE BEGIN 2 */
  
  // SPI CS
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, SET);
   // USBのリセット
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, RESET);
  
  setbuf(stdout, NULL );
  //UART_Util_Init(&huart1);
  //uint8_t a = 'B';
  //HAL_UART_Transmit(&huart1, &a, 1, 100);
  printf("\r\n%s,%s\r\n",__DATE__,__TIME__);
  printf("MeasurementBoard_fw\r\n");
  //Encoder_Init(&hspi1, RES_12BIT, htim17.Init.Period, htim17.Init.Prescaler);
  //HAL_TIM_Base_Start_IT(&htim17);
  //HAL_TIM_Base_Start_IT(&htim16);
  //printf("%8d\n",SamplingFreq_Hz);

  /* Register two command line commands to show task stats and run time stats
  respectively. */
  vRegisterSampleCLICommands();
  vRegisterCLICommands();
  //vRegisterbq34z100G1CLICommands();
  //vRegisterMA7xxCLICommands();
  IncEncoder_Init(&htim2);
  //vRegisterEEPROMCLICommands();
  //vRegisterbq769x0CLICommands();
  //vRegisterbq78350CLICommands();
  vRegisterScrambleWheelCLICommands();
  scramble_RegisterTasks();
  UsbCDC_RegisterTasks();
  
  osThreadSuspend(rs485TransmitTaskHandle);
  osThreadSuspend(rs485DribbleTaskHandle);
  
  //osThreadSuspend(COMSendTaskHandle);
  //osThreadSuspend(EncoderProcessTaskHandle);

  
  vUARTCommandConsoleStart();
  sFilterConfig.FilterBank = 0;
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  sFilterConfig.FilterIdHigh = 0x0000;
  sFilterConfig.FilterIdLow = 0x0000;
  sFilterConfig.FilterMaskIdHigh = 0x0000;
  sFilterConfig.FilterMaskIdLow = 0x0000;
  sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
  sFilterConfig.FilterActivation = ENABLE;
  sFilterConfig.SlaveStartFilterBank = 14;

  if(HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK)
  {

    Error_Handler();
  }

  if (HAL_CAN_Start(&hcan) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }

/*
  if(HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox) != HAL_OK)
  {
      Error_Handler();
  }

  while(HAL_CAN_GetTxMailboxesFreeLevel(&hcan) != 3) {}

  if(HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0) != 1)
  {

      Error_Handler();
  }

  if(HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
  {

      Error_Handler();
  }

  if((RxHeader.StdId != 0x11)                     ||
     (RxHeader.RTR != CAN_RTR_DATA)               ||
     (RxHeader.IDE != CAN_ID_STD)                 ||
     (RxHeader.DLC != 2)                          ||
     ((RxData[0]<<8 | RxData[1]) != 0xCAFE))
  {

      Error_Handler();
  }
  */

  /* USER CODE END 2 */

  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();
  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    osDelay(10);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB|RCC_PERIPHCLK_USART1
                              |RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_SYSCLK;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_SYSCLK;
  PeriphClkInit.USBClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void printb(uint16_t v) {
  #define CHAR_BIT 8
  uint16_t mask = (int16_t)1 << (sizeof(v) * CHAR_BIT - 1);
  do putch(mask & v ? '1' : '0');
  while (mask >>= 1);
}

void MC_Speed_Filter(void){
  
  speed_sum_sp_filt = 0;
  speed_tmp_array[index_array] = MechSpeed_RPM;
  if(array_completed == false){
    for(int16_t i = (index_array-1); i>=0; i--){
      speed_sum_sp_filt = speed_sum_sp_filt + speed_tmp_array[i];
    }
    index_array++;
    if(index_array >= FILTER_DEEP){
      index_array = 0;
      array_completed = true;
    }
  }else{
    for(int16_t i = (FILTER_DEEP-1); i >= 0; i--){
      speed_sum_sp_filt = speed_sum_sp_filt + speed_tmp_array[i];
    }
    index_array++;
    if(index_array >= FILTER_DEEP){
      index_array = 0;
    }
  }
  MechSpeed_filterd_RPM = speed_sum_sp_filt>>FILTER_DEEP_SHIFT;
}

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM15 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM15) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */
  if(htim->Instance == TIM17){
    UpdateAngle();
  }
  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
