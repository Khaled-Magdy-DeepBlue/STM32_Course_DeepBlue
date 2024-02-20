/*
 * File: FEE.c
 * Driver Name: [[ FEE (Flash EEPROM Emulation) ]]
 * SW Layer:   MIDWARE
 * Author:     Khaled Magdy
 * -------------------------------------------
 * For More Information, Tutorials, etc.
 * Visit Website: www.DeepBlueMbedded.com
 */
#include "FEE.h"
#include "string.h"

HAL_StatusTypeDef FEE_Write(uint32_t address, uint16_t *data, uint32_t dataSize)
{
    HAL_StatusTypeDef status = HAL_OK;
    uint32_t pageAddress = address & ~(FLASH_PAGE_SIZE - 1); // Get the start address of the page
    uint16_t buffer[FEE_BUFFER_LEN]; // Create a buffer to hold the entire page
    HAL_FLASH_Unlock();
    // Read the entire page into the buffer
    for (uint32_t i = 0; i < FEE_BUFFER_LEN; i++) {
        buffer[i] = *(__IO uint16_t*)(pageAddress + i * 2);
    }
    // Modify the values at the desired address
    uint32_t offset = address - pageAddress;
    for (uint32_t i = 0; i < dataSize; i++) {
        buffer[offset / 2 + i] = data[i];
    }
    // Erase the page
    FLASH_EraseInitTypeDef eraseInitStruct;
    eraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
    eraseInitStruct.PageAddress = pageAddress;
    eraseInitStruct.NbPages = 1;
    uint32_t pageError;
    status = HAL_FLASHEx_Erase(&eraseInitStruct, &pageError);
    if (status != HAL_OK) {
        HAL_FLASH_Lock();
        return status;
    }
    // Write the modified buffer back to the page
    for (uint32_t i = 0; i < FEE_BUFFER_LEN; i++) {
        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, pageAddress + i * 2, buffer[i]);
        if (status != HAL_OK) {
            break;
        }
    }
    HAL_FLASH_Lock();
    return status;
}

void FEE_Read(uint32_t address, uint16_t *data, uint32_t dataSize)
{
    for (uint32_t i = 0; i < dataSize; i++) {
        data[i] = *(__IO uint16_t*)address;
        address += 2;
    }
}

// Generic APIs For Any Data Type
void FEE_WriteData(uint32_t address, void *data, size_t dataSize)
{
    uint16_t bytes[dataSize / 2];
    memcpy(bytes, data, dataSize);
    FEE_Write(address, bytes, dataSize);
}

void FEE_ReadData(uint32_t address, void *data, size_t dataSize)
{
    uint16_t bytes[dataSize / 2];
    FEE_Read(address, bytes, dataSize);
    memcpy(data, bytes, dataSize);
}

