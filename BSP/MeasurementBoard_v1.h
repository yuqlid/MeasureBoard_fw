/*
 *  MeasurementBoard_v1.h
 *
 *  Created on: 2018/12/02
 *      Author: Yuki
 */

#ifndef __MEASUREMENTBOARD_V1
#define __MEASUREMENTBOARD_V1
#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h" //CubeMXでピンをUserdefienしたため読んでる．今後はCubeMXで定義せずにBSPのヘッダファイルで定義したほうが良いかもしれない
#include "gpio.h"

typedef enum
{
  LED0 = 0,
  LED1 = 1,
  LED2 = 2
} LED_TypeDef;

void LED_On(LED_TypeDef LED);
void LED_Off(LED_TypeDef LED);
void LED_Toggle(LED_TypeDef LED);

#ifdef __cplusplus
}
#endif
#endif /* MEASUREMENTBOARD_V1 */