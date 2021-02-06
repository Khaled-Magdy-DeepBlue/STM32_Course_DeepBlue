/*
 * File: LEDS_cfg.c
 * Driver Name: [[ LEDs Driver ]]
 * SW Layer:   ECUAL
 * Created on: Jun 28, 2020
 * Author:     Khaled Magdy
 * -------------------------------------------
 * For More Information, Tutorials, etc.
 * Visit Website: www.DeepBlueMbedded.com
 *
 */

#include "LEDS.h"

const LED_CfgType LED_CfgParam[LED_UNITS] =
{
	// LED 1 Configurations
    {
    	GPIOB,
		GPIO_PIN_12
	},
	// LED 2 Configurations
	{
	    GPIOB,
		GPIO_PIN_13
    }
};
