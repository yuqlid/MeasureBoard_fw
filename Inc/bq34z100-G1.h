/*
 *  bq34z100-G1.h
 *
 *  Created on: 2020/10/10
 *      Author: Yuki Kusakabe
 */

#ifndef __BQ43Z100_G1_H_
#define __BQ43Z100_G1_H_

#include "stdint.h"

#ifdef __cplusplus
 extern "C" {
#endif

#define BQ34Z100G1_I2C_ADDR 0b1010101

typedef enum {
    CONTROL_STATUS      = 0x0000,
    DEVICE_TYPE         = 0x0001,
    FW_VERSION          = 0x0002,
    HW_VERSION          = 0x0003,
    RESET_DATA          = 0x0005,
    PREV_MACWRITE       = 0x0007,
    CHEM_ID             = 0x0008,
    BOARD_OFFSET        = 0x0009,
    CC_OFFSET           = 0x000A,
    CC_OFFSET_SAVE      = 0x000B,
    DF_VERSION          = 0x000C,
    SET_FULLSLEEP       = 0x0010,
    STATIC_CHEM_CHKSUM  = 0x0017,
    SEALED              = 0x0020,
    IT_ENABLE           = 0x0021,
    CAL_ENABLE          = 0x002D,
    DEVICE_RESET        = 0x0041,
    EXIT_CAL            = 0x0080,
    ENTER_CAL           = 0x0081,
    OFFSET_CAL          = 0x0082,
    
} bq34z100G1_ControlSubcommands;

typedef enum {
    CONTROL = 0x00,
    SOC     = 0x02,     // StateOfCharge
    ME      = 0x03,     // MaxError
    RM      = 0x04,     // RemainingCapacity
    FCC     = 0x06,     // FullChargeCapacity
    VOLT    = 0x08,     // Voltage
    AI      = 0x0A,     // AverageCurrent
    TEMP    = 0x0C,     // Temperature
    FLAGS   = 0x0E,     // Flags
    I       = 0x10,     // Current
    FLAGSB  = 0x12,     // FlagsB
    
} bq34z100G1_StandardCommands;

typedef enum {

    ATTE        = 0x18,     // AverageTimeToEmpty
    ATTF        = 0x1A,     // AverageTimeToFull
    PCHG        = 0x1C,     // PassedCharge
    DoD0T       = 0x1E,     // DoD0Time
    AE          = 0x24,     // AvailableEnergy
    AP          = 0x26,     // AveragePower
    SERNUM      = 0x28,     // Serial Number
    INTTEMP     = 0x2A,     // Internal_Temperature
    CC          = 0x2C,     // CycleCount
    SOH         = 0x2E,     // StateOfHealth
    CHGV        = 0x30,     // ChargeVoltage
    CHGI        = 0x32,     // ChargeCurrent
    PKCFG       = 0x3A,     // PackConfiguration
    DCAP        = 0x3C,     // DesignCapacity
    DFCLS       = 0x3E,     // DataFlashClass
    DFBLK       = 0x3F,     // DataFlashBlock
    A_DF        = 0x40,     // Authenticate / BlockData
    ACKS_DFD    = 0x54,     // AuthenticateCheckSum / BlockData
    DFD         = 0x55,     // BlockData
    DFDCKS      = 0x60,     // BlockDataCheckSum
    DFDCNTL     = 0x61,     // BlockDataControl
    GN          = 0x62,     // GridNumber
    LS          = 0x63,     // GridNumber
    DEOC        = 0x64,     // DoD@EoC
    QS          = 0x66,     // QStart
    TRC         = 0x68,     // TrueRC
    TFCC        = 0x6A,     // TrueFCC
    ST          = 0x6C,     // StateTime
    QPC         = 0x6E,     // QMaxPassedQ
    DOD0        = 0x70,     // DOD0
    QD0         = 0x72,     // QmaxDOD0
    QT          = 0x74,     // QmaxTime

} bq34z100G1_ExtendeCommands;
/*
typedef enum {
    Safty = 0x02,
    Charge_Inhibit_Cfg  = 0x20,
    Charge  = 0x22,
    Charge_Termination  = 0x24,
    Data = 0x30,
    Discharge = 0x31,
    ManufacturerData = 0x38,
    Lifetime_Data = 0x3B,
    Lifetime_Temp_Samples
    Registers
    Lifetime_Resolution
    LED Display
    Power
    Manufacturer_Info
    IT_Cfg
    Current_Thresholds
    State
    R_a0
    R_a0x
} bq34z100G1_DataFlashSubClass;
*/
void BQ34Z100G1_UNSEAL(void);
void BQ34Z100G1_SEAL(void);
void BQ34Z100G1_RESET(void);
void BQ34Z100G1_BlockDataControl(void);
void BQ34Z100G1_DataFlashClass(void);
void BQ34Z100G1_DataFlashBlock(void);
uint16_t BQ34Z100G1_GetTemprature10degreeCelsius(void);

#ifdef __cplusplus
}
#endif

#endif /* __BQ43Z100_G1_H_ */
