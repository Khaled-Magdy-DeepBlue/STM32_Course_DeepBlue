/*
 * File: LEDS.h
 * Driver Name: [[ LEDs Driver ]]
 * SW Layer:   ECUAL
 * Created on: Jun 28, 2020
 * Author:     Khaled Magdy
 * -------------------------------------------
 * For More Information, Tutorials, etc.
 * Visit Website: www.DeepBlueMbedded.com
 *
 */

#ifndef LEDS_H_
#define LEDS_H_

#define HAL_GPIO_MODULE_ENABLED

#include "stm32f1xx_hal.h"


// The Number OF LED Units To Be Used In The Project
#define LED_UNITS  2


typedef struct
{
	GPIO_TypeDef * LED_GPIO;
	/*
	 * GPIOA
	 * GPIOB
	 * ...
	 * */
	uint16_t       LED_PIN;
	/*
	 * GPIO_PIN_0
	 * GPIO_PIN_1
	 * ...
	 * */
}LED_CfgType;


/*-----[ Prototypes For All Functions ]-----*/

void LEDs_Init(void);
void LED_ON(uint16_t au16_Instance);
void LED_OFF(uint16_t au16_Instance);

#endif /* LEDS_H_ */
