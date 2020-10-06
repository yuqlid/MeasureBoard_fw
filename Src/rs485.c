/*
 *  rs485.c
 *
 *  Created on: 2018/09/23
 *      Author: Yuki
 */

#include "rs485.h"
#include "main.h"
#include "usart.h"

uint8_t Packet[128] = {0};

void RS485_Transmit(uint8_t ID, uint8_t addr, uint8_t *TxData, uint8_t size){

    uint8_t parity = 0x00;
    //uint8_t Packet[6 + size];
    
    //for(uint8_t i = 0; 6 + size; i++)Packet[i] = 0;

    Packet[0] = 0xFA;
    Packet[1] = 0xAF;
    Packet[2] = ID;
    Packet[3] = addr;
    Packet[4] = size;

    for(uint8_t i = 0; i < size; i++)Packet[i + 5] = TxData[i];
    for(uint8_t i = 2; i < (5 + size); i++)parity ^= Packet[i];

    Packet[5 + size] = parity;

    HAL_UART_Transmit_DMA(&huart3, Packet, 6 + size);

}
