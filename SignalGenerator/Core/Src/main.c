/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "../ECUAL/LCD16x2/LCD16x2.h"
#include "../util/DWT_Delay.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
//123132
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 RTC_HandleTypeDef hrtc;

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */

uint16_t SIN_LU[] = {0,1,2,2,3,4,5,5,6,7,8,9,9,10,11,12,12,13,14,15,16,16,17,18,19,19,20,21,22,23,23,24,25,26,26,27,28,29,29,30,31,32,32,33,34,35,36,36,37,38,39,39,40,41,41,42,43,44,44,45,46,47,47,48,49,50,50,51,52,52,53,54,54,55,56,57,57,58,59,59,60,61,61,62,63,63,64,65,66,66,67,68,68,69,69,70,71,71,72,73,73,74,75,75,76,77,77,78,78,79,80,80,81,81,82,83,83,84,84,85,86,86,87,87,88,88,89,90,90,91,91,92,92,93,93,94,94,95,95,96,96,97,97,98,98,99,99,100,100,101,101,102,102,103,103,104,104,105,105,105,106,106,107,107,108,108,108,109,109,110,110,110,111,111,112,112,112,113,113,113,114,114,114,115,115,115,116,116,116,117,117,117,118,118,118,118,119,119,119,120,120,120,120,121,121,121,121,121,122,122,122,122,123,123,123,123,123,124,124,124,124,124,124,125,125,125,125,125,125,125,125,126,126,126,126,126,126,126,126,126,126,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

void EnviarUart(char mensaje[])
{
	HAL_UART_Transmit_IT(&huart3, (uint8_t *)mensaje, strlen((uint8_t *)mensaje) );
}

volatile uint8_t RX_String[32];
volatile uint8_t texto1[32];
volatile uint8_t texto2[32];

enum states {WAITING, UPDATE, GENERATE};
enum states state = WAITING;
enum signals {SIN, SQR, TRI, SAW};
enum signals senal;


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	state = UPDATE;
	HAL_UART_Receive_IT(&huart3, &RX_String, 32);
}

void Awaiting(void)
{
	LCD_Set_Cursor(1, 1);
	LCD_Write_String("Awaiting");
	HAL_Delay(250);

	LCD_Set_Cursor(1, 9);
	LCD_Write_String(".");
	HAL_Delay(250);

	LCD_Set_Cursor(1, 10);
	LCD_Write_String(".");
	HAL_Delay(250);

	LCD_Set_Cursor(1, 11);
	LCD_Write_String(".");
	HAL_Delay(250);

	LCD_Set_Cursor(1, 9);
	LCD_Write_String("   ");
	HAL_Delay(250);

	EnviarUart("Esperando\r\n");
}
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
  MX_RTC_Init();
  MX_USART3_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  //HAL_TIM_Base_Init(&htim2);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  DWT_Delay_Init();

  /*Estructura Canal de Salida*/
  struct channel{
	uint8_t Id;
	uint16_t Frequency;
	uint8_t Offset;
  } Output;

  /*Valor del AutoReset Register*/
  uint32_t MAXVALUE = TIM2->ARR;
  uint32_t DCVAL = MAXVALUE/2;

  /*Incializar el LCD*/
  LCD_Init();
  LCD_Clear();

  HAL_Delay(1000);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  // Comienzo la recepción de datos
  HAL_UART_Receive_IT(&huart3, &RX_String, 32);
  while (1)
  {
	  switch(state)
	  {
	  case WAITING:
		  Awaiting();
		  break;

	  case UPDATE:
		  // PARSE
		  sscanf(RX_String,"[%d-%d-%d]" , &Output.Id , &Output.Frequency , &Output.Offset );
		  char nombre[6];
		  if (Output.Id == 0){strcpy( nombre, "SIN" );}
		  if (Output.Id == 1){strcpy( nombre, "TRI" );}
		  if (Output.Id == 2){strcpy( nombre, "SQR" );}
		  if (Output.Id == 3){strcpy( nombre, "SAW" );}

		  sprintf(texto1 , "SIG:%.3s OFF:%d", nombre , Output.Offset);
		  sprintf(texto2 , "   FRQ:%d" , Output.Frequency);

		  // MODIFY DISPLAY
		  LCD_Clear();

		  LCD_Set_Cursor(1,1);
		  LCD_Write_String( texto1 );
		  LCD_Set_Cursor(2,1);
		  LCD_Write_String( texto2 );

		  //
		  if (Output.Id == 0)
		  {
			  senal = SIN;
			  EnviarUart("eleji seno");
		  } else if (Output.Id == 2)
		  {
			  senal = SQR;
			  EnviarUart("eleji sqr");
		  } else if (Output.Id == 1)
		  {
			  senal = TRI;
			  EnviarUart("eleji tri");
		  } else if (Output.Id == 3)
		  {
			  senal = SAW;
			  EnviarUart("eleji saw");
		  }

		  state = GENERATE;
		  break;

	  case GENERATE:

		  switch(senal)
		  {
		  	  case SIN:
		  		//EnviarUart("genero seno");
		  		  for(int i = 0; i < MAXVALUE; i++)
		  		  {
		  			  TIM2->CCR1 = DCVAL + SIN_LU[i];
		  			  DWT_Delay_us(929/Output.Frequency);
		  		  }

		  		  for(int i = MAXVALUE; i > 0; i--)
		  		  {
		  			  TIM2->CCR1 = DCVAL + SIN_LU[i];
					  DWT_Delay_us(929/Output.Frequency);
		  		  }

				  for(int i = 0; i < MAXVALUE; i++)
				  {
					  TIM2->CCR1 = DCVAL - SIN_LU[i];
					  DWT_Delay_us(929/Output.Frequency);
				  }

				  for(int i = MAXVALUE; i > 0; i--)
				  {
					  TIM2->CCR1 = DCVAL - SIN_LU[i];
					  DWT_Delay_us(929/Output.Frequency);
				  }
		  		break;
		  	  case SQR:
		  		//EnviarUart("genero sqr");
		  		  for(int i = 0; i<MAXVALUE/2; i++)
		  		  {
		  			  TIM2->CCR1 = (MAXVALUE);
		  			  DWT_Delay_us(3906/Output.Frequency);
		  		  }
		  		  for(int i = MAXVALUE/2; i<MAXVALUE; i++)
		  		  {
		  			  TIM2->CCR1 = (0);
		  			  DWT_Delay_us(3906/Output.Frequency);
		  		  }
		  		break;
		  	  case TRI:
		  		EnviarUart("genero tri");
		  		  for(int i = 0; i<MAXVALUE/2; i++)
		  		  {
		  			  TIM2->CCR1 = (2*i);
		  			  DWT_Delay_us(3906/Output.Frequency);
		  		  }
		  		  for(int i = MAXVALUE/2; i>0; i--)
		  		  {
		  			  TIM2->CCR1 = (2*i);
		  			  DWT_Delay_us(3906/Output.Frequency);
		  		  }
		  		break;
		  	  case SAW:
		  		EnviarUart("genero saw");
		  		  for(int i = 0; i<MAXVALUE; i++)
		  		  {
		  			  TIM2->CCR1 = (i);
		  			  DWT_Delay_us(3906/Output.Frequency);
		  		  }
		  		break;
		  }
		  break;
	  }
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 255;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 128;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

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
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LCD_D7_Pin|LCD_D6_Pin|LCD_D5_Pin|LCD_D4_Pin
                          |LCD_E_Pin|LCD_RS_Pin|OFFSET_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LCD_D7_Pin LCD_D6_Pin LCD_D5_Pin LCD_D4_Pin
                           LCD_E_Pin LCD_RS_Pin OFFSET_Pin */
  GPIO_InitStruct.Pin = LCD_D7_Pin|LCD_D6_Pin|LCD_D5_Pin|LCD_D4_Pin
                          |LCD_E_Pin|LCD_RS_Pin|OFFSET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

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
