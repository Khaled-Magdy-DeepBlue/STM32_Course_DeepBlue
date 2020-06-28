/*
 * File: Timer_Delay.c
 * Driver Name: [[ Timer Delay ]]
 * SW Layer:   ECUAL
 * Created on: Jun 28, 2020
 * Author:     Khaled Magdy
 * -------------------------------------------
 * For More Information, Tutorials, etc.
 * Visit Website: www.DeepBlueMbedded.com
 *
 */

#include "Timer_Delay.h"

#define TIMER  TIM4
volatile static TIM_HandleTypeDef HTIMx;
volatile static uint32_t gu32_ticks = 0;

void TimerDelay_Init(void)
{
	gu32_ticks = (HAL_RCC_GetHCLKFreq() / 1000000);
	HTIMx.Instance = TIMER;

    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};


    HTIMx.Init.Prescaler = gu32_ticks-1;
    HTIMx.Init.CounterMode = TIM_COUNTERMODE_UP;
    HTIMx.Init.Period = 65535;
    HTIMx.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HTIMx.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    if (HAL_TIM_Base_Init(&HTIMx) != HAL_OK)
    {
      Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(&HTIMx, &sClockSourceConfig) != HAL_OK)
    {
      Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(&HTIMx, &sMasterConfig) != HAL_OK)
    {
      Error_Handler();
    }

    HAL_TIM_Base_Start(&HTIMx);

}

void delay_us(volatile uint16_t au16_us)
{
	HTIMx.Instance->CNT = 0;
	while (HTIMx.Instance->CNT < au16_us);
}

void delay_ms(volatile uint16_t au16_ms)
{
	while(au16_ms > 0)
	{
		HTIMx.Instance->CNT = 0;
		au16_ms--;
		while (HTIMx.Instance->CNT < 1000);
	}
}

