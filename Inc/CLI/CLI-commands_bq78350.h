/*
 *  CLI-commands_bq78350.h
 *
 *  Created on: 2021/08/04
 *      Author: Yuki Kusakabe
 */

#ifndef __CLI_COMMANDS_BQ78350_H
#define __CLI_COMMANDS_BQ78350_H
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

#include "bq78350.h"

void vRegisterbq78350CLICommands( void );

#ifdef __cplusplus
}
#endif
#endif /* __CLI_COMMANDS_BQ78350_H */
