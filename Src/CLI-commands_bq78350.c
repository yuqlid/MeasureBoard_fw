/*
 *  CLI-commands_bq769x0.c
 *
 *  Created on: 2021/07/04
 *      Author: Yuki Kusakabe
 */

#include "CLI-commands_bq78350.h"
#include "xprintf.h"
#include "i2c.h"
#include "sbs.h"

static BaseType_t prvReadRegister( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    const char *pcParameter;
    BaseType_t xParameterStringLength;
    BaseType_t xReturn;
    uint16_t rxdata = {0};
    char str[10] = {0};
    long map = 0;
    static UBaseType_t uxParameterNumber = 0;

    ( void ) pcCommandString;
    ( void ) xWriteBufferLen;
    configASSERT( pcWriteBuffer );

    if(uxParameterNumber == 0){
        sprintf( pcWriteBuffer, "bq78350 Register Read\r\n");
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
			BQ78350_Read((uint8_t)map, &rxdata, 1);
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
    uint16_t rxdata = 0;
    uint16_t txdata = 0;
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
        sprintf( pcWriteBuffer, "bq78350 Register Write\r\n");
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
            txdata |= (0xFFFF & registerdata);
			BQ78350_Write((uint8_t)map, &txdata, 1);

            osDelay(20);

			BQ78350_Read((uint8_t)map, &rxdata, 1);
            
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

static BaseType_t prvvolt( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	//const char *pcParameter;
	//BaseType_t xParameterStringLength;
	BaseType_t xReturn;
	//static UBaseType_t uxParameterNumber = 0;

    uint16_t  DevAddress = 0x16;
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	char configstr[20] = {0};
    uint16_t packVoltage = 0;
    uint16_t cellvoltage[15] = {0};

    HAL_I2C_Mem_Read(&hi2c1, DevAddress, VOLTAGE, I2C_MEMADD_SIZE_8BIT, &packVoltage, 2, 1000);
    xsprintf(configstr, "%5d", packVoltage);
	sprintf( pcWriteBuffer, "Battery Voltage\r\nUnit : mV\r\nPack    : " );
	strncat( pcWriteBuffer, ( char * ) configstr, strlen( configstr ) );
	strncat( pcWriteBuffer, (const char *)("\r\n"), strlen( "\r\n" ) );
    for(int8_t i = 14; i >= 0 ; i--){
        HAL_I2C_Mem_Read(&hi2c1, DevAddress, CELLVOLTAGE_15 + i, I2C_MEMADD_SIZE_8BIT, &cellvoltage[14 - i], 2, 1000);
        xsprintf(configstr, "Cell %2d : ", i + 1);
        strncat( pcWriteBuffer, ( char * ) configstr, strlen( configstr ) );
        xsprintf(configstr, "%5d", cellvoltage[i]);
        strncat( pcWriteBuffer, ( char * ) configstr, strlen( configstr ) );
        strncat( pcWriteBuffer, (const char *)("\r\n"), strlen( "\r\n" ) );
    }
	xReturn = pdFALSE;
	return xReturn;
}

static BaseType_t prvReadAFECellMap( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	//const char *pcParameter;
	//BaseType_t xParameterStringLength;
	BaseType_t xReturn;
	//static UBaseType_t uxParameterNumber = 0;

    uint16_t  DevAddress = 0x16;
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	char configstr[10] = {0};
    //ManufacturerBlockAccess()で0x44ACの値を読み出し
    const uint8_t txdata[3] = {0x02, 0xAC, 0x44};
    const uint8_t rxdata[5] = {0};

    HAL_I2C_Mem_Write(&hi2c1, DevAddress, MANUFACTURER_BLOCK_ACCESS, I2C_MEMADD_SIZE_8BIT, txdata, sizeof(txdata)/sizeof(txdata[0]), 10);
    HAL_I2C_Mem_Read(&hi2c1, DevAddress, MANUFACTURER_BLOCK_ACCESS, I2C_MEMADD_SIZE_8BIT, rxdata, sizeof(rxdata)/sizeof(rxdata[0]), 10);
    const uint16_t addr = *(uint16_t *)(&rxdata[1]);
    const uint16_t data = rxdata[3] << 8 | rxdata[4];
	sprintf( pcWriteBuffer, "Data Flash : " );
    xsprintf(configstr, "0x%04X ", addr);
	strncat( pcWriteBuffer, ( char * ) configstr, strlen( configstr ) );
    strncat( pcWriteBuffer, ( char * ) "Read  : ", strlen( "Read  : " ) );
    xsprintf(configstr, "0x%04X", data);
	strncat( pcWriteBuffer, ( char * ) configstr, strlen( configstr ) );
	strncat( pcWriteBuffer, (const char *)("\r\n"), strlen( "\r\n" ) );
    
	xReturn = pdFALSE;
	return xReturn;
}

static BaseType_t prvWriteAFECellMap( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	//const char *pcParameter;
	//BaseType_t xParameterStringLength;
	BaseType_t xReturn;
	//static UBaseType_t uxParameterNumber = 0;

    uint16_t  DevAddress = 0x16;
	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );
	char configstr[40] = {0};
    //ManufacturerBlockAccess()で0x44ACに0x0273を書き込み
    //アクセスするアドレスはリトルエンディアンだけどデータはビッグエンディアン
    uint8_t txdata[5] = {0x04,  0xAC, 0x44, 0x02, 0x73};

    HAL_I2C_Mem_Write(&hi2c1, DevAddress, MANUFACTURER_BLOCK_ACCESS, I2C_MEMADD_SIZE_8BIT, txdata, sizeof(txdata)/sizeof(txdata[0]), 10);
    
	sprintf( pcWriteBuffer, "Data Flash : 0x44AC Write : 0x0273");
	strncat( pcWriteBuffer, ( char * ) configstr, strlen( configstr ) );
	strncat( pcWriteBuffer, (const char *)("\r\n"), strlen( "\r\n" ) );

	xReturn = pdFALSE;
	return xReturn;
}

static const CLI_Command_Definition_t xParameterbq769x0_ReadRegister =
{
    "r",
    "\r\nr [Register Map(hex)]:\r\n Read bq78350 Register\r\n",
    prvReadRegister, /* The function to run. */
    1 /* Onw parameter is expected. */
};

static const CLI_Command_Definition_t xParameterbq769x0_WriteRegister =
{
    "w",
    "\r\nw [Register Map(hex)] [Value(hex)]:\r\n Write bq78350 Register\r\n",
    prvWriteRegister, /* The function to run. */
    2 /* Two parameters are expected. */
};

static const CLI_Command_Definition_t xParametervolt =
{
	"volt",
	"\r\nvolt:\r\n Read Battery Voltage\r\n",
	prvvolt, /* The function to run. */
	0 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xParameterRAFECellMap =
{
	"r_afe",
	"\r\nr_afe:\r\n ManufacturerBlockAccess Write AFE Cell Map (0x44AC) to 0x0273\r\n",
	prvReadAFECellMap, /* The function to run. */
	0 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xParameterWAFECellMap =
{
	"w_afe",
	"\r\nw_afe:\r\n ManufacturerBlockAccess Read AFE Cell Map (0x44AC)\r\n",
	prvWriteAFECellMap, /* The function to run. */
	0 /* No parameters are expected. */
};

void vRegisterbq78350CLICommands( void ){

	FreeRTOS_CLIRegisterCommand( &xParameterbq769x0_ReadRegister );
	FreeRTOS_CLIRegisterCommand( &xParameterbq769x0_WriteRegister );
    FreeRTOS_CLIRegisterCommand( &xParametervolt );
    FreeRTOS_CLIRegisterCommand( &xParameterRAFECellMap );
    FreeRTOS_CLIRegisterCommand( &xParameterWAFECellMap );

}