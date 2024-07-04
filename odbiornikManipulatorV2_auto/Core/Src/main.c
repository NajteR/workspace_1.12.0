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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define START_BYTE '*'
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t uartData[4]; // For receiving 4 bytes
uint16_t c[8] = {0}; // Array to hold control data for motors
float pwmRight = 0;
float pwmLeft = 0;
float step = 0.25;
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
  void stoppodst();
  void stopczlon5();
  void stopczlon1();
  void stopczlon2();
  void stopczlon3();
  void stopczlon4();

  void motor5control();
  void motor4control();
  void motor3control();
  void motor2control();
  void motor1control();
  void motor0control();
  void rightside();
  void leftside();
  void stopLmotorF();
  void stopRmotorF();
  void LmotorF();
  void LmotorB();
  void RmotorF();
  void RmotorB();
  void PWMval(uint8_t ch, uint16_t val);

  uint16_t map(uint16_t input_value, uint16_t minRange, uint16_t maxRange, uint16_t New_minRange, uint16_t New_maxRange);
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
    if (HAL_UART_Receive(&huart2, uartData, 4, HAL_MAX_DELAY) == HAL_OK)
    {
      if (uartData[0] == START_BYTE)
      {
        uint8_t lpwm = uartData[1];
        uint8_t rpwm = uartData[2];
        uint8_t checksum = uartData[3];

        if (checksum == (lpwm + rpwm + START_BYTE))
        {
          pwmLeft = map(lpwm, 32, 127, -1000, 1000);
          pwmRight = map(rpwm, 32, 127, -1000, 1000);

          rightside();
          leftside();
          motor5control();
          motor4control();
          motor3control();
          motor2control();
          motor1control();
          motor0control();
        }
      }
    }

    if (HAL_GetTick() - timereset > 500)
    {
      stoppodst();
      stopczlon5();
      stopczlon1();
      stopczlon2();
      stopczlon3();
      stopczlon4();
      stopLmotorF();
      stopRmotorF();
    }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */


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
  return (((input_value - minRange) * (New_maxRange - New_minRange)) / (maxRange - minRange) + New_minRange);
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

void stopMotorControl(uint8_t motor)
{
	  void stoppodst();
	  void stopczlon5();
	  void stopczlon1();
	  void stopczlon2();
	  void stopczlon3();
	  void stopczlon4();
	  void stopRmotorF();
	  void stopLmotorF();

  switch (motor)
  {
  case 0:
    stoppodst();
    break;
  case 1:
    stopczlon1();
    break;
  case 2:
    stopczlon2();
    break;
  case 3:
    stopczlon3();
    break;
  case 4:
    stopczlon4();
    break;
  case 5:
    stopczlon5();
    break;
  case 6:
    stopRmotorF();
    break;
  case 7:
    stopLmotorF();
    break;
  }
}

void motorControl(uint8_t motor, GPIO_TypeDef* dirPort1, uint16_t dirPin1, GPIO_TypeDef* dirPort2, uint16_t dirPin2, uint16_t pwmChannel, uint16_t pwmValue)
{
  if (pwmValue > 50) {
    HAL_GPIO_WritePin(dirPort1, dirPin1, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(dirPort2, dirPin2, GPIO_PIN_SET);
    PWMval(pwmChannel, pwmValue);
  }
  else if (pwmValue < -50) {
    HAL_GPIO_WritePin(dirPort1, dirPin1, GPIO_PIN_SET);
    HAL_GPIO_WritePin(dirPort2, dirPin2, GPIO_PIN_RESET);
    pwmValue = -pwmValue; // Make sure pwmValue is positive for PWM
    PWMval(pwmChannel, pwmValue);
  }
  else
  {
	  PWMval(pwmChannel, 0);
  }

}

void rightside()
{
  float targetPwm = pwmRight; // Ensure pwmRight and targetPwm are defined and used correctly
  pwmRight += (targetPwm > pwmRight) ? step : -step;
  motorControl(6, R_D1_GPIO_Port, R_D1_Pin, R_D2_GPIO_Port, R_D2_Pin, 6, pwmRight);
}

void leftside()
{
  float targetPwm = pwmLeft; // Ensure pwmLeft and targetPwm are defined and used correctly
  pwmLeft += (targetPwm > pwmLeft) ? step : -step;
  motorControl(7, L_D1_GPIO_Port, L_D1_Pin, L_D2_GPIO_Port, L_D2_Pin, 7, pwmLeft);
}

void motor0control() { motorControl(0, Podstawa_D1_GPIO_Port, Podstawa_D1_Pin, Podstawa_D2_GPIO_Port, Podstawa_D2_Pin, 0, 1000); }
void motor1control() { motorControl(1, Czlon1_D1_GPIO_Port, Czlon1_D1_Pin, Czlon1_D2_GPIO_Port, Czlon1_D2_Pin, 1, 1000); }
void motor2control() { motorControl(2, Czlon2_D1_GPIO_Port, Czlon2_D1_Pin, Czlon2_D2_GPIO_Port, Czlon2_D2_Pin, 2, 1000); }
void motor3control() { motorControl(3, Czlon3_D1_GPIO_Port, Czlon3_D1_Pin, Czlon3_D2_GPIO_Port, Czlon3_D2_Pin, 3, 1000); }
void motor4control() { motorControl(4, Czlon4_D1_GPIO_Port, Czlon4_D1_Pin, Czlon4_D2_GPIO_Port, Czlon4_D2_Pin, 4, 1000); }
void motor5control() { motorControl(5, Czlon5_D1_GPIO_Port, Czlon5_D1_Pin, Czlon5_D2_GPIO_Port, Czlon5_D2_Pin, 5, 1000); }

void stoppodst() { HAL_GPIO_WritePin(Podstawa_D1_GPIO_Port, Podstawa_D1_Pin, GPIO_PIN_SET); HAL_GPIO_WritePin(Podstawa_D2_GPIO_Port, Podstawa_D2_Pin, GPIO_PIN_SET); }
void stopczlon1() { HAL_GPIO_WritePin(Czlon1_D1_GPIO_Port, Czlon1_D1_Pin, GPIO_PIN_SET); HAL_GPIO_WritePin(Czlon1_D2_GPIO_Port, Czlon1_D2_Pin, GPIO_PIN_SET); }
void stopczlon2() { HAL_GPIO_WritePin(Czlon2_D1_GPIO_Port, Czlon2_D1_Pin, GPIO_PIN_SET); HAL_GPIO_WritePin(Czlon2_D2_GPIO_Port, Czlon2_D2_Pin, GPIO_PIN_SET); }
void stopczlon3() { HAL_GPIO_WritePin(Czlon3_D1_GPIO_Port, Czlon3_D1_Pin, GPIO_PIN_SET); HAL_GPIO_WritePin(Czlon3_D2_GPIO_Port, Czlon3_D2_Pin, GPIO_PIN_SET); }
void stopczlon4() { HAL_GPIO_WritePin(Czlon4_D1_GPIO_Port, Czlon4_D1_Pin, GPIO_PIN_SET); HAL_GPIO_WritePin(Czlon4_D2_GPIO_Port, Czlon4_D2_Pin, GPIO_PIN_SET); }
void stopczlon5() { HAL_GPIO_WritePin(Czlon5_D1_GPIO_Port, Czlon5_D1_Pin, GPIO_PIN_SET); HAL_GPIO_WritePin(Czlon5_D2_GPIO_Port, Czlon5_D2_Pin, GPIO_PIN_SET); }
void stopLmotorF() { HAL_GPIO_WritePin(L_D1_GPIO_Port, L_D1_Pin, GPIO_PIN_SET); HAL_GPIO_WritePin(L_D2_GPIO_Port, L_D2_Pin, GPIO_PIN_SET); }
void stopRmotorF() { HAL_GPIO_WritePin(R_D1_GPIO_Port, R_D1_Pin, GPIO_PIN_SET); HAL_GPIO_WritePin(R_D2_GPIO_Port, R_D2_Pin, GPIO_PIN_SET); }

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
