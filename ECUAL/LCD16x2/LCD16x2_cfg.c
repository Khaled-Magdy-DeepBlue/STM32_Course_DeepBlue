/*
 * File: LCD16x2_cfg.c
 * Driver Name: [[ LCD16x2 Display (GPIO 4-Bit Mode) ]]
 * SW Layer:   ECUAL
 * Created on: Jun 28, 2020
 * Author:     Khaled Magdy
 * -------------------------------------------
 * For More Information, Tutorials, etc.
 * Visit Website: www.DeepBlueMbedded.com
 *
 */

#include "LCD16x2.h"

const LCD16x2_CfgType LCD16x2_CfgParam =
{
	GPIOB,
	GPIO_PIN_12,
	GPIO_PIN_13,
	GPIO_PIN_14,
	GPIO_PIN_15,
	GPIO_PIN_3,
	GPIO_PIN_4,
	50
};
