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

volatile uint16_t    angleraw = 0;
volatile uint16_t    angleraw_z1= 0;
volatile int16_t     velocity = 0;
volatile int16_t     velocity_z1 = 0;

uint8_t     spirxbuf[2] = {0,0};
uint16_t    SamplingFreq_Hz = 1;
uint16_t    AMT23_CPR = 1;

void Encoder_Init(SPI_HandleTypeDef* hspi, Resolution_TypeDef resolution_shift_set, uint16_t Period, uint16_t Prescaler){

    hspi_amt23 = hspi;
    resolition_shift = resolution_shift_set;
    SamplingFreq_Hz = HAL_RCC_GetHCLKFreq()/(( Period + 1) * (Prescaler +1));
    AMT23_CPR <<= (14 - (uint8_t)resolition_shift);
}

void UpdateAngle(void){
    HAL_SPI_Receive_IT(hspi_amt23, spirxbuf, 2);
}

uint16_t GetAngle_raw(void){
    return angleraw;
}

float GetAngle_deg(void){
    return 0;
}

float GetAngle_rad(void){
    return 0;
}

int16_t GetVelcity_cnt(void){
    
    velocity = angleraw - angleraw_z1;
    
    if(angleraw_z1 >= 0 && velocity < -2000)velocity +=4095;
    else if(angleraw_z1 < 0 && velocity > 2000)velocity -=4095;
    velocity_z1 = velocity;

    return velocity;
}

int32_t GetVelcity_RPM(void){
  return ( ( GetVelcity_cnt() * 60 ) * SamplingFreq_Hz ) / AMT23_CPR;
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi){

    angleraw_z1 = angleraw;
    angleraw = 0x0FFF&((spirxbuf[0]<<5)|spirxbuf[1]>>3);
    //printb((uint16_t)((rxbuf[0]<<8)|rxbuf[1]));
    //printf(",%04d\n",angleraw);
}