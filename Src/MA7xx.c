/*
 *  MA7xx.c
 *
 *  Created on: 2020/11/03
 *      Author: Yuki
 */

#include "MA7xx.h"

static volatile uint16_t angle = 0;

uint16_t MA7xx_GetAngle(void){

    HAL_SPI_Receive(&hspi1, &angle, 1, 100);
    return angle;
}