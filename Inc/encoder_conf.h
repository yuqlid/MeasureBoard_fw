/*
 *  encoder_conf.h
 *
 *  Created on: 2020/11/05
 *      Author: Yuki
 */

#ifndef __ENCODER_CONF_H
#define __ENCODER_CONF_H
#ifdef __cplusplus
 extern "C" {
#endif


#if !defined(HW_SOURCE) && !defined(HW_HEADER)

#define ENC_HEADER  "MA730.h"
#define ENC_SOURCE  "MA730.c"

//#define ENC_HEADER  "MA732.h"
//#define ENC_SOURCE  "MA730.c"

//#define ENC_HEADER  "MA330.h"
//#define ENC_SOURCE  "MA330.c"

#endif

#ifndef ENC_SOURCE
#error "No encoder source file set"
#endif

#ifndef ENC_HEADER
#error "No encoder header file set"
#endif

#include "MA7xx.h"


#ifdef __cplusplus
}
#endif
#endif /* __ENCODER_CONF_H */
