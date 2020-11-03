/*
 *  CLI-commands_bq34z100-G1.h
 *
 *  Created on: 2020/11/03
 *      Author: Yuki
 */

#ifndef __CLI_COMMANDS_BQ34Z100G1_H
#define __CLI_COMMANDS_BQ34Z100G1_H
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

#include "bq34z100-G1.h"

void vRegisterbq34z100G1CLICommands( void );

#ifdef __cplusplus
}
#endif
#endif /* __CLI_COMMANDS_BQ34Z100G1_H */
