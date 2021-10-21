/*
 *  HEDL5540.c
 *
 *  Created on: 2018/09/23
 *      Author: Yuki
 */

#include "cmsis_os.h"
#include "HEDL5540.h"

static TIM_HandleTypeDef *htim_hedl5540;

osThreadId IncEncoderProcessTaskHandle;
static uint32_t IncEncoderProcessTaskBuffer[ 256 ];
static  osStaticThreadDef_t IncEncoderProcessTaskControlBlock;


void IncEncoderProcessTask(void const * argument){

    HAL_TIM_Encoder_Start(htim_hedl5540, TIM_CHANNEL_ALL);

    for(;;){

        osDelay(10);
    }
}

void HEDL5540_Encoder_Init(TIM_HandleTypeDef* htim){

    htim_hedl5540 = htim;

    osThreadStaticDef(incencoderprocessTask, IncEncoderProcessTask, osPriorityNormal, 0, 256, IncEncoderProcessTaskBuffer, &IncEncoderProcessTaskControlBlock);
    IncEncoderProcessTaskHandle = osThreadCreate(osThread(incencoderprocessTask), NULL);

}

uint32_t GetCount_raw(void){
    return  htim_hedl5540->Instance->CNT;
}
/*
float GetAngle_deg(void){
    return 0;
}

float GetANgle_rad(void){
    return 0;
}
*/