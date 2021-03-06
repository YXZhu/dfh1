/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

/* USER CODE BEGIN Includes */
#include "echo.h"
//#include "hmc5983.h"
//#include "flash.h"
#include "moto.h"
#include "uart.h"
#include "math.h"
#include "mpu.h"
//#include "dmpctl.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim8;
TIM_HandleTypeDef htim9;

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart2_tx;
DMA_HandleTypeDef hdma_usart3_rx;
DMA_HandleTypeDef hdma_usart3_tx;

osThreadId defaultTaskHandle;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/


osThreadId Echo_1Handle;
osThreadId Echo_2Handle;
osThreadId Echo_3Handle;
osThreadId Echo_4Handle;
osThreadId Echo_5Handle;
osThreadId Echo_6Handle;
osThreadId startinitHandle;
osThreadId main_1Handle;

osThreadId moto_jzHandle;
osThreadId moto_controlHandle;
osThreadId TestHandle;
osThreadId mpuHandle;
osThreadId bzHandle;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM4_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM9_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_TIM8_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C2_Init(void);
void StartDefaultTask(void const * argument);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
                                

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
 extern uint16_t EDjl1,EDjl2,EDjl3,EDjl4,EDjl5,EDjl6;
 extern int16_t YAW,PITCH,ROLL;
 extern uint16_t SA,SB,Timec;
 extern int16_t SPDA,SPDB;
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
extern void Echo_1task(void const * argument);
extern void Echo_2task(void const * argument);
extern void Echo_3task(void const * argument);
extern void Echo_4task(void const * argument);
extern void Echo_5task(void const * argument);
extern void Echo_6task(void const * argument);
void startinittask(void const * argument);
void main_1task(void const * argument);
void moto_jztask(void const * argument);
extern void moto_controltask(void const * argument);
extern void mputask(void const * argument);
void Testtask(void const * argument);
extern void bztask(void const * argument);
/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  Echo_Init();
	
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  MX_USART2_UART_Init();
  MX_TIM9_Init();
  MX_USART3_UART_Init();
  MX_TIM8_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();

  /* USER CODE BEGIN 2 */
   HAL_TIM_PWM_Start(&htim9,TIM_CHANNEL_1);
   HAL_TIM_PWM_Start(&htim9,TIM_CHANNEL_2);
	HAL_TIM_Base_Start_IT(&htim1);
   HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_IC_Start_IT(&htim1,TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim1,TIM_CHANNEL_2);
	HAL_TIM_IC_Start_IT(&htim1,TIM_CHANNEL_3);
	HAL_TIM_IC_Start_IT(&htim1,TIM_CHANNEL_4);
	HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_3);
	HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_4);
	
	HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_1);
	HAL_TIM_Encoder_Start(&htim4,TIM_CHANNEL_2);
	HAL_TIM_Encoder_Start(&htim8,TIM_CHANNEL_1);
	HAL_TIM_Encoder_Start(&htim8,TIM_CHANNEL_2);
//	 GPIO_InitTypeDef GPIO_InitStruct;
//	 GPIO_InitStruct.Pin = GPIO_PIN_15;      //此行原有
//    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;   //GPIO配置为输出
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;         //强上拉
//    HAL_GPIO_Init(GPIOG,&GPIO_InitStruct);
	 //UDelayUS(100000);
	// HAL_GPIO_WritePin(GPIOG, GPIO_PIN_15,GPIO_PIN_SET);
	//HAL_Delay(2000);
	
	
  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* definition and creation of Echo_1 */ 
  osThreadDef(main_1, main_1task, osPriorityRealtime, 0, 256);
  main_1Handle = osThreadCreate(osThread(main_1), NULL);
  
  osThreadDef(Echo_1, Echo_1task, osPriorityRealtime, 0, 128);
  Echo_1Handle = osThreadCreate(osThread(Echo_1), NULL);

  /* definition and creation of Echo_2 */
  osThreadDef(Echo_2, Echo_2task, osPriorityRealtime, 0, 128);
  Echo_2Handle = osThreadCreate(osThread(Echo_2), NULL);

  /* definition and creation of Echo_3 */
  osThreadDef(Echo_3, Echo_3task, osPriorityRealtime, 0, 128);
  Echo_3Handle = osThreadCreate(osThread(Echo_3), NULL);

  /* definition and creation of Echo_4 */
  osThreadDef(Echo_4, Echo_4task, osPriorityRealtime, 0, 128);
  Echo_4Handle = osThreadCreate(osThread(Echo_4), NULL);

  osThreadDef(Echo_5,Echo_5task,osPriorityRealtime, 0, 128);
  Echo_5Handle = osThreadCreate(osThread(Echo_5), NULL);

  osThreadDef(Echo_6, Echo_6task, osPriorityRealtime, 0, 128);
  Echo_6Handle = osThreadCreate(osThread(Echo_6), NULL);

//  osThreadDef(startinit, startinittask, osPriorityNormal, 0, 64);
//  startinitHandle = osThreadCreate(osThread(startinit), NULL);
  
  osThreadDef(moto_control, moto_controltask, osPriorityRealtime, 0, 128);
  moto_controlHandle = osThreadCreate(osThread(moto_control), NULL);
  osThreadDef(moto_jz, moto_jztask, osPriorityRealtime, 0, 128);
  moto_jzHandle = osThreadCreate(osThread(moto_jz), NULL);

	osThreadDef(bz, bztask, osPriorityRealtime, 0,128);  //要调
	bzHandle = osThreadCreate(osThread(bz), NULL);
  /* definition and creation of Echo_5 */

//	osThreadDef(Test, Testtask, osPriorityRealtime, 0, 256);
//	TestHandle = osThreadCreate(osThread(Test), NULL);
  /* definition and creation of Echo_6 */


  osThreadDef(mpu, mputask, osPriorityRealtime, 0, 128);  //要调
  mpuHandle = osThreadCreate(osThread(mpu), NULL);
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */
 

  /* Start scheduler */
  osKernelStart();
  
  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}

/* I2C1 init function */
static void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* I2C2 init function */
static void MX_I2C2_Init(void)
{

  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM1 init function */
static void MX_TIM1_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_IC_InitTypeDef sConfigIC;

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 84-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 50000;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_IC_Init(&htim1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_BOTHEDGE;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM2 init function */
static void MX_TIM2_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_IC_InitTypeDef sConfigIC;

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 84-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 50000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_IC_Init(&htim2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_BOTHEDGE;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM4 init function */
static void MX_TIM4_Init(void)
{

  TIM_Encoder_InitTypeDef sConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 0;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 65535;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  if (HAL_TIM_Encoder_Init(&htim4, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM8 init function */
static void MX_TIM8_Init(void)
{

  TIM_Encoder_InitTypeDef sConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim8.Instance = TIM8;
  htim8.Init.Prescaler = 0;
  htim8.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim8.Init.Period = 65535;
  htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim8.Init.RepetitionCounter = 0;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  if (HAL_TIM_Encoder_Init(&htim8, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim8, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM9 init function */
static void MX_TIM9_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim9.Instance = TIM9;
  htim9.Init.Prescaler = 84-1;
  htim9.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim9.Init.Period = 10000-1;
  htim9.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim9) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim9, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_Init(&htim9) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim9, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_ConfigChannel(&htim9, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_TIM_MspPostInit(&htim9);

}

/* USART2 init function */
static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART3 init function */
static void MX_USART3_UART_Init(void)
{

  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
  /* DMA1_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
  /* DMA1_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, Trig_5_Pin|Trig_1_Pin|Trig_2_Pin|Trig_3_Pin 
                          |Trig_4_Pin|Trig_6_Pin|D_Pin|C_Pin 
                          |B_Pin|A_Pin|ENB2_Pin|ENA2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, N1_Pin|N2_Pin|N3_Pin|N4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : HW_1_Pin */
  GPIO_InitStruct.Pin = HW_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(HW_1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Trig_5_Pin Trig_1_Pin Trig_2_Pin Trig_3_Pin 
                           Trig_4_Pin Trig_6_Pin */
  GPIO_InitStruct.Pin = Trig_5_Pin|Trig_1_Pin|Trig_2_Pin|Trig_3_Pin 
                          |Trig_4_Pin|Trig_6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pin : mgstart_Pin */
  GPIO_InitStruct.Pin = mgstart_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(mgstart_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : N1_Pin N2_Pin N3_Pin N4_Pin */
  GPIO_InitStruct.Pin = N1_Pin|N2_Pin|N3_Pin|N4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : D_Pin C_Pin B_Pin A_Pin 
                           ENB2_Pin ENA2_Pin */
  GPIO_InitStruct.Pin = D_Pin|C_Pin|B_Pin|A_Pin 
                          |ENB2_Pin|ENA2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
extern uint8_t moto_control1;

void startinittask(void const * argument)
{  
	uint8_t aa = 0;
	//uint16_t shudu = 300;
	for(;;)
	{
		if(HAL_GPIO_ReadPin(mgstart_GPIO_Port,mgstart_Pin) == 0)
		{
			osDelay(10);
			if(HAL_GPIO_ReadPin(mgstart_GPIO_Port,mgstart_Pin) == 0)
			{
				osDelay(300);
				if(HAL_GPIO_ReadPin(mgstart_GPIO_Port,mgstart_Pin) == 1)
				{
					if(aa !=1)
					{
					   Echo_Init();
						TIM3->CCR1 = 500; 
						moto_stop();
						moto_control1 = 0;
						osThreadDef(main_1, main_1task, osPriorityRealtime, 0, 128);
	               main_1Handle = osThreadCreate(osThread(main_1), NULL);
						osThreadDef(bz, bztask, osPriorityRealtime, 0, 128);  //要调
                  bzHandle = osThreadCreate(osThread(bz), NULL);
					   if(aa == 2) vTaskDelete(TestHandle);
					   aa = 1;
						vTaskSuspend(mpuHandle);

					}
					else
					{
						 vTaskDelete(main_1Handle);
					vTaskDelete(bzHandle);
						aa = 0;
						moto_stop();
						moto_control1 = 0;
					}
					
				}
				else
				{
					if(aa !=1)
					{						
					while(HAL_GPIO_ReadPin(mgstart_GPIO_Port,mgstart_Pin) == 0){}
					osThreadDef(Test, Testtask, osPriorityNormal, 0, 256);
            	TestHandle = osThreadCreate(osThread(Test), NULL);
					aa = 2;
					}
				}
			}
		}			
		osDelay(10);
		
  }
}

extern int32_t setSPA,setSPB;
int32_t setSPEED,speed;
uint8_t setangle,angleJS,motofrontorturn,setfangx = 0;//前后
extern int16_t ReadDisA,ReadDisB;

<<<<<<< HEAD
#define LowSpeed 5000
#define TurnDis 150
=======
#define LowSpeed 3000
#define TurnDis 140
>>>>>>> parent of 415e836... 淇敼3
#define LowDis 400
#define ChangeDis 300 //距离变化最大值
#define TurnTime 5

#define Turnworkdis 370 //检测转弯后继续行走距离

#define frontJ 1
#define backJ 2
#define front 5
#define back 6
#define right 4
#define left 3

void main_1task(void const * argument)
{
	//setangle = 0;
	angleJS = 0;
	motofrontorturn = 0;
<<<<<<< HEAD
	setSPEED = 9999;
	osDelay(500);
=======
	setSPEED = 7000;
	osDelay(1000);
>>>>>>> parent of 415e836... 淇敼3
	vTaskSuspend(mpuHandle);
	//HAL_GPIO_WritePin(GPIOG, GPIO_PIN_15,GPIO_PIN_SET);
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 5;
	xLastWakeTime = xTaskGetTickCount();
	for(;;)
	{
//		        ulTaskNotifyTake(pdTRUE, 
//                 portMAX_DELAY); /* 无限等待 */
		//echo_1();
	
		switch(angleJS)//4为右 3为左
		{
			case 0:
			{
				if(EDjl1>LowDis)
				{
					if(EDjl2 > ChangeDis)
					{
						speed = LowSpeed;
						moto_control1 = front;
					}
					else
					{
						speed = setSPEED;
						moto_control1 = frontJ;
					}
				}
				else 
				{
						speed = LowSpeed;
					 if(EDjl5 > ChangeDis)
					 {
						 if(EDjl3 < ChangeDis)
						 {
							 if(EDjl1 < TurnDis)
							{
								moto_control1 = right;
								vTaskSuspend(main_1Handle);
								angleJS = 1;
							}
						}
					}
				}						 
			}
			break;
			case 1:
			{
				if(EDjl1 > ChangeDis) 
				{
					speed = setSPEED;
					moto_control1 = frontJ;
				}
				else
				{
					speed = LowSpeed;
					if(EDjl5 > ChangeDis)
					{
						if(EDjl3 < ChangeDis)
						{
							if(EDjl1 < TurnDis)
						   {
								moto_control1 = right;
								vTaskSuspend(main_1Handle);
								angleJS = 2;
							}
						}
					}
				}
			}
			break;
			case 2:
			{
				do
				{
					moto_control1 = frontJ;
					speed = setSPEED;					
					osDelayUntil(&xLastWakeTime, xFrequency);	
				}					
				while(EDjl4 > ChangeDis);
				while(EDjl4 < ChangeDis)
				{
					moto_control1 = frontJ;
					speed = setSPEED;					
					osDelayUntil(&xLastWakeTime, xFrequency);	
				}						
				while(EDjl4 > ChangeDis)
				{
					moto_control1 = frontJ;
					speed = LowSpeed;	
               if(EDjl5 > ChangeDis)
					{
						moto_control1 = 0;
						moto_frontDis(Turnworkdis,Turnworkdis,LowSpeed);
						moto_control1 = left;
						vTaskSuspend(main_1Handle);
						angleJS = 3;
						break;
					}
					osDelayUntil(&xLastWakeTime, xFrequency);
				}
			}
	      break;
			case 3:
			{
				if(EDjl6 > LowDis)
				{
					if(EDjl3 > ChangeDis)
					{
						moto_control1 = back;
						speed = setSPEED;
					}
					else
					{
						moto_control1 = backJ;
						speed = setSPEED;
					}
				}
				else
				{
					if(EDjl5 > ChangeDis)
					{
						if(EDjl6 < TurnDis)
						{
							if(EDjl4 > ChangeDis)
							{

								moto_control1 = left;
								vTaskSuspend(main_1Handle);
								angleJS = 4;
							}
						}
						else
						{
							moto_control1 = back;
							speed = LowSpeed;
						}
					}
				}
			}
			break;
			case 4:
			{
				do
				{
					moto_control1 = backJ;
					speed = setSPEED;					
					osDelayUntil(&xLastWakeTime, xFrequency);	
				}					
				while(EDjl5 > ChangeDis);
				while(EDjl5 < ChangeDis)
				{
					moto_control1 = backJ;
					speed = setSPEED;					
					osDelayUntil(&xLastWakeTime, xFrequency);	
				}						
				while(EDjl5 > ChangeDis)
				{
					moto_control1 = backJ;
					speed = LowSpeed;	
               if(EDjl4 > ChangeDis)
					{
						moto_control1 = 0;
						moto_backDis(Turnworkdis,Turnworkdis,LowSpeed);
						moto_control1 = right;
						vTaskSuspend(main_1Handle);
						angleJS = 5;
						break;
					}
					osDelayUntil(&xLastWakeTime, xFrequency);
				}
			}
	      break;
			case 5:
			{
				if(EDjl1 > LowDis)
				{
					if(EDjl2 > ChangeDis)
					{
						moto_control1 = front;
						speed = setSPEED;
					}
					else
					{
						moto_control1 = frontJ;
						speed = setSPEED;
					}
				}
				else
				{
					if(EDjl4 > ChangeDis)
					{
						if(EDjl1 < TurnDis)
						{
							if(EDjl5 > ChangeDis)
							{

								moto_control1 = right;
								vTaskSuspend(main_1Handle);
								angleJS = 6;
								
							}
						}
						else
						{
							moto_control1 = front;
							speed = LowSpeed;
						}
					}
				}
			}
			break;
//			case 6:
//			{
//            setfangx ++;
// 				if(setfangx == 2) angleJS = 7;
//				else angleJS = 2;
//			}
//	      break;
			case 6:
			{
				do
				{
					moto_control1 = frontJ;
					speed = setSPEED;					
					osDelayUntil(&xLastWakeTime, xFrequency);	
				}					
				while(EDjl4 > ChangeDis);
				while(EDjl4 < ChangeDis)
				{
					moto_control1 = frontJ;
					speed = setSPEED;					
					osDelayUntil(&xLastWakeTime, xFrequency);	
				}						
				while(EDjl4 > ChangeDis)
				{
					moto_control1 = frontJ;
					speed = LowSpeed;	
               if(EDjl5 > ChangeDis)
					{
						moto_control1 = 0;
						moto_frontDis(Turnworkdis,Turnworkdis,LowSpeed);
						moto_control1 = left;
						vTaskSuspend(main_1Handle);
						angleJS = 7;
						break;
						//goto s;
					}
					osDelayUntil(&xLastWakeTime, xFrequency);
				}
				
			}
	      break;
			case 7:
			{
				if(EDjl6 > LowDis)
				{
					if(EDjl3 > ChangeDis)
					{
						moto_control1 = back;
						speed = setSPEED;
					}
					else
					{
						moto_control1 = backJ;
						speed = setSPEED;
					}
				}
				else
				{
					if(EDjl5 > ChangeDis)
					{
						if(EDjl6 < TurnDis)
						{
							if(EDjl4 > ChangeDis)
							{

								moto_control1 = left;
								vTaskSuspend(main_1Handle);
								angleJS = 8;
							}
						}
						else
						{
							moto_control1 = back;
							speed = LowSpeed;
						}
					}
				}
			}
			break;
			case 8:
			{
				do
				{
					moto_control1 = backJ;
					speed = setSPEED;					
					osDelayUntil(&xLastWakeTime, xFrequency);	
				}					
				while(EDjl5 > ChangeDis);
				while(EDjl5 < ChangeDis)
				{
					moto_control1 = backJ;
					speed = setSPEED;					
					osDelayUntil(&xLastWakeTime, xFrequency);	
				}						
				while(EDjl5 > ChangeDis)
				{
					moto_control1 = backJ;
					speed = LowSpeed;	
               if(EDjl4 > ChangeDis)
					{
						moto_control1 = 0;
						moto_backDis(Turnworkdis,Turnworkdis,LowSpeed);
						moto_control1 = right;
						vTaskSuspend(main_1Handle);
						angleJS = 9;
						break;
					}
					osDelayUntil(&xLastWakeTime, xFrequency);
				}
			}
	      break;
			case 9:
			{
				if(EDjl1 > LowDis)
				{
					if(EDjl2 > ChangeDis)
					{
						moto_control1 = front;
						speed = setSPEED;
					}
					else
					{
						moto_control1 = frontJ;
						speed = setSPEED;
					}
				}
				else
				{
					if(EDjl4 > ChangeDis)
					{
						if(EDjl1 < TurnDis)
						{
							if(EDjl5 > ChangeDis)
							{

								moto_control1 = right;
								vTaskSuspend(main_1Handle);
								angleJS = 10;
								
							}
						}
						else
						{
							moto_control1 = front;
							speed = LowSpeed;
						}
					}
				}
			}
			break;
			case 10:
			{
				do
				{
					moto_control1 = frontJ;
					speed = setSPEED;					
					osDelayUntil(&xLastWakeTime, xFrequency);	
				}					
				while(EDjl4 > ChangeDis);
				while(EDjl4 < ChangeDis)
				{
					moto_control1 = frontJ;
					speed = setSPEED;					
					osDelayUntil(&xLastWakeTime, xFrequency);	
				}						
				while(EDjl4 > ChangeDis)
				{
					moto_control1 = frontJ;
					speed = LowSpeed;	
               if(EDjl1 < TurnDis)
					{
						moto_control1 = right;
						vTaskSuspend(main_1Handle);
						angleJS = 11;
						break;
					}
					osDelayUntil(&xLastWakeTime, xFrequency);
				}
			}
			break;
			case 11:
			{
				if(EDjl1>LowDis)
				{
					speed = setSPEED;
					moto_control1 = frontJ;
				}
				else 
				{
					speed = LowSpeed;
					moto_control1 = front;
					 if(EDjl4 > ChangeDis)
					 {
						// if(EDjl5 > ChangeDis)
						 //{
							 if(EDjl1 < TurnDis)
							{
								moto_control1 = right;
								vTaskSuspend(main_1Handle);
								angleJS = 12;
							}
						//}
					}
				}						 
			}
			break;
			case 12:
			{
				if(EDjl2 < ChangeDis)
				{
					speed = setSPEED;
					moto_control1 = backJ;
				}
				else
				{
					moto_control1 = 0;
					moto_stop();
					angleJS = 14;
				}
			}
			break;
			default:break;
		}
		
//		if(angleJS<10)   ///3为右 4为左
//		{
//					if(setangle == 1)
//					{
//						if(EDjl1>LowDis)
//						{
//							if(EDjl3>300)
//							{
//								if(EDjl5>300)
//								{
//									if(motofrontorturn == 0)
//									{
//										if(!(EDjl2<300&EDjl4<300))
//										{
//											speed = LowSpeed;
//											moto_control1 = 5;
//										}
//									}
//								}
//							}
//							if(EDjl2>300)
//							{
////								if(EDjl3>300)////改过
////								{
////									if(EDjl5>300)
////									{
//////										setSPA = 3000;
//////										setSPB = 3000;
//////										moto_front();
////									}
////								}
//							}
//							else
//							{
//								  if(angleJS>=2)
//								  {
//									  if(EDjl5<300)
//									  {
//										  if(EDjl3<300)
//										  {
//											  motofrontorturn = 1;
//										  }
//									  }
//								  }
//								  if(EDjl4<300)
//								  {
//									speed = setSPEED;
//							
//									moto_control1 = 1;
//								  }/////？？？？
//								if(EDjl3<300)
//								{
//                           speed = setSPEED;
//						
//									moto_control1 = 1;
//									if(EDjl4>300&angleJS>=2)//g
//									 {
//										 if(motofrontorturn == 1)
//										 {
//											 if(EDjl5>300)//g
//											 {												 
//													moto_control1 = 4;
//													vTaskSuspend(main_1Handle);
//													angleJS++;///js = 3
//													setangle = 0;
//													motofrontorturn = 0;
//											 }
//										 }
//									}						
//								 }	
//							}

//						}
//						else
//						{
//							if(angleJS<2)
//							{
//								if(EDjl2<300)
//								{
//									if(EDjl3<300)
//									{
//											
//										if(EDjl1<TurnDis)
//										{
//											if(EDjl4>300)
//											{
//												if(EDjl5>300)
//												{
//													moto_control1 = 3;
//													vTaskSuspend(main_1Handle);
//													angleJS++;	
//													motofrontorturn = 0;
//												}
//											}
//										}
//										else
//										{
//											speed = LowSpeed;
//											
//											moto_control1 = 1;
//										}
//									}
//								}
//							}
//							else
//							{	
//								//if(angleJS<10)
//								//{
//									if(EDjl1<TurnDis)
//									{
//										if(EDjl3>300)
//										{
//											if(EDjl5>300)
//											{
//												moto_control1 = 3;
//												vTaskSuspend(main_1Handle);
//												angleJS++;	///666666							
//												motofrontorturn = 0;
//											}
//										}
//									
//									}
//									else
//									{
//											speed = LowSpeed;
//											moto_control1 = 5;
//										
//									}
//								
//									
//							//}
//						}
//						}
//					}
//					else ///setangle = 0;
//					{
//						if(EDjl1<LowDis)//g
//						{
//							if(EDjl2>300)
//							{
//								if(EDjl4>300)
//								{
//									if(motofrontorturn == 0)
//									{
//										speed = LowSpeed;
//										moto_control1 = 6;
//									
//									}
//								}
//							}
//						}
//						else
//						{
//							if(EDjl3<300)
//							{								
//									speed = setSPEED;
//								    //moto_back();//g
//									moto_control1 = 2;
//								if(EDjl4<300) motofrontorturn = 1;
//								else
//								{
//									if(EDjl5>300)
//									{
//										if(motofrontorturn == 1)
//										{
//											moto_control1 = 3;
//											vTaskSuspend(main_1Handle);
//											angleJS++; ///5555
//											motofrontorturn = 0;
//											setangle = 1;
//										}
//									}
//								}
//							}
//							if(EDjl6<=LowDis&setangle == 0)
//							{
//								if(EDjl3>300)
//								{
//									speed = LowSpeed;
//									moto_control1 = 6;
//									
//									if(EDjl5>300)
//									{
//										if(EDjl4>300)
//										{
//											if(EDjl6<TurnDis)
//											{
//												moto_control1 = 4;
//												vTaskSuspend(main_1Handle);
//												angleJS++; // 44444
//												motofrontorturn = 0;
//											
//											}
//											
//										}
//									}
//								}
//							}
//						}
//					}
//			//		if(angleJS == 2)
//			//		{
//			//			moto_stop();
//			//			moto_control1 = 0;
//			//		}
//			 
//		}
//      else
//		{
//			if(EDjl1>LowDis)
//			{
//			  if(EDjl2<300)
//			  {
//				  if(EDjl3<300)
//				  {
//					  if(motofrontorturn != 2)
//					  {
//						speed = setSPEED;
//					
//						moto_control1 = 1;
//					  }
//					  else
//					  {
//						speed = setSPEED;
//					
//						moto_control1 = 2; 
//					  }
//				  }
//			  }
//			  else
//			  {
//				  if(EDjl4>300)
//				  {
//					  if(angleJS >= 12)
//					  {
//						  if(EDjl3>300)
//						  {
//							  osDelay(500);
//							  moto_stop();
//							  moto_control1 = 0;
//							  vTaskDelete(main_1Handle);
//						  }
//					  }
//				  }
//			  }
//		   }
//			else
//			{
//				if(EDjl2<300)
//				{
//					if(EDjl3<300)
//					{
//						if(EDjl1<TurnDis)
//						{
//							if(EDjl5>300)
//							{
//								moto_control1 = 3;
//								vTaskSuspend(main_1Handle);
//								angleJS++;	
//								//motofrontorturn = 0;
//							}
//						}
//						else
//						{
//							speed = LowSpeed;
//						
//							moto_control1 = 1;
//						}
//					}
//				}
//				else
//				{
//					if(EDjl1<TurnDis)
//					{
//						if(EDjl3>300)
//						{
//							moto_control1 = 3;
//						   vTaskSuspend(main_1Handle);
//						   angleJS++;	
//							//moto_back();
//							//moto_control1 = 2;
//							motofrontorturn = 2;
//						}
//					}
//					else
//					{
//							speed = LowSpeed;
//						
//							moto_control1 = 5;
//					}
//				}
//			}
//		}
					
		osDelayUntil(&xLastWakeTime, xFrequency);			  
		//osDelay (5);		
	}
}

void Testtask(void const * argument)
{
  /* USER CODE BEGIN StartTask05 */
  /* Infinite loop */
	//uint8_t JL[] = {"前    cm左   cm右   cm后   cm"};
	uint16_t c;
uint8_t JL[] = {"F---.- L---.- E---.- R---.- G---.- B---.- ---°  -----, -----, ----- --- ---\n\r"};
//uint8_t JL1[] = {"--------------------------\n\r"}; 
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 20;
	xLastWakeTime = xTaskGetTickCount();
  for(;;)
  { 

		print_usart2("F%d\r\nL%d\r\nE%d\r\nR%d\r\nG%d\r\nB%d\r\n",EDjl1,EDjl2,EDjl3,EDjl4,EDjl5,EDjl6);
			  
	    //angle1 = angle;    
	   //JL[1] = echo1[0]/10000+48;
		JL[1] = EDjl1%10000/1000+48;
		JL[2] = EDjl1%10000%1000/100+48;
		JL[3] = EDjl1%10000%1000%100/10+48;
	   JL[5] = EDjl1%10000%1000%100%10+48;
	  
	  //	JL[11] = echo1[1]/10000+48;
		JL[8] = EDjl2%10000/1000+48;
		JL[9] = EDjl2%10000%1000/100+48;
		JL[10] = EDjl2%10000%1000%100/10+48;
	   JL[12] = EDjl2%10000%1000%100%10+48;
	  
	  // JL[21] = EDjl1/10000+48;
		JL[15] = EDjl3%10000/1000+48;
		JL[16] = EDjl3%10000%1000/100+48;
		JL[17] = EDjl3%10000%1000%100/10+48;
	   JL[19] = EDjl3%10000%1000%100%10+48;
	  
   	//JL[14] = EDjl4/10000+48;
		JL[22] = EDjl4%10000/1000+48;
		JL[23] = EDjl4%10000%1000/100+48;
		JL[24] = EDjl4%10000%1000%100/10+48;
	   JL[26] = EDjl4%10000%1000%100%10+48;
		
		JL[29] = EDjl5%10000/1000+48;
		JL[30] = EDjl5%10000%1000/100+48;
		JL[31] = EDjl5%10000%1000%100/10+48;
	   JL[33] = EDjl5%10000%1000%100%10+48;
	  
   	//JL[14] = EDjl4/10000+48;
		JL[36] = EDjl6%10000/1000+48;
		JL[37] = EDjl6%10000%1000/100+48;
		JL[38] = EDjl6%10000%1000%100/10+48;
	   JL[40] = EDjl6%10000%1000%100%10+48;
		
//		JL[42] = angle1/100+48;
//		JL[43] = angle1%100/10+48;
//	   JL[44] = angle1%100%10+48;
		
		if(YAW<0) JL[48] = '-',c = -YAW;
		else JL[48] = ' ',c = YAW;
		
		JL[49] = c/10000+48;
		JL[50] = c%10000/1000+48;
	   JL[51] = c%10000%1000/100+48;
		JL[52] = c%10000%1000%100/10+48;
		JL[53] = c%10000%1000%100%10+48;
		
		if(PITCH<0) JL[55] = '-',c = -PITCH;
		else JL[55] = ' ',c = PITCH;
		
		JL[56] = c/10000+48;
		JL[57] = c%10000/1000+48;
	   JL[58] = c%10000%1000/100+48;
		JL[59] = c%10000%1000%100/10+48;
		JL[60] = c%10000%1000%100%10+48;
		
		if(ROLL<0) JL[62] = '-',c = -ROLL;
		else JL[62] = ' ',c = ROLL;
		
		JL[63] = c/10000+48;
		JL[64] = c%10000/1000+48;
	   JL[65] = c%10000%1000/100+48;
		JL[66] = c%10000%1000%100/10+48;
		JL[67] = c%10000%1000%100%10+48;
		
		JL[69] = SPDA/100+48;
		JL[70] = SPDA%100/10+48;
		JL[71] = SPDA%100%10+48;
		
		JL[73] = SPDB/100+48;
		JL[74] = SPDB%100/10+48;
		JL[75] = SPDB%100%10+48;
//		JL[9] = EDjl2/100+48;
//		JL[10] = EDjl2%100/10+48;
//		JL[11] = EDjl2%100%10+48;
//		
//		JL[16] = EDjl3/100+48;
//		JL[17] = EDjl3%100/10+48;
//		JL[18] = EDjl3%100%10+48;
//		
//		JL[23] = EDjl4/100+48;
//		JL[24] = EDjl4%100/10+48;
//		JL[25] = EDjl4%100%10+48;
	// HAL_UART_Transmit_DMA(&huart3,JL,78);
	 osDelayUntil(&xLastWakeTime, xFrequency);
  }
  /* USER CODE END StartTask05 */
}



/* USER CODE END 4 */

/* StartDefaultTask function */
void StartDefaultTask(void const * argument)
{

  /* USER CODE BEGIN 5 */
  /* Infinite loop */

  for(;;)
  {
		osDelay(1);
  }
  /* USER CODE END 5 */ 
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM14 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
/* USER CODE BEGIN Callback 0 */

/* USER CODE END Callback 0 */
  if (htim->Instance == TIM14) {
    HAL_IncTick();
  if(Timec == 25)
  {
	  Timec = 0;
	 SPDA = __HAL_TIM_GET_COUNTER(&htim8);
	 SPDB = __HAL_TIM_GET_COUNTER(&htim4);
	  ReadDisA = ReadDisA + SPDA;
	  ReadDisB = ReadDisB + SPDB;
	  __HAL_TIM_SET_COUNTER(&htim8,0);
	  __HAL_TIM_SET_COUNTER(&htim4,0);
  }
  else Timec ++;  
  }
/* USER CODE BEGIN Callback 1 */
//  if(Timec == 25)
//  {
//	  Timec = 0;
//	 SPDA = __HAL_TIM_GET_COUNTER(&htim8);
//	 SPDB = __HAL_TIM_GET_COUNTER(&htim4);
//	  ReadDisA = ReadDisA + SPDA;
//	  ReadDisB = ReadDisB + SPDB;
//	  __HAL_TIM_SET_COUNTER(&htim8,0);
//	  __HAL_TIM_SET_COUNTER(&htim4,0);
//  }
//  else Timec ++;  
/* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
