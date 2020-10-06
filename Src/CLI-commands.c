/*
 *  CLI-commands.c
 *
 *  Created on: 2020/08/02
 *      Author: Yuki
 */

#include "CLI-commands.h"

#include "MeasurementBoard_v1.h"
#include "scramble_tasks.h"

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
			strncat( pcWriteBuffer, (const char *)("\r\n"), strlen( " rpm\r\n" ) );

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

    data = 0x01;
	RS485_Transmit(0x10, 0x30, &data, 1);
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

void vRegisterScrambleCLICommands( void )
{
	/* Register all the command line commands defined immediately above. */
	FreeRTOS_CLIRegisterCommand( &xParameterRS485Periodic );
	FreeRTOS_CLIRegisterCommand( &xParameterSetTargetSPeed );
	FreeRTOS_CLIRegisterCommand( &xParameterEncoderCalibrate );
}
