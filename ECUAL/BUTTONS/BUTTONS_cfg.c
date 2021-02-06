/*
 * BUTTONS_cfg.c
 *
 *  Created on: Jul 8, 2020
 *      Author: Khaled Magdy
 */

#include "BUTTONS.h"

const BTN_CfgType BTN_CfgParam[BTN_UNITS] =
{
	// Button 1 Configurations
    {
    	GPIOA,
		GPIO_PIN_6,
		GPIO_NOPULL,
		BTNS_FILTER_ORDER
	},
	// Button 2 Configurations
	{
	    GPIOA,
		GPIO_PIN_7,
		GPIO_NOPULL,
		BTNS_FILTER_ORDER
    }
};

uint8_t Filters_Buffers[BTN_UNITS][BTNS_FILTER_ORDER+1] = {0};
