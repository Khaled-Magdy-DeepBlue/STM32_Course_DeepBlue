/*
 * File: LEDS.c
 * Driver Name: [[ LEDs Driver ]]
 * SW Layer:   ECUAL
 * Created on: Jun 28, 2020
 * Author:     Khaled Magdy
 * -------------------------------------------
 * For More Information, Tutorials, etc.
 * Visit Website: www.DeepBlueMbedded.com
 *
 */


#include "LEDS.h"
#include "LEDS_cfg.h"



void LEDs_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	uint8_t i = 0;

	for(i=0; i<LED_UNITS; i++)
	{
		/*--------[ Configure The LED GPIO Pin ]-------*/
		if(LED_CfgParam[i].LED_GPIO == GPIOA)
	   {
		    __HAL_RCC_GPIOA_CLK_ENABLE();
		}
		else if(LED_CfgParam[i].LED_GPIO == GPIOB)
		{
			__HAL_RCC_GPIOB_CLK_ENABLE();
		}
		else if(LED_CfgParam[i].LED_GPIO == GPIOC)
		{
			__HAL_RCC_GPIOC_CLK_ENABLE();
		}
		else if(LED_CfgParam[i].LED_GPIO == GPIOD)
		{
			__HAL_RCC_GPIOD_CLK_ENABLE();
		}
		else if(LED_CfgParam[i].LED_GPIO == GPIOE)
		{
			__HAL_RCC_GPIOE_CLK_ENABLE();
		}
		GPIO_InitStruct.Pin = LED_CfgParam[i].LED_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
		HAL_GPIO_Init(LED_CfgParam[i].LED_GPIO, &GPIO_InitStruct);
	}
}

void LED_ON(uint16_t au16_Instance)
{
	HAL_GPIO_WritePin(LED_CfgParam[au16_Instance].LED_GPIO, LED_CfgParam[au16_Instance].LED_PIN, 1);
}

void LED_OFF(uint16_t au16_Instance)
{
	HAL_GPIO_WritePin(LED_CfgParam[au16_Instance].LED_GPIO, LED_CfgParam[au16_Instance].LED_PIN, 0);
}
