/*
 *  HEDL5540.c
 *
 *  Created on: 2018/09/23
 *      Author: Yuki
 */

#include "HEDL5540.h"

static TIM_HandleTypeDef *htim_hedl5540;

void HEDL5540_Encoder_Init(TIM_HandleTypeDef* htim){

    htim_hedl5540 = htim;
    HAL_TIM_Encoder_Start(htim_hedl5540, TIM_CHANNEL_ALL);

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