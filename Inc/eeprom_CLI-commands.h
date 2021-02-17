/*
 *  eeprom_CLI-commands.h
 *
 *  Created on: 2021/02/14
 *      Author: Yuki
 */

#ifndef __EEPROM_CLI_COMMANDS_H
#define __EEPROM_CLI_COMMANDS_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Standard includes. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"

#include "cmsis_os.h"

#include "FreeRTOS_CLI.h"

void vRegisterEEPROMCLICommands( void );

#ifdef __cplusplus
}
#endif
#endif /* __EEPROM_CLI_COMMANDS_H */
