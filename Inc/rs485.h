/*
 *  rs485.h
 *
 *  Created on: 2020/07/17
 *      Author: Yuki
 */

#ifndef __RS485
#define __RS485
#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

void RS485_Transmit(uint8_t ID, uint8_t addr, uint8_t *TxData, uint8_t size);

#ifdef __cplusplus
}
#endif
#endif /* __RS485 */
