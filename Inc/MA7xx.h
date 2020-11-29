/*
 *  MA7xx.h
 *
 *  Created on: 2020/11/03
 *      Author: Yuki
 */

#ifndef __MA7XX
#define __MA7XX
#ifdef __cplusplus
 extern "C" {
#endif

#include "spi.h"

#include "encoder_conf.h"

#include ENC_HEADER

#ifndef ENCODER_NAME
#error "No encoder name set"
#endif

uint16_t (*MA7xx_GetAngle)(void);
uint8_t (*MA7xx_ReadRegister)(uint8_t);
uint8_t (*MA7xx_WriteRegister)(uint8_t, uint8_t);
uint32_t (*delay)(uint32_t);

void Encoder_Init_MA7xx(void);

#ifdef __cplusplus
}
#endif
#endif /* __MA7XX */
