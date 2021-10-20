/*
 *  CLI-commands.c
 *
 *  Created on: 2020/08/02
 *      Author: Yuki
 */

#include "CLI-commands_Scramble.h"

#include "MeasurementBoard_v1.h"
#include "scramble_tasks.h"

extern osThreadId rs485TransmitTaskHandle;
extern osThreadId rs485DribbleTaskHandle;

static BaseType_t prvDribblePeriodicCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
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
        sprintf( pcWriteBuffer, "RS485_Dribbled : False\r\n" );
        osThreadSuspend(rs485DribbleTaskHandle);
        LED_Off(LED2);
    }else{
        state = true;
        sprintf( pcWriteBuffer, "RS485_Dribble : True\r\n" );
        osThreadResume(rs485DribbleTaskHandle);
    }
    // まだ出力する処理が残ってたらpdTRUEにする．
    xReturn = pdFALSE;
	return xReturn;
}

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
		sprintf( pcWriteBuffer, "New Target Velocity :" );
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

static BaseType_t prvSetNerIDCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	const char *const pcHeader = "Invalid Input\r\n";
	const char *const pcFooder = "\r\nNew ID is enabled after Reset.\r\n";
	const char *pcParameter;
	BaseType_t xParameterStringLength;
	BaseType_t xReturn;
	static UBaseType_t uxParameterNumber = 0;
	long send_id = 0;
	static long id = 0x10;
	static bool inputerr = false;
	uint8_t new_id;

	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );
    
	if(uxParameterNumber == 0){
		sprintf( pcWriteBuffer, "Override ID : " );
		uxParameterNumber = 1;
		xReturn = pdPASS;
	}else{
		/* Obtain the parameter string. */
		pcParameter = FreeRTOS_CLIGetParameter(
							pcCommandString,		/* The command string itself. */
							uxParameterNumber,		/* Return the next parameter. */
							&xParameterStringLength	/* Store the parameter string length. */
		);

		if(uxParameterNumber == 1){
			send_id = strtol(pcParameter, NULL , 10);
			memset( pcWriteBuffer, 0x00, xWriteBufferLen );
			if(send_id >= 0 && send_id <= 3){
				strncat( pcWriteBuffer, ( char * ) pcParameter, ( size_t ) xParameterStringLength );
				strncat( pcWriteBuffer, (const char *)(" to "), strlen( " to " ) );
				id += send_id;
			}else{
				strncat( pcWriteBuffer, pcHeader, strlen(pcHeader) );
				return pdFALSE;
			}
		}
		if(uxParameterNumber == 2){

			new_id = strtol(pcParameter, NULL , 10);
			if(new_id >= 0 && new_id <= 3){
				memset( pcWriteBuffer, 0x00, xWriteBufferLen );
				strncat( pcWriteBuffer, ( char * ) pcParameter, ( size_t ) xParameterStringLength );
				strncat( pcWriteBuffer, ( char * ) pcFooder, ( size_t ) pcFooder );
				new_id += 0x10;
				RS485_Transmit(id, 0x40, &new_id, 1);
				
			}else{
				memset( pcWriteBuffer, 0x00, xWriteBufferLen );
				strncat( pcWriteBuffer, pcHeader, strlen(pcHeader) );
			}
			xReturn = pdFALSE;
			id = 0x10;
			uxParameterNumber = 0;

		}else{
				xReturn = pdTRUE;
				uxParameterNumber++;
		}
	}
	return xReturn;
}

static BaseType_t prvSetDribbleDutyCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	const char *pcParameter;
	BaseType_t xParameterStringLength;
	BaseType_t xReturn;
	static UBaseType_t uxParameterNumber = 0;
	long duty = 0;

	/* Remove compile time warnings about unused parameters, and check the
	write buffer is not NULL.  NOTE - for simplicity, this example assumes the
	write buffer length is adequate, so does not check for buffer overflows. */
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );
    
	if(uxParameterNumber == 0){
		sprintf( pcWriteBuffer, "New Duty (-4095 ~ 4095) :" );
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
			char data[] = " d";
			memset( pcWriteBuffer, 0x00, xWriteBufferLen );
			//sprintf( pcWriteBuffer, "%d rpm ", ( int ) uxParameterNumber );
			strncat( pcWriteBuffer, ( char * ) pcParameter, ( size_t ) xParameterStringLength );
			strncat( pcWriteBuffer, ( char * ) data, strlen( data ) );
			strncat( pcWriteBuffer, (const char *)("\r\n"), strlen( "\r\n" ) );

			duty = strtol(pcParameter, NULL , 10);
			setDribbleDuty(&duty);

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
	//static bool state = false;

	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	sprintf( pcWriteBuffer, "Enocder Calibration\r\n" );

    data = 0x01;
	RS485_Transmit(0x10, 0x30, &data, 1);
    xReturn = pdFALSE;
	return xReturn;
}

static const CLI_Command_Definition_t xParameterRS485Periodic =
{
	"csv",
	"\r\ncsv:\r\n Toggle RS485 Cyclic synchronous velocity mode\r\n",
	prvPS485PeriodicCommand, /* The function to run. */
	0 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xParameterRS485Dribble =
{
	"dribble",
	"\r\ndribble:\r\n Switch RS485 transmit Dribble command Periodic of Not\r\n",
	prvDribblePeriodicCommand, /* The function to run. */
	0 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xParameterSetTargetSPeed =
{
	"v",
	"\r\nv:\r\n Set Target velocity [rpn]\r\n",
	prvSetTargetSpeedCommand, /* The function to run. */
	1 /* One parameter is expected. */
};

static const CLI_Command_Definition_t xParameterSetNewID =
{
	"id",
	"\r\nid:\r\n Set New ID targetID(0~3) NewID(0~3)\r\n",
	prvSetNerIDCommand, /* The function to run. */
	2 /* One parameter is expected. */
};

static const CLI_Command_Definition_t xParameterSetDribbleDuty =
{
	"duty",
	"\r\nduty:\r\n Set Dribble Duty\r\n",
	prvSetDribbleDutyCommand, /* The function to run. */
	1 /* One parameter is expected. */
};

static const CLI_Command_Definition_t xParameterEncoderCalibrate =
{
	"calib",
	"\r\ncalib:\r\n Calibrate Encoder Offset\r\n",
	prvEncoderCalibrateCommand, /* The function to run. */
	0 /* No parameters are expected. */
};

void vRegisterScrambleWheelCLICommands( void )
{
	/* Register all the command line commands defined immediately above. */
	FreeRTOS_CLIRegisterCommand( &xParameterRS485Periodic );
	FreeRTOS_CLIRegisterCommand( &xParameterSetTargetSPeed );
	FreeRTOS_CLIRegisterCommand( &xParameterEncoderCalibrate );
	FreeRTOS_CLIRegisterCommand( &xParameterSetNewID );

}

void vRegisterScrambleDribbleCLICommands( void )
{
	/* Register all the command line commands defined immediately above. */
	FreeRTOS_CLIRegisterCommand( &xParameterRS485Dribble );
	FreeRTOS_CLIRegisterCommand( &xParameterSetDribbleDuty );
}
