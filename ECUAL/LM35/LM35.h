/*
 * File: LM35.h
 * Driver Name: [[ LM35 Temperature Sensor ]]
 * SW Layer:   ECUAL
 * Created on: Jun 28, 2020
 * Ver: 1.0
 * Author:     Khaled Magdy
 * -------------------------------------------
 * For More Information, Tutorials, etc.
 * Visit Website: www.DeepBlueMbedded.com
 *
 */

#ifndef LM35_H_
#define LM35_H_


#define HAL_ADC_MODULE_ENABLED

#include "stm32f1xx_hal.h"

// The Number OF LM35 Sensors To Be Used In The Project
#define SENSORS_NUM  1

typedef struct
{
	GPIO_TypeDef * LM35_GPIO;
	ADC_TypeDef*   ADC_Instance;
	uint32_t       ADC_CH;
}LM35_CfgType;


/*-----[ Prototypes For All Functions ]-----*/
void LM35_Init(uint16_t LM35_Instance);
float LM35_Read(uint16_t LM35_Instance);

#endif /* LM35_H_ */
