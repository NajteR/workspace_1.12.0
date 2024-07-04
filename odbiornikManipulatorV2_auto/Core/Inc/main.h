/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32l1xx_hal.h"

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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define L_D1_Pin GPIO_PIN_2
#define L_D1_GPIO_Port GPIOC
#define L_PWM_Pin GPIO_PIN_1
#define L_PWM_GPIO_Port GPIOA
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define Czlon4PWM_Pin GPIO_PIN_6
#define Czlon4PWM_GPIO_Port GPIOA
#define Czlon5PWM_Pin GPIO_PIN_7
#define Czlon5PWM_GPIO_Port GPIOA
#define Czlon4_D1_Pin GPIO_PIN_5
#define Czlon4_D1_GPIO_Port GPIOC
#define R_PWM_Pin GPIO_PIN_0
#define R_PWM_GPIO_Port GPIOB
#define CE_Pin GPIO_PIN_1
#define CE_GPIO_Port GPIOB
#define CSN_Pin GPIO_PIN_2
#define CSN_GPIO_Port GPIOB
#define Czlon1_D1_Pin GPIO_PIN_10
#define Czlon1_D1_GPIO_Port GPIOB
#define Czlon5_D2_Pin GPIO_PIN_12
#define Czlon5_D2_GPIO_Port GPIOB
#define Czlon3_D2_Pin GPIO_PIN_6
#define Czlon3_D2_GPIO_Port GPIOC
#define Czlon2_D2_Pin GPIO_PIN_7
#define Czlon2_D2_GPIO_Port GPIOC
#define Czlon3_D1_Pin GPIO_PIN_8
#define Czlon3_D1_GPIO_Port GPIOC
#define Czlon1_D2_Pin GPIO_PIN_8
#define Czlon1_D2_GPIO_Port GPIOA
#define Czlon2_D1_Pin GPIO_PIN_9
#define Czlon2_D1_GPIO_Port GPIOA
#define Czlon5_D1_Pin GPIO_PIN_11
#define Czlon5_D1_GPIO_Port GPIOA
#define Czlon4_D2_Pin GPIO_PIN_12
#define Czlon4_D2_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define L_D2_Pin GPIO_PIN_10
#define L_D2_GPIO_Port GPIOC
#define R_D2_Pin GPIO_PIN_12
#define R_D2_GPIO_Port GPIOC
#define R_D1_Pin GPIO_PIN_2
#define R_D1_GPIO_Port GPIOD
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define Podstawa_D2_Pin GPIO_PIN_4
#define Podstawa_D2_GPIO_Port GPIOB
#define Podstawa_D1_Pin GPIO_PIN_5
#define Podstawa_D1_GPIO_Port GPIOB
#define PodstawaPWM_Pin GPIO_PIN_6
#define PodstawaPWM_GPIO_Port GPIOB
#define Czlon1PWM_Pin GPIO_PIN_7
#define Czlon1PWM_GPIO_Port GPIOB
#define Czlon2PWM_Pin GPIO_PIN_8
#define Czlon2PWM_GPIO_Port GPIOB
#define Czlon3PWM_Pin GPIO_PIN_9
#define Czlon3PWM_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
