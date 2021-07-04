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

static BaseType_t prvReadRegister( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    const char *pcParameter;
    BaseType_t xParameterStringLength;
    BaseType_t xReturn;
    uint8_t rxdata = {0};
    char str[10] = {0};
    long map = 0;
    static UBaseType_t uxParameterNumber = 0;

    ( void ) pcCommandString;
    ( void ) xWriteBufferLen;
    configASSERT( pcWriteBuffer );

    if(uxParameterNumber == 0){
        sprintf( pcWriteBuffer, "ba769x0 Register Read\r\n");
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

            map = strtol(pcParameter, NULL , 16);
			BQ769X0_Read((uint8_t)map, &rxdata, 1);
            xsprintf(str, "0x%02x", rxdata);

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
    uint8_t rxdata = 0;
    uint8_t txdata = 0;
    char str[10] = {0};
	char str2[10] = {0};
    static volatile long map = 0;
	volatile long registerdata = 0;
	uint16_t data = 0;
    static UBaseType_t uxParameterNumber = 0;
    //static bool state = false;

    ( void ) pcCommandString;
    ( void ) xWriteBufferLen;
    configASSERT( pcWriteBuffer );

    if(uxParameterNumber == 0){
        sprintf( pcWriteBuffer, "bq769x0 Register Write\r\n");
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
            map = strtol(pcParameter, NULL , 16);
            memset( pcWriteBuffer, 0x00, xWriteBufferLen );

            sprintf( pcWriteBuffer, "Register : ");
            strncat( pcWriteBuffer, ( char * ) pcParameter, ( size_t ) xParameterStringLength );

        }
        if(uxParameterNumber == 2){
            registerdata = strtol(pcParameter, NULL , 16);
            txdata |= (0xFF & registerdata);
			BQ769X0_Write((uint8_t)map, &txdata, 1);

            osDelay(20);

			BQ769X0_Read((uint8_t)map, &rxdata, 1);
            
            xsprintf(str, "0x%02x", rxdata);
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

static const CLI_Command_Definition_t xParameterbq769x0_ReadRegister =
{
    "r",
    "\r\nr [Register Map(hex)]:\r\n Read bq769x0 Register\r\n",
    prvReadRegister, /* The function to run. */
    1 /* Onw parameter is expected. */
};

static const CLI_Command_Definition_t xParameterbq769x0_WriteRegister =
{
    "w",
    "\r\nw [Register Map(hex)] [Value(hex)]:\r\n Write bq769x0 Register\r\n",
    prvWriteRegister, /* The function to run. */
    2 /* Two parameters are expected. */
};

void vRegisterbq769x0CLICommands( void ){

    FreeRTOS_CLIRegisterCommand( &xParameterReadSYSCTRL2 );
	FreeRTOS_CLIRegisterCommand( &xParameterCHG );
	FreeRTOS_CLIRegisterCommand( &xParameterDSG );
	FreeRTOS_CLIRegisterCommand( &xParameterbq769x0_ReadRegister );
	FreeRTOS_CLIRegisterCommand( &xParameterbq769x0_WriteRegister );
}