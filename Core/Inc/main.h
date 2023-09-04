/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

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
#define ADC_NRDY_Pin GPIO_PIN_0
#define ADC_NRDY_GPIO_Port GPIOC
#define ADC_NRDY_EXTI_IRQn EXTI0_IRQn
#define PWM_SW_1_Pin GPIO_PIN_0
#define PWM_SW_1_GPIO_Port GPIOA
#define PWM_SW_2_Pin GPIO_PIN_1
#define PWM_SW_2_GPIO_Port GPIOA
#define PWM_SW_3_Pin GPIO_PIN_2
#define PWM_SW_3_GPIO_Port GPIOA
#define PWM_SW_4_Pin GPIO_PIN_3
#define PWM_SW_4_GPIO_Port GPIOA
#define PWM_SW_5_Pin GPIO_PIN_6
#define PWM_SW_5_GPIO_Port GPIOA
#define PWM_SW_6_Pin GPIO_PIN_7
#define PWM_SW_6_GPIO_Port GPIOA
#define ADC_NSS_Pin GPIO_PIN_12
#define ADC_NSS_GPIO_Port GPIOB
#define ADC_SCK_Pin GPIO_PIN_13
#define ADC_SCK_GPIO_Port GPIOB
#define ADC_OUT_Pin GPIO_PIN_14
#define ADC_OUT_GPIO_Port GPIOB
#define ADC_IN_Pin GPIO_PIN_15
#define ADC_IN_GPIO_Port GPIOB
#define PWM_SW_7_Pin GPIO_PIN_6
#define PWM_SW_7_GPIO_Port GPIOB
#define PWM_SW_8_Pin GPIO_PIN_7
#define PWM_SW_8_GPIO_Port GPIOB
#define PWM_SW_9_Pin GPIO_PIN_8
#define PWM_SW_9_GPIO_Port GPIOB
#define PWM_SW_10_Pin GPIO_PIN_9
#define PWM_SW_10_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
