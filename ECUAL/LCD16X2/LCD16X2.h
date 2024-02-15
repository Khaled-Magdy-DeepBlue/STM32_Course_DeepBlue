/*
 * File: LCD16X2.h
 * Driver Name: [[ LCD16X2 Display (GPIO 4-Bit Mode) ]]
 * SW Layer:   ECUAL
 * Created on: Jun 28, 2020
 * Ver: 1.1
 * Author:     Khaled Magdy
 * -------------------------------------------
 * For More Information, Tutorials, etc.
 * Visit Website: www.DeepBlueMbedded.com
 *
 */

#ifndef LCD16X2_H_
#define LCD16X2_H_

#include "stdint.h"

#define LCD16X2_MAX	1	// Maximum Number of LCD16x2 Modules in Your Project
#define LCD16X2_1	0	// LCD16X2 Instance Number 1 (Add more if you need)


//-----[ Prototypes For All Functions ]-----

extern void LCD16X2_Init(uint8_t);   // Initialize The LCD For 4-Bit Interface
extern void LCD16X2_Clear(uint8_t);  // Clear The LCD Display

extern void LCD16X2_Set_Cursor(uint8_t, unsigned char, unsigned char);  // Set Cursor Position

extern void LCD16X2_Write_Char(uint8_t, char);    // Write Character To LCD At Current Position
extern void LCD16X2_Write_String(uint8_t, char*); // Write A String To LCD

extern void LCD16X2_SL(uint8_t);  // Shift The Entire Display To The Left
extern void LCD16X2_SR(uint8_t);  // Shift The Entire Display To The Right

#endif /* LCD16X2_H_ */
