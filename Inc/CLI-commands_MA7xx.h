/*
 *  CLI-commands_MA7xx.h
 *
 *  Created on: 2020/11/03
 *      Author: Yuki
 */

#ifndef __CLI_COMMANDS_MA7XX_H
#define __CLI_COMMANDS_MA7XX_H
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


void vRegisterMA7xxCLICommands( void );

#ifdef __cplusplus
}
#endif
#endif /* __CLI_COMMANDS_MA7XX_H */
