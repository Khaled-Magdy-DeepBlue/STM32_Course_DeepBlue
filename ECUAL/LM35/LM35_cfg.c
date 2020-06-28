/*
 * File: LM35_cfg.c
 * Driver Name: [[ LM35 Temperature Sensor ]]
 * SW Layer:   ECUAL
 * Created on: Jun 28, 2020
 * Author:     Khaled Magdy
 * -------------------------------------------
 * For More Information, Tutorials, etc.
 * Visit Website: www.DeepBlueMbedded.com
 *
 */

#include "LM35.h"

const LM35_CfgType LM35_CfgParam[SENSORS_NUM] =
{
	// LM35 Sensor 1 Configurations
    {
	    GPIOA,
		ADC1,
		ADC_CHANNEL_7
	}
};
