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

static BaseType_t prvVoltage( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	//const char *pcParameter;
	//BaseType_t xParameterStringLength;
	BaseType_t xReturn;
	//static UBaseType_t uxParameterNumber = 0;

	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

    // Battery Cell Serial Number 1~15
    const uint8_t Cell_Serial = 6;

	char configstr[20] = {0};
    uint16_t packVoltage = 0;
    uint16_t cellvoltage[15] = {0};

    HAL_I2C_Mem_Read(&hi2c1, BQ78350_I2C_ADDR, VOLTAGE, I2C_MEMADD_SIZE_8BIT, &packVoltage, 2, 1000);
	sprintf( pcWriteBuffer, "Battery Voltage Unit : V\r\n" );
	strncat( pcWriteBuffer, (const char *) configstr, strlen( configstr ) );

    for(int8_t i = 0; i < Cell_Serial ; i++){
        HAL_I2C_Mem_Read(&hi2c1, BQ78350_I2C_ADDR, CELLVOLTAGE_1 - i, I2C_MEMADD_SIZE_8BIT, cellvoltage + i, 2, 1000);
        xsprintf(configstr, "Cell %2d : ", i + 1);
        strncat( pcWriteBuffer, (const char *) configstr, strlen( configstr ) );
        xsprintf(configstr, "%2d.%03d", cellvoltage[i]/1000, cellvoltage[i]%1000);
        strncat( pcWriteBuffer, (const char *) configstr, strlen( configstr ) );
        strncat( pcWriteBuffer, (const char *)("\r\n"), strlen( "\r\n" ) );
    }
    strncat( pcWriteBuffer, (const char *)  "Pack    : ", strlen( "Pack    : " ) );
    xsprintf(configstr, "%2d.%03d", packVoltage/1000, packVoltage%1000);
    strncat( pcWriteBuffer, (const char *) configstr, strlen( configstr ) );
    strncat( pcWriteBuffer, (const char *)("\r\n"), strlen( "\r\n" ) );

	xReturn = pdFALSE;
	return xReturn;
}

static BaseType_t prvDAStatus2( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	//const char *pcParameter;
	//BaseType_t xParameterStringLength;
	BaseType_t xReturn;
	//static UBaseType_t uxParameterNumber = 0;

	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	char configstr[20] = {0};
    uint8_t dastatus2[17] = {0};

    HAL_I2C_Mem_Read(&hi2c1, BQ78350_I2C_ADDR, DASTATUS_2, I2C_MEMADD_SIZE_8BIT, dastatus2, 17, 1000);
    
    const int16_t K2degC = 2731;    // Unit : 0.1 K[degC]
    int16_t TS1_temp = *(int16_t *)(&dastatus2[5]); // Unit : 0.1 K[degC]
    int16_t TS2_temp = *(int16_t *)(&dastatus2[7]); // Unit : 0.1 K[degC]
    TS1_temp -= K2degC;
    TS2_temp -= K2degC;

    xsprintf(configstr, "%3d.%d", TS1_temp/10, TS1_temp%10);
	sprintf( pcWriteBuffer, "DAStatus2\r\nTS1_Temperature : " );
	strncat( pcWriteBuffer, ( char * ) configstr, strlen( configstr ) );
	strncat( pcWriteBuffer, (const char *)("\r\nTS2_Temperature : "), strlen( "\r\nTS2_Temperature : " ) );
    xsprintf(configstr, "%3d.%d", TS2_temp/10, TS2_temp%10);
    strncat( pcWriteBuffer, ( char * ) configstr, strlen( configstr ) );
	strncat( pcWriteBuffer, (const char *)("\r\n"), strlen( "\r\n" ) );
    
	xReturn = pdFALSE;
	return xReturn;
}

static BaseType_t prvfet( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	//const char *pcParameter;
	//BaseType_t xParameterStringLength;
	BaseType_t xReturn;
	//static UBaseType_t uxParameterNumber = 0;

	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );
	
	uint8_t		RxData = 0xFF;
	uint16_t	TxData = 0x1197;
	char configstr[10] = {0};

    BQ78350_Write(HOST_FET_CONTROL, &TxData, 1);
    osDelay(1);
    TxData = 0x03;
    BQ78350_Write(HOST_FET_CONTROL, &TxData, 1);

    sprintf( pcWriteBuffer, "HostFETControl");
	strncat( pcWriteBuffer, ( char * ) configstr, strlen( configstr ) );
	strncat( pcWriteBuffer, (const char *)("\r\n"), strlen( "\r\n" ) );

	xReturn = pdFALSE;
	return xReturn;
}

static BaseType_t prvReadTemperatureEnable( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	//const char *pcParameter;
	//BaseType_t xParameterStringLength;
	BaseType_t xReturn;
	//static UBaseType_t uxParameterNumber = 0;

	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	char configstr[10] = {0};
    //ManufacturerBlockAccess()で0x44ACの値を読み出し
    const uint8_t txdata[3] = {0x02, 0xA9, 0x44};
    const uint8_t rxdata[4] = {0};

    HAL_I2C_Mem_Write(&hi2c1, BQ78350_I2C_ADDR, MANUFACTURER_BLOCK_ACCESS, I2C_MEMADD_SIZE_8BIT, txdata, sizeof(txdata)/sizeof(txdata[0]), 10);
    HAL_I2C_Mem_Read(&hi2c1, BQ78350_I2C_ADDR, MANUFACTURER_BLOCK_ACCESS, I2C_MEMADD_SIZE_8BIT, rxdata, sizeof(rxdata)/sizeof(rxdata[0]), 10);
    const uint16_t addr = *(uint16_t *)(&rxdata[1]);
    const uint8_t data = rxdata[3];
	sprintf( pcWriteBuffer, "Data Flash : " );
    xsprintf(configstr, "0x%04X ", addr);
	strncat( pcWriteBuffer, ( char * ) configstr, strlen( configstr ) );
    strncat( pcWriteBuffer, ( char * ) "Read  : ", strlen( "Read  : " ) );
    xsprintf(configstr, "0x%02X", data);
	strncat( pcWriteBuffer, ( char * ) configstr, strlen( configstr ) );
	strncat( pcWriteBuffer, (const char *)("\r\n"), strlen( "\r\n" ) );
    
	xReturn = pdFALSE;
	return xReturn;
}

static BaseType_t prvWriteTemperatureEnable( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	//const char *pcParameter;
	//BaseType_t xParameterStringLength;
	BaseType_t xReturn;
	//static UBaseType_t uxParameterNumber = 0;

	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );
	char configstr[40] = {0};
    //ManufacturerBlockAccess()で0x44ACに0x0273を書き込み
    //アクセスするアドレスはリトルエンディアンだけどデータはビッグエンディアン
    uint8_t txdata[4] = {0x03,  0xA9, 0x44, 0x0B};
    

    HAL_I2C_Mem_Write(&hi2c1, BQ78350_I2C_ADDR, MANUFACTURER_BLOCK_ACCESS, I2C_MEMADD_SIZE_8BIT, txdata, sizeof(txdata)/sizeof(txdata[0]), 10);
    uint16_t addr = *(uint16_t *)(&txdata[1]);
    uint8_t data = txdata[3];

    sprintf( pcWriteBuffer, "Data Flash : " );
    xsprintf(configstr, "0x%04X ", addr);
	strncat( pcWriteBuffer, ( char * ) configstr, strlen( configstr ) );
    strncat( pcWriteBuffer, ( char * ) "Write : ", strlen( "Write : " ) );
    xsprintf(configstr, "0x%02X", data);
	strncat( pcWriteBuffer, ( char * ) configstr, strlen( configstr ) );
	strncat( pcWriteBuffer, (const char *)("\r\n"), strlen( "\r\n" ) );

	xReturn = pdFALSE;
	return xReturn;
}

static BaseType_t prvReadAFECellMap( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	//const char *pcParameter;
	//BaseType_t xParameterStringLength;
	BaseType_t xReturn;
	//static UBaseType_t uxParameterNumber = 0;

	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	char configstr[10] = {0};
    //ManufacturerBlockAccess()で0x44ACの値を読み出し
    const uint8_t txdata[3] = {0x02, 0xAC, 0x44};
    const uint8_t rxdata[5] = {0};

    HAL_I2C_Mem_Write(&hi2c1, BQ78350_I2C_ADDR, MANUFACTURER_BLOCK_ACCESS, I2C_MEMADD_SIZE_8BIT, txdata, sizeof(txdata)/sizeof(txdata[0]), 10);
    HAL_I2C_Mem_Read(&hi2c1, BQ78350_I2C_ADDR, MANUFACTURER_BLOCK_ACCESS, I2C_MEMADD_SIZE_8BIT, rxdata, sizeof(rxdata)/sizeof(rxdata[0]), 10);
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

static BaseType_t prvReadTemperature( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	//const char *pcParameter;
	//BaseType_t xParameterStringLength;
	BaseType_t xReturn;
	//static UBaseType_t uxParameterNumber = 0;

	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );

	char configstr[10] = {0};

    int16_t temperature = 0;;
    uint8_t dastatus2[17] = {0};

    HAL_I2C_Mem_Read(&hi2c1, BQ78350_I2C_ADDR, TEMPERATURE, I2C_MEMADD_SIZE_8BIT, &temperature, 2, 1000);
    HAL_I2C_Mem_Read(&hi2c1, BQ78350_I2C_ADDR, DASTATUS_2, I2C_MEMADD_SIZE_8BIT, dastatus2, 17, 1000);

    // Temperature  Unit : 0.1 K[degC]
    int16_t TS1_temp = *(int16_t *)(&dastatus2[5]);
    int16_t TS2_temp = *(int16_t *)(&dastatus2[7]);
    //int16_t TS3_temp = *(int16_t *)(&dastatus2[9]);
    int16_t Cell_temp = *(int16_t *)(&dastatus2[11]);
    //int16_t FET_temp = *(int16_t *)(&dastatus2[13]);
    int16_t Int_Gauge_temp = *(int16_t *)(&dastatus2[15]);

    // Convert Kelvin to degree Celsius
    const int16_t K2degC = 2731;    // Unit : 0.1 K[degC]
    temperature -= K2degC;
    TS1_temp -= K2degC;
    TS2_temp -= K2degC;
    Cell_temp -= K2degC;
    Int_Gauge_temp -= K2degC;
    
    xsprintf(configstr, "%4d.%d", temperature/10, temperature%10);
	sprintf( pcWriteBuffer, "Temperature Unit : degree Celsius\r\nSBS Cmd (0x08) : " );
    xsprintf(configstr, "%3d.%d", temperature/10, temperature%10);
    strncat( pcWriteBuffer, ( char * ) configstr, strlen( configstr ) );
    strncat( pcWriteBuffer, (const char *)("\r\nTS1            : "), strlen("\r\nTS1            : " ) );
    xsprintf(configstr, "%3d.%d", TS1_temp/10, TS1_temp%10);
	strncat( pcWriteBuffer, ( char * ) configstr, strlen( configstr ) );
	strncat( pcWriteBuffer, (const char *)("\r\nTS2            : "), strlen("\r\nTS2            : " ) );
    xsprintf(configstr, "%3d.%d", TS2_temp/10, TS2_temp%10);
    strncat( pcWriteBuffer, ( char * ) configstr, strlen( configstr ) );
    strncat( pcWriteBuffer, (const char *)("\r\nCell           : "), strlen("\r\nCell           : " ) );
    xsprintf(configstr, "%3d.%d", Cell_temp/10, Cell_temp%10);
    strncat( pcWriteBuffer, ( char * ) configstr, strlen( configstr ) );
    strncat( pcWriteBuffer, (const char *)("\r\nInternal_Gauge : "), strlen("\r\nInternal_Gauge : " ) );
    xsprintf(configstr, "%3d.%d", Int_Gauge_temp/10, Int_Gauge_temp%10);
    strncat( pcWriteBuffer, ( char * ) configstr, strlen( configstr ) );
	strncat( pcWriteBuffer, (const char *)("\r\n"), strlen( " degC\r\n" ) );
    
    
	xReturn = pdFALSE;
	return xReturn;
}

static BaseType_t prvReadFETOptions( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
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
    const uint8_t txdata[3] = {0x02, 0x5F, 0x44};
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

static BaseType_t prvLEDDisplayEnable( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
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

    uint16_t txdata = LEDDISPLAYENABLE;
    BQ78350_Write(MANUFACTURER_ACCESS, (uint8_t *)&txdata, 1);

	sprintf( pcWriteBuffer, "LEDDisplayEnable()");
	strncat( pcWriteBuffer, ( char * ) configstr, strlen( configstr ) );
	strncat( pcWriteBuffer, (const char *)("\r\n"), strlen( "\r\n" ) );

	xReturn = pdFALSE;
	return xReturn;
}

static BaseType_t prvCHG_TET( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
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

    uint16_t txdata = CHG_FET;
    BQ78350_Write(MANUFACTURER_ACCESS, (uint8_t *)&txdata, 1);

	sprintf( pcWriteBuffer, "CHG_FET()");
	strncat( pcWriteBuffer, ( char * ) configstr, strlen( configstr ) );
	strncat( pcWriteBuffer, (const char *)("\r\n"), strlen( "\r\n" ) );

	xReturn = pdFALSE;
	return xReturn;
}

static BaseType_t prvDSG_TET( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
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

    uint16_t txdata = DSG_FET;
    BQ78350_Write(MANUFACTURER_ACCESS, (uint8_t *)&txdata, 1);

	sprintf( pcWriteBuffer, "DSG_FET()");
	strncat( pcWriteBuffer, ( char * ) configstr, strlen( configstr ) );
	strncat( pcWriteBuffer, (const char *)("\r\n"), strlen( "\r\n" ) );

	xReturn = pdFALSE;
	return xReturn;
}

static BaseType_t prvStatus( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
	//const char *pcParameter;
	//BaseType_t xParameterStringLength;
	BaseType_t xReturn;
	//static UBaseType_t uxParameterNumber = 0;

	( void ) pcCommandString;
	( void ) xWriteBufferLen;
	configASSERT( pcWriteBuffer );
	char configstr[40] = {0};
    const char NameStr[] = "\r\nDeviceName       : ";
    const char ChemistryStr[] = "\r\nDeviceChemistry  : ";

    uint8_t ManufacturerNameStr[18] = {0};
    uint8_t DeviceNameStr[8] = {0};
    uint8_t DeviceChemistryStr[5] = {0};

    HAL_I2C_Mem_Read(&hi2c1, BQ78350_I2C_ADDR, MANUFACTURER_NAME, I2C_MEMADD_SIZE_8BIT, (uint8_t *)ManufacturerNameStr, sizeof(ManufacturerNameStr)/sizeof(ManufacturerNameStr[0]), 10);
    HAL_I2C_Mem_Read(&hi2c1, BQ78350_I2C_ADDR, DEVICE_NAME, I2C_MEMADD_SIZE_8BIT, (uint8_t *)DeviceNameStr, sizeof(DeviceNameStr)/sizeof(DeviceNameStr[0]), 10);
    HAL_I2C_Mem_Read(&hi2c1, BQ78350_I2C_ADDR, DEVICE_CHEMISTRY, I2C_MEMADD_SIZE_8BIT, (uint8_t *)DeviceChemistryStr, sizeof(DeviceChemistryStr)/sizeof(DeviceChemistryStr[0]), 10);

	sprintf( pcWriteBuffer, "ManufacturerName : ");
	strncat( pcWriteBuffer, ( const char * ) (&ManufacturerNameStr[1]) , sizeof(ManufacturerNameStr)/sizeof(ManufacturerNameStr[0]) - 1 );
    strncat( pcWriteBuffer, ( const char * ) NameStr, strlen( NameStr ) );
    strncat( pcWriteBuffer, ( const char * ) (&DeviceNameStr[1]), sizeof(DeviceNameStr)/sizeof(DeviceNameStr[0]) - 1 );
	strncat( pcWriteBuffer, ( const char *) ChemistryStr, strlen( ChemistryStr ) );
    strncat( pcWriteBuffer, ( const char * ) (&DeviceChemistryStr[1]), sizeof(DeviceChemistryStr)/sizeof(DeviceChemistryStr[0]) - 1 );
	strncat( pcWriteBuffer, ( const char *)("\r\n"), strlen( "\r\n" ) );
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
	"\r\nvolt:\r\n 0x09 Voltage\r\n 0x31 CellVoltage15\r\n   |       |      \r\n 0x3F CellVoltage1\r\n",
	prvVoltage, /* The function to run. */
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

static const CLI_Command_Definition_t xParameterfet =
{
	"fet",
	"\r\nfet:\r\n HostFETControl\r\n",
	prvfet, /* The function to run. */
	0 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xParameterReadFETOptions =
{
	"fetop",
	"\r\nfetop:\r\n ManufacturerBlockAccess Read FET Options (0x445F)\r\n",
	prvReadFETOptions, /* The function to run. */
	0 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xParameterLEDDisplayEnable =
{
	"led",
	"\r\nled:\r\n ManufacturerAccess() LEDDisplayEnable() (0x0027)\r\n",
	prvLEDDisplayEnable, /* The function to run. */
	0 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xParameterCHG_FET =
{
	"chg",
	"\r\nchg:\r\n ManufacturerAccess() CHG_FET() (0x001F)\r\n",
	prvCHG_TET, /* The function to run. */
	0 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xParameterDSG_FET =
{
	"dsg",
	"\r\ndsg:\r\n ManufacturerAccess() DSG_FET() (0x0020)\r\n",
	prvDSG_TET, /* The function to run. */
	0 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xParameterStatus =
{
	"name",
	"\r\nname:\r\n 0x20 ManufacturerName\r\n 0x21 DeviceName\r\n 0x22 DeviceChemistry\r\n",
	prvStatus, /* The function to run. */
	0 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xParameterTemperature =
{
	"temp",
	"\r\ntemp:\r\n 0x08 Temperature\r\n",
	prvReadTemperature, /* The function to run. */
	0 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xParameterWTemperatureEnable =
{
	"rtempen",
	"\r\nrtempen:\r\n 0x20 ManufacturerName\r\n 0x21 DeviceName\r\n 0x22 DeviceChemistry\r\n",
	prvReadTemperatureEnable, /* The function to run. */
	0 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xParameterRTemperatureEnable =
{
	"wtempen",
	"\r\nwtempen:\r\n 0x20 ManufacturerName\r\n 0x21 DeviceName\r\n 0x22 DeviceChemistry\r\n",
	prvWriteTemperatureEnable, /* The function to run. */
	0 /* No parameters are expected. */
};

static const CLI_Command_Definition_t xParameterDAStatus2 =
{
	"da2",
	"\r\nda2:\r\n 0x72 DAStatus2\r\n",
	prvDAStatus2, /* The function to run. */
	0 /* No parameters are expected. */
};

void vRegisterbq78350CLICommands( void ){

	FreeRTOS_CLIRegisterCommand( &xParameterbq769x0_ReadRegister );
	FreeRTOS_CLIRegisterCommand( &xParameterbq769x0_WriteRegister );
    FreeRTOS_CLIRegisterCommand( &xParametervolt );
    FreeRTOS_CLIRegisterCommand( &xParameterRAFECellMap );
    FreeRTOS_CLIRegisterCommand( &xParameterWAFECellMap );
    FreeRTOS_CLIRegisterCommand( &xParameterfet );
    FreeRTOS_CLIRegisterCommand( &xParameterReadFETOptions );
    FreeRTOS_CLIRegisterCommand( &xParameterLEDDisplayEnable );
    FreeRTOS_CLIRegisterCommand( &xParameterCHG_FET );
    FreeRTOS_CLIRegisterCommand( &xParameterDSG_FET );
    FreeRTOS_CLIRegisterCommand( &xParameterStatus );
    FreeRTOS_CLIRegisterCommand( &xParameterTemperature );
    FreeRTOS_CLIRegisterCommand( &xParameterWTemperatureEnable );
    FreeRTOS_CLIRegisterCommand( &xParameterRTemperatureEnable );
    FreeRTOS_CLIRegisterCommand( &xParameterDAStatus2 );

}