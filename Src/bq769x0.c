/*
 * bq769x0.c
 *
 *  Created on: 2021/07/03
 *      Author: Yuki Kusakabe
 */

#include "bq769x0.h"
#include "i2c.h"

void BQ769X0_Read(uint8_t addr, uint8_t *pData, uint16_t len){

    //HAL_I2C_Mem_Read(&hi2c1, BQ769X0_I2C_ADDR << 1, addr, I2C_MEMADD_SIZE_8BIT, pData, len, 1000);
}

void BQ769X0_Write(uint8_t addr, uint8_t *pData, uint16_t len){

    //HAL_I2C_Mem_Write(&hi2c1, BQ769X0_I2C_ADDR << 1, addr, I2C_MEMADD_SIZE_8BIT, pData, len, 1000);
}

uint8_t BQ769X0_READ_SYS_CTRL2(void){

    uint8_t data = 0x00;

    BQ769X0_Read(SYS_CTRL2, &data, 1);

    return data;
}
