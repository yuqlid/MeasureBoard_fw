/*
 *  CLI-commands_MA7xx.c
 *
 *  Created on: 2020/11/03
 *      Author: Yuki
 */

#include "CLI-commands_MA7xx.h"
#include "xprintf.h"
#include "MA7xx.h"
// /#include "HEDL5540.h"


static BaseType_t prvReadPos( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	//const char *pcParameter;
	// xParameterStringLength;
	BaseType_t xReturn;
	uint16_t data = 0;
	uint16_t txdata = 0;
    char str[10] = {0};
	char str2[20] = {0};
	uint16_t pos = 0;
    //static UBaseType_t uxParameterNumber = 0;

	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

    pos = MA7xx_GetAngle();

    xsprintf(str, "%d", pos);

    memset( pcWriteBuffer, 0x00, xWriteBufferLen );
    sprintf( pcWriteBuffer, "MA7xx Current Position\r\nPosition : ");
    strncat( pcWriteBuffer, ( char * ) str, strlen( str ) );
    strncat( pcWriteBuffer, (const char *)("\r\n"), strlen( "\r\n" ) );

    xReturn = pdFALSE;
	return xReturn;
}

static BaseType_t prvReadOffset( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	//const char *pcParameter;
	// xParameterStringLength;
	BaseType_t xReturn;
	uint16_t rxdata = 0;
    char str[10] = {0};
	uint16_t offset = 0;
    //static UBaseType_t uxParameterNumber = 0;
	//static bool state = false;

	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

    offset = MA7xx_ReadRegister(Z_7_0 >>8);
	offset |= (MA7xx_ReadRegister(Z_15_8 >>8)) << 8;

    xsprintf(str, "%d", offset);

    memset( pcWriteBuffer, 0x00, xWriteBufferLen );
    sprintf( pcWriteBuffer, "MA7xx Position Offset\r\nOffset : ");
    strncat( pcWriteBuffer, ( char * ) str, strlen( str ) );
    strncat( pcWriteBuffer, (const char *)("\r\n"), strlen( "\r\n" ) );

    xReturn = pdFALSE;
	return xReturn;
}

static BaseType_t prvWriteOffset( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    const char *pcParameter;
    BaseType_t xParameterStringLength;
    BaseType_t xReturn;
    uint16_t rxdata = 0;
    uint16_t txdata = 0;
    char str[10] = {0};
    long offset = 0;
    uint16_t data = 0;
    static UBaseType_t uxParameterNumber = 0;
    //char str[10] = {0};
	char str2[10] = {0};
    //static bool state = false;

    ( void ) pcCommandString;
    ( void ) xWriteBufferLen;
    configASSERT( pcWriteBuffer );

    if(uxParameterNumber == 0){
        sprintf( pcWriteBuffer, "MA7xx Write Position Offset\r\n");
        uxParameterNumber = 1;
        xReturn = pdPASS;
    }else{
        /* Obtain the parameter string. */
        pcParameter = FreeRTOS_CLIGetParameter(
                            pcCommandString,        /* The command string itself. */
                            uxParameterNumber,        /* Return the next parameter. */
                            &xParameterStringLength    /* Store the parameter string length. */
        );

        if(pcParameter != NULL){

            offset = strtol(pcParameter, NULL , 10);

            txdata = CMD_W | Z_7_0 | (0x00FF & offset);
            HAL_SPI_TransmitReceive(&hspi1, &txdata, &rxdata, 1, 100);
            osDelay(20);
            HAL_SPI_Receive(&hspi1, &rxdata, 1, 100);
            data = 0xFF & (rxdata >> 8);

            txdata = CMD_W | Z_15_8 | (0x00FF & (offset >> 8));
            HAL_SPI_TransmitReceive(&hspi1, &txdata, &rxdata, 1, 100);
            osDelay(20);
            HAL_SPI_Receive(&hspi1, &rxdata, 1, 100);
            data |= rxdata;

            xsprintf(str, "%5d", offset);
            xsprintf(str2, "%5d", data);

            memset( pcWriteBuffer, 0x00, xWriteBufferLen );
            sprintf( pcWriteBuffer, "New OffSet : ");
            //strncat( pcWriteBuffer, (const char *)(" New OffSet : "), strlen( " New OffSet : " ) );
            strncat( pcWriteBuffer, ( char * ) str, strlen( str ) );
            strncat( pcWriteBuffer, (const char *)("\r\nVerify OffSet : "), strlen( "\r\nVerify OffSet : " ) );
            strncat( pcWriteBuffer, ( char * ) str2, strlen( str2 ) );
            strncat( pcWriteBuffer, (const char *)("\r\n"), strlen( "\r\n" ) );

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

static BaseType_t prvReadRegister( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    const char *pcParameter;
    BaseType_t xParameterStringLength;
    BaseType_t xReturn;
    uint16_t rxdata[2] = {0};
    int16_t txdata[2] = {0};
    char str[10] = {0};
    long offset = 0;
	uint16_t data = 0;
    static UBaseType_t uxParameterNumber = 0;

    ( void ) pcCommandString;
    ( void ) xWriteBufferLen;
    configASSERT( pcWriteBuffer );

    if(uxParameterNumber == 0){
        sprintf( pcWriteBuffer, "MA7xx Register Read\r\n");
        uxParameterNumber = 1;
        xReturn = pdPASS;
    }else{
        /* Obtain the parameter string. */
        pcParameter = FreeRTOS_CLIGetParameter(
                            pcCommandString,        /* The command string itself. */
                            uxParameterNumber,        /* Return the next parameter. */
                            &xParameterStringLength    /* Store the parameter string length. */
        );

        if(pcParameter != NULL){

            offset = strtol(pcParameter, NULL , 10);

            data = MA7xx_ReadRegister(offset);
            xsprintf(str, "0x%02x", data);

            memset( pcWriteBuffer, 0x00, xWriteBufferLen );
            sprintf( pcWriteBuffer, "Register : ");
			strncat( pcWriteBuffer, ( char * ) pcParameter, ( size_t ) xParameterStringLength );
			strncat( pcWriteBuffer, (const char *)(" val : "), strlen( " val : " ) );
            strncat( pcWriteBuffer, ( char * ) str, strlen( str ) );
            strncat( pcWriteBuffer, (const char *)("\r\n"), strlen( "\r\n" ) );

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

static BaseType_t prvWriteRegister( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    const char *pcParameter;
    BaseType_t xParameterStringLength;
    BaseType_t xReturn;
    uint16_t rxdata = 0;
    static uint16_t txdata = 0;
    char str[10] = {0};
	char str2[10] = {0};
    long offset = 0;
	long registerdata = 0;
	uint16_t data = 0;
    static UBaseType_t uxParameterNumber = 0;
    //static bool state = false;

    ( void ) pcCommandString;
    ( void ) xWriteBufferLen;
    configASSERT( pcWriteBuffer );

    if(uxParameterNumber == 0){
        sprintf( pcWriteBuffer, "MA7xx Register Write\r\n");
        uxParameterNumber = 1;
        xReturn = pdPASS;
    }else{
        /* Obtain the parameter string. */
        pcParameter = FreeRTOS_CLIGetParameter(
                            pcCommandString,        /* The command string itself. */
                            uxParameterNumber,        /* Return the next parameter. */
                            &xParameterStringLength    /* Store the parameter string length. */
        );

        if(uxParameterNumber == 1){
            offset = strtol(pcParameter, NULL , 10);
            txdata = CMD_W | (0xFF00 & (offset << 8));
            memset( pcWriteBuffer, 0x00, xWriteBufferLen );

            sprintf( pcWriteBuffer, "Register : ");
            strncat( pcWriteBuffer, ( char * ) pcParameter, ( size_t ) xParameterStringLength );

        }
        if(uxParameterNumber == 2){
            registerdata = strtol(pcParameter, NULL , 10);
            txdata |= (0xFF & registerdata);
            HAL_SPI_TransmitReceive(&hspi1, &txdata, &rxdata, 1, 100);

            osDelay(20);

            HAL_SPI_Receive(&hspi1, &rxdata, 1, 100);
            data = 0xFF & (rxdata >> 8);
            
            xsprintf(str, "0x%02x", data);
            xsprintf(str2, "0x%02x", txdata & 0xFF);

            memset( pcWriteBuffer, 0x00, xWriteBufferLen );
            strncat( pcWriteBuffer, (const char *)(" New val : "), strlen( " New val : " ) );
            strncat( pcWriteBuffer, ( char * ) str2, strlen( str2 ) );
            strncat( pcWriteBuffer, (const char *)(" Verify val : "), strlen( " Verify val : " ) );
            strncat( pcWriteBuffer, ( char * ) str2, strlen( str2 ) );
            strncat( pcWriteBuffer, (const char *)("\r\n"), strlen( "\r\n" ) );
        }
        if(uxParameterNumber == 3){

			/* No more parameters were found.  Make sure the write buffer does
            not contain a valid string. */
            pcWriteBuffer[ 0 ] = 0x00;
            /* No more data to return. */
            xReturn = pdFALSE;
            /* Start over the next time this command is executed. */
            uxParameterNumber = 0;

        }else{
            /* There might be more parameters to return after this one. */
            xReturn = pdTRUE;
            uxParameterNumber++;
        }
    }
    return xReturn;
}


static const CLI_Command_Definition_t xParameterMA7xx_ReadPos =
{
	"pos",
	"\r\npos:\r\n Read MA7xx Angle\r\n",
	prvReadPos, /* The function to run. */
	0 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xParameterMA7xx_ReadOffset =
{
	"ro",
	"\r\nro:\r\n Read MA7xx Zero Position Angle\r\n",
	prvReadOffset, /* The function to run. */
	0 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xParameterMA7xx_WriteOffset =
{
    "wo",
    "\r\nwo:\r\n Write MA7xx Zero Position Angle\r\n",
    prvWriteOffset, /* The function to run. */
    1 /* Onw parameter is expected. */
};

static const CLI_Command_Definition_t xParameterMA7xx_ReadRegister =
{
    "r",
    "\r\nr:\r\n Write MA7xx Register\r\n",
    prvReadRegister, /* The function to run. */
    1 /* Onw parameter is expected. */
};

static const CLI_Command_Definition_t xParameterMA7xx_WriteRegister =
{
    "w",
    "\r\nw:\r\n Write MA7xx Register\r\n",
    prvWriteRegister, /* The function to run. */
    2 /* Two parameters are expected. */
};

void vRegisterMA7xxCLICommands( void ){

    
    FreeRTOS_CLIRegisterCommand( &xParameterMA7xx_ReadPos );
	FreeRTOS_CLIRegisterCommand( &xParameterMA7xx_ReadOffset );
	FreeRTOS_CLIRegisterCommand( &xParameterMA7xx_WriteOffset );
	FreeRTOS_CLIRegisterCommand( &xParameterMA7xx_ReadRegister );
	FreeRTOS_CLIRegisterCommand( &xParameterMA7xx_WriteRegister );

}
