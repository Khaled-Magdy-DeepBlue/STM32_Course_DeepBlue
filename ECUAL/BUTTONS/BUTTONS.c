/*
 * File: BUTTONS.c
 * Driver Name: [[ BUTTONS Driver ]]
 * SW Layer:   ECUAL
 * Created on: Jun 28, 2020
 * Author:     Khaled Magdy
 * -------------------------------------------
 * For More Information, Tutorials, etc.
 * Visit Website: www.DeepBlueMbedded.com
 *
 */


#include "BUTTONS.h"
#include "BUTTONS_cfg.h"
#include "../../MATH/FIR/FIR.h"

FIR_Filter_cfg BTNS_Filters[BTN_UNITS] = {0};
uint8_t* gu8_BTN_States;


void BTNs_Init(uint8_t* BTN_States)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	uint8_t i = 0;

	for(i=0; i<BTN_UNITS; i++)
	{
		/*--------[ Configure The LED GPIO Pin ]-------*/
		if(BTN_CfgParam[i].BTN_GPIO == GPIOA)
		{
			__HAL_RCC_GPIOA_CLK_ENABLE();
		}
		else if(BTN_CfgParam[i].BTN_GPIO == GPIOB)
		{
			__HAL_RCC_GPIOB_CLK_ENABLE();
		}
		else if(BTN_CfgParam[i].BTN_GPIO == GPIOC)
		{
			__HAL_RCC_GPIOC_CLK_ENABLE();
		}
		else if(BTN_CfgParam[i].BTN_GPIO == GPIOD)
		{
			__HAL_RCC_GPIOD_CLK_ENABLE();
		}
		else if(BTN_CfgParam[i].BTN_GPIO == GPIOE)
		{
			__HAL_RCC_GPIOE_CLK_ENABLE();
		}
		GPIO_InitStruct.Pin = BTN_CfgParam[i].BTN_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = BTN_CfgParam[i].PULL_MODE;
		HAL_GPIO_Init(BTN_CfgParam[i].BTN_GPIO, &GPIO_InitStruct);

		/* Configure The Buttons Filter */
		BTNS_Filters[i].Filter_Order = BTN_CfgParam[i].Filter_Order;
		BTNS_Filters[i].Data_Buffer = Filters_Buffers[i];
		BTNS_Filters[i].Buffer_Index = 0;
	}

	/* Init The Buttons States Buffer */
	gu8_BTN_States = BTN_States;
}


void BTN_Read(uint16_t au16_Instance, uint8_t* BTN_State)
{
	*BTN_State = HAL_GPIO_ReadPin(BTN_CfgParam[au16_Instance].BTN_GPIO, BTN_CfgParam[au16_Instance].BTN_PIN);
}


void BTN_Main(void)
{
	uint8_t i = 0;
	uint8_t Read_State = 0;
	uint8_t FilterSum = 0;
	uint8_t Filtered_State = 0;

	for(i=0; i <BTN_UNITS; i++)
	{
		/* Read The Button's Pin State*/
		Read_State = HAL_GPIO_ReadPin(BTN_CfgParam[i].BTN_GPIO, BTN_CfgParam[i].BTN_PIN);
		/* Call The SUM Filter Function */
		SUM_BUFFER(Read_State, &FilterSum, &BTNS_Filters[i]);
		if(FilterSum == BTN_CfgParam[i].Filter_Order+1)
		{
			Filtered_State = BTN_PRESSED;
			/* Return The Filtered Output State In The Buffer*/
			gu8_BTN_States[i] = Filtered_State;
		}
		if(FilterSum == 0)
		{
			Filtered_State = BTN_RELEASED;
			/* Return The Filtered Output State In The Buffer*/
			gu8_BTN_States[i] = Filtered_State;
		}
	}
}
