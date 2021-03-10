/*
 * File: HCSR04.c
 * Driver Name: [[ HC-SR04 Ultrasonic Sensor ]]
 * SW Layer:   ECUAL
 * Created on: Jun 28, 2020
 * Author:     Khaled Magdy
 * -------------------------------------------
 * For More Information, Tutorials, etc.
 * Visit Website: www.DeepBlueMbedded.com
 *
 */

#include "../HCSR04/HCSR04.h"

#include "../../util/DWT_Delay.h"
#include "../HCSR04/HCSR04_cfg.h"

typedef struct
{
	uint8_t  EDGE_STATE;
	uint16_t TMR_OVC;
	uint32_t TMR_PSC;
	uint32_t TMR_ARR;
	uint32_t T1;
	uint32_t T2;
	uint32_t DIFF;
	float    DISTANCE;
	TIM_HandleTypeDef* HTIM;
	HAL_TIM_ActiveChannel ACTIV_CH;
}HCSR04_info;

static HCSR04_info gs_HCSR04_info[HCSR04_UNITS] = {0};



void HCSR04_Init(uint8_t au8_HCSR04_Instance, TIM_HandleTypeDef* TMR_Handle)
{
	GPIO_InitTypeDef TRIG_GPIO_InitStruct = {0};
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_IC_InitTypeDef sConfigIC = {0};

    DWT_Delay_Init();

	/*--------[ Configure The HCSR04 TRIGGER GPIO Pin ]-------*/

    if(HCSR04_CfgParam[au8_HCSR04_Instance].TRIG_GPIO == GPIOA)
    {
    	__HAL_RCC_GPIOA_CLK_ENABLE();
    }
    else if(HCSR04_CfgParam[au8_HCSR04_Instance].TRIG_GPIO == GPIOB)
    {
    	__HAL_RCC_GPIOB_CLK_ENABLE();
    }
    else if(HCSR04_CfgParam[au8_HCSR04_Instance].TRIG_GPIO == GPIOC)
    {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    }
    else if(HCSR04_CfgParam[au8_HCSR04_Instance].TRIG_GPIO == GPIOD)
    {
        __HAL_RCC_GPIOD_CLK_ENABLE();
    }
    else if(HCSR04_CfgParam[au8_HCSR04_Instance].TRIG_GPIO == GPIOE)
    {
        __HAL_RCC_GPIOE_CLK_ENABLE();
    }
    TRIG_GPIO_InitStruct.Pin = HCSR04_CfgParam[au8_HCSR04_Instance].TRIG_PIN;
    TRIG_GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    TRIG_GPIO_InitStruct.Pull = GPIO_NOPULL;
    TRIG_GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(HCSR04_CfgParam[au8_HCSR04_Instance].TRIG_GPIO, &TRIG_GPIO_InitStruct);

	/*--------[ Initialize The HCSR04 Static Global Info ]-------*/

	gs_HCSR04_info[au8_HCSR04_Instance].TMR_PSC = HCSR04_CfgParam[au8_HCSR04_Instance].TIM_Instance->PSC;
	gs_HCSR04_info[au8_HCSR04_Instance].TMR_ARR = HCSR04_CfgParam[au8_HCSR04_Instance].TIM_Instance->ARR;
	gs_HCSR04_info[au8_HCSR04_Instance].TMR_OVC = 0;
		gs_HCSR04_info[au8_HCSR04_Instance].HTIM = TMR_Handle;
	if(gs_HCSR04_info[au8_HCSR04_Instance].TMR_ARR == 0)
	{
		gs_HCSR04_info[au8_HCSR04_Instance].TMR_ARR = 65535;
	}
	if(HCSR04_CfgParam[au8_HCSR04_Instance].IC_TIM_CH == TIM_CHANNEL_1)
	{
		gs_HCSR04_info[au8_HCSR04_Instance].ACTIV_CH = HAL_TIM_ACTIVE_CHANNEL_1;
	}
	else if(HCSR04_CfgParam[au8_HCSR04_Instance].IC_TIM_CH == TIM_CHANNEL_2)
	{
		gs_HCSR04_info[au8_HCSR04_Instance].ACTIV_CH = HAL_TIM_ACTIVE_CHANNEL_2;
	}
	else if(HCSR04_CfgParam[au8_HCSR04_Instance].IC_TIM_CH == TIM_CHANNEL_3)
	{
		gs_HCSR04_info[au8_HCSR04_Instance].ACTIV_CH = HAL_TIM_ACTIVE_CHANNEL_3;
	}
	else if(HCSR04_CfgParam[au8_HCSR04_Instance].IC_TIM_CH == TIM_CHANNEL_4)
	{
		gs_HCSR04_info[au8_HCSR04_Instance].ACTIV_CH = HAL_TIM_ACTIVE_CHANNEL_4;
	}

	/*--------[ Configure The HCSR04 IC Timer Channel ]-------*/

	TMR_Handle->Instance = HCSR04_CfgParam[au8_HCSR04_Instance].TIM_Instance;
	TMR_Handle->Init.Prescaler = gs_HCSR04_info[au8_HCSR04_Instance].TMR_PSC;
	TMR_Handle->Init.CounterMode = TIM_COUNTERMODE_UP;
	TMR_Handle->Init.Period = gs_HCSR04_info[au8_HCSR04_Instance].TMR_ARR;
	TMR_Handle->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	TMR_Handle->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	HAL_TIM_Base_Init(TMR_Handle);
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(TMR_Handle, &sClockSourceConfig);
	HAL_TIM_IC_Init(TMR_Handle);
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(TMR_Handle, &sMasterConfig);
	sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
	sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
	sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
	sConfigIC.ICFilter = 0;
	HAL_TIM_IC_ConfigChannel(TMR_Handle, &sConfigIC, HCSR04_CfgParam[au8_HCSR04_Instance].IC_TIM_CH);

	/*--------[ Start The ICU Channel ]-------*/

	HAL_TIM_Base_Start_IT(TMR_Handle);
	HAL_TIM_IC_Start_IT(TMR_Handle, HCSR04_CfgParam[au8_HCSR04_Instance].IC_TIM_CH);
}


void HCSR04_TMR_OVF_ISR(TIM_HandleTypeDef* htim)
{
	uint8_t i = 0;
	for(i=0; i<HCSR04_UNITS; i++)
	{
		if(htim->Instance == HCSR04_CfgParam[i].TIM_Instance)
		{
			gs_HCSR04_info[i].TMR_OVC++;
		}
	}
}


void HCSR04_TMR_IC_ISR(TIM_HandleTypeDef* htim)
{
	uint8_t i = 0;
	uint32_t PS = 0;
	for(i=0; i<HCSR04_UNITS; i++)
	{
		if((htim->Instance == HCSR04_CfgParam[i].TIM_Instance) && (htim->Channel == gs_HCSR04_info[i].ACTIV_CH))
		{
			if (gs_HCSR04_info[i].EDGE_STATE == 0)
			{
				// Capture T1 & Reverse The ICU Edge Polarity
				gs_HCSR04_info[i].T1 = HAL_TIM_ReadCapturedValue(htim, HCSR04_CfgParam[i].IC_TIM_CH);
				gs_HCSR04_info[i].EDGE_STATE = 1;
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, HCSR04_CfgParam[i].IC_TIM_CH, TIM_INPUTCHANNELPOLARITY_FALLING);
				gs_HCSR04_info[i].TMR_OVC = 0;
			}
			else if (gs_HCSR04_info[i].EDGE_STATE == 1)
			{
				// Read The Current ARR & Prescaler Values For The Timer
				PS = HCSR04_CfgParam[i].TIM_Instance->PSC;
				gs_HCSR04_info[i].TMR_ARR = HCSR04_CfgParam[i].TIM_Instance->ARR;
				// Capture T2 & Calculate The Distance
				gs_HCSR04_info[i].T2 = HAL_TIM_ReadCapturedValue(htim, HCSR04_CfgParam[i].IC_TIM_CH);
				gs_HCSR04_info[i].T2 += (gs_HCSR04_info[i].TMR_OVC * (gs_HCSR04_info[i].TMR_ARR+1));
				gs_HCSR04_info[i].DIFF = gs_HCSR04_info[i].T2 - gs_HCSR04_info[i].T1;
				// Write The Distance Value To The Global Struct & Reverse The ICU Edge
				gs_HCSR04_info[i].DISTANCE = (gs_HCSR04_info[i].DIFF * 0.017)/(HCSR04_CfgParam[i].TIM_CLK_MHz/(PS+1));
				gs_HCSR04_info[i].EDGE_STATE = 0;
				__HAL_TIM_SET_CAPTUREPOLARITY(htim, HCSR04_CfgParam[i].IC_TIM_CH, TIM_INPUTCHANNELPOLARITY_RISING);
			}
		}
	}
}

float HCSR04_Read(uint8_t au8_HCSR04_Instance)
{
	return gs_HCSR04_info[au8_HCSR04_Instance].DISTANCE;
}

void HCSR04_Trigger(uint8_t au8_HCSR04_Instance)
{
	HAL_GPIO_WritePin(HCSR04_CfgParam[au8_HCSR04_Instance].TRIG_GPIO, HCSR04_CfgParam[au8_HCSR04_Instance].TRIG_PIN, 1);
	DWT_Delay_us(2);
	HAL_GPIO_WritePin(HCSR04_CfgParam[au8_HCSR04_Instance].TRIG_GPIO, HCSR04_CfgParam[au8_HCSR04_Instance].TRIG_PIN, 0);
}
