/*
 *  AMT23.c
 *
 *  Created on: 2018/09/23
 *      Author: Yuki
 */

#include "AMT23.h"
#include "main.h"

static SPI_HandleTypeDef *hspi_amt23;
static Resolution_TypeDef resolition_shift;

void Encoder_Init(SPI_HandleTypeDef* hspi, Resolution_TypeDef resolution_shift_set){

    hspi_amt23 = hspi;
    resolition_shift = resolution_shift_set;

}

uint16_t GetAngle_raw(void){
    uint8_t rxbuf[2] = {0,0};
    uint16_t angleraw = 0;
    HAL_SPI_Receive(hspi_amt23, rxbuf, 2, 100);
    printb((uint16_t)((rxbuf[0]<<8)|rxbuf[1]));
    angleraw = 0x7FFF&((rxbuf[0]<<8)|rxbuf[1]);
    angleraw >>= 3;
    angleraw &= 0x0FFF;
    return angleraw;
}

float GetAngle_deg(void){
    return 0;
}

float GetANgle_rad(void){
    return 0;
}