/*
 * File: LCD16x2.c
 * Driver Name: [[ LCD16x2 Display (GPIO 4-Bit Mode) ]]
 * SW Layer:   ECUAL
 * Created on: Jun 28, 2020
 * Author:     Khaled Magdy
 * -------------------------------------------
 * For More Information, Tutorials, etc.
 * Visit Website: www.DeepBlueMbedded.com
 *
 */

#include "LCD16x2.h"
#include "LCD16x2_cfg.h"
#include "../../util/DWT_Delay.h"


//-----[ Alphanumeric LCD 16x2 Routines ]-----

void LCD_DATA(unsigned char Data)
{
    if(Data & 1)
    	HAL_GPIO_WritePin(LCD16x2_CfgParam.LCD_GPIO, LCD16x2_CfgParam.D4_PIN, 1);
    else
    	HAL_GPIO_WritePin(LCD16x2_CfgParam.LCD_GPIO, LCD16x2_CfgParam.D4_PIN, 0);
    if(Data & 2)
    	HAL_GPIO_WritePin(LCD16x2_CfgParam.LCD_GPIO, LCD16x2_CfgParam.D5_PIN, 1);
    else
    	HAL_GPIO_WritePin(LCD16x2_CfgParam.LCD_GPIO, LCD16x2_CfgParam.D5_PIN, 0);
    if(Data & 4)
    	HAL_GPIO_WritePin(LCD16x2_CfgParam.LCD_GPIO, LCD16x2_CfgParam.D6_PIN, 1);
    else
    	HAL_GPIO_WritePin(LCD16x2_CfgParam.LCD_GPIO, LCD16x2_CfgParam.D6_PIN, 0);
    if(Data & 8)
    	HAL_GPIO_WritePin(LCD16x2_CfgParam.LCD_GPIO, LCD16x2_CfgParam.D7_PIN, 1);
    else
    	HAL_GPIO_WritePin(LCD16x2_CfgParam.LCD_GPIO, LCD16x2_CfgParam.D7_PIN, 0);
}
void LCD_CMD(unsigned char a_CMD)
{
    // Select Command Register
    HAL_GPIO_WritePin(LCD16x2_CfgParam.LCD_GPIO, LCD16x2_CfgParam.RS_PIN, 0);
    // Move The Command Data To LCD
    LCD_DATA(a_CMD);
    // Send The EN Clock Signal
    HAL_GPIO_WritePin(LCD16x2_CfgParam.LCD_GPIO, LCD16x2_CfgParam.EN_PIN, 1);
    DWT_Delay_us(LCD16x2_CfgParam.LCD_EN_Delay);
    HAL_GPIO_WritePin(LCD16x2_CfgParam.LCD_GPIO, LCD16x2_CfgParam.EN_PIN, 0);
    DWT_Delay_us(LCD16x2_CfgParam.LCD_EN_Delay);
}
void LCD_Clear()
{
    LCD_CMD(0);
    LCD_CMD(1);
    DWT_Delay_us(70);
}
void LCD_Set_Cursor(unsigned char r, unsigned char c)
{
    unsigned char Temp,Low4,High4;
    if(r == 1)
    {
      Temp  = 0x80 + c - 1; //0x80 is used to move the cursor
      High4 = Temp >> 4;
      Low4  = Temp & 0x0F;
      LCD_CMD(High4);
      LCD_CMD(Low4);
    }
    if(r == 2)
    {
      Temp  = 0xC0 + c - 1;
      High4 = Temp >> 4;
      Low4  = Temp & 0x0F;
      LCD_CMD(High4);
      LCD_CMD(Low4);
    }
    DWT_Delay_ms(2);
}
void LCD_Init()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	// Initialize The DWT Delay Function Which Will Be Used Later On
	DWT_Delay_Init();
    // IO Pin Configurations
	if(LCD16x2_CfgParam.LCD_GPIO == GPIOA)
	    __HAL_RCC_GPIOA_CLK_ENABLE();
	else if(LCD16x2_CfgParam.LCD_GPIO == GPIOB)
	    __HAL_RCC_GPIOB_CLK_ENABLE();
	else if(LCD16x2_CfgParam.LCD_GPIO == GPIOC)
		__HAL_RCC_GPIOC_CLK_ENABLE();
	else if(LCD16x2_CfgParam.LCD_GPIO == GPIOD)
		__HAL_RCC_GPIOD_CLK_ENABLE();
	HAL_GPIO_WritePin(LCD16x2_CfgParam.LCD_GPIO, LCD16x2_CfgParam.D4_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD16x2_CfgParam.LCD_GPIO, LCD16x2_CfgParam.D5_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD16x2_CfgParam.LCD_GPIO, LCD16x2_CfgParam.D6_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD16x2_CfgParam.LCD_GPIO, LCD16x2_CfgParam.D7_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD16x2_CfgParam.LCD_GPIO, LCD16x2_CfgParam.RS_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD16x2_CfgParam.LCD_GPIO, LCD16x2_CfgParam.EN_PIN, GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = LCD16x2_CfgParam.D4_PIN | LCD16x2_CfgParam.D5_PIN |
			LCD16x2_CfgParam.D6_PIN |LCD16x2_CfgParam.D7_PIN | LCD16x2_CfgParam.RS_PIN |
			LCD16x2_CfgParam.EN_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LCD16x2_CfgParam.LCD_GPIO, &GPIO_InitStruct);

	// The Init. Procedure As Described In The Datasheet
    HAL_GPIO_WritePin(LCD16x2_CfgParam.LCD_GPIO, LCD16x2_CfgParam.RS_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD16x2_CfgParam.LCD_GPIO, LCD16x2_CfgParam.EN_PIN, GPIO_PIN_RESET);
    LCD_DATA(0x00);
    DWT_Delay_ms(150);
    LCD_CMD(0x03);
    DWT_Delay_ms(5);
    LCD_CMD(0x03);
    DWT_Delay_us(150);
    LCD_CMD(0x03);
    LCD_CMD(0x02);
    LCD_CMD(0x02);
    LCD_CMD(0x08);
    LCD_CMD(0x00);
    LCD_CMD(0x0C);
    LCD_CMD(0x00);
    LCD_CMD(0x06);
    LCD_CMD(0x00);
    LCD_CMD(0x01);
}
void LCD_Write_Char(char Data)
{
   char Low4,High4;
   Low4  = Data & 0x0F;
   High4 = Data & 0xF0;

   HAL_GPIO_WritePin(LCD16x2_CfgParam.LCD_GPIO, LCD16x2_CfgParam.RS_PIN, 1);

   LCD_DATA(High4>>4);
   HAL_GPIO_WritePin(LCD16x2_CfgParam.LCD_GPIO, LCD16x2_CfgParam.EN_PIN, 1);
   DWT_Delay_us(LCD16x2_CfgParam.LCD_EN_Delay);
   HAL_GPIO_WritePin(LCD16x2_CfgParam.LCD_GPIO, LCD16x2_CfgParam.EN_PIN, 0);
   DWT_Delay_us(LCD16x2_CfgParam.LCD_EN_Delay);

   LCD_DATA(Low4);
   HAL_GPIO_WritePin(LCD16x2_CfgParam.LCD_GPIO, LCD16x2_CfgParam.EN_PIN, 1);
   DWT_Delay_us(LCD16x2_CfgParam.LCD_EN_Delay);
   HAL_GPIO_WritePin(LCD16x2_CfgParam.LCD_GPIO, LCD16x2_CfgParam.EN_PIN, 0);
   DWT_Delay_us(LCD16x2_CfgParam.LCD_EN_Delay);
}
void LCD_Write_String(char *str)
{
    int i;
    for(i=0;str[i]!='\0';i++)
       LCD_Write_Char(str[i]);
}
void LCD_SL()
{
    LCD_CMD(0x01);
    LCD_CMD(0x08);
    DWT_Delay_us(35);
}
void LCD_SR()
{
    LCD_CMD(0x01);
    LCD_CMD(0x0C);
    DWT_Delay_us(35);
}
