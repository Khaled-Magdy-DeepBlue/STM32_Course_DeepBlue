/*
 * File: DC_MOTOR_cfg.c
 * Driver Name: [[ DC MOTOR ]]
 * SW Layer:   ECUAL
 * Created on: Jun 28, 2020
 * Author:     Khaled Magdy
 * -------------------------------------------
 * For More Information, Tutorials, etc.
 * Visit Website: www.DeepBlueMbedded.com
 *
 */

#include "../DC_MOTOR/DC_MOTOR.h"

const DC_MOTOR_CfgType DC_MOTOR_CfgParam[DC_MOTOR_UNITS] =
{
	// DC MOTOR 1 Configurations
    {
	    GPIOB,
		GPIOB,
		GPIO_PIN_12,
		GPIO_PIN_13,
		TIM2,
		TIM_CHANNEL_1,
		72,
		DC_MOTOR_F_PWM,
		DC_MOTOR_PWM_RES
	}
};
