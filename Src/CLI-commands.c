/*
 *  CLI-commands.c
 *
 *  Created on: 2020/08/02
 *      Author: Yuki
 */

#include "CLI-commands.h"

#include "MeasurementBoard_v1.h"
#include "scramble_tasks.h"
#include "can.h"
#include "i2c.h"
#include "xprintf.h"
#include "bq34z100-G1.h"

#include <stdio.h>

extern osThreadId rs485TransmitTaskHandle;


static BaseType_t prvPS485PeriodicCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	//const char *pcParameter;
	// xParameterStringLength;
	BaseType_t xReturn;
	//static UBaseType_t uxParameterNumber = 0;
	static bool state = false;

	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );
    
    if(state){
        state = false;
        sprintf( pcWriteBuffer, "RS485_periodicSend : False\r\n" );
        osThreadSuspend(rs485TransmitTaskHandle);
		osThreadSuspend(COMSendTaskHandle);
        LED_Off(LED2);
    }else{
        state = true;
        sprintf( pcWriteBuffer, "RS485_periodicSend : True\r\n" );
        osThreadResume(rs485TransmitTaskHandle);
		osThreadResume(COMSendTaskHandle);
    }
    // まだ出力する処理が残ってたらpdTRUEにする．
    xReturn = pdFALSE;
	return xReturn;
}

static BaseType_t prvSetTargetSpeedCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	const char *pcParameter;
	BaseType_t xParameterStringLength;
	BaseType_t xReturn;
	static UBaseType_t uxParameterNumber = 0;
	long speed = 0;

	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );
    
	if(uxParameterNumber == 0){
		sprintf( pcWriteBuffer, "New Target Speed :" );
		uxParameterNumber = 1;
		xReturn = pdPASS;
	}else{
		/* Obtain the parameter string. */
		pcParameter = FreeRTOS_CLIGetParameter(
							pcCommandString,		/* The command string itself. */
							uxParameterNumber,		/* Return the next parameter. */
							&xParameterStringLength	/* Store the parameter string length. */
		);

		if(pcParameter != NULL){
			/* Return the parameter string. */
			char data[] = " rpm";
			memset( pcWriteBuffer, 0x00, xWriteBufferLen );
			//sprintf( pcWriteBuffer, "%d rpm ", ( int ) uxParameterNumber );
			strncat( pcWriteBuffer, ( char * ) pcParameter, ( size_t ) xParameterStringLength );
			strncat( pcWriteBuffer, ( char * ) data, strlen( data ) );
			strncat( pcWriteBuffer, (const char *)("\r\n"), strlen( "\r\n" ) );

			speed = strtol(pcParameter, NULL , 10);
			setTargetSpeed(&speed);

			/* There might be more parameters to return after this one. */
			xReturn = pdTRUE;
			uxParameterNumber++;

		}else{
			/* No more parameters were found.  Make sure the write buffer does
			not contain a valid string. */
			pcWriteBuffer[ 0 ] = 0x00;

			/* No more data to return. */
			xReturn = pdFALSE;

			/* Start over the next time this command is executed. */
			uxParameterNumber = 0;
		}
	}

	return xReturn;
}

static BaseType_t prvEncoderCalibrateCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	//const char *pcParameter;
	// xParameterStringLength;
	BaseType_t xReturn;
	uint8_t data = 0;
	//static UBaseType_t uxParameterNumber = 0;
	static bool state = false;

	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	sprintf( pcWriteBuffer, "Enocder Calibration\r\n" );

    data = 0x01;
	RS485_Transmit(0x10, 0x30, &data, 1);
    xReturn = pdFALSE;
	return xReturn;
}

static BaseType_t prvCanTest( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	//const char *pcParameter;
	// xParameterStringLength;
	BaseType_t xReturn;
	uint8_t data = 0;
	//static UBaseType_t uxParameterNumber = 0;
	static bool state = false;

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

static BaseType_t prvBattInfo( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	//const char *pcParameter;
	//BaseType_t xParameterStringLength;
	BaseType_t xReturn;
	//static UBaseType_t uxParameterNumber = 0;

	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	uint8_t		RxData[2];
	uint16_t 	temeprature;
	uint16_t 	temeprature2;
	uint16_t 	RxData16;
	char tempstr[10] = {0};

	HAL_I2C_Mem_Read(&hi2c1, BQ34Z100G1_I2C_ADDR << 1, 0x0C, I2C_MEMADD_SIZE_8BIT, RxData, 2, 1000);

	RxData16 = *(uint16_t *)RxData;

	RxData16 -= 2731;

	temeprature = RxData16 / 10;
	temeprature2 = RxData16 % 10;

	xsprintf(tempstr, "%d.%d", temeprature, temeprature2);

	sprintf( pcWriteBuffer, "Info : " );
	strncat( pcWriteBuffer, ( char * ) tempstr, strlen( tempstr ) );
	strncat( pcWriteBuffer, (const char *)(" degC\r\n"), strlen( " degC\r\n" ) );

	xReturn = pdFALSE;
	return xReturn;
}

static const CLI_Command_Definition_t xParameterRS485Periodic =
{
	"com_periodic",
	"\r\ncom_periodic:\r\n Switch RS485 transmit command Periodic of Not\r\n",
	prvPS485PeriodicCommand, /* The function to run. */
	0 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xParameterSetTargetSPeed =
{
	"speed",
	"\r\nspeed:\r\n Set Target Speed [rpn]\r\n",
	prvSetTargetSpeedCommand, /* The function to run. */
	1 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xParameterEncoderCalibrate =
{
	"calib",
	"\r\ncalib:\r\n Calibrate Encoder Offset\r\n",
	prvEncoderCalibrateCommand, /* The function to run. */
	0 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xParameterCanTest =
{
	"can",
	"\r\ncan:\r\n Transmit Can Test Message\r\n",
	prvCanTest, /* The function to run. */
	0 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xParameterBattInfo =
{
	"batt",
	"\r\nbatt:\r\n Show bq34z100-G1 info\r\n",
	prvBattInfo, /* The function to run. */
	0 /* No parameters are expected. */
};


void vRegisterScrambleCLICommands( void )
{
	/* Register all the command line commands defined immediately above. */
	FreeRTOS_CLIRegisterCommand( &xParameterRS485Periodic );
	FreeRTOS_CLIRegisterCommand( &xParameterSetTargetSPeed );
	FreeRTOS_CLIRegisterCommand( &xParameterEncoderCalibrate );
	FreeRTOS_CLIRegisterCommand( &xParameterCanTest );
	FreeRTOS_CLIRegisterCommand( &xParameterBattInfo );
}