/*
 *  MA702.c
 *
 *  Created on: 2020/11/29
 *      Author: Yuki
 */

#include "spi.h"
#include "MA7xx.h"
#include "MA702.h"
#include "cmsis_os.h"

void Encoder_Init_MA7xx(void){

    MA7xx_GetAngle = MA702_GetAngle;
    MA7xx_ReadRegister = MA702_ReadResiter;
    MA7xx_WriteRegister = MA702_WriteResiter;
    MA7xx_ReadPPT = MA702_ReadPPT;
    MA7xx_ReadRotateDir = MA702_ReadRotateDir;
    delay = osDelay;
    
}

uint16_t MA702_GetAngle(void){
    uint16_t angle = 0;
    HAL_SPI_Receive(&hspi1, &angle, 1, 100);
    return angle;
}

bool MA702_ReadRotateDir(void){

    return (MA702_ReadResiter(RD >> 8) >> 7);
}

bool MA702_ReadETX(void){

    return (0x01 & MA702_ReadResiter(ETY_ETX >> 8));
}

bool MA702_ReadETY(void){

    return (0x01 & (MA702_ReadResiter(ETY_ETX >> 8) >> 1));
}

uint16_t MA702_ReadPPT(void){
    
    uint16_t PPT = 0;
    PPT =  MA702_ReadResiter(PPT_1_0_ILIP >> 8) >> 6;
    PPT |=  MA702_ReadResiter(PPT_7_2 >> 8) << 2;
    
    return PPT;
}

uint8_t MA702_ReadBCT(void){

    return MA702_ReadResiter(BCT >> 8);

}

uint8_t MA702_ReadResiter(uint8_t Addr){

    uint16_t tx[2] = {CMD_R | (0xFF00 & (Addr << 8)), 0};
    uint16_t rx[2] = {0};
    HAL_SPI_TransmitReceive(&hspi1, &tx, &rx, 2, 100);

	return ((rx[1] & 0xFF00) >> 8);
}

uint8_t MA702_WriteResiter(uint8_t Addr, uint8_t data){

    uint16_t tx = CMD_W | (0xFF00 & (Addr << 8)) |  (0x00FF & data);
    uint16_t rx = 0;
    HAL_SPI_TransmitReceive(&hspi1, &tx, &rx, 1, 100);
    delay(20);
    HAL_SPI_Receive(&hspi1, &rx, 1, 100);
	return (0xFF & (rx >> 8));
}