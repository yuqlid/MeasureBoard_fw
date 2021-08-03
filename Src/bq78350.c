/*
 * bq78350.c
 *
 *  Created on: 2021/08/04
 *      Author: Yuki Kusakabe
 */

#include "i2c.h"
#include "bq78350.h"

void BQ78350_Read(uint8_t addr, uint8_t *pData, uint16_t len){
    
    HAL_I2C_Mem_Read(&hi2c1, BQ78350_I2C_ADDR, addr, I2C_MEMADD_SIZE_8BIT, (uint8_t *)pData, 2 * len, 1000);
}

void BQ78350_Write(uint8_t addr, uint8_t *pData, uint16_t len){

    HAL_I2C_Mem_Read(&hi2c1, BQ78350_I2C_ADDR, addr, I2C_MEMADD_SIZE_8BIT, (uint8_t *)pData, 2 * len, 1000);
}

