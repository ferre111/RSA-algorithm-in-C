/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "rsa.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define GIVE_PUBLIC_KEY_CMD			"Give public key"
#define PUBLIC_KEY_MESSAGE_HEADER	"Public key:"
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;

/* USER CODE BEGIN PV */
static rsa_structure rsa_ctx;
static rsa_public_key rsa_second_board_public_key;
static uint8_t receive_buffer[512];
static volatile bool new_encrypted_message;
static volatile uint32_t last_encrypted_message_length;
static volatile bool public_key_obtained;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */
static void decrypt_message(char *buff);
static void encrypt_message(char *message, uint32_t *encrypted_message);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  rsa_init(59, 67, &rsa_ctx);
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */
  HAL_UARTEx_ReceiveToIdle_DMA(&huart2, receive_buffer, sizeof(receive_buffer) / sizeof(receive_buffer[0]));
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while(!public_key_obtained) {
	  HAL_UART_Transmit_IT(&huart2, (uint8_t*)GIVE_PUBLIC_KEY_CMD, sizeof(GIVE_PUBLIC_KEY_CMD));
	  HAL_Delay(1000);
  }
  char *message_tx = "Standardy i systemy komunikacyjne";
  uint32_t message_encrypted[strlen(message_tx) + 1];

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  HAL_Delay(1000);
	  if (public_key_obtained) {
			encrypt_message(message_tx, message_encrypted);
			HAL_UART_Transmit_IT(&huart2, (uint8_t*)&message_encrypted, (strlen(message_tx) + 1) * sizeof(uint32_t));
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

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* USART2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DATA_CORRUPTED_GPIO_Port, DATA_CORRUPTED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : DATA_CORRUPTED_Pin */
  GPIO_InitStruct.Pin = DATA_CORRUPTED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DATA_CORRUPTED_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	static char tmp_buffer[128];
    // Check if UART2 trigger the Callback
    if(huart->Instance == USART2)
    {
    	receive_buffer[Size] = '\0';

    	if (new_encrypted_message) {
    		new_encrypted_message = false;
    		last_encrypted_message_length = 0;
    		HAL_GPIO_WritePin(DATA_CORRUPTED_GPIO_Port, DATA_CORRUPTED_Pin, true);
    	}

    	if (!strncmp((char*)receive_buffer, GIVE_PUBLIC_KEY_CMD, sizeof(GIVE_PUBLIC_KEY_CMD) - 1)) {
    		rsa_public_key *public_key = rsa_get_public_key(&rsa_ctx);
    		int len = snprintf(tmp_buffer, sizeof(tmp_buffer), "%s%lu,%lu", PUBLIC_KEY_MESSAGE_HEADER, public_key->e, public_key->n);
    		if (len) {
    			HAL_UART_Transmit_IT(&huart2, (uint8_t*)tmp_buffer, len + 1);
    		}
    		HAL_UARTEx_ReceiveToIdle_DMA(&huart2, receive_buffer, sizeof(receive_buffer) / sizeof(receive_buffer[0]));
    	} else if (!strncmp((char*)receive_buffer, PUBLIC_KEY_MESSAGE_HEADER, sizeof(PUBLIC_KEY_MESSAGE_HEADER) - 1)) {
    		rsa_second_board_public_key.e = atoi((char*)&receive_buffer[sizeof(PUBLIC_KEY_MESSAGE_HEADER) - 1]);
    		char *ptr = strtok((char*)receive_buffer, ",");
    		ptr = strtok(NULL, ",");
			rsa_second_board_public_key.n = atoi(ptr);
			public_key_obtained = true;
			HAL_UARTEx_ReceiveToIdle_DMA(&huart2, receive_buffer, sizeof(receive_buffer) / sizeof(receive_buffer[0]));
    	} else {
    		new_encrypted_message = true;
    		last_encrypted_message_length = Size;
    	}
    }
}

static void decrypt_message(char *buff) {
	for (uint32_t i = 0; i < last_encrypted_message_length; i += 4) {
		buff[i / 4] = (char)rsa_decrypt(*(uint32_t*)&receive_buffer[i], &rsa_ctx);
	}
}

static void encrypt_message(char *message, uint32_t *encrypted_message) {
	uint32_t i = 0;
	for (; i < strlen(message); i++) {
		encrypted_message[i] = rsa_encrypt((uint32_t)message[i], &rsa_second_board_public_key);
	}
	encrypted_message[i] = rsa_encrypt((uint32_t)'\0', &rsa_second_board_public_key);
}
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

#ifdef  USE_FULL_ASSERT
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
