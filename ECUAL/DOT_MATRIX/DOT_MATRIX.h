/*
 * File: DOT_MATRIX.h
 * Driver Name: [[ DOT MATRIX MAX7219 ]]
 * SW Layer:   ECUAL
 * Created on: Jun 28, 2020
 * Ver: 1.0
 * Author:     Khaled Magdy
 * -------------------------------------------
 * For More Information, Tutorials, etc.
 * Visit Website: www.DeepBlueMbedded.com
 *
 */

#ifndef DOT_MATRIX_H_
#define DOT_MATRIX_H_

// Replace The "#include" File Below With The CPU Relevant One For Your Board
#include "stm32f1xx.h"

// The Number OF Separate DOT MATRIX Units (Whether Single or Cascaded) To Be Used In The Project
#define DOT_MATRIX_UNITS  1
// The Matrix Units Count Of The Longest Cascaded String In The System
#define MAX_CASCADED_NUM  2

// DOT Matrix SPI Options
#define DOT_MATRIX_SPI    SPI1

// DOT Matrix Timer Base Options
#define MATRIX_TIMER_EN   1
#define MATRIX_TIMER      TIM4
#define MATRIX_TIMER_CLK  72
#define MATRIX_TIME_BASE  1

// DOT Matrix Other Definitions
#define STATIC_MODE       0
#define SCROLL_MODE       1

// DOT MATRIX Configuration Parameter Structure
typedef struct
{
	GPIO_TypeDef * SS_GPIO;
	/*
	 * GPIOA or GPIOB ...
	 * */
	uint16_t       SS_PIN;
	/*
	 * GPIO_PIN_0 or GPIO_PIN_1 ...
	 * */
	uint16_t       SCROLL_SPEED;
	/*
	 * Any Value [0-65535]
	 * The Bigger This Number, The Slower Scrolling Becomes
	 * */
	uint8_t        CASCADED_DEVICES;
	/*
	 * Any Number of Devices [ 1 - 255 ]
	 * */
	uint8_t        BRIGHTNESS;
	/*
	 * Any Value [ 0 -> 15 ]
	 * */
	uint8_t        SCROLL_Mode;
	/*
	 * STATIC_MODE or SCROLL_MODE
	 * */
}DOT_MATRIX_CfgType;

/*-----[ Prototypes For All Functions ]-----*/

// Init Functions Without & With Dedicated Timer
void DOT_MATRIX_Init(SPI_HandleTypeDef * hspi);
void DOT_MATRIX_Init_TMR(SPI_HandleTypeDef * hspi, TIM_HandleTypeDef* TMR_Handle);
// STATOC MODE Functions
void MATRIX_CLEAR(uint8_t au8_MATRIX_Instance);
void MATRIX_Write_Char(uint8_t au8_MATRIX_Instance, uint8_t myChar);
// SCROLL MODE Functions
void DOT_MATRIX_Main(void);
void MATRIX_TMR_OVF_ISR(TIM_HandleTypeDef* htim);
void MATRIX_SCROLL_SetSpeed(uint8_t au8_MATRIX_Instance, uint16_t au16_SPEED);
void MATRIX_DisplayMessage(uint8_t au8_MATRIX_Instance, char* ArrayPointer, uint16_t ArraySize);

#endif /* DOT_MATRIX_H_ */
