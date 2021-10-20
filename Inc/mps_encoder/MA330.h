/*
 * MA330.h
 *
 *  Created on: 2020/07/13
 *      Author: Yuki Kusakabe
 */

#ifndef __MA330_H_
#define __MA330_H_

#ifdef __cplusplus
 extern "C" {
#endif

#define ENCODER_NAME    "MA330"

#define CMD_W   0x8000
#define CMD_R   0x4000

typedef enum {
    CW  = 0,
    CCW = 1,
} Rotate_Dir;

typedef enum {
    Z_7_0           = 0x0000,
    Z_15_8          = 0x0100,
    BCT             = 0x0200,
    ETY_ETX         = 0x0300,
    PPT_1_0_ILIP    = 0x0400,
    PPT_9_2         = 0x0500,
    MGLT_MGHT       = 0x0600,
    NPP             = 0x0700,
    RD              = 0x0900,
    FW              = 0x0E00,
    HYS             = 0x1000,
    MGH_MGL         = 0x1B00,
} MA330_RegAddr;

#ifdef __cplusplus
}
#endif

#endif /* __MA330_H_ */
