/*
 * uart_util_hal.c
 *
 *  Created on: 2018/05/08
 *      Author: Yuki
 */
// http://www.eevblog.com/forum/microcontrollers/_best_-way-to-load-uart-data-to-ring-buffer-with-stm32hal/msg1071292/#msg1071292

#include "main.h"
#include "uart_util_hal.h"
#include "cmsis_os.h"

extern UART_HandleTypeDef huart3;
static UART_HandleTypeDef *huart_print;

static UART_Buffer print_tx_buf;

static uint8_t  print_rx_buf[UART_BUFFER_SIZE] __attribute__ ((aligned (8)));
static uint32_t print_rx_index;

#if defined(STM32F303xC)
#define DMA_WRITE_PTR(hu) ( (UART_BUFFER_SIZE - hu->hdmarx->Instance->CNDTR) & (UART_BUFFER_SIZE - 1) )
#endif

void UART_Util_Init(UART_HandleTypeDef *huart){

  huart_print = huart;
  HAL_UART_Receive_DMA(huart_print, (uint8_t *)print_rx_buf, UART_BUFFER_SIZE);
  print_rx_index = 0;
  print_tx_buf.head = 0;
  print_tx_buf.tail = 0;
}

static bool msgrx_circ_buf_is_empty(UART_HandleTypeDef *huart, uint32_t *rx_index) {

  if( *rx_index == DMA_WRITE_PTR(huart)){
      return true;
  }
  return false;
}

static uint8_t msgrx_circ_buf_get(UART_HandleTypeDef *huart, uint8_t *rx_buf, uint32_t *rx_index) {

  uint8_t c = 0;

  if( *rx_index != DMA_WRITE_PTR(huart)){
      c = rx_buf[(*rx_index)++];
      *rx_index &= (UART_BUFFER_SIZE - 1);
  }
  return c;
}

static uint16_t UART_GetBufferSize(UART_Buffer *buf){

  if (buf->tail == buf->head){
    return 0;
  }
  else if ((buf->tail) < (buf->head)){
    return (UART_BUFFER_SIZE - buf->head) + buf->tail;
  }
  return (buf->tail - buf->head);
}

static int16_t UART_WriteBuffer(UART_Buffer *buf, uint8_t data){
    while (UART_GetBufferSize(buf) >= (UART_BUFFER_SIZE - 2)){  //??
      HAL_Delay(1);
    }
    __disable_irq();
    buf->data[buf->tail] = data;
    buf->tail = (buf->tail + 1) % UART_BUFFER_SIZE;
    __enable_irq();
    return 1;
}

int16_t UART_Util_SendByte(UART_HandleTypeDef *huart, UART_Buffer *buf, uint8_t data){

  int16_t ret, len;
  ret = UART_WriteBuffer(buf, data);

  __disable_irq();
  if(huart->gState == HAL_UART_STATE_READY){
    // Transmit size
    len = UART_GetBufferSize(buf);
    if(buf->head + len > UART_BUFFER_SIZE){
        len = UART_BUFFER_SIZE - buf->head;
    }
    // Transmit
    if(HAL_UART_Transmit_DMA(huart, &(buf->data[buf->head]), len) == HAL_OK){
        buf->pending_size = len;
    }
  }
  __enable_irq();
  return ret;
}
/*
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {

  UART_Buffer *tx_buffer;
  uint16_t len = 0;

  if (huart->Instance == huart_print->Instance){
      tx_buffer = &print_tx_buf;
  }else{
      return;
  }

  // Transmitted Size
  len = tx_buffer->pending_size;

  // head
  tx_buffer->head = (tx_buffer->head + len) % UART_BUFFER_SIZE;

  // Transmit size
  len = UART_GetBufferSize(tx_buffer);
  if(len == 0){
    // complete
    return;
  }
  if(tx_buffer->head + len > UART_BUFFER_SIZE){
      len = UART_BUFFER_SIZE - tx_buffer->head;
  }

  if(HAL_UART_Transmit_DMA(huart, &(tx_buffer->data[tx_buffer->head]), len) == HAL_OK){
      tx_buffer->pending_size = len;
  }
}
*/

/**************************************************************************/
/*!
    High Level function.
 */
/**************************************************************************/
/* Send 1 character */
int putch(uint8_t data){

    return UART_Util_SendByte(huart_print, &print_tx_buf, data);
}

/* Receive 1 character */
uint8_t getch(void)
{
    if ( !msgrx_circ_buf_is_empty(huart_print, &print_rx_index) )
        return msgrx_circ_buf_get(huart_print, print_rx_buf, &print_rx_index);
    else
        return false;
}
/*
int __io_putchar(int ch){
    return putch(ch);
}
*/
int __io_getchar(void){
    while( msgrx_circ_buf_is_empty(huart_print, &print_rx_index) ){
        //HAL_Delay(1);
        osDelay(1);
    }
    return msgrx_circ_buf_get(huart_print, print_rx_buf, &print_rx_index);
}

