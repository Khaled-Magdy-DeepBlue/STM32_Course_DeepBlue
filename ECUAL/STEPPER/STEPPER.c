/*
 * File: STEPPER.c
 * Driver Name: [[ STEPPER Motor ]]
 * SW Layer:   ECUAL
 * Created on: Jun 28, 2020
 * Author:     Khaled Magdy
 * -------------------------------------------
 * For More Information, Tutorials, etc.
 * Visit Website: www.DeepBlueMbedded.com
 *
 */

#include "../../util/DWT_Delay.h"
#include "STEPPER.h"
#include "STEPPER_cfg.h"

//---------------------------[ Globals ]---------------------------
typedef struct
{
	uint32_t  Steps;
	uint32_t  Ticks;
	uint32_t  Max_Ticks;
	uint16_t  RPM;
	uint8_t   Blocked;
	uint8_t   Step_Index;
	uint8_t   Max_Index;
	uint8_t   Dir;
}STEPPER_info;

static STEPPER_info gs_STEPPER_info[STEPPER_UNITS] = {0};

static uint8_t UNIPOLAR_WD_PATTERN[4][4] = {
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1}
};
static uint8_t UNIPOLAR_FS_PATTERN[4][4] = {
		{1, 1, 0, 0},
		{0, 1, 1, 0},
		{0, 0, 1, 1},
		{1, 0, 0, 1}
};
static uint8_t UNIPOLAR_HS_PATTERN[8][4] = {
		{1, 0, 0, 0},
		{1, 1, 0, 0},
		{0, 1, 0, 0},
		{0, 1, 1, 0},
		{0, 0, 1, 0},
		{0, 0, 1, 1},
		{0, 0, 0, 1},
		{1, 0, 0, 1}
};

//----------------------------[ Functions' Definitions ]---------------------------

void STEPPERS_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
    uint8_t i = 0, j = 0;

    DWT_Delay_Init();

    /*--------[ Configure The Stepper IN(1-4) GPIO Pins ]-------*/
    for(i = 0; i<STEPPER_UNITS; i++)
    {
    	for(j=0; j<4; j++)
    	{
    		if(STEPPER_CfgParam[i].IN_GPIO[j] == GPIOA)
    		{
    		    __HAL_RCC_GPIOA_CLK_ENABLE();
    		}
    		else if(STEPPER_CfgParam[i].IN_GPIO[j] == GPIOB)
    		{
    		    __HAL_RCC_GPIOB_CLK_ENABLE();
    		}
    		else if(STEPPER_CfgParam[i].IN_GPIO[j] == GPIOC)
    		{
    		    __HAL_RCC_GPIOC_CLK_ENABLE();
    		}/*
    		else if(STEPPER_CfgParam[i].IN_GPIO[j] == GPIOD)
    		{
    		    __HAL_RCC_GPIOD_CLK_ENABLE();
    		}
    		else if(STEPPER_CfgParam[i].IN_GPIO[j] == GPIOE)
    		{
    		    __HAL_RCC_GPIOE_CLK_ENABLE();
    		}*/
    		GPIO_InitStruct.Pin = STEPPER_CfgParam[i].IN_PIN[j];
    		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    		GPIO_InitStruct.Pull = GPIO_NOPULL;
    		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    		HAL_GPIO_Init(STEPPER_CfgParam[i].IN_GPIO[j], &GPIO_InitStruct);
    	}
    	gs_STEPPER_info[i].Dir = DIR_CW;
    	gs_STEPPER_info[i].Step_Index = 0;
    	gs_STEPPER_info[i].Steps = 0;
    	gs_STEPPER_info[i].Ticks = 0;
    	gs_STEPPER_info[i].Max_Ticks = 0;
    	gs_STEPPER_info[i].Blocked = 0;
    	if(STEPPER_CfgParam[i].STEPPING_Mode == FULL_STEP_DRIVE || STEPPER_CfgParam[i].STEPPING_Mode == WAVE_DRIVE)
    	{
    		gs_STEPPER_info[i].Max_Index = 4;
    	}
    	else if(STEPPER_CfgParam[i].STEPPING_Mode == HALF_STEP_DRIVE)
    	{
    		gs_STEPPER_info[i].Max_Index = 8;
    	}
    }
}

void STEPPERS_Init_TMR(TIM_HandleTypeDef* TMR_Handle)
{
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    uint32_t ARR_Value = 0;

	STEPPERS_Init();

	/*--------[ Configure The Stepper Timer Base If Enabled ]-------*/
    if(STEPPER_TIMER_EN == 1)
    {
    	ARR_Value = (STEPPER_TIMER_CLK * 10.0 * STEPPER_TIME_BASE);
    	TMR_Handle->Instance = STEPPER_TIMER;
    	TMR_Handle->Init.Prescaler = 99;
    	TMR_Handle->Init.CounterMode = TIM_COUNTERMODE_UP;
    	TMR_Handle->Init.Period = ARR_Value-1;
    	TMR_Handle->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    	TMR_Handle->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    	HAL_TIM_Base_Init(TMR_Handle);
    	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    	HAL_TIM_ConfigClockSource(TMR_Handle, &sClockSourceConfig);
    	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    	HAL_TIMEx_MasterConfigSynchronization(TMR_Handle, &sMasterConfig);
    	HAL_TIM_Base_Start_IT(TMR_Handle);
    }
}

void STEPPER_SetSpeed(uint8_t au8_STEPPER_Instance, uint16_t au16_RPM)
{
	uint32_t Total_Steps = 0;

	gs_STEPPER_info[au8_STEPPER_Instance].RPM = au16_RPM;
	if(STEPPER_CfgParam[au8_STEPPER_Instance].STEPPING_Mode == HALF_STEP_DRIVE)
	{
		Total_Steps = STEPPER_CfgParam[au8_STEPPER_Instance].STEPS_PER_REV << 1;
	}
	else
	{
		Total_Steps = STEPPER_CfgParam[au8_STEPPER_Instance].STEPS_PER_REV;
	}
	gs_STEPPER_info[au8_STEPPER_Instance].Max_Ticks = (60000.0)/(STEPPER_TIME_BASE * Total_Steps * au16_RPM);
}

static void STEPPER_One_Step(uint8_t i)
{
	// For UniPolar Stepper Motors
	if(STEPPER_CfgParam[i].STEPPER_Cfg == STEPPER_UNIPOLAR)
	{
		if(STEPPER_CfgParam[i].STEPPING_Mode == WAVE_DRIVE)
		{
			HAL_GPIO_WritePin(STEPPER_CfgParam[i].IN_GPIO[0], STEPPER_CfgParam[i].IN_PIN[0], UNIPOLAR_WD_PATTERN[gs_STEPPER_info[i].Step_Index][0]);
			HAL_GPIO_WritePin(STEPPER_CfgParam[i].IN_GPIO[1], STEPPER_CfgParam[i].IN_PIN[1], UNIPOLAR_WD_PATTERN[gs_STEPPER_info[i].Step_Index][1]);
			HAL_GPIO_WritePin(STEPPER_CfgParam[i].IN_GPIO[2], STEPPER_CfgParam[i].IN_PIN[2], UNIPOLAR_WD_PATTERN[gs_STEPPER_info[i].Step_Index][2]);
			HAL_GPIO_WritePin(STEPPER_CfgParam[i].IN_GPIO[3], STEPPER_CfgParam[i].IN_PIN[3], UNIPOLAR_WD_PATTERN[gs_STEPPER_info[i].Step_Index][3]);
		}
		else if(STEPPER_CfgParam[i].STEPPING_Mode == FULL_STEP_DRIVE)
		{
			HAL_GPIO_WritePin(STEPPER_CfgParam[i].IN_GPIO[0], STEPPER_CfgParam[i].IN_PIN[0], UNIPOLAR_FS_PATTERN[gs_STEPPER_info[i].Step_Index][0]);
			HAL_GPIO_WritePin(STEPPER_CfgParam[i].IN_GPIO[1], STEPPER_CfgParam[i].IN_PIN[1], UNIPOLAR_FS_PATTERN[gs_STEPPER_info[i].Step_Index][1]);
			HAL_GPIO_WritePin(STEPPER_CfgParam[i].IN_GPIO[2], STEPPER_CfgParam[i].IN_PIN[2], UNIPOLAR_FS_PATTERN[gs_STEPPER_info[i].Step_Index][2]);
			HAL_GPIO_WritePin(STEPPER_CfgParam[i].IN_GPIO[3], STEPPER_CfgParam[i].IN_PIN[3], UNIPOLAR_FS_PATTERN[gs_STEPPER_info[i].Step_Index][3]);
		}
		else if(STEPPER_CfgParam[i].STEPPING_Mode == HALF_STEP_DRIVE)
		{
			HAL_GPIO_WritePin(STEPPER_CfgParam[i].IN_GPIO[0], STEPPER_CfgParam[i].IN_PIN[0], UNIPOLAR_HS_PATTERN[gs_STEPPER_info[i].Step_Index][0]);
			HAL_GPIO_WritePin(STEPPER_CfgParam[i].IN_GPIO[1], STEPPER_CfgParam[i].IN_PIN[1], UNIPOLAR_HS_PATTERN[gs_STEPPER_info[i].Step_Index][1]);
			HAL_GPIO_WritePin(STEPPER_CfgParam[i].IN_GPIO[2], STEPPER_CfgParam[i].IN_PIN[2], UNIPOLAR_HS_PATTERN[gs_STEPPER_info[i].Step_Index][2]);
			HAL_GPIO_WritePin(STEPPER_CfgParam[i].IN_GPIO[3], STEPPER_CfgParam[i].IN_PIN[3], UNIPOLAR_HS_PATTERN[gs_STEPPER_info[i].Step_Index][3]);
		}
	}
	// For BiPolar Stepper Motors
	else if(STEPPER_CfgParam[i].STEPPER_Cfg == STEPPER_BIPOLAR)
	{

	}
	// Update & Check The Index
	if(gs_STEPPER_info[i].Dir == DIR_CCW)
	{
		if(gs_STEPPER_info[i].Step_Index == 0)
		{
			gs_STEPPER_info[i].Step_Index = gs_STEPPER_info[i].Max_Index;
		}
		gs_STEPPER_info[i].Step_Index--;
	}
	else if(gs_STEPPER_info[i].Dir == DIR_CW)
	{
		gs_STEPPER_info[i].Step_Index++;
		if(gs_STEPPER_info[i].Step_Index == gs_STEPPER_info[i].Max_Index)
		{
			gs_STEPPER_info[i].Step_Index = 0;
		}
	}
}

void STEPPER_Step_Blocking(uint8_t au8_STEPPER_Instance, uint32_t au32_Steps, uint8_t au8_DIR)
{
	uint32_t i = 0;
	uint32_t DelayTimeMs = 0;

	gs_STEPPER_info[au8_STEPPER_Instance].Blocked = 1;
	DelayTimeMs = (60000/(gs_STEPPER_info[au8_STEPPER_Instance].RPM * STEPPER_CfgParam[au8_STEPPER_Instance].STEPS_PER_REV));
	// Send The Control Signals
	for(i=0; i<au32_Steps; i++)
	{
		STEPPER_One_Step(au8_STEPPER_Instance);
		DWT_Delay_ms(DelayTimeMs);
	}
	gs_STEPPER_info[au8_STEPPER_Instance].Blocked = 0;
}

void STEPPER_Step_NonBlocking(uint8_t au8_STEPPER_Instance, uint32_t au32_Steps, uint8_t au8_DIR)
{
	gs_STEPPER_info[au8_STEPPER_Instance].Steps += au32_Steps;
	gs_STEPPER_info[au8_STEPPER_Instance].Dir = au8_DIR;
}

void STEPPER_Stop(uint8_t au8_STEPPER_Instance)
{
	gs_STEPPER_info[au8_STEPPER_Instance].Steps = 0;
}

void STEPPER_Main(void)
{
	uint8_t i = 0;

	for(i=0; i<STEPPER_UNITS; i++)
	{
		if((gs_STEPPER_info[i].Ticks >= gs_STEPPER_info[i].Max_Ticks) && (gs_STEPPER_info[i].Blocked != 1) && (gs_STEPPER_info[i].Steps > 0))
		{
			STEPPER_One_Step(i);
			gs_STEPPER_info[i].Steps--;
			gs_STEPPER_info[i].Ticks = 0;
		}
		else
		{
			gs_STEPPER_info[i].Ticks++;
		}
	}
}

void STEPPER_TMR_OVF_ISR(TIM_HandleTypeDef* htim)
{
	uint8_t i = 0;

	if(htim->Instance == STEPPER_TIMER)
	{
		for(i=0; i<STEPPER_UNITS; i++)
		{
			if((gs_STEPPER_info[i].Ticks >= gs_STEPPER_info[i].Max_Ticks) && (gs_STEPPER_info[i].Blocked != 1) && (gs_STEPPER_info[i].Steps > 0))
			{
				STEPPER_One_Step(i);
				gs_STEPPER_info[i].Steps--;
				gs_STEPPER_info[i].Ticks = 0;
			}
			else
			{
				gs_STEPPER_info[i].Ticks++;
			}
		}
	}
}



