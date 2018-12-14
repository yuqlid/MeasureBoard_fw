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

uint16_t angleraw = 0;
uint16_t angleraw_z1= 0;
int16_t velocity = 0;
int16_t velocity_z1 = 0;
uint8_t rxbuf[2] = {0,0};

void Encoder_Init(SPI_HandleTypeDef* hspi, Resolution_TypeDef resolution_shift_set){

    hspi_amt23 = hspi;
    resolition_shift = resolution_shift_set;

}

void UpdateAngle(void){
    HAL_SPI_Receive_IT(hspi_amt23, rxbuf, 2);
}

uint16_t GetAngle_raw(void){
    return angleraw;
}

int16_t GetVelcity_raw(void){
    int16_t tmp = 0;
    velocity = angleraw - angleraw_z1;
    
    if(velocity_z1 >= 0 && velocity < -2000)velocity +=4095;
    else if(velocity_z1 <= 0 && velocity > 2000)velocity -=4095;
    velocity_z1 = velocity;
    return velocity;
}

float GetAngle_deg(void){
    return 0;
}

float GetANgle_rad(void){
    return 0;
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi){

    angleraw_z1 = angleraw;
    angleraw = 0x0FFF&((rxbuf[0]<<5)|rxbuf[1]>>3);
    //printb((uint16_t)((rxbuf[0]<<8)|rxbuf[1]));
    //printf(",%04d\n",angleraw);
}