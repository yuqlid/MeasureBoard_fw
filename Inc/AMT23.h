/*
 *  AMT23.h
 *
 *  Created on: 2018/09/23
 *      Author: Yuki
 */

#ifndef __AMT23
#define __AMT23
#ifdef __cplusplus
 extern "C" {
#endif

#include "spi.h"

extern uint16_t    SamplingFreq_Hz;

typedef enum
{
  RES_14BIT = 0,
  RES_12BIT = 2
} Resolution_TypeDef;

void Encoder_Init(SPI_HandleTypeDef* hspi, Resolution_TypeDef resolution_shift_set, uint16_t Period, uint16_t Prescaler);

void UpdateAngle(void);
uint16_t GetAngle_raw(void);
float GetAngle_deg(void);
float GetANgle_rad(void);
int16_t GetVelcity_cnt(void);
int32_t GetVelcity_RPM(void);

#ifdef __cplusplus
}
#endif
#endif /* AMT23 */
