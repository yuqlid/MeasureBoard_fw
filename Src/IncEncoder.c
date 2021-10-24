/*
 *  IncEncoder.c
 *
 *  Created on: 2021/10/24
 *      Author: Yuki Kusakabe
 */

#include "cmsis_os.h"
#include "IncEncoder.h"

static TIM_HandleTypeDef *IncEncHandle;

osThreadId IncEncoderProcessTaskHandle;
static uint32_t IncEncoderProcessTaskBuffer[ 256 ];
static  osStaticThreadDef_t IncEncoderProcessTaskControlBlock;

static int32_t vel_cnt_raw = 0;


void IncEncoderProcessTask(void const * argument){

    HAL_TIM_Encoder_Start(IncEncHandle, TIM_CHANNEL_ALL);

    for(;;){

        vel_cnt_raw = (int32_t)GetCount_raw() - 0x7fffffff;
        IncEncHandle->Instance->CNT = 0x7fffffff;
        osDelay(1);
    }
}

void IncEncoder_Init(TIM_HandleTypeDef* htim){

    IncEncHandle = htim;

    osThreadStaticDef(incencoderprocessTask, IncEncoderProcessTask, osPriorityNormal, 0, 256, IncEncoderProcessTaskBuffer, &IncEncoderProcessTaskControlBlock);
    IncEncoderProcessTaskHandle = osThreadCreate(osThread(incencoderprocessTask), NULL);

}

uint32_t GetCount_raw(void){
    return  IncEncHandle->Instance->CNT;
}

int32_t GetVelocity_rpm(void){
    return vel_cnt_raw * 1000 * 60 / (2048 * 4);
}
/*
float GetAngle_deg(void){
    return 0;
}

float GetANgle_rad(void){
    return 0;
}
*/