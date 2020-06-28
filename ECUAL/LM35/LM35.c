/*
 * File: LM35.c
 * Driver Name: [[ LM35 Temperature Sensor ]]
 * SW Layer:   ECUAL
 * Created on: Jun 28, 2020
 * Author:     Khaled Magdy
 * -------------------------------------------
 * For More Information, Tutorials, etc.
 * Visit Website: www.DeepBlueMbedded.com
 *
 */

#include "LM35.h"
#include "LM35_cfg.h"
#include "../../util/DWT_Delay.h"

static ADC_HandleTypeDef hadc = {0};
static ADC_ChannelConfTypeDef sConfig = {0};
static uint8_t calibrated = 0;

void LM35_Init(uint16_t LM35_Instance)
{
	if(LM35_CfgParam[LM35_Instance].LM35_GPIO == GPIOA)
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();
	}
	else if(LM35_CfgParam[LM35_Instance].LM35_GPIO == GPIOB)
	{
		__HAL_RCC_GPIOB_CLK_ENABLE();
	}
	else if(LM35_CfgParam[LM35_Instance].LM35_GPIO == GPIOC)
	{
		__HAL_RCC_GPIOC_CLK_ENABLE();
	}
	else if (LM35_CfgParam[LM35_Instance].LM35_GPIO == GPIOD)
	{
		__HAL_RCC_GPIOD_CLK_ENABLE();
	}
	hadc.Instance = LM35_CfgParam[LM35_Instance].ADC_Instance;
	hadc.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc.Init.ContinuousConvMode = DISABLE;
	hadc.Init.DiscontinuousConvMode = DISABLE;
	hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc.Init.NbrOfConversion = 1;
	HAL_ADC_Init(&hadc);
    sConfig.Channel = LM35_CfgParam[LM35_Instance].ADC_CH;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES_5;
	HAL_ADC_ConfigChannel(&hadc, &sConfig);

	if(calibrated == 0)
	{
		HAL_ADCEx_Calibration_Start(&hadc);
		calibrated = 1;
	}
	DWT_Delay_Init();
}


float LM35_Read(uint16_t LM35_Instance)
{
	uint32_t AD_RES;
	float Temperature = 0;

	// Select The Sensor Instance ADC Channel
	sConfig.Channel = LM35_CfgParam[LM35_Instance].ADC_CH;
	HAL_ADC_ConfigChannel(&hadc, &sConfig);
	// Start ADC Conversion
	HAL_ADC_Start(&hadc);
    HAL_ADC_PollForConversion(&hadc, 1);
    // Get The Result & Convert To Temp In c_degrees
	AD_RES = HAL_ADC_GetValue(&hadc);
	Temperature = (AD_RES * 0.0805664);

	return Temperature;
}
