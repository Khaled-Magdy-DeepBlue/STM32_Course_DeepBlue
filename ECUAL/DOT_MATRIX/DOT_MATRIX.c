/*
 * File: DOT_MATRIX.c
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
#include "DOT_MATRIX_cfg.h"

//---------------------------[ Globals ]---------------------------

SPI_HandleTypeDef * g_hspi;

typedef struct
{
	uint32_t  Ticks;
	uint32_t  Max_Ticks;
	uint16_t  Arr_Index;
	uint8_t   Col_Index;
	uint8_t   BufferInit;
	uint8_t   Buffer[MAX_CASCADED_NUM*8];
	uint16_t  ArrSize;
	char*     ArrPtr;
}DOT_MATRIX_info;

static DOT_MATRIX_info gs_MATRIX_info[DOT_MATRIX_UNITS] = {0};

// Font 7x5 Bitmap (ASCII)
char FONT_7x5[96][5] = {
 {0b00000000,0b00000000,0b00000000,0b00000000,0b00000000}, // space
 {0b00000000,0b00000000,0b01001111,0b00000000,0b00000000}, // !
 {0b00000000,0b00000111,0b00000000,0b00000111,0b00000000}, // "
 {0b00010100,0b01111111,0b00010100,0b01111111,0b00010100}, // #
 {0b00100100,0b00101010,0b01111111,0b00101010,0b00010010}, // $
 {0b00100011,0b00010011,0b00001000,0b01100100,0b01100010}, // %
 {0b00110110,0b01001001,0b01010101,0b00100010,0b01010000}, // &
 {0b00000000,0b00000101,0b00000011,0b00000000,0b00000000}, // '
 {0b00000000,0b00011100,0b00100010,0b01000001,0b00000000}, // (
 {0b00000000,0b01000001,0b00100010,0b00011100,0b00000000}, // )
 {0b00010100,0b00001000,0b00111110,0b00001000,0b00010100}, // *
 {0b00001000,0b00001000,0b00111110,0b00001000,0b00001000}, // +
 {0b00000000,0b01010000,0b00110000,0b00000000,0b00000000}, // ,
 {0b00001000,0b00001000,0b00001000,0b00001000,0b00001000}, // -
 {0b00000000,0b01100000,0b01100000,0b00000000,0b00000000}, // .
 {0b00100000,0b00010000,0b00001000,0b00000100,0b00000010}, // /
 {0b00111110,0b01010001,0b01001001,0b01000101,0b00111110}, // 0
 {0b00000000,0b01000010,0b01111111,0b01000000,0b00000000}, // 1
 {0b01000010,0b01100001,0b01010001,0b01001001,0b01000110}, // 2
 {0b00100001,0b01000001,0b01000101,0b01001011,0b00110001}, // 3
 {0b00011000,0b00010100,0b00010010,0b01111111,0b00010000}, // 4
 {0b00100111,0b01000101,0b01000101,0b01000101,0b00111001}, // 5
 {0b00111100,0b01001010,0b01001001,0b01001001,0b00110000}, // 6
 {0b00000011,0b01110001,0b00001001,0b00000101,0b00000011}, // 7
 {0b00110110,0b01001001,0b01001001,0b01001001,0b00110110}, // 8
 {0b00000110,0b01001001,0b01001001,0b00101001,0b00011110}, // 9
 {0b00000000,0b01101100,0b01101100,0b00000000,0b00000000}, // :
 {0b00000000,0b01010110,0b00110110,0b00000000,0b00000000}, // ;
 {0b00001000,0b00010100,0b00100010,0b01000001,0b00000000}, // <
 {0b00010100,0b00010100,0b00010100,0b00010100,0b00010100}, // =
 {0b00000000,0b01000001,0b00100010,0b00010100,0b00001000}, // >
 {0b00000010,0b00000001,0b01010001,0b00001001,0b00000110}, // ?
 {0b00110010,0b01001001,0b01111001,0b01000001,0b00111110}, // @
 {0b01111110,0b00010001,0b00010001,0b00010001,0b01111110}, // A
 {0b01111111,0b01001001,0b01001001,0b01001001,0b00111110}, // B
 {0b00111110,0b01000001,0b01000001,0b01000001,0b00100010}, // C
 {0b01111111,0b01000001,0b01000001,0b01000001,0b00111110}, // D
 {0b01111111,0b01001001,0b01001001,0b01001001,0b01001001}, // E
 {0b01111111,0b00001001,0b00001001,0b00001001,0b00000001}, // F
 {0b00111110,0b01000001,0b01001001,0b01001001,0b00111010}, // G
 {0b01111111,0b00001000,0b00001000,0b00001000,0b01111111}, // H
 {0b01000001,0b01000001,0b01111111,0b01000001,0b01000001}, // I
 {0b00110000,0b01000001,0b01000001,0b00111111,0b00000001}, // J
 {0b01111111,0b00001000,0b00010100,0b00100010,0b01000001}, // K
 {0b01111111,0b01000000,0b01000000,0b01000000,0b01000000}, // L
 {0b01111111,0b00000010,0b00001100,0b00000010,0b01111111}, // M
 {0b01111111,0b00000100,0b00001000,0b00010000,0b01111111}, // N
 {0b00111110,0b01000001,0b01000001,0b01000001,0b00111110}, // O
 {0b01111111,0b00001001,0b00001001,0b00001001,0b00000110}, // P
 {0b00111110,0b01000001,0b01010001,0b00100001,0b01011110}, // Q
 {0b01111111,0b00001001,0b00001001,0b00011001,0b01100110}, // R
 {0b01000110,0b01001001,0b01001001,0b01001001,0b00110001}, // S
 {0b00000001,0b00000001,0b01111111,0b00000001,0b00000001}, // T
 {0b00111111,0b01000000,0b01000000,0b01000000,0b00111111}, // U
 {0b00001111,0b00110000,0b01000000,0b00110000,0b00001111}, // V
 {0b00111111,0b01000000,0b00111000,0b01000000,0b00111111}, // W
 {0b01100011,0b00010100,0b00001000,0b00010100,0b01100011}, // X
 {0b00000011,0b00000100,0b01111000,0b00000100,0b00000011}, // Y
 {0b01100001,0b01010001,0b01001001,0b01000101,0b01000011}, // Z
 {0b01111111,0b01000001,0b01000001,0b00000000,0b00000000}, // [
 {0b00000010,0b00000100,0b00001000,0b00010000,0b00100000}, // '\'
 {0b00000000,0b00000000,0b01000001,0b01000001,0b01111111}, // ]
 {0b00000100,0b00000010,0b00000001,0b00000010,0b00000100}, // ^
 {0b01000000,0b01000000,0b01000000,0b01000000,0b01000000}, // _
 {0b00000000,0b00000001,0b00000010,0b00000100,0b00000000}, // `
 {0b00100000,0b01010100,0b01010100,0b01010100,0b01111000}, // a
 {0b01111111,0b01001000,0b01000100,0b01000100,0b00111000}, // 0b
 {0b00111000,0b01000100,0b01000100,0b01000100,0b00100000}, // c
 {0b00111000,0b01000100,0b01000100,0b01001000,0b01111111}, // d
 {0b00111000,0b01010100,0b01010100,0b01010100,0b00011000}, // e
 {0b00001000,0b01111110,0b00001001,0b00000001,0b00000010}, // f
 {0b00001100,0b01010010,0b01010010,0b01010010,0b00111110}, // g
 {0b01111111,0b00001000,0b00000100,0b00000100,0b01111000}, // h
 {0b00000000,0b01000100,0b01111101,0b01000000,0b00000000}, // i
 {0b00100000,0b01000000,0b01000100,0b00111101,0b00000000}, // j
 {0b01111111,0b00010000,0b00101000,0b01000100,0b00000000}, // k
 {0b00000000,0b01000001,0b01111111,0b01000000,0b00000000}, // l
 {0b01111000,0b00000100,0b00001000,0b00000100,0b01111000}, // m
 {0b01111100,0b00001000,0b00000100,0b00000100,0b01111000}, // n
 {0b00111000,0b01000100,0b01000100,0b01000100,0b00111000}, // o
 {0b01111100,0b00010100,0b00010100,0b00010100,0b00001000}, // p
 {0b00001000,0b00010100,0b00010100,0b01111100,0b00000000}, // q
 {0b01111100,0b00001000,0b00000100,0b00000100,0b00001000}, // r
 {0b01001000,0b01010100,0b01010100,0b01010100,0b00100000}, // s
 {0b00000100,0b00111111,0b01000100,0b01000000,0b00100000}, // t
 {0b00111100,0b01000000,0b01000000,0b00100000,0b01111100}, // u
 {0b00011100,0b00100000,0b01000000,0b00100000,0b00011100}, // v
 {0b00111100,0b01000000,0b00110000,0b01000000,0b00111100}, // w
 {0b01000100,0b00101000,0b00010000,0b00101000,0b01000100}, // x
 {0b00001100,0b01010000,0b01010000,0b01010000,0b00111100}, // y
 {0b01000100,0b01100100,0b01010100,0b01001100,0b01000100}, // z
 {0b00000000,0b00001000,0b00110110,0b01000001,0b00000000}, // {
 {0b00000000,0b00000000,0b01111111,0b00000000,0b00000000}, // |
 {0b00000000,0b01000001,0b00110110,0b00001000,0b00000000}, // }
 {0b00001000,0b00000100,0b00000100,0b00001000,0b00000100}  // ~
};

//-----------------------------[ Static Functions' Definitions ]----------------------------

// Static Functions Used By Library But Not Visible To User Who Include The Library (Driver)

static void SPI_TX_Byte(uint8_t au8_DATA)
{
	HAL_SPI_Transmit(g_hspi, &au8_DATA, 1, 10);
}

static void SS_ENABLE(uint8_t au8_MATRIX_Instance)
{
	HAL_GPIO_WritePin(DOT_MATRIX_CfgParam[au8_MATRIX_Instance].SS_GPIO, DOT_MATRIX_CfgParam[au8_MATRIX_Instance].SS_PIN, 0);
}

static void SS_DISABLE(uint8_t au8_MATRIX_Instance)
{
	HAL_GPIO_WritePin(DOT_MATRIX_CfgParam[au8_MATRIX_Instance].SS_GPIO, DOT_MATRIX_CfgParam[au8_MATRIX_Instance].SS_PIN, 1);
}

static void TxByte(uint8_t au8_MATRIX_Instance, uint8_t au8_Col, uint8_t au8_Val)
{
	SS_ENABLE(au8_MATRIX_Instance);
	SPI_TX_Byte(au8_Col);
	SPI_TX_Byte(au8_Val);
	SS_DISABLE(au8_MATRIX_Instance);
}

static void DisplayBuffer(uint8_t au8_MATRIX_Instance)
{
	uint8_t i = 0;

    for(i = 0; i<DOT_MATRIX_CfgParam[au8_MATRIX_Instance].CASCADED_DEVICES; i++)
    {
	    for(uint8_t j = 1; j < 9; j++)
	    {
	    	SS_ENABLE(au8_MATRIX_Instance);
		   for(uint8_t k = 0; k < i; k++)
		   {
			   SPI_TX_Byte(0x00);
		       SPI_TX_Byte(0x00);
		   }
		   SPI_TX_Byte(j);
		   SPI_TX_Byte(gs_MATRIX_info[au8_MATRIX_Instance].Buffer[j + i*8 - 1]);
		   for(uint8_t k = DOT_MATRIX_CfgParam[au8_MATRIX_Instance].CASCADED_DEVICES-1; k > i; k--)
		   {
			   SPI_TX_Byte(0x00);
			   SPI_TX_Byte(0x00);
		   }
		   SS_DISABLE(au8_MATRIX_Instance);
	    }
    }
}

static void PushToBuffer(uint8_t au8_MATRIX_Instance, uint8_t au8_NewByte)
{
	uint8_t i = 0;

	for(i = 0; i < DOT_MATRIX_CfgParam[au8_MATRIX_Instance].CASCADED_DEVICES*8 - 1; i++)
	{
		gs_MATRIX_info[au8_MATRIX_Instance].Buffer[i] = gs_MATRIX_info[au8_MATRIX_Instance].Buffer[i+1];
	}
	gs_MATRIX_info[au8_MATRIX_Instance].Buffer[DOT_MATRIX_CfgParam[au8_MATRIX_Instance].CASCADED_DEVICES*8 - 1] = au8_NewByte;
}

//----------------------------[ Library Functions' Definitions ]---------------------------

// Functions To Be Used By The User Who Include The Library (Driver)

void DOT_MATRIX_Init(SPI_HandleTypeDef * hspi)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
    uint8_t i = 0, j = 0;

    g_hspi = hspi;

    /*--------[ Configure The SS GPIO Pins ]-------*/
    for(i = 0; i<DOT_MATRIX_UNITS; i++)
    {
    	if(DOT_MATRIX_CfgParam[i].SS_GPIO == GPIOA)
    	{
    		__HAL_RCC_GPIOA_CLK_ENABLE();
    	}
    	else if(DOT_MATRIX_CfgParam[i].SS_GPIO == GPIOB)
    	{
    		__HAL_RCC_GPIOB_CLK_ENABLE();
    	}
    	else if(DOT_MATRIX_CfgParam[i].SS_GPIO == GPIOC)
    	{
    		__HAL_RCC_GPIOC_CLK_ENABLE();
    	}
    	else if(DOT_MATRIX_CfgParam[i].SS_GPIO == GPIOD)
    	{
    		__HAL_RCC_GPIOD_CLK_ENABLE();
    	}
    	else if(DOT_MATRIX_CfgParam[i].SS_GPIO == GPIOE)
    	{
    		__HAL_RCC_GPIOE_CLK_ENABLE();
    	}
    	GPIO_InitStruct.Pin = DOT_MATRIX_CfgParam[i].SS_PIN;
    	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    	GPIO_InitStruct.Pull = GPIO_NOPULL;
    	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    	HAL_GPIO_Init(DOT_MATRIX_CfgParam[i].SS_GPIO, &GPIO_InitStruct);
    }
    /*--------[ Configure The SPI Peripheral ]-------*/
    g_hspi->Instance = DOT_MATRIX_SPI;
    g_hspi->Init.Mode = SPI_MODE_MASTER;
    g_hspi->Init.Direction = SPI_DIRECTION_2LINES;
    g_hspi->Init.DataSize = SPI_DATASIZE_8BIT;
    g_hspi->Init.CLKPolarity = SPI_POLARITY_LOW;
    g_hspi->Init.CLKPhase = SPI_PHASE_1EDGE;
    g_hspi->Init.NSS = SPI_NSS_SOFT;
    g_hspi->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
    g_hspi->Init.FirstBit = SPI_FIRSTBIT_MSB;
    g_hspi->Init.TIMode = SPI_TIMODE_DISABLE;
    g_hspi->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    g_hspi->Init.CRCPolynomial = 10;
    HAL_SPI_Init(g_hspi);

    /*--------[ Initialize & Configure The MAX7219 Devices ]-------*/
    for(i = 0; i<DOT_MATRIX_UNITS; i++)
    {
    	// Set BCD Decode Mode
    	SS_ENABLE(i);
    	for(j = 0; j<DOT_MATRIX_CfgParam[i].CASCADED_DEVICES; j++)
    	{
    	    SPI_TX_Byte(0x09);
    	    SPI_TX_Byte(0x00);
    	}
    	SS_DISABLE(i);
    	// Set Dot Matrix Unit Brightness
    	SS_ENABLE(i);
    	for(j = 0; j<DOT_MATRIX_CfgParam[i].CASCADED_DEVICES; j++)
    	{
    		SPI_TX_Byte(0x0A); // Select Intensity register
    		SPI_TX_Byte(DOT_MATRIX_CfgParam[i].BRIGHTNESS);
    	}
    	SS_DISABLE(i);
    	// Set Display Refresh
    	SS_ENABLE(i);
    	for(j = 0; j<DOT_MATRIX_CfgParam[i].CASCADED_DEVICES; j++)
    	{
    		SPI_TX_Byte(0x0B); // Select Scan-Limit register
    		SPI_TX_Byte(0x07); // 1-7
    	}
    	SS_DISABLE(i);
    	// Turn ON The Display
    	SS_ENABLE(i);
    	for(j = 0; j<DOT_MATRIX_CfgParam[i].CASCADED_DEVICES; j++)
    	{
    		SPI_TX_Byte(0x0C);
    		SPI_TX_Byte(0x01);
    	}
    	SS_DISABLE(i);
    	// Disable Display Test
    	SS_ENABLE(i);
    	for(j = 0; j<DOT_MATRIX_CfgParam[i].CASCADED_DEVICES; j++)
    	{
    		SPI_TX_Byte(0x0F);
    		SPI_TX_Byte(0x00);
    	}
    	SS_DISABLE(i);

        // Initialize The Global Structure
        gs_MATRIX_info[i].Ticks = 0;
        gs_MATRIX_info[i].BufferInit = 0;
        gs_MATRIX_info[i].Arr_Index = 0;
        gs_MATRIX_info[i].Col_Index = 0;
        gs_MATRIX_info[i].Max_Ticks = DOT_MATRIX_CfgParam[i].SCROLL_SPEED;
        for(j = 0; j < DOT_MATRIX_CfgParam[i].CASCADED_DEVICES*8; j++)
        {
        	gs_MATRIX_info[i].Buffer[j] = 0x00;
        }
    }
}

void DOT_MATRIX_Init_TMR(SPI_HandleTypeDef * hspi, TIM_HandleTypeDef* TMR_Handle)
{
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    uint32_t ARR_Value = 0;

	DOT_MATRIX_Init(hspi);

	/*--------[ Configure The Matrix Timer Base If Enabled ]-------*/
    if(MATRIX_TIMER_EN == 1)
    {
    	ARR_Value = (MATRIX_TIMER_CLK * 10.0 * MATRIX_TIME_BASE);
    	TMR_Handle->Instance = MATRIX_TIMER;
    	TMR_Handle->Init.Prescaler = 99;
    	TMR_Handle->Init.CounterMode = TIM_COUNTERMODE_UP;
    	TMR_Handle->Init.Period = ARR_Value-1;
    	TMR_Handle->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    	TMR_Handle->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    	HAL_TIM_Base_Init(TMR_Handle);
    	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    	HAL_TIM_ConfigClockSource(TMR_Handle, &sClockSourceConfig);
    	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    	HAL_TIMEx_MasterConfigSynchronization(TMR_Handle, &sMasterConfig);
    	HAL_TIM_Base_Start_IT(TMR_Handle);
    }
}

void MATRIX_SCROLL_SetSpeed(uint8_t au8_MATRIX_Instance, uint16_t au16_SPEED)
{
	gs_MATRIX_info[au8_MATRIX_Instance].Max_Ticks = au16_SPEED;
}

void MATRIX_CLEAR(uint8_t au8_MATRIX_Instance)
{
	uint8_t i = 0, col = 1;

	for(i = 0; i<DOT_MATRIX_CfgParam[au8_MATRIX_Instance].CASCADED_DEVICES; i++)
	{
		for(col = 1; col < 9; col++)
		{
			TxByte(au8_MATRIX_Instance, col, 0x00);
		}
	}
}

void MATRIX_Write_Char(uint8_t au8_MATRIX_Instance, uint8_t myChar)
{
    uint8_t Column = 1;
    uint8_t FONT_W = 5;

    TxByte(au8_MATRIX_Instance, Column, 0x00);
    for(Column=2; Column<=FONT_W+1; Column++)
    {
    	TxByte(au8_MATRIX_Instance, Column, FONT_7x5[myChar-32][Column-2]);
    }
    TxByte(au8_MATRIX_Instance, Column, 0x00);
}

void MATRIX_DisplayMessage(uint8_t au8_MATRIX_Instance, char* ArrayPointer, uint16_t ArraySize)
{
	gs_MATRIX_info[au8_MATRIX_Instance].ArrPtr = ArrayPointer;
	gs_MATRIX_info[au8_MATRIX_Instance].ArrSize = ArraySize-1;
	gs_MATRIX_info[au8_MATRIX_Instance].BufferInit = 1;
}

void DOT_MATRIX_Main(void)
{
	uint8_t i = 0, ch, space = 0;

	for(i=0; i<DOT_MATRIX_UNITS; i++)
	{
		if(gs_MATRIX_info[i].Ticks >= gs_MATRIX_info[i].Max_Ticks)
		{
			if(gs_MATRIX_info[i].BufferInit == 1 && DOT_MATRIX_CfgParam[i].SCROLL_Mode == SCROLL_MODE)
			{
				if(gs_MATRIX_info[i].Col_Index == 5)
				{
					gs_MATRIX_info[i].Arr_Index++;
					gs_MATRIX_info[i].Col_Index = 0;
					space = 1;
				}
				if(gs_MATRIX_info[i].Arr_Index == gs_MATRIX_info[i].ArrSize)
				{
					gs_MATRIX_info[i].Arr_Index = 0;
				}
				if(space)
				{
					PushToBuffer(i, 0x00); // Add White Space To Separate Characters
				}
				else
				{
					ch = (*(gs_MATRIX_info[i].ArrPtr + gs_MATRIX_info[i].Arr_Index)-32);
					PushToBuffer(i, FONT_7x5[ch][gs_MATRIX_info[i].Col_Index++]);
				}
				DisplayBuffer(i);
			}
			gs_MATRIX_info[i].Ticks = 0;
		}
		else
		{
			gs_MATRIX_info[i].Ticks++;
		}
	}
}

void MATRIX_TMR_OVF_ISR(TIM_HandleTypeDef* htim)
{
	uint8_t i = 0, ch, space = 0;

	if(htim->Instance == MATRIX_TIMER)
	{
		for(i=0; i<DOT_MATRIX_UNITS; i++)
		{
			if(gs_MATRIX_info[i].Ticks >= gs_MATRIX_info[i].Max_Ticks)
			{
				if(gs_MATRIX_info[i].BufferInit == 1 && DOT_MATRIX_CfgParam[i].SCROLL_Mode == SCROLL_MODE)
				{
					if(gs_MATRIX_info[i].Col_Index == 5)
					{
						gs_MATRIX_info[i].Arr_Index++;
						gs_MATRIX_info[i].Col_Index = 0;
						space = 1;
					}
					if(gs_MATRIX_info[i].Arr_Index == gs_MATRIX_info[i].ArrSize)
					{
						gs_MATRIX_info[i].Arr_Index = 0;
					}
					if(space)
					{
						PushToBuffer(i, 0x00); // Add White Space To Separate Characters
					}
					else
					{
						ch = (*(gs_MATRIX_info[i].ArrPtr + gs_MATRIX_info[i].Arr_Index)-32);
						PushToBuffer(i, FONT_7x5[ch][gs_MATRIX_info[i].Col_Index++]);
					}
					DisplayBuffer(i);
				}
				gs_MATRIX_info[i].Ticks = 0;
			}
			else
			{
				gs_MATRIX_info[i].Ticks++;
			}
		}
	}
}



