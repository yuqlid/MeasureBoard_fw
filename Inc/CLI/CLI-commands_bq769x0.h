/*
 *  CLI-commands_bq769x0.h
 *
 *  Created on: 2021/07/03
 *      Author: Yuki Kusakabe
 */

#ifndef __CLI_COMMANDS_BQ769X0_H
#define __CLI_COMMANDS_BQ769X0_H
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

#include "bq769x0.h"

void vRegisterbq769x0CLICommands( void );

#ifdef __cplusplus
}
#endif
#endif /* __CLI_COMMANDS_BQ769X0_H */
