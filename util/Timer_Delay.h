/*
 * File: Timer_Delay.h
 * Driver Name: [[ Timer Delay ]]
 * SW Layer:   ECUAL
 * Created on: Jun 28, 2020
 * Author:     Khaled Magdy
 * -------------------------------------------
 * For More Information, Tutorials, etc.
 * Visit Website: www.DeepBlueMbedded.com
 *
 */

#ifndef TIMER_DELAY_H_
#define TIMER_DELAY_H_

#define HAL_TIM_MODULE_ENABLED

#include "stm32f1xx_hal.h"


void TimerDelay_Init(void);
void delay_us(uint16_t au16_us);
void delay_ms(uint16_t au16_ms);

#endif /* TIMER_DELAY_H_ */
