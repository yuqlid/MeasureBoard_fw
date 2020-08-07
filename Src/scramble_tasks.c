/*
 *  scramble_tasks.c
 *
 *  Created on: 2020/08/02
 *      Author: Yuki
 */

#include "scramble_tasks.h"
#include "MeasurementBoard_v1.h"
#include "AMT23.h"

static int32_t speed_rpm_fil;

osThreadId rs485TransmitTaskHandle;
static uint32_t rs485TransmitTaskBuffer[ 256 ];
static osStaticThreadDef_t rs485TransmitTaskControlBlock;

osThreadId COMSendTaskHandle;
static uint32_t COMSendTaskBuffer[ 256 ];
static osStaticThreadDef_t COMSendTaskControlBlock;

osThreadId EncoderProcessTaskHandle;
static uint32_t EncoderProcessTaskBuffer[ 256 ];
static  osStaticThreadDef_t EncoderProcessTaskControlBlock;

void EncoderProcessTask(void const * argument){

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
        osDelay(100);
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

void scramble_RegisterTasks(void){

    osThreadStaticDef(rs485Task, rs485TransmitTask, osPriorityNormal, 0, 256, rs485TransmitTaskBuffer, &rs485TransmitTaskControlBlock);
    rs485TransmitTaskHandle = osThreadCreate(osThread(rs485Task), NULL);

    osThreadStaticDef(comTask, COMSendTask, osPriorityNormal, 0, 256, COMSendTaskBuffer, &COMSendTaskControlBlock);
    COMSendTaskHandle = osThreadCreate(osThread(comTask), NULL);

    osThreadStaticDef(encoderprocessTask, EncoderProcessTask, osPriorityNormal, 0, 256, EncoderProcessTaskBuffer, &EncoderProcessTaskControlBlock);
    EncoderProcessTaskHandle = osThreadCreate(osThread(encoderprocessTask), NULL);
}