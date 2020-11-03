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

typedef enum {
    CW  = 0,
    CCW = 1,
} Rotate_Dir;

typedef enum {
    Z_7_0           = 0x00,
    Z_15_8          = 0x01,
    BCT             = 0x02,
    ETY_ETX         = 0x03,
    PPT_1_0_ILIP    = 0x04,
    PPT_9_2         = 0x05,
    MGLT_MGHT       = 0x06,
    NPP             = 0x07,
    RD              = 0x09,
    FW              = 0x0E,
    HYS             = 0x10,
    MGH_MGL         = 0x1B,
} MA330_RegAddr;

#ifdef __cplusplus
}
#endif

#endif /* __MA330_H_ */
