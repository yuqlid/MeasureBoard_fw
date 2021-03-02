/*
 *  eeprom_CLI-commands.c
 *
 *  Created on: 2021/02/14
 *      Author: Yuki
 */

#include "eeprom_CLI-commands.h"

//include "MeasurementBoard_v1.h"
#include "xprintf.h"
#include "eeprom.h"
#include "i2c.h"
#include <stdio.h>

static BaseType_t prvWriteall( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	//const char *pcParameter;
	//BaseType_t xParameterStringLength;
	BaseType_t xReturn;
	//static UBaseType_t uxParameterNumber = 0;

	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	uint8_t	RxData = 0;
	char datastr[10] = {0};
	char addrstr[10] = {0};
	#define  datasize 16
	uint8_t data[datasize] = {
		0x00,	// 書き込み開始アドレス上位バイト
		0x00,	// 書き込み開始アドレス下位バイト
		0x80,
		0x0E,
		0x00,
		0xCC,
		0x88,
		0x13,
		0xf0,
		0x00,
		0x00,
		0x00,
		0x00,
		0x80,
		0x00,
		0x00
	};
	//80 02 00 00 00 00 00 00
	//80 03 00 6E FF 00 FF 00 00 00
	//80 0E 00 CC 88 13 f0 00 00 00 00 80 00 00
	//80 0E 00 CC 88 13 ff 00 00 00 00 80 00 00
	//80 0E 00 CC 88 13 FF 00 00 00 00 80 00 00
	//80 0E 00 CC 88 13 FF 00 00 00 00 80 00 00
	//80 0E 00 CC 88 13 f0 00 00 00 00 80 00 00
	//80 03 00 6E FF 00 FF 00 00 00
	//80 03 00 00 00 00 00 00 00 00

	memset( pcWriteBuffer, 0x00, xWriteBufferLen );
	HAL_I2C_Master_Transmit(&hi2c1, EEPROM_24FC512_I2C_ADDR << 1, data, datasize, 1000);

	for (uint8_t i = 0; i < datasize - 2; i++){
		//EEPROM_Write(i, &(data[i+2]), 1);
		xsprintf(addrstr, "0x%03X", i);
		xsprintf(datastr, "0x%02X", data[i+2]);
		strncat( pcWriteBuffer, ( char * ) addrstr, strlen( addrstr ) );
		strncat( pcWriteBuffer, (const char *)(" : "), strlen( " : " ) );
		strncat( pcWriteBuffer, ( char * ) datastr, strlen( datastr ) );
		strncat( pcWriteBuffer, (const char *)("\r\n"), strlen( "\r\n" ) );
	}

	xReturn = pdFALSE;
	return xReturn;
}

static BaseType_t prvReadall( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	//const char *pcParameter;
	//BaseType_t xParameterStringLength;
	BaseType_t xReturn;
	//static UBaseType_t uxParameterNumber = 0;

	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	uint8_t	RxData = 0;
	char datastr[10] = {0};
	char addrstr[10] = {0};
	uint16_t addr[16] = {
		0x0,
		0x1,
		0x2,
		0x3,
		0x4,
		0x5,
		0x6,
		0x7,
		0x8,
		0x9,
		0xA,
		0xB,
		0xC,
		0xD,
		0xE,
		0xF
	};
	
	memset( pcWriteBuffer, 0x00, xWriteBufferLen );

	for (uint8_t i = 0; i < 16; i++){
		EEPROM_Read(addr[i], &RxData, 1);
		xsprintf(addrstr, "0x%03X", addr[i]);
		xsprintf(datastr, "0x%02X", RxData);
		strncat( pcWriteBuffer, ( char * ) addrstr, strlen( addrstr ) );
		strncat( pcWriteBuffer, (const char *)(" : "), strlen( " : " ) );
		strncat( pcWriteBuffer, ( char * ) datastr, strlen( datastr ) );
		strncat( pcWriteBuffer, (const char *)("\r\n"), strlen( "\r\n" ) );
	}

	xReturn = pdFALSE;
	return xReturn;
}

static BaseType_t prvPDIControl( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	//const char *pcParameter;
	//BaseType_t xParameterStringLength;
	BaseType_t xReturn;
	//static UBaseType_t uxParameterNumber = 0;

	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	uint8_t	RxData = 0;
	uint16_t addr = 0x0;
	char configstr[10] = {0};
	
	EEPROM_Read(addr, &RxData, 1);

	//sprintf( pcWriteBuffer, "PDI Control Register : " );
	xsprintf( pcWriteBuffer, "Register 0x%X : ", addr);
	xsprintf(configstr, "0x%X", RxData);
	strncat( pcWriteBuffer, ( char * ) configstr, strlen( configstr ) );
	strncat( pcWriteBuffer, (const char *)("\r\n"), strlen( "\r\n" ) );

	xReturn = pdFALSE;
	return xReturn;
}


static BaseType_t prvPDIControlSPI( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	//const char *pcParameter;
	//BaseType_t xParameterStringLength;
	BaseType_t xReturn;
	//static UBaseType_t uxParameterNumber = 0;

	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	uint8_t	RxData = 0x00;
	char configstr[10] = {0};
	uint8_t data[3] = {
		0x0,
		0x6,
		0xff
	};
	
	//EEPROM_Write(7, &RxData, 1);
	HAL_I2C_Master_Transmit(&hi2c1, EEPROM_24FC512_I2C_ADDR << 1, data, 3, 1000);

	xsprintf(configstr, "0x%X", RxData);
	sprintf( pcWriteBuffer, "PDI Control Register : " );
	strncat( pcWriteBuffer, ( char * ) configstr, strlen( configstr ) );
	strncat( pcWriteBuffer, (const char *)("\r\n"), strlen( "\r\n" ) );

	xReturn = pdFALSE;
	return xReturn;
}

static const CLI_Command_Definition_t xParameterEEPROM_Write =
{
	"write",
	"\r\nwrite:\r\n Write ESC Config Data\r\n",
	prvWriteall, /* The function to run. */
	0 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xParameterEEPROM_ReadAll =
{
	"all",
	"\r\nall:\r\n Read Configratabl Core Register\r\n",
	prvReadall, /* The function to run. */
	0 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xParameterEEPROM_PDIControl =
{
	"pdi",
	"\r\npdi:\r\n Process Data Interface\r\n",
	prvPDIControl, /* The function to run. */
	0 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xParameterEEPROM_PDIControlSPI =
{
	"spi",
	"\r\nspi:\r\n Process Data Interface Set SPI(0x80)\r\n",
	prvPDIControlSPI, /* The function to run. */
	0 /* No parameters are expected. */
};

void vRegisterEEPROMCLICommands( void ){
    	/* Register all the command line commands defined immediately above. */
	FreeRTOS_CLIRegisterCommand( &xParameterEEPROM_PDIControl );
	FreeRTOS_CLIRegisterCommand( &xParameterEEPROM_PDIControlSPI );
	FreeRTOS_CLIRegisterCommand( &xParameterEEPROM_ReadAll );
	FreeRTOS_CLIRegisterCommand( &xParameterEEPROM_Write );
	
	
}