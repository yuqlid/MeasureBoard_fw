/*
 *  IncEncoder.c
 *
 *  Created on: 2021/10/24
 *      Author: Yuki Kusakabe
 */

#include "IncEncoder.h"

static TIM_HandleTypeDef *IncEncHandle;
extern osMessageQId CDCQueueHandle;

osThreadId IncEncoderProcessTaskHandle;
static uint32_t IncEncoderProcessTaskBuffer[ 256 ];
static  osStaticThreadDef_t IncEncoderProcessTaskControlBlock;

static int32_t vel_cnt_raw = 0;


void IncEncoderProcessTask(void const * argument){

    HAL_TIM_Encoder_Start(IncEncHandle, TIM_CHANNEL_ALL);

    for(;;){

        vel_cnt_raw = (int32_t)GetCount_raw() - 0x7fffffff;
        IncEncHandle->Instance->CNT = 0x7fffffff;
        /* 
        メッセージを読むタスクが止まっているとその間タイムアウトまでブロッキングする仕様なのか？
        今のプログラムの仕様だと，メッセージ読み出してUSBで送信するタスクが動いていない間はメッセージ送信を止めるか，
        メッセージのバッファがいっぱいになったらうがわきする設定に変えたほうが良さそう
        */
        osMessagePut(CDCQueueHandle, GetVelocity_rpm(), 0);
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);
        osDelay(10);
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
    return vel_cnt_raw * 100 * 60 / (2048 * 4);
}
/*
float GetAngle_deg(void){
    return 0;
}

float GetANgle_rad(void){
    return 0;
}
*/