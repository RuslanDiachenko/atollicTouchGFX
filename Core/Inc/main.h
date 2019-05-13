/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "cmsis_os.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern osMutexId debugMutexHandle;
extern UART_HandleTypeDef huart2;
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
extern void customPrintf(char *str, ...);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LCD_DISP_Pin GPIO_PIN_9
#define LCD_DISP_GPIO_Port GPIOF
#define FLASH_IC_CS_Pin GPIO_PIN_15
#define FLASH_IC_CS_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */
#define HSE_VALUE    ((uint32_t)16000000U)
#define LCD_DISP_Pin GPIO_PIN_9
#define LCD_DISP_GPIO_Port GPIOF
#define FLASH_IC_CS_Pin GPIO_PIN_15
#define FLASH_IC_CS_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

#define FW_VERSION_MAJOR  0
#define FW_VERSION_MINOR  5
#define FW_VERSION_BUILD  1

typedef enum
{
  RV_SUCCESS,
  RV_FAILURE,
  RV_NULLPTR,
  RV_NOT_READY
} RV_t;

typedef enum
{
  NONE = 0,
  DATE_TIME_CHANGED,
  SLEEP_AFTER_TIMER
} ui_msg_type_t;

typedef struct
{
  uint8_t hour;
  uint8_t minute;
  uint8_t seconds;
  uint8_t hF;
  uint8_t dayOfWeek;
  uint8_t day;
  uint8_t month;
} date_time_state_t;

typedef struct
{
  ui_msg_type_t         msgType;
  date_time_state_t     dateTime;
} ui_state_t;

typedef struct
{
  uint8_t screenState;
  uint8_t infinity;
  uint8_t duration;
} sleep_after_state_t;

#define DEBUG
#if defined(DEBUG)

#define DISPLAY_LOG(severity, cmp, msg, ...)                                              \
                      {                                                                   \
                        osMutexWait(debugMutexHandle, osWaitForever);                     \
                        customPrintf((char *)("<" severity ">" "<" cmp ">" "<%-30s(%04i)> " msg "\r\n"),\
                               __FUNCTION__, __LINE__, ##__VA_ARGS__);                    \
                        osMutexRelease(debugMutexHandle);                                 \
                      }
#define DBG_LOG(cmp, msg, ...)   DISPLAY_LOG("DEBUG", cmp, msg, ##__VA_ARGS__)
#define DBG_WARN(cmp, msg, ...)  DISPLAY_LOG("WARNING", cmp, msg, ##__VA_ARGS__)
#define DBG_ERR(cmp, msg, ...)   DISPLAY_LOG("ERROR", cmp, msg, ##__VA_ARGS__)
#define DBG_CRIT(cmp, msg, ...)  DISPLAY_LOG("CRITICAL", cmp, msg, ##__VA_ARGS__)

#else

#define DBG_LOG(cmp, ...)
#define DBG_WARN(cmp, ...)
#define DBG_ERR(cmp, ...)
#define DBG_CRIT(cmp, ...)

#endif

/* components list used for debugging */
#define ETH_MGR       "ETH "
#define HTTPD_MGR     "HTTP"
#define SWITCH_MGR    "SWCH"
#define GUI_MGR		  "GUI "

#define SAGE_PASSWORD "111111"
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
