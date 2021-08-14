/*
 * bq78350.c
 *
 *  Created on: 2021/08/04
 *      Author: Yuki Kusakabe
 */

#include "i2c.h"
#include "bq78350.h"

void BQ78350_ReadByte(uint8_t addr, uint8_t *pData){

    BQ78350_ReadBlock(addr, (uint8_t *)pData, 1) ;
}

void BQ78350_WriteByte(uint8_t addr, uint8_t *pData){

    BQ78350_WriteBlock(addr, (uint8_t *)pData, 1) ;
}

void BQ78350_ReadWord(uint8_t addr, uint16_t *pData){

    BQ78350_ReadBlock(addr, (uint8_t *)pData, 2) ;
}

void BQ78350_WriteWord(uint8_t addr, uint16_t *pData){

    BQ78350_WriteBlock(addr, (uint8_t *)pData, 2) ;
}

void BQ78350_ReadBlock(uint8_t addr, uint8_t *pData, uint16_t len){

    uint8_t send_addr = addr;
    HAL_SMBUS_Master_Transmit_IT(&hsmbus1, BQ78350_I2C_ADDR, &send_addr, 1, SMBUS_FIRST_FRAME);
    while(HAL_SMBUS_GetState(&hsmbus1) != HAL_SMBUS_STATE_READY);
    HAL_SMBUS_Master_Receive_IT(&hsmbus1, BQ78350_I2C_ADDR, pData, len, SMBUS_LAST_FRAME_NO_PEC);
    while(HAL_SMBUS_GetState(&hsmbus1) != HAL_SMBUS_STATE_READY);

}

void BQ78350_WriteBlock(uint8_t addr, uint8_t *pData, uint16_t len){

   // HAL_I2C_Mem_Write(&hi2c1, BQ78350_I2C_ADDR, addr, I2C_MEMADD_SIZE_8BIT, (uint8_t *)pData, len, 1000);
}
