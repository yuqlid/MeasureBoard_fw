/*
 *  MA7xx.c
 *
 *  Created on: 2020/11/03
 *      Author: Yuki
 */

#include "MA7xx.h"

#include ENC_SOURCE

//static volatile uint16_t angle = 0;

uint16_t (*MA7xx_GetAngle)(void);
uint8_t (*MA7xx_ReadRegister)(uint8_t);
uint8_t (*MA7xx_WriteRegister)(uint8_t, uint8_t);
bool (* MA7xx_ReadRotateDir)(void);
uint16_t (* MA7xx_ReadPPT)(void);
uint32_t (*delay)(uint32_t);
/*
uint16_t MA7xx_GetAngle(void){

    HAL_SPI_Receive(&hspi1, &angle, 1, 100);
    return angle;
}
*/