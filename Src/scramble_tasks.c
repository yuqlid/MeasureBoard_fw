/*
 *  scramble_tasks.c
 *
 *  Created on: 2020/08/02
 *      Author: Yuki
 */

#include "main.h"
#include "tim.h"
#include "scramble_tasks.h"
#include "MeasurementBoard_v1.h"
#include "AMT23.h"
#include "rs485.h"
#include <string.h>
#include <stdarg.h>

#include "cmsis_os.h"
#include "IncEncoder.h"
#include "uart_util_hal.h"

typedef enum{
  HEAD_1 = 0xFA,
  HEAD_2 = 0xAF
}Header_Typedef;


extern osSemaphoreId RS485transmitSemaphoreHandle;

static int32_t speed_rpm_fil = 0;
int32_t target_speed_rpm = 0;
int32_t duty = 0;

osThreadId rs485TransmitTaskHandle;
static uint32_t rs485TransmitTaskBuffer[ 256 ];
static osStaticThreadDef_t rs485TransmitTaskControlBlock;

osThreadId rs485ReceiveTaskHandle;
static uint32_t rs485ReceiveTaskBuffer[ 256 ];
static osStaticThreadDef_t rs485ReceiveTaskControlBlock;

osThreadId rs485DribbleTaskHandle;
static uint32_t rs485DribbleTaskBuffer[ 256 ];
static osStaticThreadDef_t rs485DribbleTaskControlBlock;

osThreadId EncoderProcessTaskHandle;
static uint32_t EncoderProcessTaskBuffer[ 256 ];
static  osStaticThreadDef_t EncoderProcessTaskControlBlock;

void com_printf(const char* format, ...);

void setTargetSpeed(long *speed_rpm){
    target_speed_rpm = *speed_rpm;
}

void setDribbleDuty(long *setduty){
    duty = *setduty;
}

void EncoderProcessTask(void const * argument){

    uint8_t i = 0;

    #define LENGTH 16
    int32_t speed_temp[LENGTH];
    int32_t speed_sum = 0;
    for(int8_t j = 0; j < LENGTH; j++){
        speed_temp[j] = 0;
    }

    HAL_TIM_Base_Start_IT(&htim17);

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
    for(;;)
    {
        for(ID = 0x10; ID < 0x14; ID++){
            RS485_Transmit(ID, 0x00, (uint8_t *)&target_speed_rpm, 4);
            LED_Toggle(LED2);
            osDelay(1);
        }
        osDelay(6);
    }
}


// ドリブル用モータへ定周期通信
void rs485DribbleTask(void const * argument){

    uint8_t txbuf[4] = {0xAA, 0xCC, 0x00, 0x00};

    for(;;)
    {
        txbuf[2] = 0;
        txbuf[3] = 0;
        if(duty > 4095){
            duty = 4095;
        }else if(duty < -4095){
            duty = -4095;
        }
        txbuf[2] = duty & 0xFF;
        txbuf[3] = (duty & 0xFF00) >> 8;
        if(duty > 0){
            txbuf[3] |= 0x80;
        }

        RS485_Transmit(0x01, 0x00, txbuf, 4);
        LED_Toggle(LED2);
        osDelay(10);
    }
}


void scramble_RegisterTasks(void){
    /*
    osThreadStaticDef(encoderprocessTask, EncoderProcessTask, osPriorityNormal, 0, 256, EncoderProcessTaskBuffer, &EncoderProcessTaskControlBlock);
    EncoderProcessTaskHandle = osThreadCreate(osThread(encoderprocessTask), NULL);
    */
    osThreadStaticDef(rs485dribbleTask, rs485DribbleTask, osPriorityNormal, 0, 256, rs485DribbleTaskBuffer, &rs485DribbleTaskControlBlock);
    rs485DribbleTaskHandle = osThreadCreate(osThread(rs485dribbleTask), NULL);

    osThreadStaticDef(rs485transmitTask, rs485TransmitTask, osPriorityNormal, 0, 256, rs485TransmitTaskBuffer, &rs485TransmitTaskControlBlock);
    rs485TransmitTaskHandle = osThreadCreate(osThread(rs485transmitTask), NULL);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
    
    if(huart->Instance == USART3){
        osSemaphoreRelease(RS485transmitSemaphoreHandle);
    }
}