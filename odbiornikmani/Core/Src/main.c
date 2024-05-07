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
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "nRF24_Defs.h"
#include "nRF24.h"
#include "string.h"
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
uint8_t nrfdata[6];
uint16_t lpwm = 0;
uint16_t rpwm = 0;
uint16_t pwm1 = 0;
uint16_t pwm2 = 0;
uint16_t steringpwm = 0;
uint16_t lowvalue = 450;
uint16_t higvalue = 550;
uint32_t time;
uint32_t timereset;
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
  void triggercontrol();
  void PWMval(uint8_t ch, uint16_t val);
  void joytrigcontrol();
  void joystickcontrol();
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

  //nRF initialize and settings

  nRF24_Init(&hspi1);
  nRF24_SetRXAddress(0, "Odb");
  nRF24_SetTXAddress("Nad");
  nRF24_RX_Mode();

  //Set timer 2 to PWM function

  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
  time = HAL_GetTick();
  timereset = HAL_GetTick();


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  //Check if there is receive message

	  if(nRF24_RXAvailible())
	  	  	  {
		  	  //if working blink diode once per second
		  	  	 if (HAL_GetTick()-time >1000)
		  	  	 {
		  	  		HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
		  	  		time = HAL_GetTick();
		  	  	 }
		  	  	 //read received data and save to nrfdata variable
	  	  		 nRF24_ReadRXPaylaod(nrfdata);
	  	  		 timereset = HAL_GetTick();
	  	  	  }
	  //if there is no signal restart nRF24 and stop motor
	  if(HAL_GetTick()-timereset > 1000)
	  {
		  nRF24_Init(&hspi1);
		  nRF24_SetRXAddress(0, "Odb");
		  nRF24_SetTXAddress("Nad");
		  nRF24_RX_Mode();
		  HAL_GPIO_WritePin(DIR_ML1_GPIO_Port, DIR_ML1_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(DIR_MR1_GPIO_Port, DIR_MR1_Pin, GPIO_PIN_RESET);
		  PWMval(1, 0);
		  PWMval(2, 0);

	  }
	  	  /*
	  if(nRF24_RXAvailible())
	  	  {
	  		  nRF24_ReadRXPaylaod(Nrf24_Message);
	  		  HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
	  		  //MessageLength = sprintf(Message, "%c\n\r", Nrf24_Message[0]);
	  		  //HAL_UART_Transmit(&huart2, Message, MessageLength, 1000);
	  	  }
	  	  */
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

uint16_t map(uint16_t input_value, uint16_t minRange, uint16_t maxRange, uint16_t New_minRange, uint16_t New_maxRange)
{
	return (((input_value - minRange)*(New_maxRange - New_minRange))/(maxRange - minRange) + New_minRange);
}
void PWMval(uint8_t ch, uint16_t val)
{
	switch (ch)
	{
	case 1:
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, val);
	break;
	case 2:
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, val);
	break;
	}
}
void triggercontrol()
{

	lpwm = map(nrfdata[3],32,127,0,1000);
	rpwm = map(nrfdata[4],32,127,0,1000);
	HAL_GPIO_WritePin(DIR_ML2_GPIO_Port, DIR_ML2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DIR_MR2_GPIO_Port, DIR_MR2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DIR_ML1_GPIO_Port, DIR_ML1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(DIR_MR1_GPIO_Port, DIR_MR1_Pin, GPIO_PIN_SET);
	PWMval(1, lpwm);
	PWMval(2, rpwm);
}
void joytrigcontrol()
{
	int power;
	int irpwm;
	int ilpwm;
	pwm1 = map(nrfdata[3],32,127,0,1000);
	pwm2 = map(nrfdata[4],32,127,0,1000);
	HAL_GPIO_WritePin(DIR_ML2_GPIO_Port, DIR_ML2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DIR_MR2_GPIO_Port, DIR_MR2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DIR_ML1_GPIO_Port, DIR_ML1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(DIR_MR1_GPIO_Port, DIR_MR1_Pin, GPIO_PIN_SET);
	steringpwm = map(nrfdata[1],32,127,0,1000);
	power = pwm2 - pwm1;
	if (power < 0)
		{
			HAL_GPIO_WritePin(DIR_ML1_GPIO_Port, DIR_ML1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(DIR_MR1_GPIO_Port, DIR_MR1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(DIR_ML2_GPIO_Port, DIR_ML2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(DIR_MR2_GPIO_Port, DIR_MR2_Pin, GPIO_PIN_SET);
			power = power * (-1);
		}
	if(steringpwm < lowvalue)
		{
			lpwm = power;
			irpwm = power - (1000-map(steringpwm,0,lowvalue,0,1000));
			if (irpwm < 0)
			{
				rpwm = 0;
			}
			else
			{
				rpwm = irpwm;
			}
		}
	else if(steringpwm > higvalue)
		{
			ilpwm = power - map(steringpwm,higvalue,1000,0,1000);
			rpwm = power;
			if (ilpwm < 0)
			{
				lpwm = 0;
			}
			else
			{
				lpwm = ilpwm;
			}
		}
	else
		{
			rpwm = power;
			lpwm = power;
		}
	PWMval(1, rpwm);
	PWMval(2, lpwm);
}

void joystickcontrol()
{
	lpwm = map(nrfdata[1],32,127,0,1000);
	rpwm = map(nrfdata[2],32,127,0,1000);
	if(lowvalue <= lpwm && lpwm <= higvalue && lowvalue <= rpwm && rpwm <= higvalue)
	{
		HAL_GPIO_WritePin(DIR_ML2_GPIO_Port, DIR_ML2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DIR_MR2_GPIO_Port, DIR_MR2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DIR_ML1_GPIO_Port, DIR_ML1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(DIR_MR1_GPIO_Port, DIR_MR1_Pin, GPIO_PIN_RESET);
		HAL_Delay(10);
		PWMval(1, 0);
		PWMval(2, 0);
	}
	else
	{
		if (rpwm < lowvalue)
		{
			if(lpwm < lowvalue)
			{
				rpwm =1000- map(rpwm,0,lowvalue,0,1000);
				lpwm = rpwm + 1000-map(lpwm,0,lowvalue,0,1000);
				HAL_GPIO_WritePin(DIR_ML2_GPIO_Port, DIR_ML2_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(DIR_MR2_GPIO_Port, DIR_MR2_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(DIR_ML1_GPIO_Port, DIR_ML1_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(DIR_MR1_GPIO_Port, DIR_MR1_Pin, GPIO_PIN_SET);
				HAL_Delay(10);
				PWMval(1, rpwm);
				PWMval(2, lpwm);
			}
			else if(lpwm >higvalue)
			{
				lpwm = 1000-map(lpwm,higvalue,1000,0,1000);
				rpwm = 1000-map(rpwm,0,lowvalue,0,1000)+ lpwm;
				HAL_GPIO_WritePin(DIR_ML2_GPIO_Port, DIR_ML2_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(DIR_MR2_GPIO_Port, DIR_MR2_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(DIR_ML1_GPIO_Port, DIR_ML1_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(DIR_MR1_GPIO_Port, DIR_MR1_Pin, GPIO_PIN_SET);
				HAL_Delay(10);
				PWMval(1, rpwm);
				PWMval(2, lpwm);

			}
			else if(lpwm >= lowvalue && lpwm <= higvalue)
			{
				rpwm = 1000- map(rpwm,0,lowvalue,0,1000);
				HAL_GPIO_WritePin(DIR_ML2_GPIO_Port, DIR_ML2_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(DIR_MR2_GPIO_Port, DIR_MR2_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(DIR_ML1_GPIO_Port, DIR_ML1_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(DIR_MR1_GPIO_Port, DIR_MR1_Pin, GPIO_PIN_SET);
				HAL_Delay(10);
				PWMval(1, rpwm);
				PWMval(2, rpwm);

			}


		}
		else if (rpwm > higvalue)
		{
			if(lpwm < lowvalue)
			{
				rpwm = map(rpwm,higvalue,1000,0,1000);
				lpwm = 1000-map(lpwm,0,lowvalue,0,1000)+rpwm;
				HAL_GPIO_WritePin(DIR_ML1_GPIO_Port, DIR_ML1_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(DIR_MR1_GPIO_Port, DIR_MR1_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(DIR_ML2_GPIO_Port, DIR_ML2_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(DIR_MR2_GPIO_Port, DIR_MR2_Pin, GPIO_PIN_SET);
				HAL_Delay(10);
				PWMval(1, lpwm);
				PWMval(2, rpwm);

			}
			else if(lpwm > higvalue)
			{
				rpwm = map(rpwm,higvalue,1000,0,1000);
				lpwm = map(lpwm,0,higvalue,0,1000)+rpwm;
				HAL_GPIO_WritePin(DIR_ML1_GPIO_Port, DIR_ML1_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(DIR_MR1_GPIO_Port, DIR_MR1_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(DIR_ML2_GPIO_Port, DIR_ML2_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(DIR_MR2_GPIO_Port, DIR_MR2_Pin, GPIO_PIN_SET);
				HAL_Delay(10);
				PWMval(1, rpwm);
				PWMval(2, lpwm);
			}
			else if(lpwm >= lowvalue && lpwm <= higvalue)
			{
				rpwm = map(rpwm,higvalue,1000,0,1000);
				HAL_GPIO_WritePin(DIR_ML1_GPIO_Port, DIR_ML1_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(DIR_MR1_GPIO_Port, DIR_MR1_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(DIR_ML2_GPIO_Port, DIR_ML2_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(DIR_MR2_GPIO_Port, DIR_MR2_Pin, GPIO_PIN_SET);
				HAL_Delay(10);
				PWMval(1, rpwm);
				PWMval(2, rpwm);

			}


		}
		else if (rpwm <= higvalue && rpwm >= lowvalue)
		{
			if (lpwm > higvalue)
			{
				HAL_GPIO_WritePin(DIR_ML1_GPIO_Port, DIR_ML1_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(DIR_MR2_GPIO_Port, DIR_MR2_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(DIR_MR1_GPIO_Port, DIR_MR1_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(DIR_ML2_GPIO_Port, DIR_ML2_Pin, GPIO_PIN_SET);
				HAL_Delay(10);
				PWMval(1,0);
				PWMval(2,900);

			}
			if(lpwm < lowvalue)
			{
				HAL_GPIO_WritePin(DIR_ML1_GPIO_Port, DIR_ML1_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(DIR_MR1_GPIO_Port, DIR_MR1_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(DIR_ML2_GPIO_Port, DIR_ML2_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(DIR_MR2_GPIO_Port, DIR_MR2_Pin, GPIO_PIN_SET);
				HAL_Delay(10);
				PWMval(1,900);
				PWMval(2,0);
			}

		}


	}
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
