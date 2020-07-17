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

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
uint32_t defaultTaskBuffer[ 128 ];
osStaticThreadDef_t defaultTaskControlBlock;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void COMSendTask(void const * argument);
void rs485TransmitTask(void const * argument);
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
  osThreadStaticDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128, defaultTaskBuffer, &defaultTaskControlBlock);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  osThreadStaticDef(rs485Task, rs485TransmitTask, osPriorityNormal, 0, 128, rs485TransmitTaskBuffer, &rs485TransmitTaskControlBlock);
  defaultTaskHandle = osThreadCreate(osThread(rs485Task), NULL);

  osThreadStaticDef(comTask, COMSendTask, osPriorityNormal, 0, 128, COMSendTaskBuffer, &COMSendTaskControlBlock);
  defaultTaskHandle = osThreadCreate(osThread(comTask), NULL);
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
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
void rs485TransmitTask(void const * argument){

  uint8_t ID;
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
    UpdateAngle();
    osDelay(2);
    //MC_Speed_Filter();
    printf("%04d\n",GetAngle_raw());
    i++;
    
    if(i >= 4){
      //printf("%04d,%04d\n",MechSpeed_RPM, MechSpeed_filterd_RPM);   //RPM Out
      i =0;
    }
  }
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
