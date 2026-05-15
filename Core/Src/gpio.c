/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, output_06_Pin|output_08_Pin|output_14_Pin|output_12_Pin
                          |output_07_Pin|output_05_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, gpio_output_19_additional_Pin|output_04_Pin|output_11_Pin|output_02_Pin
                          |output_01_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, output_15_Pin|output_13_Pin|output_16_Pin|output_09_Pin
                          |output_17_Pin|output_18_Pin|output_10_Pin|output_03_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : input_07_Pin input_08_Pin input_09_Pin input_18_Pin
                           input_16_Pin input_15_Pin input_17_Pin input_01_Pin
                           input_02_Pin input_03_Pin */
  GPIO_InitStruct.Pin = input_07_Pin|input_08_Pin|input_09_Pin|input_18_Pin
                          |input_16_Pin|input_15_Pin|input_17_Pin|input_01_Pin
                          |input_02_Pin|input_03_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : input_11_Pin input_13_Pin */
  GPIO_InitStruct.Pin = input_11_Pin|input_13_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pins : input_10_Pin input_12_Pin input_05_Pin */
  GPIO_InitStruct.Pin = input_10_Pin|input_12_Pin|input_05_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : output_06_Pin output_08_Pin output_14_Pin output_12_Pin
                           output_07_Pin output_05_Pin */
  GPIO_InitStruct.Pin = output_06_Pin|output_08_Pin|output_14_Pin|output_12_Pin
                          |output_07_Pin|output_05_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : gpio_output_19_additional_Pin output_04_Pin output_11_Pin output_02_Pin
                           output_01_Pin */
  GPIO_InitStruct.Pin = gpio_output_19_additional_Pin|output_04_Pin|output_11_Pin|output_02_Pin
                          |output_01_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : input_14_Pin input_06_Pin gpio_input_19_additional_Pin */
  GPIO_InitStruct.Pin = input_14_Pin|input_06_Pin|gpio_input_19_additional_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : output_15_Pin output_13_Pin output_16_Pin output_09_Pin
                           output_17_Pin output_18_Pin output_10_Pin output_03_Pin */
  GPIO_InitStruct.Pin = output_15_Pin|output_13_Pin|output_16_Pin|output_09_Pin
                          |output_17_Pin|output_18_Pin|output_10_Pin|output_03_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : input_04_Pin */
  GPIO_InitStruct.Pin = input_04_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(input_04_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
