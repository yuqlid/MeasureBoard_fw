/*
 *  CLI-commands.c
 *
 *  Created on: 2020/08/02
 *      Author: Yuki
 */

#include "CLI-commands.h"

#include "MeasurementBoard_v1.h"

static BaseType_t prvCanTest( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	//const char *pcParameter;
	// xParameterStringLength;
	BaseType_t xReturn;
	//uint8_t data = 0;
	//static UBaseType_t uxParameterNumber = 0;
	//static bool state = false;

	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	CAN_TxHeaderTypeDef   TxHeader;
	uint8_t               TxData[8];
	uint32_t              TxMailbox;

	TxHeader.StdId = 0x01;
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.DLC = 8;
	TxHeader.TransmitGlobalTime = DISABLE;
	TxData[0] = 0x00;
	TxData[1] = 0x01;
	TxData[2] = 0x02;
	TxData[3] = 0x03;
	TxData[4] = 0x04;
	TxData[5] = 0x05;
	TxData[6] = 0x06;
	TxData[7] = 0x07;

	HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox);

	sprintf( pcWriteBuffer, "CAN Test Message\r\n" );

    xReturn = pdFALSE;
	return xReturn;
}


static const CLI_Command_Definition_t xParameterCanTest =
{
	"can",
	"\r\ncan:\r\n Transmit Can Test Message\r\n",
	prvCanTest, /* The function to run. */
	0 /* No parameters are expected. */
};


void vRegisterCLICommands( void )
{
	/* Register all the command line commands defined immediately above. */
	FreeRTOS_CLIRegisterCommand( &xParameterCanTest );
}
