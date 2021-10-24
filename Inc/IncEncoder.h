/*
 *  IncEncoder.h
 *
 *  Created on: 2021/10/24
 *      Author: Yuki Kusakabe
 */

#ifndef INCENCODER_H_
#define INCENCODER_H_
#ifdef __cplusplus
 extern "C" {
#endif

#include "cmsis_os.h"
#include "tim.h"

extern osThreadId IncEncoderProcessTaskHandle;
void IncEncoder_Init(TIM_HandleTypeDef* htim);
uint32_t GetCount_raw(void);
int32_t GetVelocity_rpm(void);

#ifdef __cplusplus
}
#endif
#endif /* INCENCODER_H_ */
