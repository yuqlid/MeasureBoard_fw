/*
 *  eeprom-G1.h
 *
 *  Created on: 2021/02/14
 *      Author: Yuki Kusakabe
 */

#ifndef __EEPROM_H_
#define __EEPROM_H_

#include "stdint.h"

#ifdef __cplusplus
 extern "C" {
#endif

#define EEPROM_24FC512_I2C_ADDR 0b1010000

typedef enum {
    PDICONTROL  = 0x140,
    ESCCONFIG   = 0x141,
    PDICONFIG   = 0x150,
    SYNCLATCHCONFIG   = 0x151,
    SSPULSELEN1   = 0x982,
    SSPULSELEN2   = 0x983,
    EXPDICONFIG1   = 0x152,
    EXPDICONFIG2   = 0x153,
    STATIONALIAS1   = 0x012,
    STATIONALIAS2   = 0x013,
    MIICONTROL1   = 0x510,
    MIICONTROL2   = 0x511,
    ASICCONFIG1 = 0x142,
    ASICCONFIG2 = 0x143,
    RESERVED1   = 0x144,
    RESERVED2   = 0x145,
} EEPROM_Commands;

void EEPROM_Read(uint16_t addr, uint8_t *pData, uint16_t len);
void EEPROM_Write(uint16_t addr, uint8_t *pData, uint16_t len);

void PDIControl_Read(void);


#ifdef __cplusplus
}
#endif

#endif /* __EEPROM_H_ */
