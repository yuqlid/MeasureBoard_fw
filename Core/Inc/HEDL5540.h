/*
 *  HEDL5540.h
 *
 *  Created on: 2018/09/23
 *      Author: Yuki
 */

#ifndef __HEDL5540
#define __HEDL5540
#ifdef __cplusplus
 extern "C" {
#endif

#include "tim.h"

void HEDL5540_Encoder_Init(TIM_HandleTypeDef* htim);
uint32_t GetCount_raw(void);
float GetAngle_deg(void);
float GetANgle_rad(void);

#ifdef __cplusplus
}
#endif
#endif /* HEDL5540 */
