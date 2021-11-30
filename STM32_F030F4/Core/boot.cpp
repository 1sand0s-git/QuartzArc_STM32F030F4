/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F030F4 Demo Board                                               */
/*                                                                         */
/*   System: Core                                                          */
/*   Role: System Initialization                                           */
/*   Filename: boot.cpp                                                    */
/*   Date: 30th November 2021                                              */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Includes
#include "boot.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

  //------------------------------
  //System Initialization Function


//SystemInitialize
//System Initialize Function
//
//Used to initialize CPU caches, NVIC and SysTick, as well as oscillators, PLLs, system clocks, bus clocks and some peripheral clocks
//Also enables all GPIO ports, and both DMA1 and DMA2 peripherals
//
//Returns QA_OK if successful, or QA_Fail if initialization failed
QA_Result SystemInitialize(void) {

	//----------------------
	//Enable Prefetch Buffer
	__HAL_FLASH_PREFETCH_BUFFER_ENABLE();


	//--------------------------
	//Set NVIC Priority Grouping
	//HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);


	//------------
	//Init SysTick
	if (HAL_InitTick(TICK_INT_PRIORITY) != HAL_OK) {
		return QA_Fail;
	}


	//-------------------------
	//Setup Power Configuration
	__HAL_RCC_PWR_CLK_ENABLE();
	//__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULARTOR_VOLTAGE_SCALE1);


	//---------------------
	//Configure Oscillators
	//
	//Configure High Speed External (HSE) oscillator to be used, and setup primary PLL to provide 48MHz clock
	//NOTE: HSE Oscillator on STM32F030F4 Demo Board is supplied by an 8MHz crystal
	RCC_OscInitTypeDef RCC_OscInit = {0};
	RCC_OscInit.OscillatorType        = RCC_OSCILLATORTYPE_HSE |
			                                RCC_OSCILLATORTYPE_HSI |
															        RCC_OSCILLATORTYPE_HSI14;  //Define which oscillators are to be configured
	RCC_OscInit.HSEState              = RCC_HSE_ON;
	RCC_OscInit.HSIState              = RCC_HSI_ON;
	RCC_OscInit.HSICalibrationValue   = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInit.HSI14State            = RCC_HSI14_ON;
	RCC_OscInit.HSI14CalibrationValue = RCC_HSI14CALIBRATION_DEFAULT;
  RCC_OscInit.PLL.PLLState          = RCC_PLL_ON;
  RCC_OscInit.PLL.PLLSource         = RCC_PLLSOURCE_HSI;
  RCC_OscInit.PLL.PLLMUL            = RCC_PLL_MUL12;
  RCC_OscInit.PLL.PREDIV            = RCC_PREDIV_DIV1;

  if (HAL_RCC_OscConfig(&RCC_OscInit) != HAL_OK) {
  	return QA_Fail;
  }


  //-------------------
  //Enable SysCfg Clock
  __HAL_RCC_SYSCFG_CLK_ENABLE();


  //--------------------------
  //Configure CPU & Bus Clocks
  RCC_ClkInitTypeDef RCC_ClkInit = {0};
  RCC_ClkInit.ClockType      = RCC_CLOCKTYPE_HCLK |
  		                         RCC_CLOCKTYPE_SYSCLK |
													     RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInit.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInit.AHBCLKDivider  = RCC_SYSCLK_DIV1;
  RCC_ClkInit.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInit, FLASH_LATENCY_1) != HAL_OK) {
  	return QA_Fail;
  }


  //------------------
  //Enable GPIO Clocks
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();


  //Return
  return QA_OK;
}


