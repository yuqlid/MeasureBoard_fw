/*
 *  scramble_tasks.h
 *
 *  Created on: 2020/08/02
 *      Author: Yuki
 */

#ifndef __SCRAMBLE_TAKSS_H
#define __SCRAMBLE_TAKSS_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Standard includes. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "cmsis_os.h"
#include "FreeRTOS_CLI.h"

extern osThreadId rs485DribbleTaskHandle;
extern osThreadId rs485TransmitTaskHandle;
extern osThreadId COMSendTaskHandle;
extern osThreadId EncoderProcessTaskHandle;

void setTargetSpeed(long *speed_rpm);
void setDribbleDuty(long *setduty);
void scramble_RegisterTasks(void);

#ifdef __cplusplus
}
#endif
#endif /* __SCRAMBLE_TAKSS_H */
