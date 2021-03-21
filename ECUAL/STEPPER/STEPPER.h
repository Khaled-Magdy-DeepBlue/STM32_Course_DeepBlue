/*
 * File: STEPPER.h
 * Driver Name: [[ STEPPER Motor ]]
 * SW Layer:   ECUAL
 * Created on: Jun 28, 2020
 * Ver: 1.0
 * Author:     Khaled Magdy
 * -------------------------------------------
 * For More Information, Tutorials, etc.
 * Visit Website: www.DeepBlueMbedded.com
 *
 */

#ifndef STEPPER_H_
#define STEPPER_H_

// Replace The "#include" File Below With The CPU Relevant One For Your Board
#include "stm32l4xx.h"

// The Number OF Stepper Motors To Be Used In The Project
#define STEPPER_UNITS      1

// Stepper Timer Base Options
#define STEPPER_TIMER_EN   1
#define STEPPER_TIMER      TIM15
#define STEPPER_TIMER_CLK  80
#define STEPPER_TIME_BASE  1

// Stepper Motor Configurations & Mode
#define WAVE_DRIVE        0
#define FULL_STEP_DRIVE   1
#define HALF_STEP_DRIVE   2
#define STEPPER_UNIPOLAR  0
#define STEPPER_BIPOLAR   1
#define DIR_CW            0
#define DIR_CCW           1

// Stepper Configuration Parameter Structure
typedef struct
{
	GPIO_TypeDef * IN_GPIO[4];
	uint16_t       IN_PIN[4];
	uint16_t       STEPS_PER_REV;
	uint8_t        STEPPER_Cfg;
	uint8_t        STEPPING_Mode;
}STEPPER_CfgType;

/*-----[ Prototypes For All Functions ]-----*/

void STEPPERS_Init(void);
void STEPPERS_Init_TMR(TIM_HandleTypeDef* TMR_Handle);
void STEPPER_SetSpeed(uint8_t au8_STEPPER_Instance, uint16_t au16_RPM);
void STEPPER_Step_Blocking(uint8_t au8_STEPPER_Instance, uint32_t au32_Steps, uint8_t au8_DIR);
void STEPPER_Step_NonBlocking(uint8_t au8_STEPPER_Instance, uint32_t au32_Steps, uint8_t au8_DIR);
void STEPPER_Stop(uint8_t au8_STEPPER_Instance);
void STEPPER_Main(void);
void STEPPER_TMR_OVF_ISR(TIM_HandleTypeDef* htim);

#endif /* STEPPER_H_ */
