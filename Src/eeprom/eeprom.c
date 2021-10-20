/*
 * eeprom.c
 *
 *  Created on: 2021/02/14
 *      Author: Yuki Kusakabe
 */

#include "eeprom.h"
#include "i2c.h"

void EEPROM_Read(uint16_t addr, uint8_t *pData, uint16_t len){

    HAL_I2C_Mem_Read(&hi2c1, EEPROM_24FC512_I2C_ADDR << 1, addr, I2C_MEMADD_SIZE_16BIT, pData, len, 1000);
}

void EEPROM_Write(uint16_t addr, uint8_t *pData, uint16_t len){

    HAL_I2C_Mem_Write(&hi2c1, EEPROM_24FC512_I2C_ADDR << 1, addr, I2C_MEMADD_SIZE_16BIT, pData, len, 1000);
}


void PDIControl_Read(void){

    uint8_t data;

    data = 0x00;
    EEPROM_Read(PDICONTROL, data, 1);

}