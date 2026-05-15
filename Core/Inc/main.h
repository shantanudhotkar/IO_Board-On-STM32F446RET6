/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

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
#define input_07_Pin GPIO_PIN_13
#define input_07_GPIO_Port GPIOC
#define input_08_Pin GPIO_PIN_14
#define input_08_GPIO_Port GPIOC
#define input_09_Pin GPIO_PIN_15
#define input_09_GPIO_Port GPIOC
#define input_11_Pin GPIO_PIN_0
#define input_11_GPIO_Port GPIOH
#define input_13_Pin GPIO_PIN_1
#define input_13_GPIO_Port GPIOH
#define input_18_Pin GPIO_PIN_0
#define input_18_GPIO_Port GPIOC
#define input_16_Pin GPIO_PIN_1
#define input_16_GPIO_Port GPIOC
#define input_15_Pin GPIO_PIN_2
#define input_15_GPIO_Port GPIOC
#define input_17_Pin GPIO_PIN_3
#define input_17_GPIO_Port GPIOC
#define input_10_Pin GPIO_PIN_0
#define input_10_GPIO_Port GPIOA
#define input_12_Pin GPIO_PIN_1
#define input_12_GPIO_Port GPIOA
#define output_06_Pin GPIO_PIN_6
#define output_06_GPIO_Port GPIOA
#define output_08_Pin GPIO_PIN_7
#define output_08_GPIO_Port GPIOA
#define gpio_output_19_additional_Pin GPIO_PIN_5
#define gpio_output_19_additional_GPIO_Port GPIOC
#define input_14_Pin GPIO_PIN_0
#define input_14_GPIO_Port GPIOB
#define output_15_Pin GPIO_PIN_1
#define output_15_GPIO_Port GPIOB
#define output_13_Pin GPIO_PIN_2
#define output_13_GPIO_Port GPIOB
#define output_16_Pin GPIO_PIN_10
#define output_16_GPIO_Port GPIOB
#define output_09_Pin GPIO_PIN_12
#define output_09_GPIO_Port GPIOB
#define output_17_Pin GPIO_PIN_15
#define output_17_GPIO_Port GPIOB
#define output_04_Pin GPIO_PIN_6
#define output_04_GPIO_Port GPIOC
#define output_11_Pin GPIO_PIN_7
#define output_11_GPIO_Port GPIOC
#define output_02_Pin GPIO_PIN_8
#define output_02_GPIO_Port GPIOC
#define output_01_Pin GPIO_PIN_9
#define output_01_GPIO_Port GPIOC
#define output_14_Pin GPIO_PIN_8
#define output_14_GPIO_Port GPIOA
#define output_12_Pin GPIO_PIN_9
#define output_12_GPIO_Port GPIOA
#define output_07_Pin GPIO_PIN_11
#define output_07_GPIO_Port GPIOA
#define output_05_Pin GPIO_PIN_12
#define output_05_GPIO_Port GPIOA
#define input_05_Pin GPIO_PIN_15
#define input_05_GPIO_Port GPIOA
#define input_01_Pin GPIO_PIN_10
#define input_01_GPIO_Port GPIOC
#define input_02_Pin GPIO_PIN_11
#define input_02_GPIO_Port GPIOC
#define input_03_Pin GPIO_PIN_12
#define input_03_GPIO_Port GPIOC
#define input_04_Pin GPIO_PIN_2
#define input_04_GPIO_Port GPIOD
#define output_18_Pin GPIO_PIN_4
#define output_18_GPIO_Port GPIOB
#define output_10_Pin GPIO_PIN_6
#define output_10_GPIO_Port GPIOB
#define input_06_Pin GPIO_PIN_7
#define input_06_GPIO_Port GPIOB
#define output_03_Pin GPIO_PIN_8
#define output_03_GPIO_Port GPIOB
#define gpio_input_19_additional_Pin GPIO_PIN_9
#define gpio_input_19_additional_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
