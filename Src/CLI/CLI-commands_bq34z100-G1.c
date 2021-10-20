/*
 *  CLI-commands_bq34z100-G1.c
 *
 *  Created on: 2020/11/03
 *      Author: Yuki
 */

#include "CLI-commands_bq34Z100-G1.h"
#include "xprintf.h"

volatile bool FAS = false;
volatile bool SS = false;


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
	uint16_t 	pack_configration;
	char tempstr[10] = {0};
	char configstr[10] = {0};

	RxData[0] = 0x00;
	RxData[1] = 0x00;
	uint16_t oldchecksum, newchecksum;
	uint8_t TxData;
	uint16_t temp;
	
	BQ34Z100G1_BlockDataControl();
	BQ34Z100G1_DataFlashClass(0x40);
	BQ34Z100G1_DataFlashBlock(0x00);
	osDelay(1);
	
	BQ34Z100G1_Read(A_DF + 4 + 1, RxData, 1);
	pack_configration = *(uint16_t *)RxData;

	oldchecksum = BQ34Z100G1_BlockDataChecksum_Read();
	
	TxData = 0x43;
	BQ34Z100G1_Write(A_DF + 4 + 1,  &TxData, 1);

	temp = (255 - oldchecksum - RxData[0]) & 0xFF;
	newchecksum = 255 - ((temp + TxData) & 0xFF );
	TxData = 0xFF & newchecksum;
	BQ34Z100G1_BlockDataChecksum_Write(TxData);

	xsprintf(configstr, "0x%X", RxData[0]);
	sprintf( pcWriteBuffer, "Info : " );
	strncat( pcWriteBuffer, (const char *)("Pack Configuration : "), strlen( "Pack Configuration : " ) );
	strncat( pcWriteBuffer, ( char * ) configstr, strlen( configstr ) );
	strncat( pcWriteBuffer, (const char *)("\r\n"), strlen( "\r\n" ) );

	xReturn = pdFALSE;
	return xReturn;
}

static BaseType_t prvBatt_GetMode( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	//const char *pcParameter;
	//BaseType_t xParameterStringLength;
	BaseType_t xReturn;
	//static UBaseType_t uxParameterNumber = 0;

	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	uint8_t		RxData[2];

	const char *const str_seald = "SEALED State\r\n";
	const char *const str_unseald = "UNSEALED State\r\n";
	const char *const str_fullacccess = "FULL ACCESS State\r\n";
	char *strptr;
	char configstr[10] = {0};
	
	RxData[0] = 0x00;
	RxData[1] = 0x00;
	BQ34Z100G1_Write(CONTROL, RxData, 2);
	BQ34Z100G1_Read(CONTROL, RxData, 2);
	//HAL_I2C_Mem_Write(&hi2c1, BQ34Z100G1_I2C_ADDR << 1, CONTROL, I2C_MEMADD_SIZE_8BIT, RxData, 2, 1000);
	//HAL_I2C_Mem_Read(&hi2c1, BQ34Z100G1_I2C_ADDR << 1, CONTROL, I2C_MEMADD_SIZE_8BIT, RxData, 2, 1000);

	FAS = (RxData[1] >> 6) & 0x01;
	SS = (RxData[1] >> 5) & 0x01;

	if(FAS){
		if(SS){
			strptr = str_seald;
		}else{
			strptr = str_unseald;
		}
	}else{
		if(!SS){
			strptr = str_fullacccess;
		}
	}

	xsprintf(configstr, "0x%X", *(uint16_t *)RxData);

	sprintf( pcWriteBuffer, "CONTROL_STATUS : " );
	strncat( pcWriteBuffer, ( char * ) configstr, strlen( configstr ) );
	strncat( pcWriteBuffer, (const char *)("\r\n"), strlen( "\r\n" ) );
	if(!(!FAS && SS)){
		strncat( pcWriteBuffer, ( char * ) strptr, strlen( strptr ) );
		strncat( pcWriteBuffer, (const char *)("\r\n"), strlen( "\r\n" ) );
	}
	xReturn = pdFALSE;
	return xReturn;
}

static BaseType_t prvBattSerialNum( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	//const char *pcParameter;
	//BaseType_t xParameterStringLength;
	BaseType_t xReturn;
	//static UBaseType_t uxParameterNumber = 0;

	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	uint8_t		RxData[2];

	char serialnum[20] = {0};
	char serialnum2[20] = {0};
	
	RxData[0] = 0x00;
	RxData[1] = 0x00;
	BQ34Z100G1_Read(SERNUM, RxData, 2);
	
	xsprintf(serialnum, "0x%X", *(uint16_t *)RxData);

	BQ34Z100G1_BlockDataControl();

	BQ34Z100G1_DataFlashClass(48);
	BQ34Z100G1_DataFlashBlock(0x00);
	osDelay(1);

	BQ34Z100G1_Read(0x44 + 1, RxData, 2);

	xsprintf(serialnum2, "0x%X", *(uint16_t *)RxData);

	sprintf( pcWriteBuffer, "Serial Number : " );
	strncat( pcWriteBuffer, ( char * ) serialnum, strlen( serialnum ) );
	strncat( pcWriteBuffer, ( char * ) ", ", strlen( ", " ) );
	strncat( pcWriteBuffer, ( char * ) serialnum2, strlen( serialnum2 ) );
	strncat( pcWriteBuffer, (const char *)("\r\n"), strlen( "\r\n" ) );

	xReturn = pdFALSE;
	return xReturn;
}

static BaseType_t prvBatt_unseal( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	//const char *pcParameter;
	//BaseType_t xParameterStringLength;
	BaseType_t xReturn;
	//static UBaseType_t uxParameterNumber = 0;

	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	BQ34Z100G1_UNSEAL();

	sprintf( pcWriteBuffer, "unseal cmd sent.\r\n" );
	xReturn = pdFALSE;
	return xReturn;
}

static BaseType_t prvBatt_seal( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	//const char *pcParameter;
	//BaseType_t xParameterStringLength;
	BaseType_t xReturn;
	//static UBaseType_t uxParameterNumber = 0;

	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	BQ34Z100G1_SEAL();

	sprintf( pcWriteBuffer, "seal cmd sent.\r\n" );
	xReturn = pdFALSE;
	return xReturn;
}

static BaseType_t prvBatt_fullaccess( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	//const char *pcParameter;
	//BaseType_t xParameterStringLength;
	BaseType_t xReturn;
	//static UBaseType_t uxParameterNumber = 0;

	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	if(FAS && !SS){
		BQ34Z100G1_FULLACCESS();
		sprintf( pcWriteBuffer, "fullaccess cmd sent.\r\n" );
	}else{
		sprintf( pcWriteBuffer, "Current Mode is not UNSEALED State.\r\nChange the Mode to UNSEALED first\r\n" );
	}

	xReturn = pdFALSE;
	return xReturn;
}

static BaseType_t prvBatt_reset( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	//const char *pcParameter;
	//BaseType_t xParameterStringLength;
	BaseType_t xReturn;
	//static UBaseType_t uxParameterNumber = 0;

	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	BQ34Z100G1_RESET();

	sprintf( pcWriteBuffer, "reset cmd sent.\r\n" );
	xReturn = pdFALSE;
	return xReturn;
}

static BaseType_t prvBatt_volt( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	//const char *pcParameter;
	//BaseType_t xParameterStringLength;
	BaseType_t xReturn;
	//static UBaseType_t uxParameterNumber = 0;

	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );
	uint16_t voltage;
	uint8_t	RxData[16] = {0};
	char configstr[10] = {0};

	voltage = BQ34Z100G1_GetVoltagemilliV();

	BQ34Z100G1_BlockDataControl();
	BQ34Z100G1_DataFlashClass(104);
	BQ34Z100G1_DataFlashBlock(0x00);
	osDelay(1);

	BQ34Z100G1_Read(0x40 + 14, RxData, 2);
	voltage = RxData[0] << 8 | RxData[1];
	xsprintf(configstr, "%d", voltage);

	sprintf( pcWriteBuffer, "Volgate Div : " );
	strncat( pcWriteBuffer, ( char * ) configstr, strlen( configstr ) );
	strncat( pcWriteBuffer, (const char *)("\r\n"), strlen( "\r\n" ) );
	xReturn = pdFALSE;
	return xReturn;
}

static BaseType_t prvBattTI( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	//const char *pcParameter;
	//BaseType_t xParameterStringLength;
	BaseType_t xReturn;
	//static UBaseType_t uxParameterNumber = 0;

	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	uint8_t	RxData[11] = {0};
	uint8_t	lion[4] = {0};
	uint16_t 	pack_configration;
	char tempstr[10] = {0};
	char configstr[10] = {0};

	uint16_t oldchecksum, newchecksum;
	uint8_t TxData;
	uint16_t temp;
	
	sprintf( pcWriteBuffer, "Device Name : " );

	BQ34Z100G1_BlockDataControl();

	BQ34Z100G1_DataFlashClass(48);
	
	BQ34Z100G1_DataFlashBlock(0x01);
	osDelay(1);

	BQ34Z100G1_Read(0x40, RxData, 11);

	strncat( pcWriteBuffer, ( char * ) RxData, 11);

	BQ34Z100G1_Read(0x57 + 1, lion, 4);
	BQ34Z100G1_Read(0x4B + 1, RxData, 11);
	
	strncat( pcWriteBuffer, (const char *)("\r\nDevice evChemistry : "), strlen("\r\nDevice evChemistry : ") );
	strncat( pcWriteBuffer, ( char * ) lion, 4 );
	strncat( pcWriteBuffer, (const char *)("\r\nManufacturer Name : "), strlen("\r\nManufacturer Name : " ) );
	strncat( pcWriteBuffer, ( char * ) RxData, 11);
	strncat( pcWriteBuffer, (const char *)("\r\n"), strlen( "\r\n" ) );

	xReturn = pdFALSE;
	return xReturn;
}

static BaseType_t prvBattled( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	//const char *pcParameter;
	//BaseType_t xParameterStringLength;
	BaseType_t xReturn;
	//static UBaseType_t uxParameterNumber = 0;

	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	uint8_t	RxData[11] = {0};
	char configstr[10] = {0};
	
	BQ34Z100G1_BlockDataControl();
	BQ34Z100G1_DataFlashClass(67);
	BQ34Z100G1_DataFlashBlock(0x00);
	osDelay(1);

	BQ34Z100G1_Read(0x40, RxData, 1);

	xsprintf(configstr, "0x%X", RxData[0]);
	sprintf( pcWriteBuffer, "LED Hold Time : " );
	strncat( pcWriteBuffer, ( char * ) configstr, strlen( configstr ) );
	strncat( pcWriteBuffer, (const char *)("\r\n"), strlen( "\r\n" ) );

	xReturn = pdFALSE;
	return xReturn;
}


static const CLI_Command_Definition_t xParameterBattInfo =
{
	"batt",
	"\r\nbatt:\r\n Show bq34z100-G1 info\r\n",
	prvBattInfo, /* The function to run. */
	0 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xParameterBattMode =
{
	"batt_mode",
	"\r\nbatt_mode:\r\n Show bq34z100-G1 CONTROL_STATUS and SECURITY MODE\r\n",
	prvBatt_GetMode, /* The function to run. */
	0 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xParameterBatt_unseal =
{
	"unseal",
	"\r\nunseal:\r\n bq34z100-G1  set UNSEALED mode\r\n",
	prvBatt_unseal, /* The function to run. */
	0 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xParameterBatt_seal =
{
	"seal",
	"\r\nseal:\r\n bq34z100-G1  set SEALED mode\r\n",
	prvBatt_seal, /* The function to run. */
	0 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xParameterBatt_reset =
{
	"batt_reset",
	"\r\nbatt_reset:\r\n bq34z100-G1  Reset\r\n",
	prvBatt_reset, /* The function to run. */
	0 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xParameterBatt_fullaccess =
{
	"fullaccess",
	"\r\nfullaccess:\r\n bq34z100-G1  set FULL ACCESS mode\r\n",
	prvBatt_fullaccess, /* The function to run. */
	0 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xParameterBatt_volt =
{
	"volt",
	"\r\nvolt:\r\n bq34z100-G1  show batt voltage\r\n",
	prvBatt_volt, /* The function to run. */
	0 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xParameterBatt_ti =
{
	"ti",
	"\r\nti:\r\n bq34z100-G1  show TI\r\n",
	prvBattTI, /* The function to run. */
	0 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xParameterBatt_seaial =
{
	"serial",
	"\r\nserial:\r\n bq34z100-G1  show serial number\r\n",
	prvBattSerialNum, /* The function to run. */
	0 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xParameterBatt_led =
{
	"led",
	"\r\nled:\r\n bq34z100-G1  show LED Hold Time\r\n",
	prvBattled, /* The function to run. */
	0 /* No parameters are expected. */
};


void vRegisterbq34z100G1CLICommands( void ){

    FreeRTOS_CLIRegisterCommand( &xParameterBattInfo );
	FreeRTOS_CLIRegisterCommand( &xParameterBattMode );
	FreeRTOS_CLIRegisterCommand( &xParameterBatt_unseal );
	FreeRTOS_CLIRegisterCommand( &xParameterBatt_seal );
	FreeRTOS_CLIRegisterCommand( &xParameterBatt_fullaccess );
	FreeRTOS_CLIRegisterCommand( &xParameterBatt_reset );
	FreeRTOS_CLIRegisterCommand( &xParameterBatt_volt );
	FreeRTOS_CLIRegisterCommand( &xParameterBatt_ti );
	FreeRTOS_CLIRegisterCommand( &xParameterBatt_seaial );
	FreeRTOS_CLIRegisterCommand( &xParameterBatt_led );
}