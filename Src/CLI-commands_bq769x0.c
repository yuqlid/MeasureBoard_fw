/*
 *  CLI-commands_bq769x0.c
 *
 *  Created on: 2021/07/04
 *      Author: Yuki Kusakabe
 */

#include "CLI-commands_bq769x0.h"
#include "xprintf.h"

static bool chg_state = false;
static bool dsg_state = false;

static BaseType_t prvReadSYSCTRL2( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	//const char *pcParameter;
	//BaseType_t xParameterStringLength;
	BaseType_t xReturn;
	//static UBaseType_t uxParameterNumber = 0;

	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	uint8_t		RxData = 0xFF;
	char configstr[10] = {0};


    RxData = BQ769X0_READ_SYS_CTRL2();

	xsprintf(configstr, "0x%X", RxData);
	sprintf( pcWriteBuffer, "SYS_CTRL2 : " );
	strncat( pcWriteBuffer, ( char * ) configstr, strlen( configstr ) );
	strncat( pcWriteBuffer, (const char *)("\r\n"), strlen( "\r\n" ) );

	xReturn = pdFALSE;
	return xReturn;
}

static BaseType_t prvchg( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	//const char *pcParameter;
	//BaseType_t xParameterStringLength;
	BaseType_t xReturn;
	//static UBaseType_t uxParameterNumber = 0;

	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );
	
	uint8_t		RxData = 0xFF;
	uint8_t		TxData;
	char configstr[10] = {0};

    RxData = BQ769X0_READ_SYS_CTRL2();
	TxData = RxData;
	TxData &= 0xFE;
	if(chg_state == true){
		chg_state = false;
	}else{
		chg_state = true;
	}
	TxData |= chg_state;

	BQ769X0_Write(SYS_CTRL2, &TxData, 1);

	xsprintf(configstr, "0x%X to 0x%X", RxData, TxData);
	sprintf( pcWriteBuffer, "SYS_CTRL2 : " );
	strncat( pcWriteBuffer, ( char * ) configstr, strlen( configstr ) );
	strncat( pcWriteBuffer, (const char *)("\r\n"), strlen( "\r\n" ) );

	xReturn = pdFALSE;
	return xReturn;
}

static BaseType_t prvdsg( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	//const char *pcParameter;
	//BaseType_t xParameterStringLength;
	BaseType_t xReturn;
	//static UBaseType_t uxParameterNumber = 0;

	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	
	uint8_t		RxData = 0xFF;
	uint8_t		TxData;
	char configstr[10] = {0};

    RxData = BQ769X0_READ_SYS_CTRL2();
	TxData = RxData;
	TxData &= 0xFD;
	if(dsg_state == true){
		dsg_state = false;
	}else{
		dsg_state = true;
	}
	TxData |= (dsg_state << 1);

	BQ769X0_Write(SYS_CTRL2, &TxData, 1);

	xsprintf(configstr, "0x%X to 0x%X", RxData, TxData);
	sprintf( pcWriteBuffer, "SYS_CTRL2 : " );
	strncat( pcWriteBuffer, ( char * ) configstr, strlen( configstr ) );
	strncat( pcWriteBuffer, (const char *)("\r\n"), strlen( "\r\n" ) );

	xReturn = pdFALSE;
	return xReturn;
}

static const CLI_Command_Definition_t xParameterReadSYSCTRL2 =
{
	"read",
	"\r\nread:\r\n Show bq769x0 SYS_CTRL2 Register\r\n",
	prvReadSYSCTRL2, /* The function to run. */
	0 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xParameterCHG =
{
	"chg",
	"\r\nrchg:\r\n Toggle CHG_ON bit\r\n",
	prvchg, /* The function to run. */
	0 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xParameterDSG =
{
	"dsg",
	"\r\ndsg:\r\n Toggle DSG_ON bit\r\n",
	prvdsg, /* The function to run. */
	0 /* No parameters are expected. */
};


void vRegisterbq769x0CLICommands( void ){

    FreeRTOS_CLIRegisterCommand( &xParameterReadSYSCTRL2 );
	FreeRTOS_CLIRegisterCommand( &xParameterCHG );
	FreeRTOS_CLIRegisterCommand( &xParameterDSG );
}