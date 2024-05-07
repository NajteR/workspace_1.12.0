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
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "nRF24_Defs.h"
#include "nRF24.h"
#include "string.h"
#include <math.h>
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
uint8_t nrfdata[15];
uint16_t c0 = 0;
uint16_t c1 = 0;
uint16_t c2 = 0;
uint16_t c3 = 0;
uint16_t c4 = 0;
uint16_t c5 = 0;
uint16_t c6 = 0;
uint16_t c7 = 0;
uint16_t f = 0;
uint16_t b = 0;

uint16_t rpwm = 0;
float pwm1 = 0;
float pwm2 = 0;
float step = 0.25;
uint16_t steringpwm = 0;
uint16_t lowvalue = 350;
uint16_t higvalue = 650;
float pwmRight = 0;
float pwmLeft = 0;
int ilpwm;
int irpwm;
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
	void  stoppodst();
	void stopczlon5();
	void stopczlon1();
	void stopczlon2();
	void stopczlon3();
	void stopczlon4();

	void triggergripper();
	void motor5control();
	void motor4control();
	void motor3control();
	void motor2control();
	void motor1control();
	void motor0control();
	void move();
	void rotate();
	void stopLmotorF();
	void stopRmotorF();
	void rightside();
	void leftside();
	void RmotorF();
	void RmotorB();
	void LmotorF();
	void LmotorB();
	void PWMval(uint8_t ch, uint16_t val);

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
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  /* USER CODE BEGIN 2 */

  //nRF initialize and settings

  nRF24_Init(&hspi2);
  nRF24_SetRXAddress(0, "Odb");
  nRF24_SetTXAddress("Nad");
  nRF24_RX_Mode();

  //Set timer 2 to PWM function

  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_2);
  time = HAL_GetTick();
  timereset = HAL_GetTick();
  stoppodst();
  stopczlon5();
  stopczlon1();
  stopczlon2();
  stopczlon3();
  stopczlon4();
  stopLmotorF();
  stopRmotorF();

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
		  	  		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
		  	  		time = HAL_GetTick();
		  	  	 }
		  	  	 //read received data and save to nrfdata variable
	  	  		 nRF24_ReadRXPaylaod(nrfdata);
	  	  		 timereset = HAL_GetTick();

	  	  	  }
	  //if there is no signal restart nRF24 and stop motor
	  if(HAL_GetTick()-timereset > 500)
	  {
		  nRF24_Init(&hspi2);
		  nRF24_SetRXAddress(0, "Odb");
		  nRF24_SetTXAddress("Nad");
		  nRF24_RX_Mode();
		  stoppodst();
		  stopczlon5();
		  stopczlon1();
		  stopczlon2();
		  stopczlon3();
		  stopczlon4();
//		  stopLmotorF();
//		  stopRmotorF();

		  	 			if (0 > pwmRight)
		  	 			{
		  	 				pwmRight = pwmRight + step;
		  	 			}
		  	 			else if (0 < pwmRight)
		  	 			{
		  	 				pwmRight = pwmRight - step;
		  	 			}
		  	 			if (pwmRight > 0)
		  	 				{
		  	 					RmotorF();
		  	 					PWMval(6, (int)pwmRight);
		  	 				}
		  	 			else if (pwmRight < 0)
		  	 			{
		  	 				RmotorB();
		  	 				PWMval(6, ((int)pwmRight)*-1);
		  	 			}

		  	 					if (0 > pwmLeft)
		  	 						{
		  	 							pwmLeft = pwmLeft+step;
		  	 						}
		  	 						else if (0 < pwmLeft)
		  	 						{
		  	 							pwmLeft = pwmLeft-step;
		  	 						}
		  	 						if (pwmLeft > 0)
		  	 						{
		  	 							LmotorF();
		  	 							PWMval(7, (int)pwmLeft);
		  	 						}
		  	 						else if (pwmLeft < 0)
		  	 						{
		  	 							LmotorB();
		  	 							PWMval(7, ((int)pwmLeft)*-1);
		  	 						}



	  }
	  else if (nrfdata[14] == 34 && (nrfdata[0] == 48 || nrfdata[0] == 49) &&
			  (nrfdata[1] == 50 || nrfdata[1] == 48) && (nrfdata[2] == 51 || nrfdata[2] == 48 || nrfdata[2] == 52 ) &&
			  (nrfdata[3] == 53 || nrfdata[3] == 48 || nrfdata[3] == 54) && (nrfdata[4] == 55 || nrfdata[4] == 48 || nrfdata[4] == 56) &&
			  (nrfdata[5] == 57 || nrfdata[5] == 48 || nrfdata[5] == 58) && (nrfdata[8] == 59 || nrfdata[8] == 61 || nrfdata[8] == 60) &&
			  (nrfdata[9] == 64 || nrfdata[9] == 62 || nrfdata[9] == 63)&&


			  (nrfdata[6] >= 32 && nrfdata[6] <= 127)&&
			  (nrfdata[7] >= 32 && nrfdata[7] <= 127)

			  )
	  {
		  	// triggergripper();
		  	 rightside();
		  	 leftside();
		  	 motor5control();
	 		 motor4control();
	 		 motor3control();
	 		 motor2control();
	 		 motor1control();
	 		 motor0control();

	 		// move();
	 		// rotate();
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

uint16_t map(uint16_t input_value, uint16_t minRange, uint16_t maxRange, uint16_t New_minRange, uint16_t New_maxRange)
{
	return (((input_value - minRange)*(New_maxRange - New_minRange))/(maxRange - minRange) + New_minRange);
}
void PWMval(uint8_t ch, uint16_t val)
{
	switch (ch)
	{
	case 0:
		__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, val);
	break;
	case 1:
		__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, val);
	break;
	case 2:
		__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, val);
	break;
	case 3:
		__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, val);
	break;
	case 4:
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, val);
	break;
	case 5:
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, val);
	break;
	case 6:
		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, val);
	break;
	case 7:
		__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_2, val);
	break;
	}
}
void podstR()
{
	HAL_GPIO_WritePin(Podstawa_D1_GPIO_Port, Podstawa_D1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Podstawa_D2_GPIO_Port, Podstawa_D2_Pin, GPIO_PIN_SET);

}
void podstL()
{
	HAL_GPIO_WritePin(Podstawa_D2_GPIO_Port, Podstawa_D2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Podstawa_D1_GPIO_Port, Podstawa_D1_Pin, GPIO_PIN_SET);
}

void stoppodst()
{
	HAL_GPIO_WritePin(Podstawa_D1_GPIO_Port, Podstawa_D1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Podstawa_D2_GPIO_Port, Podstawa_D2_Pin, GPIO_PIN_RESET);
	PWMval(0,0);
}

void czlon1R()
{
	HAL_GPIO_WritePin(Czlon1_D1_GPIO_Port, Czlon1_D1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Czlon1_D2_GPIO_Port, Czlon1_D2_Pin, GPIO_PIN_SET);
}
void czlon1L()
{
	HAL_GPIO_WritePin(Czlon1_D1_GPIO_Port, Czlon1_D1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Czlon1_D2_GPIO_Port, Czlon1_D2_Pin, GPIO_PIN_RESET);
}
void stopczlon1()
{
	HAL_GPIO_WritePin(Czlon1_D1_GPIO_Port, Czlon1_D1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Czlon1_D2_GPIO_Port, Czlon1_D2_Pin, GPIO_PIN_RESET);
	PWMval(1,0);
}

void czlon2R()
{
	HAL_GPIO_WritePin(Czlon2_D1_GPIO_Port, Czlon2_D1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Czlon2_D2_GPIO_Port, Czlon2_D2_Pin, GPIO_PIN_SET);
}
void czlon2L()
{
	HAL_GPIO_WritePin(Czlon2_D1_GPIO_Port, Czlon2_D1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Czlon2_D2_GPIO_Port, Czlon2_D2_Pin, GPIO_PIN_RESET);
}
void stopczlon2()
{
	HAL_GPIO_WritePin(Czlon2_D1_GPIO_Port, Czlon2_D1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Czlon2_D2_GPIO_Port, Czlon2_D2_Pin, GPIO_PIN_RESET);
	PWMval(2,0);
}

void czlon3R()
{
	HAL_GPIO_WritePin(Czlon3_D1_GPIO_Port, Czlon3_D1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Czlon3_D2_GPIO_Port, Czlon3_D2_Pin, GPIO_PIN_SET);
}
void czlon3L()
{
	HAL_GPIO_WritePin(Czlon3_D1_GPIO_Port, Czlon3_D1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Czlon3_D2_GPIO_Port, Czlon3_D2_Pin, GPIO_PIN_RESET);
}
void stopczlon3()
{
	HAL_GPIO_WritePin(Czlon3_D1_GPIO_Port, Czlon3_D1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Czlon3_D2_GPIO_Port, Czlon3_D2_Pin, GPIO_PIN_RESET);
	PWMval(3,0);
}

void czlon4R()
{
	HAL_GPIO_WritePin(Czlon4_D1_GPIO_Port, Czlon4_D1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Czlon4_D2_GPIO_Port, Czlon4_D2_Pin, GPIO_PIN_SET);
}
void czlon4L()
{
	HAL_GPIO_WritePin(Czlon4_D1_GPIO_Port, Czlon4_D1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Czlon4_D2_GPIO_Port, Czlon4_D2_Pin, GPIO_PIN_RESET);
}
void stopczlon4()
{
	HAL_GPIO_WritePin(Czlon4_D1_GPIO_Port, Czlon4_D1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Czlon4_D2_GPIO_Port, Czlon4_D2_Pin, GPIO_PIN_RESET);
	PWMval(4,0);
}

void czlon5R()
{
	HAL_GPIO_WritePin(Czlon5_D1_GPIO_Port, Czlon5_D1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Czlon5_D2_GPIO_Port, Czlon5_D2_Pin, GPIO_PIN_SET);
}
void czlon5L()
{
	HAL_GPIO_WritePin(Czlon5_D1_GPIO_Port, Czlon5_D1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Czlon5_D2_GPIO_Port, Czlon5_D2_Pin, GPIO_PIN_RESET);
}
void stopczlon5()
{
	HAL_GPIO_WritePin(Czlon5_D1_GPIO_Port, Czlon5_D1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Czlon5_D2_GPIO_Port, Czlon5_D2_Pin, GPIO_PIN_RESET);
	PWMval(5,0);
}

void LmotorF()
{
	HAL_GPIO_WritePin(L_D1_GPIO_Port,L_D1_Pin , GPIO_PIN_RESET);
	HAL_GPIO_WritePin(L_D2_GPIO_Port,L_D2_Pin , GPIO_PIN_SET);
}
void LmotorB()
{
	HAL_GPIO_WritePin(L_D2_GPIO_Port, L_D2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(L_D1_GPIO_Port, L_D1_Pin, GPIO_PIN_SET);
}
void stopLmotorF()
{
	HAL_GPIO_WritePin(L_D1_GPIO_Port, L_D1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(L_D2_GPIO_Port,L_D2_Pin , GPIO_PIN_RESET);
	PWMval(7,0);
}
void RmotorF()
{
	HAL_GPIO_WritePin(R_D1_GPIO_Port,R_D1_Pin , GPIO_PIN_RESET);
	HAL_GPIO_WritePin(R_D2_GPIO_Port,R_D2_Pin , GPIO_PIN_SET);
}
void RmotorB()
{
	HAL_GPIO_WritePin(R_D2_GPIO_Port,R_D2_Pin , GPIO_PIN_RESET);
	HAL_GPIO_WritePin(R_D1_GPIO_Port,R_D1_Pin , GPIO_PIN_SET);
}
void stopRmotorF()
{
	HAL_GPIO_WritePin(R_D1_GPIO_Port,R_D1_Pin , GPIO_PIN_RESET);
	HAL_GPIO_WritePin(R_D2_GPIO_Port, R_D2_Pin, GPIO_PIN_RESET);
	PWMval(6,0);
}


void rightside()
{
	pwm1 = map(nrfdata[6],32,127,0,1000);
	c4 = nrfdata[0];
	if (c4 == 49)
		{
		pwm1 = pwm1*-1;
		}

		if (pwm1 > pwmRight)
		{
			pwmRight = pwmRight + step;
		}
		else if (pwm1 < pwmRight)
		{
			pwmRight = pwmRight - step;
		}
		if (pwmRight > 0)
			{
				RmotorF();
				PWMval(6, (int)pwmRight);
			}
		else if (pwmRight < 0)
		{
			RmotorB();
			PWMval(6, ((int)pwmRight)*-1);
		}




}
void leftside()
{

	pwm2 = map(nrfdata[7],32,127,0,1000);
	c5 = nrfdata[1];

	if (c5 == 50)
	{
		pwm2 = pwm2 *-1;
	}

		if (pwm2 > pwmLeft)
			{
				pwmLeft = pwmLeft+step;
			}
			else if (pwm2 < pwmLeft)
			{
				pwmLeft = pwmLeft-step;
			}
			if (pwmLeft > 0)
			{
				LmotorF();
				PWMval(7, (int)pwmLeft);
			}
			else if (pwmLeft < 0)
			{
				LmotorB();
				PWMval(7, ((int)pwmLeft)*-1);
			}





}


void motor0control()
{
	c0 = (nrfdata[2]);
	if(c0 == 51)
				{
					podstL();
					PWMval(0, 1000);
				}
	else if(c0 == 52)
				{
					podstR();
					PWMval(0, 1000);
				}
	else
	{
		stoppodst();
	}
}

void motor1control()
{
	c1 = (nrfdata[3]);
	if(c1 == 53)
				{
					czlon1L();
					PWMval(1, 1000);
				}
	else if(c1 == 54)
				{
					czlon1R();
					PWMval(1, 1000);
				}
	else
	{
		stopczlon1();
	}
}

void motor2control()
{
	c2 = (nrfdata[4]);
	if(c2 == 55)
				{
					czlon2L();
					PWMval(2, 1000);
				}
	else if(c2 == 56)
				{
					czlon2R();
					PWMval(2, 1000);
				}
	else
	{
		stopczlon2();
	}
}

void motor3control()
{
	c3 = (nrfdata[5]);
	if(c3 == 57)
				{
					czlon3L();
					PWMval(3, 1000);
				}
	else if(c3 == 58)
				{
					czlon3R();
					PWMval(3, 1000);
				}
	else
	{
		stopczlon3();
	}
}

void motor4control()
{
	c6 = (nrfdata[8]);
	if(c6 == 59)
				{
					czlon4L();
					PWMval(4, 1000);
				}
	else if(c6 == 60)
				{
					czlon4R();
					PWMval(4, 1000);
				}
	else
	{
		stopczlon4();
	}
}

void motor5control()
{
	c7 = (nrfdata[9]);
	if(c7 == 62)
				{
					czlon5L();
					PWMval(5, 1000);
				}
	else if(c7 == 63)
				{
					czlon5R();
					PWMval(5, 1000);
				}
	else
	{
		stopczlon5();
	}
}

//void kinematyka()
//{
//	float l1=0.39;
//	float l2=0.335;
//	float l3=0.23;
//	float fi=0;
//
//
//
//	float x=0.5;
//	float y=0.3;
//
//	float x2=x-l3;
//	float y2=y;
//
//
//	float teta2=acos(((x2^2)+(y2^2)-(l1^2)-(l2^2))/(2*l1*l2));
//	float teta1=acos(((x2^2)+((l1+l2*cos(teta2))^2)-((y2-l2*sin(teta2))^2))/(2*x2*(l1+l2*cos(teta2))));
//	float teta3=fi-teta1-teta2;
//
//}


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