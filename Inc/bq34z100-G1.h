/*
 *  bq34z100-G1.h
 *
 *  Created on: 2020/10/10
 *      Author: Yuki Kusakabe
 */

#ifndef __BQ43Z100_G1_H_
#define __BQ43Z100_G1_H_

#include "stdint.h"

#ifdef __cplusplus
 extern "C" {
#endif

#define BQ34Z100G1_I2C_ADDR 0b1010101

typedef enum {
    CONTROL_STATUS      = 0x0000,
    DEVICE_TYPE         = 0x0001,
    FW_VERSION          = 0x0002,
    HW_VERSION          = 0x0003,
    RESET_DATA          = 0x0005,
    PREV_MACWRITE       = 0x0007,
    CHEM_ID             = 0x0008,
    BOARD_OFFSET        = 0x0009,
    CC_OFFSET           = 0x000A,
    CC_OFFSET_SAVE      = 0x000B,
    DF_VERSION          = 0x000C,
    SET_FULLSLEEP       = 0x0010,
    STATIC_CHEM_CHKSUM  = 0x0017,
    SEALED              = 0x0020,
    IT_ENABLE           = 0x0021,
    CAL_ENABLE          = 0x002D,
    DEVICE_RESET        = 0x0041,
    EXIT_CAL            = 0x0080,
    ENTER_CAL           = 0x0081,
    OFFSET_CAL          = 0x0082,
    
} bq34z100G1_ControlSubcommands;

#ifdef __cplusplus
}
#endif

#endif /* __BQ43Z100_G1_H_ */
