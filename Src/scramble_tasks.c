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
#include "usbd_cdc_if.h"
#include "cmsis_os.h"

extern osSemaphoreId RS485transmitSemaphoreHandle;

static int32_t speed_rpm_fil = 0;
int32_t target_speed_rpm = 0;

osThreadId rs485TransmitTaskHandle;
static uint32_t rs485TransmitTaskBuffer[ 256 ];
static osStaticThreadDef_t rs485TransmitTaskControlBlock;

osThreadId COMSendTaskHandle;
static uint32_t COMSendTaskBuffer[ 256 ];
static osStaticThreadDef_t COMSendTaskControlBlock;

osThreadId EncoderProcessTaskHandle;
static uint32_t EncoderProcessTaskBuffer[ 256 ];
static  osStaticThreadDef_t EncoderProcessTaskControlBlock;

void com_printf(const char* format, ...);

void setTargetSpeed(long *speed_rpm){
    target_speed_rpm = *speed_rpm;
}

void EncoderProcessTask(void const * argument){

    uint8_t i = 0;

    #define LENGTH 8
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

void COMSendTask(void const * argument){

    for(;;)
    {
        static uint8_t i =0;
        //MechSpeed_RPM = GetVelcity_RPM();
        LED_Toggle(LED0);
        osDelay(2);
        //MC_Speed_Filter();
        com_printf("%5ld,%5ld\r\n", target_speed_rpm, speed_rpm_fil);
        i++;

        if(i >= 4){
        //printf("%04d,%04d\n",MechSpeed_RPM, MechSpeed_filterd_RPM);   //RPM Out
        i =0;
        }
    }
}

void scramble_RegisterTasks(void){

    osThreadStaticDef(encoderprocessTask, EncoderProcessTask, osPriorityNormal, 0, 256, EncoderProcessTaskBuffer, &EncoderProcessTaskControlBlock);
    EncoderProcessTaskHandle = osThreadCreate(osThread(encoderprocessTask), NULL);

    osThreadStaticDef(rs485Task, rs485TransmitTask, osPriorityNormal, 0, 256, rs485TransmitTaskBuffer, &rs485TransmitTaskControlBlock);
    rs485TransmitTaskHandle = osThreadCreate(osThread(rs485Task), NULL);

    osThreadStaticDef(comTask, COMSendTask, osPriorityNormal, 0, 256, COMSendTaskBuffer, &COMSendTaskControlBlock);
    COMSendTaskHandle = osThreadCreate(osThread(comTask), NULL);

}

void com_printf(const char* format, ...){

    va_list arg;
    uint8_t len;
    static char printf_buf[255];

    va_start(arg, format);

    len = vsnprintf(printf_buf, 255, format, arg);

    if(len > 0){
        CDC_Transmit_FS((uint8_t *)printf_buf, len);
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
    
    if(huart->Instance == USART3){
        osSemaphoreRelease(RS485transmitSemaphoreHandle);
    }
}