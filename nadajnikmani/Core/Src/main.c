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
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "nRF24_Defs.h"
#include "nRF24.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t datain[15];
uint8_t datach[1];
//uint8_t rx_data;
uint32_t time;
uint32_t timechk;
//uint8_t Nrf24_Message[NRF24_PAYLOAD_SIZE];
//uint8_t Message[] = {'A','b'};
//uint8_t MessageLength;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

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


  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */

  //nRF initialize and settings

  nRF24_Init(&hspi2);
  nRF24_SetRXAddress(0,"Nad");
  nRF24_SetTXAddress("Odb");
  nRF24_TX_Mode();

  //start paramaters
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin,GPIO_PIN_SET);
  time = HAL_GetTick();
  timechk = HAL_GetTick();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  //receive first character

	  if(HAL_UART_Receive(&huart2, datach, 1, 1000) == 3)
	  {
		  HAL_NVIC_SystemReset();
	  }

	  //check first character
	  if ((int) datach[0] == 34)
	  {
		  //receive last 5 characters
		  if(HAL_UART_Receive(&huart2, datain, 14, 1000) == 3){
			  HAL_NVIC_SystemReset();
		  }
		  else if (!((datain[0] == 48 || datain[0] == 49) &&
				  (datain[1] == 50 || datain[1] == 48) && (datain[2] == 51 || datain[2] == 48 || datain[2] == 52 ) &&
				  (datain[3] == 53 || datain[3] == 48 || datain[3] == 54) && (datain[4] == 55 || datain[4] == 48 || datain[4] == 56) &&
				  (datain[5] == 57 || datain[5] == 48 || datain[5] == 58) && (datain[8] == 59 || datain[8] == 61 || datain[8] == 60) &&
				  (datain[9] == 64 || datain[9] == 62 || datain[9] == 63)&&


				  (datain[6] >= 32 && datain[6] <= 127)&&
				  (datain[7] >= 32 && datain[7] <= 127)))
		  {
			  continue;
		  }

		  datain[14] = '"';

		  //send data through nRF24
	  	  nRF24_WriteTXPayload(datain);
	  	  HAL_Delay(1);
	  	  nRF24_WaitTX();
	  	  nRF24_WriteStatus((1<<NRF24_RX_DR)|(1<<NRF24_TX_DS)|(1<<NRF24_MAX_RT));

	  	  //if its working blink once per second
	  	  if (HAL_GetTick()- time > 1000)
	  	  {
	  		  HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	  		  time = HAL_GetTick();
	  	  }
	  	  timechk = HAL_GetTick();

	  }
	  else if (HAL_GetTick() - timechk > 200)
	  {
		  HAL_NVIC_SystemReset();
	  }

	  //Feedback function

	  //HAL_UART_Receive(&huart2, datain, 2, HAL_MAX_DELAY);
	  //HAL_UART_Transmit(&huart2, datain, 2, HAL_MAX_DELAY);
	  //HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin,GPIO_PIN_SET);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLL_DIV3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
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
