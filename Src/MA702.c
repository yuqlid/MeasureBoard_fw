/*
 *  MA702.c
 *
 *  Created on: 2020/11/29
 *      Author: Yuki
 */

#include <stdint.h>
#include "spi.h"
#include "MA7xx.h"
#include "MA702.h"

void Encoder_Init_MA7xx(void){

    MA7xx_GetAngle = MA702_GetAngle;
    MA7xx_ReadRegister = MA702_ReadResiter;
}

uint16_t MA702_GetAngle(void){
    uint16_t angle = 0;
    HAL_SPI_Receive(&hspi1, &angle, 1, 100);
    return angle;
}

uint8_t MA702_ReadResiter(uint8_t Addr){

    uint16_t tx[2] = {CMD_R | (0xFF00 & (Addr << 8)), 0};
    uint16_t rx[2] = {0};
    HAL_SPI_TransmitReceive(&hspi1, &tx, &rx, 2, 100);

	return ((rx[1] & 0xFF00) >> 8);
}
