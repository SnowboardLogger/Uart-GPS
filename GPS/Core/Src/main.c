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
#include "stdlib.h"
#include "stdio.h"
#include <string.h>
#include "stm32l4xx_hal_uart.h"
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
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
#define USART1_ADDR = 0x40013800
#define USART1_CR1_OFFSET = 0x0
#define USART1_BRR _OFFSET = 0xC
#define USART1_CR2_OFFSET = 0x4
#define USART1_CR3_OFFSET = 0x8

volatile char rxBuffer[70] = "";



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
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
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  //enable GGA (contains the precision data) and RMC (contains all the minimum navigation info)
  //data on the GPS
  char inputBuffer[] = "PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0";
  HAL_UART_Transmit(&huart1, (uint8_t *) inputBuffer, sizeof(inputBuffer), 100);

  char dataBuffer[70]="ASDFADFHDFGASDFASDFSDFHDASDF";//max chars of 70 from gpgga
  int dataElementNum = 0;

  char dataType[] = "XXXXXX";

  char latitudeChar[] = "lllll.ll";
  double latitude = 0;

  char latDir = 'A';//N or S

  char longitudeChar[] = "yyyyy.yy";
  double longitude = 0;

  char longDir = 'A';//E or W

  uint8_t fix = 0;//0 = invalid, 1 = GPS fix, 2 = Dif. GPS fix

  uint8_t numSatellites = 0;

  double hdop = 0;//Horizontal Dilution of Precision
  char hdopChar[] = "x.x";

  double altitude = 0;
  char altitudeChar[] = "x.x";

  char altitudeUnits = 'M';//M = meters
  //they have a checksum, do we need to use it?


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	//The last parameter is the number of data elements to be received, this will need to be changed based on the max # bytes the GPS sends
	//The number of bytes can change based on whether the GPS has a fix or not so that's a problem
	//printf("test:2,23,32,4,23,4,423,43");
	//__HAL_UART_FLUSH_DRREGISTER(&huart1);
	HAL_UART_Receive(&huart1, (uint8_t *) dataBuffer, 68, 10000);
	//huart->RxISR(huart); might hold some data?

	int index = 0;

	for(uint8_t i = 0; i < 70; ++i){
		char letter = *(dataBuffer+i);

		//printf("%c", letter);


		if(letter == ','){
			++dataElementNum;
			index = 0;
		}

		if(dataElementNum == 0 && letter != ','){
			//datatype, either GPGGA or GPRMC
			dataType[index] = letter;
			++index;
		}


		if(strcmp(dataType,"$GPGGA") == 0 && letter != ','){
			if (dataElementNum == 1 ){
				//This is the UTC (time) if we need it
			} else if (dataElementNum == 2 ){
				latitudeChar[index] = letter;
				++index;
				if(*(dataBuffer+i+1) == ','){
					latitude = atof(latitudeChar);
				}

			} else if (dataElementNum == 3){
				latDir = letter;

			} else if (dataElementNum == 4){
				longitudeChar[index] = letter;
				++index;
				if(*(dataBuffer+i+1) == ','){
					longitude = atof(longitudeChar);
				}

			} else if (dataElementNum == 5){
				longDir = letter;

			} else if (dataElementNum == 6){
				fix = (uint8_t) letter;

			} else if (dataElementNum == 7){
				//sus?
				numSatellites = (uint8_t) letter;

			} else if (dataElementNum == 8){
				hdopChar[index] = letter;
				++index;
				if(*(dataBuffer+i+1) == ','){
					hdop = atof(hdopChar);
				}

			} else if (dataElementNum == 9){
				altitudeChar[index] = letter;
				++index;
				if(*(dataBuffer+i+1) == ','){
					altitude = atof(altitudeChar);
				}

			} else if (dataElementNum == 10){
				altitudeUnits = letter;

			} else if (dataElementNum == 11){
				break;
			}

		} else if(strcmp(dataType,"$GPRMC") == 0 ){
			if (dataElementNum == 1 ){

			} else if (dataElementNum == 2){

			} else if (dataElementNum == 3){

			} else if (dataElementNum == 4){

			} else if (dataElementNum == 5){

			} else if (dataElementNum == 6){

			} else if (dataElementNum == 7){

			} else if (dataElementNum == 8){

			} else if (dataElementNum == 9){

			} else if (dataElementNum == 10){

			} else if (dataElementNum == 11){

			} else if (dataElementNum == 12){

			} else if (dataElementNum == 13){

			} else if (dataElementNum == 14){

			} else if (dataElementNum == 15){

			}
		}




	}
	/*
	 * char latDir = "";//N or S

  char longitudeChar[] = "";
  double longitude = 0;

  char longDir = "";//E or W

  uint8_t fix = 0;//0 = invalid, 1 = GPS fix, 2 = Dif. GPS fix

  uint8_t numSatellites = 0;

  double hdop = 0;//Horizontal Dilution of Precision
  char hdopChar[] = "";

  double altitude = 0;
  char altitudeChar[] = "";

  char altitudeUnits = 'M';//M = meters*/
	printf("Latitude: %f", latitude);
	printf(" %c", latDir);
	printf(" | Longitude: %f", longitude);
	printf(" %c", longDir);
	printf(" | GPS fix?: %u", fix);
	printf(" | Satellites: %u", numSatellites);
	printf(" | HDOP: %f", hdop);
	printf(" | altitude: %f", altitude);
	printf(" %c", altitudeUnits);
	printf("\n");




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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LD3_Pin */
  GPIO_InitStruct.Pin = LD3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD3_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
PUTCHAR_PROTOTYPE
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
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
