/*
 * BUTTONS.h
 *
 *  Created on: Jul 8, 2020
 *      Author: Khaled Magdy
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_

#define HAL_GPIO_MODULE_ENABLED

#include "stm32f1xx_hal.h"


// The Number OF Button Units To Be Used In The Project
#define BTN_UNITS     2

#define BTN_PRESSED   1
#define BTN_RELEASED  0

#define BTNS_FILTER_ORDER 8

typedef struct
{
	GPIO_TypeDef * BTN_GPIO;
	/*
	 * GPIOA
	 * GPIOB
	 * ...
	 * */
	uint16_t       BTN_PIN;
	/*
	 * GPIO_PIN_0
	 * GPIO_PIN_1
	 * ...
	 * */
	uint16_t       PULL_MODE;
	/*
	 * GPIO_NOPULL
	 * GPIO_PULLUP
	 * GPIO_PULLDOWN
	 * */
	uint16_t       Filter_Order;
	/*
	 * 0
	 * 1
	 * ..
	 * N
	 * */
}BTN_CfgType;


/*-----[ Prototypes For All Functions ]-----*/

void BTNs_Init(uint8_t* BTN_States);
void BTN_Main(void);
void BTN_Read(uint16_t au16_Instance, uint8_t* BTN_State);

#endif /* BUTTONS_H_ */
