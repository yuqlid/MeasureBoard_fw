/*
 *  sbs.h
 *
 *  Created on: 2021/07/11
 *      Author: Yuki Kusakabe
 */

#ifndef __SBS_H_
#define __SBS_H_

#ifdef __cplusplus
 extern "C" {
#endif
//SBS Commands Address
#define MANUFACTURER_ACCESS                 0x00
#define REMAINING_TIME_ALARM
#define BATTERY_MODE
#define ATRATE
#define ATRATE_TIME_TO_FULL
#define ATRATE_TIME_TO_EMPTY
#define ATRATE_OK
#define TEMPERATURE                         0x08
#define VOLTAGE                             0x09
#define CURRENT
#define AVERAGE_CURRENT
#define MAX_ERROR
#define RELATIVE_STATEOFCHARGE
#define ABSOLUTE_STATEOFCHARGE
#define REMAINING_CAPACITY
#define FULLCHARGE_CAPACITY
#define RUN_TIME_TO_EMPTY
#define AVERAGE_TIME_TO_EMPTY
#define AVERAGE_TIME_TO_FULL
#define CHARGING_CURRENT
#define CHARGING_VOLTAGE
#define BATTERY_STATUS                      0x16
#define CYCLE_COUNT
#define DESIGN_CAPACITY
#define DESIGN_VOLTAGE
#define SPECIFICATION_INFO                  0x1A
#define MANUFACTURER_DATE
#define SERIALNUMBER
#define MANUFACTURER_NAME                   0x20
#define DEVICE_NAME                         0x21
#define DEVICE_CHEMISTRY                    0x22
#define MANUFACTURER_DATA                   0x23
#define HOST_FET_CONTROL                    0x2B
#define GPIO_STATUS
#define GPIO_CONTROL
#define VAUXVOLTAGE
#define AUTHENTICATE
#define CELLVOLTAGE_15                      0x31
#define CELLVOLTAGE_14                      0x32
#define CELLVOLTAGE_13                      0x33
#define CELLVOLTAGE_12                      0x34
#define CELLVOLTAGE_11                      0x35
#define CELLVOLTAGE_10                      0x36
#define CELLVOLTAGE_9                       0x37
#define CELLVOLTAGE_8                       0x38
#define CELLVOLTAGE_7                       0x39
#define CELLVOLTAGE_6                       0x3A
#define CELLVOLTAGE_5                       0x3B
#define CELLVOLTAGE_4                       0x3C
#define CELLVOLTAGE_3                       0x3D
#define CELLVOLTAGE_2                       0x3E
#define CELLVOLTAGE_1                       0x3F
#define DYNAMIC_POWER
#define EXTAVE_CELL_VOLTAGE
#define PENDING_EDV
#define STATEOFHEALTH
#define SAFETY_ALERT
#define SAFETYSTATUS
#define PF_ALERT
#define PF_STATUS
#define OPERATION_STATUS
#define CHARGING_STATUS
#define GAUGING_STATUS
#define MANUFACTURING_STATUS
#define AFE_STATUS
#define AFE_CONFIG
#define AFE_VCX
#define AFE_DATA
#define LIFETIME_DATA_BLOCK_2
#define LIFETIME_DATA_BLOCK_1
#define LIFETIME_DATA_BLOCK_3
#define LIFETIME_DATA_BLOCK_4
#define LIFETIME_DATA_BLOCK_5
#define LIFETIME_DATA_BLOCK_6
#define LIFETIME_DATA_BLOCK_7
#define MANUFACTURER_INFO                   0x70
#define DASTATUS_1                          0x71
#define DASTATUS_2                          0x72
#define CUV_SNAPSHOT
#define COV_SNAPSHOT

#define MANUFACTURER_BLOCK_ACCESS           0x44



//Manufacturer Access Commands
#define AFE_DELAY_DISABLE                   0x001C
#define SAFE_TOGGLE                         0x001D
#define PRE_CHG_FET                         0x001E
#define CHG_FET                             0x001F
#define DSG_FET                             0x0020
#define FET_CONTROL                         0x0022
#define LEDDISPLAYENABLE                    0x0027
#define DEVICE_RESET						0x0041
#define OPERATION_STATUS					0x0054
#define SECURITY_KEYS                       0x0035

//Data Flash Address
//Configuration
#define TS1_TEMPERATURE                     0x4200
#define TS2_TEMPERATURE                     0x4202
#define TS3_TEMPERATURE                     0x4204
#define GAUGE_INTERNAL_TEMPERATURE          0x4206
#define FET_OPTIONS							0x445F
#define SBS_GAUGING_CONFIG					0x4461
#define SMB_CONFIG							0x4462
#define POWER_CONFIG						0x4472
#define LED_CONFIG							0x448C
#define TEMPERATURE_ENABLE					0x44A9
#define TEMPERATURE_MODE					0x44AA
#define DA_CONFIGURATION					0x44AB
#define AFE_CELL_MAP						0x44AC
#define CEDV_GAUGING_CONFIG					0x458B
#define SOC_FLAG_CONFIG						0x45D4
#define BALANCING_CONFIG					0x460C

//Protection Settings
#define PROTECTION_CONFIG					0x44C5
#define PROTECTIONS_A_ENABLED				0x44C6
#define PROTECTIONS_B_ENABLED				0x44C7
#define PROTECTIONS_C_ENABLED				0x44C8

//Protections Value
#define CUV_THRESHOLD						0x44CC
#define CUV_DELAY							0x44CE
#define CUV_RECOVERY						0x44CF
#define COV_THRESHOLD						0x44D1
#define COV_DELAY							0x44D3
#define COV_RECOVERY						0x44D4
#define OCC_THRESHOLD						0x44D6
#define OCC_DELAY							0x44D8
#define OCC_RECOVERY_THRESHOLD				0x44D9
#define OCC_RECOVERY_DELAY					0x44DB
#define OCD_THRESHOLD						0x44DC
#define OCD_DELAY							0x44DE
#define OCD_RECOVERY_THRESHOLD				0x44DF
#define OCD_RECOVERY_DELAY					0x44E1

//Fuel Gauging Design
#define DESIGN_CAPACITY_MA_CONFIG			0x45CC
#define DESIGN_CAPACITY_CWH_CONFIG			0x45CE
#define DESIGN_VOLTAGE_CONFIG				0x45D0

//SBS Configuration
//#define MANUFACTURER_NAME					0x406F
//#define DEVICE_CHEMISTRY					0x4099
#define INITIAL_BATT_MODE					0x44A5
#define SPEC_INFO							0x44A7
#define MANUFACTURING_STATUS_INIT			0x42C8

#ifdef __cplusplus
}
#endif
#endif /* __SBS_H_ */