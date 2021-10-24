/*
 *  usb_cdc.h
 *
 *  Created on: 2021/10/24
 *      Author: Yuki Kusakabe
 */

#ifndef USB_CDCH_H_
#define USB_CDCH_H_
#ifdef __cplusplus
 extern "C" {
#endif

#include "cmsis_os.h"

extern osThreadId UsbCDC_TransmitTaskHandle;

void UsbCDC_RegisterTasks(void);

#ifdef __cplusplus
}
#endif
#endif /* USB_CDCH_H_ */
