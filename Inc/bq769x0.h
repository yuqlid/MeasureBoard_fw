/*
 *  bq769x0.h
 *
 *  Created on: 2021/07/03
 *      Author: Yuki Kusakabe
 */

#ifndef __BQ769X0_H_
#define __BQ769X0_H_

#include "stdint.h"

#ifdef __cplusplus
 extern "C" {
#endif

//#define BQ769X0_I2C_ADDR    0x08
#define BQ769X0_I2C_ADDR    0x18

typedef enum {
    SYS_STAT    = 0x00,
    CELLBAL1    = 0x01,
    CELLBAL2    = 0x02,
    CELLBAL3    = 0x03,
    SYS_CTRL1   = 0x04,
    SYS_CTRL2   = 0x05,
    PROTECT1    = 0x06,
    PROTECT2    = 0x07,
    PROTECT3    = 0x08,
    OV_TRIP     = 0x09,
    UV_TRIP     = 0x0A,
    CC_CFG      = 0x0B,
    VC1_HI      = 0x0C,
    VC1_LO      = 0x0D,
    VC2_HI      = 0x0E,
    VC2_LO      = 0x0F,
    VC3_HI      = 0x10,
    VC3_LO      = 0x0F,
    VC4_HI      = 0x12,
    VC4_LO      = 0x13,
    VC5_HI      = 0x14,
    VC5_LO      = 0x15,
    VC6_HI      = 0x16,
    VC6_LO      = 0x17,
    VC7_HI      = 0x18,
    VC7_LO      = 0x19,
    VC8_HI      = 0x1A,
    VC8_LO      = 0x1B,
    VC9_HI      = 0x1C,
    VC9_LO      = 0x1D,
    VC10_HI     = 0x1E,
    VC10_LO     = 0x1F,
    VC11_HI     = 0x20,
    VC11_LO     = 0x21,
    VC12_HI     = 0x22,
    VC12_LO     = 0x23,
    VC13_HI     = 0x24,
    VC13_LO     = 0x25,
    VC14_HI     = 0x26,
    VC14_LO     = 0x27,
    VC15_HI     = 0x28,
    VC15_LO     = 0x29,
    BAT_HI      = 0X2A,
    BAT_LO      = 0x2B,
    TS1_HI      = 0x2C,
    TS1_LO      = 0x2D,
    TS2_HI      = 0x2E,
    TS2_LO      = 0x2F,
    TS3_HI      = 0x30,
    TS3_LO      = 0x31,
    CC_HI       = 0x32,
    CC_LO       = 0x33,
    ADCGAIN1    = 0x50,
    ADCOFFSET   = 0x51,
    ADCGAIN2    = 0x59,
} bq769X0_RegisterMaps;

void BQ769X0_Read(uint8_t addr, uint8_t *pData, uint16_t len);
void BQ769X0_Write(uint8_t addr, uint8_t *pData, uint16_t len);
uint8_t BQ769X0_READ_SYS_CTRL2(void);


#ifdef __cplusplus
}
#endif

#endif /* __BQ769X0_H_ */
