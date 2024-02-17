/*
 * File: I2C_LCD_cfg.h
 * Driver Name: [[ I2C_LCD Display ]]
 * SW Layer:   ECUAL
 * Created on: Jan 28, 2024
 * Author:     Khaled Magdy
 * -------------------------------------------
 * For More Information, Tutorials, etc.
 * Visit Website: www.DeepBlueMbedded.com
 *
 */

#ifndef I2C_LCD_CFG_H_
#define I2C_LCD_CFG_H_

#include "main.h"
#include "I2C_LCD.h"

typedef struct
{
	// I2C LCD Module Instance Index
	uint8_t I2C_LCD_Instance;
	
	// I2C Hardware Peripheral Handle
	I2C_HandleTypeDef* I2C_Handle;
	
	// I2C LCD Hardware Device Address
	uint8_t I2C_LCD_Address;
	
	// I2C LCD Columns Count
	uint8_t I2C_LCD_nCol;
	
	// I2C LCD Rows Count
	uint8_t I2C_LCD_nRow;

}I2C_LCD_CfgType;

extern const I2C_LCD_CfgType I2C_LCD_CfgParam[I2C_LCD_MAX];

#endif /* I2C_LCD_CFG_H_ */
