/*
 *  bq78350.h
 *
 *  Created on: 2021/08/04
 *      Author: Yuki Kusakabe
 */

#ifndef __BQ78350_H_
#define __BQ78350_H_

#include "stdint.h"

#ifdef __cplusplus
 extern "C" {
#endif

#define BQ78350_I2C_ADDR    0x16

void BQ78350_Read(uint8_t addr, uint8_t *pData, uint16_t len);
void BQ78350_Write(uint8_t addr, uint8_t *pData, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif /* __BQ78350_H_ */
