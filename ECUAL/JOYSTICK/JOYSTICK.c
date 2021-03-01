/*
 * File: JOYSTICK.c
 * Driver Name: [[ JoyStick ]]
 * SW Layer:   ECUAL
 * Created on: Jun 28, 2020
 * Author:     Khaled Magdy
 * -------------------------------------------
 * For More Information, Tutorials, etc.
 * Visit Website: www.DeepBlueMbedded.com
 *
 */

#include "JOYSTICK.h"
#include "JOYSTICK_cfg.h"

static ADC_HandleTypeDef hadc[JOYSTICK_UNITS] = {0};
static ADC_ChannelConfTypeDef sConfig = {0};
static uint8_t calibrated = 0;

void JoyStick_Init(uint16_t JoyStick_Instance)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	if(JoyStick_CfgParam[JoyStick_Instance].JoyStick_xGPIO == GPIOA || JoyStick_CfgParam[JoyStick_Instance].JoyStick_yGPIO == GPIOA)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
	}
	else if(JoyStick_CfgParam[JoyStick_Instance].JoyStick_xGPIO == GPIOB || JoyStick_CfgParam[JoyStick_Instance].JoyStick_yGPIO == GPIOB)
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();
	}
	else if(JoyStick_CfgParam[JoyStick_Instance].JoyStick_xGPIO == GPIOC || JoyStick_CfgParam[JoyStick_Instance].JoyStick_yGPIO == GPIOC)
	{
		__HAL_RCC_GPIOC_CLK_ENABLE();
	}
	else if (JoyStick_CfgParam[JoyStick_Instance].JoyStick_xGPIO == GPIOD || JoyStick_CfgParam[JoyStick_Instance].JoyStick_yGPIO == GPIOD)
	{
		__HAL_RCC_GPIOD_CLK_ENABLE();
	}
	else if (JoyStick_CfgParam[JoyStick_Instance].JoyStick_xGPIO == GPIOE || JoyStick_CfgParam[JoyStick_Instance].JoyStick_yGPIO == GPIOE)
	{
		__HAL_RCC_GPIOD_CLK_ENABLE();
	}

	GPIO_InitStruct.Pin = JoyStick_CfgParam[JoyStick_Instance].JoyStick_xPIN;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(JoyStick_CfgParam[JoyStick_Instance].JoyStick_xGPIO, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = JoyStick_CfgParam[JoyStick_Instance].JoyStick_yPIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    HAL_GPIO_Init(JoyStick_CfgParam[JoyStick_Instance].JoyStick_yGPIO, &GPIO_InitStruct);

	hadc[JoyStick_Instance].Instance = JoyStick_CfgParam[JoyStick_Instance].ADC_Instance;
	hadc[JoyStick_Instance].Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc[JoyStick_Instance].Init.ContinuousConvMode = DISABLE;
	hadc[JoyStick_Instance].Init.DiscontinuousConvMode = DISABLE;
	hadc[JoyStick_Instance].Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc[JoyStick_Instance].Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc[JoyStick_Instance].Init.NbrOfConversion = 1;
	HAL_ADC_Init(&hadc[JoyStick_Instance]);
    sConfig.Channel = JoyStick_CfgParam[JoyStick_Instance].ADCx_CH;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES_5;
	HAL_ADC_ConfigChannel(&hadc[JoyStick_Instance], &sConfig);

	if(calibrated == 0)
	{
		HAL_ADCEx_Calibration_Start(&hadc[JoyStick_Instance]);
		calibrated = 1;
	}
}


void JoyStick_Read(uint16_t JoyStick_Instance, uint16_t* JoyStick_XY)
{
	uint32_t AD_RES;

	// Select The JoyStick Instance ADC Channel For X
	sConfig.Channel = JoyStick_CfgParam[JoyStick_Instance].ADCx_CH;
    HAL_ADC_ConfigChannel(&hadc[JoyStick_Instance], &sConfig);
	// Start ADC Conversion
	HAL_ADC_Start(&hadc[JoyStick_Instance]);
	// Poll ADC1 Peripheral & TimeOut = 1mSec
	HAL_ADC_PollForConversion(&hadc[JoyStick_Instance], 1);
	// Read The ADC Conversion Result Write It To JoyStick X
	AD_RES = HAL_ADC_GetValue(&hadc[JoyStick_Instance]);
	JoyStick_XY[0] = AD_RES;

	// Select The JoyStick Instance ADC Channel For Y
	sConfig.Channel = JoyStick_CfgParam[JoyStick_Instance].ADCy_CH;
	HAL_ADC_ConfigChannel(&hadc[JoyStick_Instance], &sConfig);
	// Start ADC Conversion
	HAL_ADC_Start(&hadc[JoyStick_Instance]);
	// Poll ADC1 Peripheral & TimeOut = 1mSec
	HAL_ADC_PollForConversion(&hadc[JoyStick_Instance], 1);
	// Read The ADC Conversion Result Write It To JoyStick Y
	AD_RES = HAL_ADC_GetValue(&hadc[JoyStick_Instance]);
	JoyStick_XY[1] = AD_RES;
}
