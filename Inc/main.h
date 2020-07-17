/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void RS485_Transmit(uint8_t ID, uint8_t addr, uint8_t *TxData, uint8_t size);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define TIM17_PERIOD (25-1)
#define TIM17_PSC (7200-1)
#define USART3_Baudrate 115200
#define USART1_Baudrate 1000000
#define TIM16_Period (100-1)
#define TIM16_Prescaler 7200
#define DIP2_Pin GPIO_PIN_13
#define DIP2_GPIO_Port GPIOC
#define TACT1_Pin GPIO_PIN_14
#define TACT1_GPIO_Port GPIOC
#define TACT2_Pin GPIO_PIN_15
#define TACT2_GPIO_Port GPIOC
#define ENC_Z_Pin GPIO_PIN_2
#define ENC_Z_GPIO_Port GPIOA
#define LED0_Pin GPIO_PIN_2
#define LED0_GPIO_Port GPIOB
#define LED1_Pin GPIO_PIN_12
#define LED1_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_13
#define LED2_GPIO_Port GPIOB
#define DIOP1_Pin GPIO_PIN_5
#define DIOP1_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
