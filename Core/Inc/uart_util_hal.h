/*
 * uart_util_hal.h
 *
 *  Created on: 2018/05/08
 *      Author: Yuki
 */
// http://www.eevblog.com/forum/microcontrollers/_best_-way-to-load-uart-data-to-ring-buffer-with-stm32hal/msg1071292/#msg1071292

#ifndef INC_UART_UTIL_HAL_H_
#define INC_UART_UTIL_HAL_H_

#include "stm32f3xx_hal.h"
#include <stdbool.h>
#include <stdint.h>

#define UART_BUFFER_SIZE       256  /* must be power of two */

typedef struct
{
    volatile uint16_t head;
    volatile uint16_t tail;
    volatile uint16_t pending_size;
    uint8_t data[UART_BUFFER_SIZE] __attribute__ ((aligned (8)));
} UART_Buffer;


//General function
void UART_Util_Init(UART_HandleTypeDef *huart);

int16_t UART_Util_SendByte(UART_HandleTypeDef *huart, UART_Buffer *buf, uint8_t data);

//High Level function.
int putch(uint8_t data);
uint8_t getch(void);


#endif /* INC_UART_UTIL_HAL_H_ */
