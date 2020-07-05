/*
 * File: SEVEN_SEGMENTS.c
 * Driver Name: [[ 7-Segments Display ]]
 * SW Layer:   ECUAL
 * Created on: Jun 28, 2020
 * Author:     Khaled Magdy
 * -------------------------------------------
 * For More Information, Tutorials, etc.
 * Visit Website: www.DeepBlueMbedded.com
 *
 */

#include "SEVEN_SEGMENTS.h"
#include "SEVEN_SEGMENTS_cfg.h"


#define MAX_DIGITS  4
#define ENABLED     1
#define DISABLED    0

typedef struct
{
	uint8_t  EN_State;
	uint8_t  Digits[MAX_DIGITS];
	uint8_t  Digit_Index;
}SEVENSEG_info;

static SEVENSEG_info gs_SEVENSEG_info[SEVEN_SEG_UNITS] = {0};

volatile static uint8_t gau8_SevGegPattern[10][7] =
{
  /* a  b  c  d  e  f  g  */
	{1, 1, 1, 1, 1, 1, 0},     /* Zero */
	{0, 1, 1, 0, 0, 0, 0},     /* One */
	{1, 1, 0, 1, 1, 0, 1},     /* Two */
	{1, 1, 1, 1, 0, 0, 1},     /* Three */
	{0, 1, 1, 0, 0, 1, 1},     /* Four */
	{1, 0, 1, 1, 0, 1, 1},     /* Five */
	{1, 0, 1, 1, 1, 1, 1},     /* Six */
	{1, 1, 1, 0, 0, 0, 0},     /* Seven */
	{1, 1, 1, 1, 1, 1, 1},     /* Eight */
	{1, 1, 1, 1, 0, 1, 1}      /* Nine */
};




void SEVEN_SEG_Init(uint16_t au16_Instance)
{
	uint8_t i = 0;
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/*--------[ Configure The 7-Segments GPIO Pins ]-------*/
	for(i=0; i<7; i++)
	{
		if(SEVEN_SEG_CfgParam[au16_Instance].SEG_GPIO[i] == GPIOA)
	    {
	        __HAL_RCC_GPIOA_CLK_ENABLE();
		}
		else if(SEVEN_SEG_CfgParam[au16_Instance].SEG_GPIO[i] == GPIOB)
		{
			__HAL_RCC_GPIOB_CLK_ENABLE();
		}
		else if(SEVEN_SEG_CfgParam[au16_Instance].SEG_GPIO[i] == GPIOC)
		{
			__HAL_RCC_GPIOC_CLK_ENABLE();
		}
		else if(SEVEN_SEG_CfgParam[au16_Instance].SEG_GPIO[i] == GPIOD)
		{
			__HAL_RCC_GPIOD_CLK_ENABLE();
		}
		else if(SEVEN_SEG_CfgParam[au16_Instance].SEG_GPIO[i] == GPIOE)
		{
			__HAL_RCC_GPIOE_CLK_ENABLE();
		}
		GPIO_InitStruct.Pin = SEVEN_SEG_CfgParam[au16_Instance].SEG_PIN[i];
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
		HAL_GPIO_Init(SEVEN_SEG_CfgParam[au16_Instance].SEG_GPIO[i], &GPIO_InitStruct);
	}
	/*--------[ Configure The 7-Segments Enable Signals GPIO Pins ]-------*/
	for(i=0; i<DIGITS; i++)
	{
		if(SEVEN_SEG_CfgParam[au16_Instance].EN_GPIO[i] == GPIOA)
		{
		    __HAL_RCC_GPIOA_CLK_ENABLE();
		}
		else if(SEVEN_SEG_CfgParam[au16_Instance].EN_GPIO[i] == GPIOB)
		{
			__HAL_RCC_GPIOB_CLK_ENABLE();
		}
		else if(SEVEN_SEG_CfgParam[au16_Instance].EN_GPIO[i] == GPIOC)
		{
			__HAL_RCC_GPIOC_CLK_ENABLE();
		}
		else if(SEVEN_SEG_CfgParam[au16_Instance].EN_GPIO[i] == GPIOD)
		{
			__HAL_RCC_GPIOD_CLK_ENABLE();
		}
		else if(SEVEN_SEG_CfgParam[au16_Instance].EN_GPIO[i] == GPIOE)
		{
			__HAL_RCC_GPIOE_CLK_ENABLE();
		}
		GPIO_InitStruct.Pin = SEVEN_SEG_CfgParam[au16_Instance].EN_PIN[i];
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
		HAL_GPIO_Init(SEVEN_SEG_CfgParam[au16_Instance].EN_GPIO[i], &GPIO_InitStruct);
	}
}

void SEVEN_SEG_Write(uint16_t au16_Instance, uint16_t au16_Data)
{
	uint8_t i = 0;

	for(i = 0; i<MAX_DIGITS; i++)
	{
		gs_SEVENSEG_info[au16_Instance].Digits[i] = (au16_Data%10);
		au16_Data /= 10;
	}
	gs_SEVENSEG_info[au16_Instance].Digit_Index = 0;
}

void SEVEN_SEG_Enable(uint16_t au16_Instance)
{
	gs_SEVENSEG_info[au16_Instance].EN_State = ENABLED;
}

void SEVEN_SEG_Disable(uint16_t au16_Instance)
{
	gs_SEVENSEG_info[au16_Instance].EN_State = DISABLED;
}



void SEVEN_SEG_Main(void)
{
	uint8_t i = 0, j = 0;
	uint8_t Digit_Index = 0;
	uint8_t Digit_Value = 0;

	for(i=0; i<SEVEN_SEG_UNITS; i++)
	{
		/* If This 7-Seg Display Unit Is Enabled*/
		if(gs_SEVENSEG_info[i].EN_State == ENABLED)
		{
			/* Check The Index Of The Digit To Be Displayed Now */
			Digit_Index = gs_SEVENSEG_info[i].Digit_Index;
			/* Disable The EN Lines Momentarily To Reduce Ghosting Effect */
			for (j=0; j<DIGITS; j++)
			{
				HAL_GPIO_WritePin(SEVEN_SEG_CfgParam[i].EN_GPIO[j], SEVEN_SEG_CfgParam[i].EN_PIN[j], 0);
			}
			/* Get & Move The Digit Value's Pattern To The 7-Seg Pins*/
			Digit_Value = gs_SEVENSEG_info[i].Digits[Digit_Index];
			for(j=0; j<7; j++)
			{
				if(gau8_SevGegPattern[Digit_Value][j] == 1)
				{
					HAL_GPIO_WritePin(SEVEN_SEG_CfgParam[i].SEG_GPIO[j], SEVEN_SEG_CfgParam[i].SEG_PIN[j], 1);
				}
				else
				{
					HAL_GPIO_WritePin(SEVEN_SEG_CfgParam[i].SEG_GPIO[j], SEVEN_SEG_CfgParam[i].SEG_PIN[j], 0);
				}
			}
			/* Now, Enable The Respective EN Line */
			HAL_GPIO_WritePin(SEVEN_SEG_CfgParam[i].EN_GPIO[Digit_Index], SEVEN_SEG_CfgParam[i].EN_PIN[Digit_Index], 1);
			/* Increment The Index Of The Digit To Be Displayed In The Next Time.. */
			gs_SEVENSEG_info[i].Digit_Index++;
			if(gs_SEVENSEG_info[i].Digit_Index == DIGITS+1)
			{
				gs_SEVENSEG_info[i].Digit_Index = 0;
			}
		}
		/* Else If This 7-Seg Display Unit Is Disabled */
		else if(gs_SEVENSEG_info[i].EN_State == DISABLED)
		{
			/* Drive All The EN Pins Low For This 7-Seg Unit */
			for (j=0; j<DIGITS; j++)
			{
				HAL_GPIO_WritePin(SEVEN_SEG_CfgParam[i].EN_GPIO[j], SEVEN_SEG_CfgParam[i].EN_PIN[j], 0);
			}
		}
	}
}
