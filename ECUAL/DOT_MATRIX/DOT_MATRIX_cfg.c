/*
 * File: DOT_MATRIX_cfg.c
 * Driver Name: [[ DOT MATRIX MAX7219 ]]
 * SW Layer:   ECUAL
 * Created on: Jun 28, 2020
 * Author:     Khaled Magdy
 * -------------------------------------------
 * For More Information, Tutorials, etc.
 * Visit Website: www.DeepBlueMbedded.com
 *
 */

#include "DOT_MATRIX.h"

const DOT_MATRIX_CfgType DOT_MATRIX_CfgParam[DOT_MATRIX_UNITS] =
{
	// DOT_MATRIX Display Unit1 Configurations
    {
		GPIOB,
		GPIO_PIN_0,
		75,   /* Scroll Speed*/
		2,    /* Number Of Cascaded Devices*/
		8,    /* Brightness Level */
		SCROLL_MODE
	}
};
