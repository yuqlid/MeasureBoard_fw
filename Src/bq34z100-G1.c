/*
 * bq34z100-G1.c
 *
 *  Created on: 2020/10/10
 *      Author: Yuki Kusakabe
 */

#include "bq34z100-G1.h"
#include "i2c.h"


void BQ34Z100G1_UNSEAL(void){

    uint8_t data[2];

    data[0] = 0x14;
    data[1] = 0x04;
    HAL_I2C_Mem_Write(&hi2c1, BQ34Z100G1_I2C_ADDR << 1, CONTROL, I2C_MEMADD_SIZE_8BIT, data, 2, 1000);
    data[0] = 0x72;
    data[1] = 0x36;
    HAL_I2C_Mem_Write(&hi2c1, BQ34Z100G1_I2C_ADDR << 1, CONTROL, I2C_MEMADD_SIZE_8BIT, data, 2, 1000);

}

void BQ34Z100G1_FULLACCESS(void){

    uint8_t data[2] = {0xFF, 0xFF};

    HAL_I2C_Mem_Write(&hi2c1, BQ34Z100G1_I2C_ADDR << 1, CONTROL, I2C_MEMADD_SIZE_8BIT, data, 2, 1000);
    HAL_I2C_Mem_Write(&hi2c1, BQ34Z100G1_I2C_ADDR << 1, CONTROL, I2C_MEMADD_SIZE_8BIT, data, 2, 1000);

}

void BQ34Z100G1_RESET(void){

    uint8_t data[2] = {0x00, 0x00};
    *(bq34z100G1_ControlSubcommands *)data = DEVICE_RESET;

    HAL_I2C_Mem_Write(&hi2c1, BQ34Z100G1_I2C_ADDR << 1, CONTROL, I2C_MEMADD_SIZE_8BIT, data, 2, 1000);
}

void BQ34Z100G1_SEAL(void){

    uint8_t data[2] = {0x00, 0x00};
    *(bq34z100G1_ControlSubcommands *)data = SEALED;

    HAL_I2C_Mem_Write(&hi2c1, BQ34Z100G1_I2C_ADDR << 1, CONTROL, I2C_MEMADD_SIZE_8BIT, data, 2, 1000);

}

void BQ34Z100G1_BlockDataControl(void){

    uint8_t txdata;
    txdata = 0;
    HAL_I2C_Mem_Write(&hi2c1, BQ34Z100G1_I2C_ADDR << 1, DFDCNTL, I2C_MEMADD_SIZE_8BIT, &txdata, 1, 1000);
    
}

void BQ34Z100G1_DataFlashClass(void){
    
    uint8_t txdata;
    txdata = 0x40;
    HAL_I2C_Mem_Write(&hi2c1, BQ34Z100G1_I2C_ADDR << 1, DFCLS, I2C_MEMADD_SIZE_8BIT, &txdata, 1, 1000);
    
}

void BQ34Z100G1_DataFlashBlock(void){

    uint8_t txdata;
    txdata = 0;
    HAL_I2C_Mem_Write(&hi2c1, BQ34Z100G1_I2C_ADDR << 1, DFBLK, I2C_MEMADD_SIZE_8BIT, &txdata, 1, 1000);
    
}

uint16_t BQ34Z100G1_GetTemprature10degreeCelsius(void){

    uint8_t RxData[2];
    uint16_t temp;

    HAL_I2C_Mem_Read(&hi2c1, BQ34Z100G1_I2C_ADDR << 1, TEMP, I2C_MEMADD_SIZE_8BIT, RxData, 2, 1000);
    temp = *(uint16_t *)RxData;
	temp -= 2731;

    return temp;
}
