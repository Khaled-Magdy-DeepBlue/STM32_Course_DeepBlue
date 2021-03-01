/*
 * File: JOYSTICK_cfg.c
 * Driver Name: [[ JoyStick ]]
 * SW Layer:   ECUAL
 * Created on: Jun 28, 2020
 * Author:     Khaled Magdy
 * -------------------------------------------
 * For More Information, Tutorials, etc.
 * Visit Website: www.DeepBlueMbedded.com
 *
 */
#include "JOYSTICK.h"

const JoyStick_CfgType JoyStick_CfgParam[JOYSTICK_UNITS] =
{
	// JoyStick unit 1 Configurations
    {
	    GPIOA,
		GPIOA,
		GPIO_PIN_6,
		GPIO_PIN_7,
		ADC1,
		ADC_CHANNEL_6,
		ADC_CHANNEL_7
	}
};

