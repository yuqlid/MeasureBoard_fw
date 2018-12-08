/*
 *  MeasurementBoard_v1.c
 *
 *  Created on: 2018/12/02
 *      Author: Yuki
 */

#include "MeasurementBoard_v1.h"

void LED_On(LED_TypeDef LED){

    if(LED == LED0) HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
    if(LED == LED1) HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
    if(LED == LED2) HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
}

void LED_Off(LED_TypeDef LED){

    if(LED == LED0) HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
    if(LED == LED1) HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
    if(LED == LED2) HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
}

void LED_Toggle(LED_TypeDef LED){

    if(LED == LED0) HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
    if(LED == LED1) HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
    if(LED == LED2) HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
}
