/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define NUM_DI              21
#define NUM_DO              17
#define DEBOUNCE_MS         25
#define MAX_CMD_LEN         32
#define HEARTBEAT_MS        5000
#define ADC_REPORT_MS       100     // send analog values every 100ms
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* ---- ADC DMA buffer ---- */
uint16_t adc_buf[2];   // [0]=PA2(CH2), [1]=PA3(CH3)

/* ---- UART RX ---- */
uint8_t  rx_byte;
char     rx_buf[MAX_CMD_LEN + 1];
uint8_t  rx_idx = 0;

/* ---- UART TX ---- */
char     tx_buf[64];

/* ---- DI state ---- */
typedef struct {
    GPIO_TypeDef* port;
    uint16_t      pin;
} GPIOPin;

const GPIOPin DI_PINS[NUM_DI] = {
    {GPIOC, GPIO_PIN_10},  // DI1
    {GPIOC, GPIO_PIN_11},  // DI2
    {GPIOC, GPIO_PIN_12},  // DI3
    {GPIOD, GPIO_PIN_2},   // DI4
    {GPIOA, GPIO_PIN_15},  // DI5
    {GPIOB, GPIO_PIN_7},   // DI6
    {GPIOC, GPIO_PIN_13},  // DI7
    {GPIOC, GPIO_PIN_14},  // DI8
    {GPIOC, GPIO_PIN_15},  // DI9
    {GPIOA, GPIO_PIN_0},   // DI10
    {GPIOH, GPIO_PIN_0},   // DI11
    {GPIOA, GPIO_PIN_1},   // DI12
    {GPIOH, GPIO_PIN_1},   // DI13
    {GPIOB, GPIO_PIN_0},   // DI14
    {GPIOC, GPIO_PIN_2},   // DI15
    {GPIOC, GPIO_PIN_1},   // DI16
    {GPIOC, GPIO_PIN_3},   // DI17
    {GPIOC, GPIO_PIN_0},   // DI18
    {GPIOB, GPIO_PIN_14},  // DI19 — EMERGENCY
    {GPIOB, GPIO_PIN_13},  // DI20 — DEADMAN
    {GPIOC, GPIO_PIN_4},   // DI21 — POWER_BTN
};

const GPIOPin DO_PINS[NUM_DO] = {
    {GPIOC, GPIO_PIN_9},   // DO1
    {GPIOC, GPIO_PIN_8},   // DO2
    {GPIOB, GPIO_PIN_8},   // DO3
    {GPIOC, GPIO_PIN_6},   // DO4
    {GPIOB, GPIO_PIN_9},   // DO5
    {GPIOC, GPIO_PIN_5},   // DO6
    {GPIOA, GPIO_PIN_12},  // DO7
    {GPIOA, GPIO_PIN_7},   // DO8
    {GPIOB, GPIO_PIN_12},  // DO9
    {GPIOB, GPIO_PIN_6},   // DO10
    {GPIOC, GPIO_PIN_7},   // DO11
    {GPIOB, GPIO_PIN_2},   // DO12
    {GPIOA, GPIO_PIN_8},   // DO13
    {GPIOB, GPIO_PIN_1},   // DO14
    {GPIOB, GPIO_PIN_10},  // DO15
    {GPIOB, GPIO_PIN_15},  // DO16
    {GPIOB, GPIO_PIN_4},   // DO17
};

uint8_t  di_state[NUM_DI];
uint8_t  di_last_state[NUM_DI];
uint32_t di_last_change[NUM_DI];

uint8_t  do_state[NUM_DO];

uint32_t last_heartbeat = 0;
uint32_t last_adc_report = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
extern UART_HandleTypeDef huart1;
extern ADC_HandleTypeDef  hadc1;
extern DAC_HandleTypeDef  hdac;

void uart_send(const char* s) {
    HAL_UART_Transmit(&huart1, (uint8_t*)s, strlen(s), 100);
}

void processCommand(const char* cmd) {
    if (strcmp(cmd, "PING") == 0) {
        uart_send("PONG\n");
        return;
    }

    if (strcmp(cmd, "DI_STATE") == 0) {
        for (uint8_t i = 0; i < NUM_DI; i++) {
            snprintf(tx_buf, sizeof(tx_buf), "DI,%d,%d\n", i+1, di_state[i]);
            uart_send(tx_buf);
        }
        return;
    }

    if (strcmp(cmd, "DO_STATE") == 0) {
        for (uint8_t i = 0; i < NUM_DO; i++) {
            snprintf(tx_buf, sizeof(tx_buf), "DO_STATE,%d,%d\n", i+1, do_state[i]);
            uart_send(tx_buf);
        }
        return;
    }

    if (strncmp(cmd, "DO,", 3) == 0) {
        const char* p = strchr(cmd + 3, ',');
        if (p == NULL) return;
        uint8_t idx = (uint8_t)atoi(cmd + 3);
        uint8_t val = (uint8_t)atoi(p + 1);
        if (idx < 1 || idx > NUM_DO) return;
        HAL_GPIO_WritePin(DO_PINS[idx-1].port, DO_PINS[idx-1].pin,
                          val ? GPIO_PIN_SET : GPIO_PIN_RESET);
        do_state[idx-1] = val ? 1 : 0;
        snprintf(tx_buf, sizeof(tx_buf), "DO_OK,%d,%d\n", idx, do_state[idx-1]);
        uart_send(tx_buf);
        return;
    }

    // ANALOG OUT: "AO,<ch>,<val>"  ch=1(PA4) or 2(PA5), val=0-4095
    if (strncmp(cmd, "AO,", 3) == 0) {
        const char* p = strchr(cmd + 3, ',');
        if (p == NULL) return;
        uint8_t ch  = (uint8_t)atoi(cmd + 3);
        uint32_t val = (uint32_t)atoi(p + 1);
        if (val > 4095) val = 4095;
        if (ch == 1)
            HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, val);
        else if (ch == 2)
            HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, val);
        snprintf(tx_buf, sizeof(tx_buf), "AO_OK,%d,%lu\n", ch, val);
        uart_send(tx_buf);
        return;
    }

    // NUKE POWER: "PB5,<0|1>"
    if (strncmp(cmd, "PB5,", 4) == 0) {
        uint8_t val = (uint8_t)atoi(cmd + 4);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5,
                          val ? GPIO_PIN_SET : GPIO_PIN_RESET);
        snprintf(tx_buf, sizeof(tx_buf), "PB5_OK,%d\n", val);
        uart_send(tx_buf);
        return;
    }
}

/* UART RX interrupt callback */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance != USART1) return;
    char c = (char)rx_byte;
    if (c == '\n') {
        rx_buf[rx_idx] = '\0';
        if (rx_idx > 0) processCommand(rx_buf);
        rx_idx = 0;
    } else if (c != '\r') {
        if (rx_idx < MAX_CMD_LEN)
            rx_buf[rx_idx++] = c;
        else
            rx_idx = 0;  // overflow — discard
    }
    HAL_UART_Receive_IT(&huart1, &rx_byte, 1);  // re-arm
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_DAC_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  /* Start ADC DMA */
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_buf, 2);

  /* Start DAC outputs at 0V */
  HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
  HAL_DAC_Start(&hdac, DAC_CHANNEL_2);

  /* Init DI state */
  for (uint8_t i = 0; i < NUM_DI; i++) {
      di_state[i]       = HAL_GPIO_ReadPin(DI_PINS[i].port, DI_PINS[i].pin);
      di_last_state[i]  = di_state[i];
      di_last_change[i] = HAL_GetTick();
  }

  /* Arm UART RX interrupt */
  HAL_UART_Receive_IT(&huart1, &rx_byte, 1);

  /* Announce ready + broadcast initial DI states */
  uart_send("READY\n");
  for (uint8_t i = 0; i < NUM_DI; i++) {
      snprintf(tx_buf, sizeof(tx_buf), "DI,%d,%d\n", i+1, di_state[i]);
      uart_send(tx_buf);
  }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  uint32_t now = HAL_GetTick();

	  /* ---- Scan DI with debounce ---- */
	  for (uint8_t i = 0; i < NUM_DI; i++) {
	      uint8_t reading = HAL_GPIO_ReadPin(DI_PINS[i].port, DI_PINS[i].pin);
	      if (reading != di_last_state[i]) {
	          di_last_change[i] = now;
	          di_last_state[i]  = reading;
	      }
	      if ((now - di_last_change[i]) > DEBOUNCE_MS) {
	          if (reading != di_state[i]) {
	              di_state[i] = reading;
	              snprintf(tx_buf, sizeof(tx_buf), "DI,%d,%d\n", i+1, di_state[i]);
	              uart_send(tx_buf);
	          }
	      }
	  }

	  /* ---- Heartbeat ---- */
	  if ((now - last_heartbeat) >= HEARTBEAT_MS) {
	      last_heartbeat = now;
	      uart_send("HB\n");
	  }

	  /* ---- Analog report ---- */
	  if ((now - last_adc_report) >= ADC_REPORT_MS) {
	      last_adc_report = now;
	      snprintf(tx_buf, sizeof(tx_buf), "AI,1,%d\n", adc_buf[0]);
	      uart_send(tx_buf);
	      snprintf(tx_buf, sizeof(tx_buf), "AI,2,%d\n", adc_buf[1]);
	      uart_send(tx_buf);
	  }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
