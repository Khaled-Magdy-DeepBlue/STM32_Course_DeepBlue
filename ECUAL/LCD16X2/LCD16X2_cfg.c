/*
 * File: LCD16X2_cfg.c
 * Driver Name: [[ LCD16X2 Display (GPIO 4-Bit Mode) ]]
 * SW Layer:   ECUAL
 * Created on: Jun 28, 2020
 * Author:     Khaled Magdy
 * -------------------------------------------
 * For More Information, Tutorials, etc.
 * Visit Website: www.DeepBlueMbedded.com
 *
 */

#include "LCD16X2_cfg.h"
#include "LCD16X2.h"

const LCD16X2_CfgType LCD16X2_CfgParam[LCD16X2_MAX] =
{
	{   /*  Configuration Parameter For LCD Instance #1   */
		LCD16X2_1,			/* Index of LCD Instance #1   */
		GPIOA,	GPIO_PIN_2, /* LCD D4 Pin GPIO Port & Pin */
		GPIOA,	GPIO_PIN_3, /* LCD D5 Pin GPIO Port & Pin */
		GPIOA,	GPIO_PIN_4, /* LCD D6 Pin GPIO Port & Pin */
		GPIOA,	GPIO_PIN_5, /* LCD D7 Pin GPIO Port & Pin */
		GPIOA,	GPIO_PIN_6, /* LCD EN Pin GPIO Port & Pin */
		GPIOA,	GPIO_PIN_7  /* LCD RS Pin GPIO Port & Pin */
	}
};
