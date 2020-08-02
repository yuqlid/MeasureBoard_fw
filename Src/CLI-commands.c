/*
 *  CLI-commands.c
 *
 *  Created on: 2020/08/02
 *      Author: Yuki
 */

#include "CLI-commands.h"

#include "MeasurementBoard_v1.h"

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
        LED_Off(LED2);
    }else{
        state = true;
        sprintf( pcWriteBuffer, "RS485_periodicSend : True\r\n" );
        osThreadResume(rs485TransmitTaskHandle);
    }
    // まだ出力する処理が残ってたらpdTRUEにする．
    xReturn = pdFALSE;
	return xReturn;
}

static const CLI_Command_Definition_t xParameterRS485Periodic =
{
	"rs485_periodic",
	"\r\nrs485_periodic:\r\n Switch RS485 transmit command Periodic of Not\r\n",
	prvPS485PeriodicCommand, /* The function to run. */
	0 /* No parameters are expected. */
};

void vRegisterScramgleCLICommands( void )
{
	/* Register all the command line commands defined immediately above. */
	FreeRTOS_CLIRegisterCommand( &xParameterRS485Periodic );
}
