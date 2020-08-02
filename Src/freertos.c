/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "MeasurementBoard_v1.h"
#include "rs485.h"
#include "AMT23.h"
#include <stdint.h>
#include <stdio.h>
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
/* USER CODE BEGIN Variables */
osThreadId rs485TransmitTaskHandle;
uint32_t rs485TransmitTaskBuffer[ 256 ];
osStaticThreadDef_t rs485TransmitTaskControlBlock;

osThreadId COMSendTaskHandle;
uint32_t COMSendTaskBuffer[ 256 ];
osStaticThreadDef_t COMSendTaskControlBlock;

osThreadId EncoderTaskHandle;
uint32_t EncoderTaskBuffer[ 256 ];
osStaticThreadDef_t EncoderTaskControlBlock;

int32_t speed_rpm_fil;
/* USER CODE END Variables */
osThreadId defaultTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void COMSendTask(void const * argument);
void rs485TransmitTask(void const * argument);
void EncoderTask(void const * argument);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];
  
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}                   
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  osThreadStaticDef(rs485Task, rs485TransmitTask, osPriorityNormal, 0, 256, rs485TransmitTaskBuffer, &rs485TransmitTaskControlBlock);
  rs485TransmitTaskHandle = osThreadCreate(osThread(rs485Task), NULL);

  osThreadStaticDef(comTask, COMSendTask, osPriorityNormal, 0, 256, COMSendTaskBuffer, &COMSendTaskControlBlock);
  COMSendTaskHandle = osThreadCreate(osThread(comTask), NULL);

  osThreadStaticDef(encoderTask, EncoderTask, osPriorityNormal, 0, 256, EncoderTaskBuffer, &EncoderTaskControlBlock);
  EncoderTaskHandle = osThreadCreate(osThread(encoderTask), NULL);
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    UpdateAngle();
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
void EncoderTask(void const * argument){

  uint8_t i = 0;
  
  #define LENGTH 8
  int32_t speed_temp[LENGTH];
  int32_t speed_sum = 0;
  for(int8_t j = 0; j < LENGTH; j++){
    speed_temp[j] = 0;
  }

  for(;;)
  {
    osEvent event = osSignalWait(0b0001, osWaitForever);
    if (event.status == osEventSignal
        && event.value.signals == 0b0001) {
          LED_Toggle(LED1);
          speed_temp[i] = GetVelcity_RPM();
          speed_sum = 0;
          for (int8_t k = 0; k < LENGTH; k++){
            speed_sum += speed_temp[k];
          }
          speed_rpm_fil = speed_sum / LENGTH;
          i++;
          if(i == LENGTH){
            i = 0;
          }
    }
  }
}
void rs485TransmitTask(void const * argument){

  uint8_t ID = 0x10;
  uint32_t send_buf = 18000;
  uint8_t Data[] = {0x00,0x00,0xFF,0xB8};
  for(;;)
  {
    LED_Toggle(LED2);
    osDelay(10);
    if(ID <= 0x13){
        RS485_Transmit(ID, 0x00, (uint8_t *)&send_buf, sizeof(Data));
        ID++;
    }else{
        ID = 0x10;
    }
  }
}

void COMSendTask(void const * argument){

  
  for(;;)
  { 
    static uint8_t i =0;
    //MechSpeed_RPM = GetVelcity_RPM();
    LED_Toggle(LED0);
    osDelay(2);
    //MC_Speed_Filter();
    printf("%04ld\n",speed_rpm_fil);
    i++;
    
    if(i >= 4){
      //printf("%04d,%04d\n",MechSpeed_RPM, MechSpeed_filterd_RPM);   //RPM Out
      i =0;
    }
  }
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
