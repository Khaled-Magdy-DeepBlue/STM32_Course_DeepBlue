/*
 * File: LCD16X2.c
 * Driver Name: [[ LCD16X2 Display (GPIO 4-Bit Mode) ]]
 * SW Layer:   ECUAL
 * Created on: Jun 28, 2020
 * Author:     Khaled Magdy
 * -------------------------------------------
 * For More Information, Tutorials, etc.
 * Visit Website: www.DeepBlueMbedded.com
 *
 */

#include "LCD16X2.h"
#include "LCD16X2_cfg.h"
#include "../../Util/Util.h"

//-----[ Alphanumeric LCD16X2 Functions ]-----

void LCD16X2_DATA(uint8_t LCD16X2_Index, unsigned char Data)
{
    if(Data & 1)
    	HAL_GPIO_WritePin(LCD16X2_CfgParam[LCD16X2_Index].D4_GPIOx, LCD16X2_CfgParam[LCD16X2_Index].D4_PINx, 1);
    else
    	HAL_GPIO_WritePin(LCD16X2_CfgParam[LCD16X2_Index].D4_GPIOx, LCD16X2_CfgParam[LCD16X2_Index].D4_PINx, 0);
    if(Data & 2)
    	HAL_GPIO_WritePin(LCD16X2_CfgParam[LCD16X2_Index].D5_GPIOx, LCD16X2_CfgParam[LCD16X2_Index].D5_PINx, 1);
    else
    	HAL_GPIO_WritePin(LCD16X2_CfgParam[LCD16X2_Index].D5_GPIOx, LCD16X2_CfgParam[LCD16X2_Index].D5_PINx, 0);
    if(Data & 4)
    	HAL_GPIO_WritePin(LCD16X2_CfgParam[LCD16X2_Index].D6_GPIOx, LCD16X2_CfgParam[LCD16X2_Index].D6_PINx, 1);
    else
    	HAL_GPIO_WritePin(LCD16X2_CfgParam[LCD16X2_Index].D6_GPIOx, LCD16X2_CfgParam[LCD16X2_Index].D6_PINx, 0);
    if(Data & 8)
    	HAL_GPIO_WritePin(LCD16X2_CfgParam[LCD16X2_Index].D7_GPIOx, LCD16X2_CfgParam[LCD16X2_Index].D7_PINx, 1);
    else
    	HAL_GPIO_WritePin(LCD16X2_CfgParam[LCD16X2_Index].D7_GPIOx, LCD16X2_CfgParam[LCD16X2_Index].D7_PINx, 0);
}

void LCD16X2_CMD(uint8_t LCD16X2_Index, unsigned char CMD)
{
    // Select Command Register
	HAL_GPIO_WritePin(LCD16X2_CfgParam[LCD16X2_Index].RS_GPIOx, LCD16X2_CfgParam[LCD16X2_Index].RS_PINx, 0);
    // Move The Command Data To LCD
	LCD16X2_DATA(LCD16X2_Index, CMD);
    // Send The EN Clock Signal
    HAL_GPIO_WritePin(LCD16X2_CfgParam[LCD16X2_Index].EN_GPIOx, LCD16X2_CfgParam[LCD16X2_Index].EN_PINx, 0);
    DELAY_US(5);
    HAL_GPIO_WritePin(LCD16X2_CfgParam[LCD16X2_Index].EN_GPIOx, LCD16X2_CfgParam[LCD16X2_Index].EN_PINx, 1);
    DELAY_US(5);
    HAL_GPIO_WritePin(LCD16X2_CfgParam[LCD16X2_Index].EN_GPIOx, LCD16X2_CfgParam[LCD16X2_Index].EN_PINx, 0);
    DELAY_US(100);
}

void LCD16X2_Clear(uint8_t LCD16X2_Index)
{
	LCD16X2_CMD(LCD16X2_Index, 0);
	LCD16X2_CMD(LCD16X2_Index, 1);
    DELAY_MS(2);
}

void LCD16X2_Set_Cursor(uint8_t LCD16X2_Index, unsigned char r, unsigned char c)
{
    unsigned char Temp,Low4,High4;
    if(r == 1)
    {
      Temp  = 0x80 + c - 1; //0x80 is used to move the cursor
      High4 = Temp >> 4;
      Low4  = Temp & 0x0F;
      LCD16X2_CMD(LCD16X2_Index, High4);
      LCD16X2_CMD(LCD16X2_Index, Low4);
    }
    if(r == 2)
    {
      Temp  = 0xC0 + c - 1;
      High4 = Temp >> 4;
      Low4  = Temp & 0x0F;
      LCD16X2_CMD(LCD16X2_Index, High4);
      LCD16X2_CMD(LCD16X2_Index, Low4);
    }
}

void LCD16X2_Init(uint8_t LCD16X2_Index)
{
	// According To Datasheet, We Must Wait At Least 40ms After Power Up Before Interacting With The LCD Module
	while(HAL_GetTick() < 50);
	// The Init. Procedure As Described In The Datasheet
	HAL_GPIO_WritePin(LCD16X2_CfgParam[LCD16X2_Index].RS_GPIOx, LCD16X2_CfgParam[LCD16X2_Index].RS_PINx, 0);
	HAL_GPIO_WritePin(LCD16X2_CfgParam[LCD16X2_Index].EN_GPIOx, LCD16X2_CfgParam[LCD16X2_Index].EN_PINx, 0);
    // Init in 4-Bit Data Mode
	LCD16X2_DATA(LCD16X2_Index, 0x00);
    DELAY_MS(150);
    LCD16X2_CMD(LCD16X2_Index, 0x03);
    DELAY_MS(5);
    LCD16X2_CMD(LCD16X2_Index, 0x03);
    DELAY_MS(5);
    LCD16X2_CMD(LCD16X2_Index, 0x03);
    DELAY_US(150);
    // The Rest of The Init Sequence As Defined in The Hitachi HD44780 Datasheet
    LCD16X2_CMD(LCD16X2_Index, 0x02);
    LCD16X2_CMD(LCD16X2_Index, 0x02);
    LCD16X2_CMD(LCD16X2_Index, 0x08);
    LCD16X2_CMD(LCD16X2_Index, 0x00);
    LCD16X2_CMD(LCD16X2_Index, 0x0C);
    LCD16X2_CMD(LCD16X2_Index, 0x00);
    LCD16X2_CMD(LCD16X2_Index, 0x06);
    LCD16X2_CMD(LCD16X2_Index, 0x00);
    LCD16X2_CMD(LCD16X2_Index, 0x01);
}

void LCD16X2_Write_Char(uint8_t LCD16X2_Index, char Data)
{
   char Low4,High4;
   Low4  = Data & 0x0F;
   High4 = Data & 0xF0;

   HAL_GPIO_WritePin(LCD16X2_CfgParam[LCD16X2_Index].RS_GPIOx, LCD16X2_CfgParam[LCD16X2_Index].RS_PINx, 1);

   LCD16X2_DATA(LCD16X2_Index, (High4>>4));
   HAL_GPIO_WritePin(LCD16X2_CfgParam[LCD16X2_Index].EN_GPIOx, LCD16X2_CfgParam[LCD16X2_Index].EN_PINx, 0);
   DELAY_US(5);
   HAL_GPIO_WritePin(LCD16X2_CfgParam[LCD16X2_Index].EN_GPIOx, LCD16X2_CfgParam[LCD16X2_Index].EN_PINx, 1);
   DELAY_US(5);
   HAL_GPIO_WritePin(LCD16X2_CfgParam[LCD16X2_Index].EN_GPIOx, LCD16X2_CfgParam[LCD16X2_Index].EN_PINx, 0);
   DELAY_US(100);

   LCD16X2_DATA(LCD16X2_Index, Low4);
   HAL_GPIO_WritePin(LCD16X2_CfgParam[LCD16X2_Index].EN_GPIOx, LCD16X2_CfgParam[LCD16X2_Index].EN_PINx, 0);
   DELAY_US(5);
   HAL_GPIO_WritePin(LCD16X2_CfgParam[LCD16X2_Index].EN_GPIOx, LCD16X2_CfgParam[LCD16X2_Index].EN_PINx, 1);
   DELAY_US(5);
   HAL_GPIO_WritePin(LCD16X2_CfgParam[LCD16X2_Index].EN_GPIOx, LCD16X2_CfgParam[LCD16X2_Index].EN_PINx, 0);
   DELAY_US(100);
}

void LCD16X2_Write_String(uint8_t LCD16X2_Index, char *str)
{
    int i;
    for(i=0; str[i]!='\0'; i++)
    {
    	LCD16X2_Write_Char(LCD16X2_Index, str[i]);
    }
}

void LCD16X2_SL(uint8_t LCD16X2_Index)
{
	LCD16X2_CMD(LCD16X2_Index, 0x01);
	LCD16X2_CMD(LCD16X2_Index, 0x08);
}

void LCD16X2_SR(uint8_t LCD16X2_Index)
{
	LCD16X2_CMD(LCD16X2_Index, 0x01);
	LCD16X2_CMD(LCD16X2_Index, 0x0C);
}
