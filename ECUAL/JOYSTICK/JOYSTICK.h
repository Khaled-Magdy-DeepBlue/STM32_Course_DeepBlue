/*
 * File: JOYSTICK.h
 * Driver Name: [[ JoyStick ]]
 * SW Layer:   ECUAL
 * Created on: Jun 28, 2020
 * Ver: 1.0
 * Author:     Khaled Magdy
 * -------------------------------------------
 * For More Information, Tutorials, etc.
 * Visit Website: www.DeepBlueMbedded.com
 *
 */

#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#define HAL_ADC_MODULE_ENABLED

#include "stm32f1xx_hal.h"

// The Number OF JoySticks To Be Used In The Project
#define JOYSTICK_UNITS  1

typedef struct
{
	GPIO_TypeDef * JoyStick_xGPIO;
	GPIO_TypeDef * JoyStick_yGPIO;
	uint16_t       JoyStick_xPIN;
	uint16_t       JoyStick_yPIN;
	ADC_TypeDef*   ADC_Instance;
	uint32_t       ADCx_CH;
	uint32_t       ADCy_CH;
}JoyStick_CfgType;


/*-----[ Prototypes For All Functions ]-----*/
void JoyStick_Init(uint16_t JoyStick_Instance);
void JoyStick_Read(uint16_t JoyStick_Instance, uint16_t* JoyStick_XY);

#endif /* JOYSTICK_H_ */
