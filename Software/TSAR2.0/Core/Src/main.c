/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
//#include <stdarg.h> //for va_list var arg functions
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
SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART1_UART_Init(void);

/* USER CODE BEGIN PFP */

//void myprintf(const char *fmt, ...);
// Begin Katie's PFP

int mainCheck(int valveCState[], int valveExState[], int batteryState);
//this is the main for this part that will call all other functions
//Will return 0 if no errors were found
int checkState(int valveCState[], int valveExState[], int valvesInError[]);	//input is a placeholder as I don't know how we get valve info into function

//A valve state didn't match, so now we here
int errorFound(int valvesInError[]);

//Take read in valve states and make them usable
void fillStruct(int valveCState[], int valveExState[]);

//Main function for the checking
//It is expected that this is what a higher function calls to initiate check
int mainCheck(int valveCState[], int valveExState[], int batteryState);

//Just realized that I was just copying code in each error level
int isError(int valvesInError[], int errorLevel);

//Just turn on Buzzer
void buzzOn();

//just turn  off Buzzer
void buzzOff();

//Turn on a specific binary (top is MSB) on LED lights
void ledOn(int whichOn);

//Turn off all LEDs
void ledOff();

//Power failure has occured
void powerFailureState();

// End Katie's PFP

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//void myprintf(const char *fmt, ...) {
//static char buffer[256];
//va_list args;
//va_start(args, fmt);
//vsnprintf(buffer, sizeof(buffer), fmt, args);
//va_end(args);
//int len = strlen(buffer);
//HAL_UART_Transmit(&huart2, (uint8_t*)buffer, len, -1);
//}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* USER CODE BEGIN 1 */

	// Begin Tiffani's variable definitions

	int sensorState[10];

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
	MX_SPI1_Init();
	MX_USART1_UART_Init();
	MX_FATFS_Init();

	/* USER CODE BEGIN 2 */

	// myprintf("\r\n~ SD card demo by kiwih ~\r\n\r\n");
	HAL_Delay(1000); //a short delay is important to let the SD card settle

	//some variables for FatFs
	FATFS FatFs; 	//Fatfs handle
	FIL fil; 		//File handle
	//FRESULT fres; //Result after operations

	//Open the file system
	f_mount(&FatFs, "", 1); // 1=mount now
	// if (fres != FR_OK) {
	// myprintf("f_mount error (%i)\r\n", fres);
	// while(1);
	//}

	//Let's get some statistics from the SD card
	// DWORD free_clusters;

	//FATFS* getFreeFs;

	//fres = f_getfree("", &free_clusters, &getFreeFs);
	//if (fres != FR_OK) {
	//myprintf("f_getfree error (%i)\r\n", fres);
	//while(1);
	//}

	//Formula comes from ChaN's documentation
	//total_sectors = (getFreeFs->n_fatent - 2) * getFreeFs->csize;
	//free_sectors = free_clusters * getFreeFs->csize;

	//myprintf("SD card stats:\r\n%10lu KiB total drive space.\r\n%10lu KiB available.\r\n", total_sectors / 2, free_sectors / 2);

	//Now let's try to open file "test.txt"
	//fres = f_open(&fil, "test.txt", FA_READ);
	// if (fres != FR_OK) {
	// myprintf("f_open error (%i)\r\n");
	//while(1);
	//}
	//myprintf("I was able to open 'test.txt' for reading!\r\n");

	//Read 30 bytes from "test.txt" on the SD card
	BYTE readBuf[30];

	//We can either use f_read OR f_gets to get data out of files
	//f_gets is a wrapper on f_read that does some string formatting for us
	//TCHAR* rres = f_gets((TCHAR*)readBuf, 30, &fil);
	//if(rres != 0) {
	//myprintf("Read string from 'test.txt' contents: %s\r\n", readBuf);
	//} else {
	//myprintf("f_gets error (%i)\r\n", fres);
	//}

	//Be a tidy kiwi - don't forget to close your file!
	//f_close(&fil);

	//Now let's try and write a file "write.txt"
	f_open(&fil, "valveStateData.txt",
			FA_WRITE | FA_OPEN_ALWAYS | FA_CREATE_ALWAYS);
	//if(fres == FR_OK) {
	//myprintf("I was able to open 'write.txt' for writing\r\n");
	//} else {
	//myprintf("f_open error (%i)\r\n", fres);
	//}

	//Copy in a string
	strncpy((char*) readBuf, "a new file is made!", 19);
	UINT bytesWrote;
	f_write(&fil, readBuf, 19, &bytesWrote);
	//if(fres == FR_OK) {
	//myprintf("Wrote %i bytes to 'write.txt'!\r\n", bytesWrote);
	//} else {
	//myprintf("f_write error (%i)\r\n");
	//}

	//Be a tidy kiwi - don't forget to close your file!
	f_close(&fil);

	//We're done, so de-mount the drive
	f_mount(NULL, "", 0);

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		// Begin Tiffani's main program

		sensorState[0] = HAL_GPIO_ReadPin(GPIOA, Hall_Effect_Sensor_One_Pin);

		sensorState[1] = HAL_GPIO_ReadPin(GPIOA, Hall_Effect_Sensor_Two_Pin);

		sensorState[2] = HAL_GPIO_ReadPin(GPIOA, Hall_Effect_Sensor_Three_Pin);

		sensorState[3] = HAL_GPIO_ReadPin(GPIOA, Hall_Effect_Sensor_Four_Pin);

		sensorState[4] = HAL_GPIO_ReadPin(GPIOB, Hall_Effect_Sensor_Five_Pin);

		sensorState[5] = HAL_GPIO_ReadPin(GPIOB, Hall_Effect_Sensor_Six_Pin);

		sensorState[6] = HAL_GPIO_ReadPin(GPIOA, Hall_Effect_Sensor_Seven_Pin);

		sensorState[7] = HAL_GPIO_ReadPin(GPIOA, Hall_Effect_Sensor_Eight_Pin);

		sensorState[8] = HAL_GPIO_ReadPin(GPIOA, Hall_Effect_Sensor_Nine_Pin);



		// End Tiffani's main program

		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		//Blink the LED every second
		// HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
		//HAL_Delay(1000);
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
	PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief SPI1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI1_Init(void) {

	/* USER CODE BEGIN SPI1_Init 0 */

	/* USER CODE END SPI1_Init 0 */

	/* USER CODE BEGIN SPI1_Init 1 */

	/* USER CODE END SPI1_Init 1 */
	/* SPI1 parameter configuration*/
	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 7;
	hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
	hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
	if (HAL_SPI_Init(&hspi1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN SPI1_Init 2 */

	/* USER CODE END SPI1_Init 2 */

}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void) {

	/* USER CODE BEGIN USART1_Init 0 */

	/* USER CODE END USART1_Init 0 */

	/* USER CODE BEGIN USART1_Init 1 */

	/* USER CODE END USART1_Init 1 */
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 38400;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_RTS_CTS;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART1_Init 2 */

	/* USER CODE END USART1_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB,
			Buzzer_Pin | SD_CS_Pin | LED_Three_Pin | LED_Two_Pin | LED_One_Pin,
			GPIO_PIN_SET);

	/*Configure GPIO pins : Hall_Effect_Sensor_Nine_Pin Hall_Effect_Sensor_One_Pin Hall_Effect_Sensor_Two_Pin Hall_Effect_Sensor_Three_Pin
	 Hall_Effect_Sensor_Four_Pin Hall_Effect_Sensor_Eight_Pin Hall_Effect_Sensor_Seven_Pin */
	GPIO_InitStruct.Pin = Hall_Effect_Sensor_Nine_Pin
			| Hall_Effect_Sensor_One_Pin | Hall_Effect_Sensor_Two_Pin
			| Hall_Effect_Sensor_Three_Pin | Hall_Effect_Sensor_Four_Pin
			| Hall_Effect_Sensor_Eight_Pin | Hall_Effect_Sensor_Seven_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : Buzzer_Pin LED_Three_Pin LED_Two_Pin LED_One_Pin */
	GPIO_InitStruct.Pin =
			Buzzer_Pin | LED_Three_Pin | LED_Two_Pin | LED_One_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : SD_CS_Pin */
	GPIO_InitStruct.Pin = SD_CS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(SD_CS_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : Hall_Effect_Sensor_Six_Pin Hall_Effect_Sensor_Five_Pin */
	GPIO_InitStruct.Pin = Hall_Effect_Sensor_Six_Pin
			| Hall_Effect_Sensor_Five_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

// Begin Katie's PF's

int mainCheck(int valveCState[], int valveExState[], int batteryState)
{
	//For tracking error status, only says that we have an error
	int errorButton = 0;

	//Keeps trak of which state of error that we are in
	//0 = all good, 1 = minor error, 2 = bigger error, 3 = major error, 4 = power failure
	int errorState = 0;

	//array keeps track of which valves are in error
	int valvesInError[10]; //0 = good, 1 = error

	//initialising error array
	//10th index is for power
	int i = 0;
	for (i=0; i<9; ++i)
	{
		valvesInError[i] = 0;
	}
	valvesInError[9] = batteryState;

	errorButton = checkState(valveCState, valveExState, valvesInError);

	if(errorButton == 1)
		{
			errorState = errorFound(valvesInError);
			isError(valvesInError, errorState);
		}
		//clear any errors that may have been occuring
		if(errorButton == 0)
		{
			buzzOff();
			ledOff();
			errorState = 0;
		}

		return errorState;
	}

//returns error level (0-4)
int checkState(int valveCState[], int valveExState[], int valvesInError[]) {
	//If 1, then an error has occured
	int errorButton = 0;

	//checking each valve for an error
	if (valveCState[0] != valveExState[0]) {
		errorButton = 1;
		valvesInError[0] = 1;
	}
	if (valveCState[1] != valveExState[1]) {
		errorButton = 1;
		valvesInError[1] = 1;
	}
	if (valveCState[2] != valveExState[2]) {
		errorButton = 1;
		valvesInError[2] = 1;
	}
	if (valveCState[3] != valveExState[3]) {
		errorButton = 1;
		valvesInError[3] = 1;
	}
	if (valveCState[4] != valveExState[4]) {
		errorButton = 1;
		valvesInError[4] = 1;
	}
	if (valveCState[5] != valveExState[5]) {
		errorButton = 1;
		valvesInError[5] = 1;
	}
	if (valveCState[6] != valveExState[6]) {
		errorButton = 1;
		valvesInError[6] = 1;
	}
	if (valveCState[7] != valveExState[7]) {
		errorButton = 1;
		valvesInError[7] = 1;
	}
	if (valveCState[8] != valveExState[8]) {
		errorButton = 1;
		valvesInError[8] = 1;
	}
	if (valvesInError[9] == 1) {
		errorButton = 1;
	}

	return errorButton;
}

//when in doubt, error out
//returns a value depending on which level of error is found
int errorFound(int valvesInError[]) {
	int errorLevel = 0;
	//index - priority
// PV_L02		//4 - 1
// PV_L03		//5 - 2
// PV_F02		//2 - 3
// PV_L01		//3 - 4
// PV_F01		//1 - 5
// SOV_F01		//0 - 6
// PV_LF01		//8 - 7
// PV_P01		//6 - 8
// PV_P02		//7 - 9
//Error level is set to highest prio valve
	if (valvesInError[7])
		errorLevel = 1;
	if (valvesInError[6])
		errorLevel = 1;
	if (valvesInError[8])
		errorLevel = 1;
	if (valvesInError[0])
		errorLevel = 1;
	if (valvesInError[1])
		errorLevel = 2;
	if (valvesInError[3])
		errorLevel = 2;
	if (valvesInError[2])
		errorLevel = 2;
	if (valvesInError[5])
		errorLevel = 3;
	if (valvesInError[4])
		errorLevel = 3;
	if (valvesInError[9]) //power failure on battery, nothing else matters
		errorLevel = 4;

	return errorLevel;
}

int isError(int valvesInError[], int errorLevel) {
	//The buzzer doesn't get turned off until error is cleared
	buzzOn();

	if (errorLevel == 4) {
		powerFailureState(); //It is currently not expected that we can return from this state without a restart
		return 1;
	}

	int i = 0;
	for (i = 0; i < 6; ++i) {
		ledOn(errorLevel);
		sleep(1);
		ledOff();
		sleep(1);
	}

	for (i = 0; i < 9; ++i) {
		if (valvesInError[i] == 1) {
			ledOn(i);
			sleep(1);
			ledOff();
		}
	}
	LedOn(errorLevel);
	return 0;
}

/*
 int bigError(int valvesInError[])
 {
 int turnOn;
 //turn on buzzer
 //turn on LEDs for error level
 //Flash for 5 seconds, .75s on, .25s off

 //Go through which valves are in error, hold LED on for 1s
 for(i=0; i<9; ++i)
 {
 if(valvesInError[i] = 1;)
 {

 }
 int someError(int valvesInError[])
 {
 }
 int smallError(int valvesInError[])
 {

 }
 */

void buzzOn() {
	//printf for debugging outside of board
	//will comment out in board testing
	printf("Buzzer ON\n");
}

void buzzOff() {
	//printf for debugging outside of board
	//will comment out in board testing
	printf("Buzzer OFF\n");

}

void ledOn(int whichOn) {
	//6,7,8 all have same light error
	if ((whichOn == 7) || (whichOn == 8))
		whichOn = 6;
	++whichOn; //change from index to actual number displayed in binary on LEDs

	//add a check for bottom 3 prio as they all get same code
	//printf for debugging outside of board
	//will comment out in board testing
	printf("LED %d ON\n", whichOn);
	if (whichOn == 7) {
		//turn on LED3
		//turn on LED2
		//turn on LED1
	}
	if (whichOn == 6) {
		//turn on LED3
		//turn on LED2
	}
	if (whichOn == 5) {
		//turn on LED3
		//turn on LED1
	}
	if (whichOn == 4) {
		//turn on LED3
	}
	if (whichOn == 3) {
		//turn on LED2
		//turn on LED1
	}
	if (whichOn == 2) {
		//turn on LED2
	}
	if (whichOn == 1) {
		//turn on LED1
	}
}

void ledOff() {
	//printf for debugging outside of board
	//will comment out in board testing
	printf("LEDs OFF\n");

}

//goes down 3-2-1 continually
void powerFailureState() {
	while (1) {
		ledOn(2);
		sleep(1);
		ledOff();

		ledOn(1);
		sleep(1);
		ledOff();

		ledOn(0);
		sleep(1);
		ledOff();
	}
}

// End Katie's PF's

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
