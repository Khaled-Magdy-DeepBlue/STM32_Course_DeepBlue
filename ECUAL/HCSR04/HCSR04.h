/*
 * File: HCSR04.h
 * Driver Name: [[ HC-SR04 Ultrasonic Sensor ]]
 * SW Layer:   ECUAL
 * Created on: Jun 28, 2020
 * Ver: 1.0
 * Author:     Khaled Magdy
 * -------------------------------------------
 * For More Information, Tutorials, etc.
 * Visit Website: www.DeepBlueMbedded.com
 *
 */

#ifndef HCSR04_H_
#define HCSR04_H_

#define HAL_TIM_MODULE_ENABLED

#include "stm32f1xx_hal.h"


// The Number OF HC-SR04 Ultrasonic Sensors To Be Used In The Project
#define HCSR04_UNITS  1

typedef struct
{
	GPIO_TypeDef * TRIG_GPIO;
	uint16_t       TRIG_PIN;
	TIM_TypeDef*   TIM_Instance;
	uint32_t       IC_TIM_CH;
	uint32_t       TIM_CLK_MHz;
}HCSR04_CfgType;


/*-----[ Prototypes For All Functions ]-----*/

void HCSR04_Init(uint8_t au8_HCSR04_Instance, TIM_HandleTypeDef* TMR_Handle);
void HCSR04_Trigger(uint8_t au8_HCSR04_Instance);
void HCSR04_TMR_OVF_ISR(TIM_HandleTypeDef* htim);
void HCSR04_TMR_IC_ISR(TIM_HandleTypeDef* htim);
float HCSR04_Read(uint8_t au8_HCSR04_Instance);

#endif /* HCSR04_H_ */
