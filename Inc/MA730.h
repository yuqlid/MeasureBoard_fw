/*
 * MA730.h
 *
 *  Created on: 2020/07/13
 *      Author: Yuki Kusakabe
 */

#ifndef __MA730_H_
#define __MA730_H_

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
    RD              = 0x09,
    MGH_MGL         = 0x1B,
} MA730_RegAddr;

#ifdef __cplusplus
}
#endif

#endif /* __MA730_H_ */
