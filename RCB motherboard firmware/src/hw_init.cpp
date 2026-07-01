/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    hw_init.cpp
  * @brief   This file provides code for hardware initialization
  * @author Petar Crnjak
  ******************************************************************************
  * @attention
  *
  * Copyright (c) Source robotics.
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

#include "hw_init.h"



/// @brief GPIO init with HAL 
/// @param  
void GPIO_init(void)
{

	__HAL_RCC_GPIOC_CLK_ENABLE(); // Enable GPIOC clock
	__HAL_RCC_GPIOA_CLK_ENABLE(); // Enable GPIOA clock
  
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET); // Set PC15 LOW, Disable brake FET
  
  
	// Configure PA8 as output
	GPIO_InitStruct.Pin = GPIO_PIN_8;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
}


// TODO HAL sensor init
/// @brief Init all digital input pins
/// @param  
void Init_Digital_Inputs(void)
{

	pinMode(Button_state, INPUT_PULLDOWN);

	// Make this pins float, CAN not used on stm32
	pinMode(PB8, INPUT);
	pinMode(PB9, INPUT);

	// Not working on V0.0 PCB version, we use raspberry pi for this
	/*
	pinMode(ESTOP_SIGNAL2, INPUT);
	pinMode(ESTOP_SIGNAL, INPUT);
	pinMode(ESTOP_MCU_onboard, INPUT);
	pinMode(Button_state, INPUT);
	//attachInterrupt(digitalPinToInterrupt(FAULT),DRV_ERROR_INTERRUPT,FALLING);

	attachInterrupt(digitalPinToInterrupt(ESTOP_SIGNAL2),ESTOP_2_INTERRUPT,CHANGE);
	attachInterrupt(digitalPinToInterrupt(ESTOP_SIGNAL),ESTOP_1_INTERRUPT,CHANGE);
	attachInterrupt(digitalPinToInterrupt(ESTOP_MCU_onboard),ESTOP_MCU_INTERRUPT,CHANGE);
	*/
	
}

/// @brief  Not used in V0.0 PCB version
void ESTOP_1_INTERRUPT(){
	__NOP();
	//controller.additional1_var = digitalReadFast(ADDITIONAL1);
}

/// @brief  Not used in V0.0 PCB version  
void ESTOP_2_INTERRUPT(){
	__NOP();
	//controller.additional1_var = digitalReadFast(ADDITIONAL1);
}

/// @brief  Not used in V0.0 PCB version
void ESTOP_MCU_INTERRUPT(){
	__NOP();
	//controller.additional1_var = digitalReadFast(ADDITIONAL1);
}


/// @brief Init all digital output pins
void Init_Digital_Outputs(void)
{
	pinMode(EN_BRAKE_FET, OUTPUT);
	pinMode(Pi_enable, OUTPUT);
	pinMode(RELAY_PIN, OUTPUT);

	pinMode(LED1, OUTPUT);

	pinMode(Supply_on_off, OUTPUT);

	pinMode(PRECHARGE_PIN, OUTPUT);
	pinMode(PWM_BRAKE, OUTPUT);
	pinMode(EEPROM_WP, OUTPUT);

}

/// @brief System clock config
void SystemClock_Config(void)
{

	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;

	/**Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	/**Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	HAL_RCC_MCOConfig(RCC_MCO, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1);

	/**Configure the Systick interrupt time
	 */
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

	/**Configure the Systick
	 */
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/// @brief  Init system clock
/// @param multiplier clock multiplier
/// Default clock multiplier is 16 for 128Mhz clock speed
void TM_SystemClock_Config(int multiplier)
{

	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;

	// revert to HSE source
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE; // <- HSE
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	// Setup PLL with new multiplier
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

	// set clock multiplier
	if (multiplier == 9)
	{
		RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	}
	else if (multiplier == 10)
	{
		RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL10;
	}
	else if (multiplier == 11)
	{
		RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL11;
	}
	else if (multiplier == 12)
	{
		RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
	}
	else if (multiplier == 13)
	{
		RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL13;
	}
	else if (multiplier == 14)
	{
		RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL14;
	}
	else if (multiplier == 15)
	{
		RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL15;
	}
	else if (multiplier == 16)
	{
		RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
	}
	else
	{ // if wrong number use stock speed
		RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	}

	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	// Go back to PLL source
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK; // <- PLL welcome back
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}

	/**Configure the Systick interrupt time
	 */
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

	/**Configure the Systick
	 */
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}
