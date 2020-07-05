/*
 * File: SEVEN_SEGMENTS.h
 * Driver Name: [[ 7-Segments Display ]]
 * SW Layer:   ECUAL
 * Created on: Jun 28, 2020
 * Author:     Khaled Magdy
 * -------------------------------------------
 * For More Information, Tutorials, etc.
 * Visit Website: www.DeepBlueMbedded.com
 *
 */

#ifndef SEVEN_SEGMENTS_H_
#define SEVEN_SEGMENTS_H_

#define HAL_GPIO_MODULE_ENABLED

#include "stm32f1xx_hal.h"


// The Number OF 7-Segments Display Units To Be Used In The Project
#define SEVEN_SEG_UNITS  1
#define DIGITS           3
typedef struct
{
	GPIO_TypeDef * SEG_GPIO[7];
	uint16_t       SEG_PIN[7];
	GPIO_TypeDef * EN_GPIO[DIGITS];
	uint16_t       EN_PIN[DIGITS];
}SEVEN_SEG_CfgType;


/*-----[ Prototypes For All Functions ]-----*/

void SEVEN_SEG_Init(uint16_t au16_Instance);
void SEVEN_SEG_Write(uint16_t au16_Instance, uint16_t au16_Data);
void SEVEN_SEG_Enable(uint16_t au16_Instance);
void SEVEN_SEG_Disable(uint16_t au16_Instance);
void SEVEN_SEG_Main(void);

#endif /* SEVEN_SEGMENTS_H_ */
