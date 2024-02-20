/*
 * File: FEE.h
 * Driver Name: [[ FEE (Flash EEPROM Emulation) ]]
 * SW Layer:   MIDWARE
 * Author:     Khaled Magdy
 * -------------------------------------------
 * For More Information, Tutorials, etc.
 * Visit Website: www.DeepBlueMbedded.com
 */
#ifndef FEE_H_
#define FEE_H_
/*
 * Disclaimer: This library is a quick & dirty implementation for FLASH wrapper functions
 * That may be helpful for basic applications, not for use in anything serious obviously.
 */
#include "stdint.h"
#include "main.h"

#define FEE_START_ADDRESS	((uint32_t)0x0800F000)  // FEE Start Address (in Flash Memory)
#define FEE_PAGE_SIZE		((uint32_t)0x400)       // FEE Page Size = 1kB (Default Page Size For STM32F103)
#define FEE_BUFFER_LEN		(FEE_PAGE_SIZE / 2U)

//-----[ Prototypes For All User External Functions ]-----

HAL_StatusTypeDef FEE_Write(uint32_t address, uint16_t *data, uint32_t dataSize);
void FEE_Read(uint32_t address, uint16_t *data, uint32_t dataSize);

// Generic APIs For Any Data Type
void FEE_WriteData(uint32_t address, void *data, size_t dataSize);
void FEE_ReadData(uint32_t address, void *data, size_t dataSize);

#endif /* FEE_H_ */
