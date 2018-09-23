/*
 *  AMT23.c
 *
 *  Created on: 2018/09/23
 *      Author: Yuki
 */

#include "AMT23.h"

static SPI_HandleTypeDef *hspi_amt23;
static Resolution_TypeDef resolition_shift;

void Encoder_Init(SPI_HandleTypeDef* hspi, Resolution_TypeDef resolution_shift_set){

    hspi_amt23 = hspi;
    resolition_shift = resolution_shift_set;

}

uint16_t GetAngle_raw(void){
    uint16_t rxbuf = 0;
    HAL_SPI_Receive(hspi_amt23, (uint8_t*)(rxbuf), 2, 100);

    rxbuf = (0x3FFF&(rxbuf))>>resolition_shift;
    return rxbuf;
}

float GetAngle_deg(void){
    return 0;
}

float GetANgle_rad(void){
    return 0;
}