/*
 *  CLI-commands_as5048a.h
 *
 *  Created on: 2020/11/15
 *      Author: Yuki
 */

#ifndef __CLI_COMMANDS_AS5048A_H
#define __CLI_COMMANDS_AS5048A_H
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


void vRegisteras5048aCLICommands( void );

#ifdef __cplusplus
}
#endif
#endif /* __CLI_COMMANDS_AS5048A_H */
