/*
 *  usb_cdc.c
 *
 *  Created on: 2021/10/24
 *      Author: Yuki Kusakabe
 */

#include "usb_cdc.h"
#include "usbd_cdc_if.h"
#include "stdarg.h"

osThreadId UsbCDC_TransmitTaskHandle;
static uint32_t UsbCDC_TransmitTaskBuffer[ 256 ];
static osStaticThreadDef_t UsbCDC_TransmitTaskControlBlock;

extern osMessageQId CDCQueueHandle;

void UsbCDC_TransmitTask(void const * argument){

    for(;;)
    {
        osEvent event = osMessageGet(CDCQueueHandle, osWaitForever);
        if(event.status == osEventMessage){
            com_printf("%ld\r\n", (int16_t)event.value.v);
        }
    }
}

void com_printf(const char* format, ...){

    va_list arg;
    uint8_t len;
    static char printf_buf[255];

    va_start(arg, format);

    len = vsnprintf(printf_buf, 255, format, arg);

    if(len > 0){
        CDC_Transmit_FS((uint8_t *)printf_buf, len);
    }
}

void UsbCDC_RegisterTasks(void){

    osThreadStaticDef(comTask, UsbCDC_TransmitTask, osPriorityNormal, 0, 256, UsbCDC_TransmitTaskBuffer, &UsbCDC_TransmitTaskControlBlock);
    UsbCDC_TransmitTaskHandle = osThreadCreate(osThread(comTask), NULL);
}