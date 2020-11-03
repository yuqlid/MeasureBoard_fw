/*
 *  CLI-commands_MA7xx.c
 *
 *  Created on: 2020/11/03
 *      Author: Yuki
 */

#include "CLI-commands_MA7xx.h"
#include "xprintf.h"
#include "MA7xx.h"

static BaseType_t prvReadPos( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	//const char *pcParameter;
	// xParameterStringLength;
	BaseType_t xReturn;
	uint16_t data = 0;
    char str[10] = {0};
    //static UBaseType_t uxParameterNumber = 0;
	//static bool state = false;

	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

    //data = MA7xx_GetAngle();
    HAL_SPI_Receive(&hspi1, &data, 1, 100);
    //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, SET);
    xsprintf(str, "%d", data);

    sprintf( pcWriteBuffer, "Angle = " );
    strncat( pcWriteBuffer, ( char * ) str, strlen( str ) );
    strncat( pcWriteBuffer, (const char *)("\r\n"), strlen( "\r\n" ) );

	return xReturn;
}


static const CLI_Command_Definition_t xParameterMA7xx_ReadPos =
{
	"pos",
	"\r\npos:\r\n Read MA7xx Angle\r\n",
	prvReadPos, /* The function to run. */
	0 /* No parameters are expected. */
};


void vRegisterMA7xxCLICommands( void ){

    
    FreeRTOS_CLIRegisterCommand( &xParameterMA7xx_ReadPos );

}