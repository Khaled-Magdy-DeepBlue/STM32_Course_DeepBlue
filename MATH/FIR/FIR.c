/*
 * File: FIR.c
 * Driver Name: [[ FIR Digital Filters ]]
 * SW Layer:   ECUAL
 * Created on: Jun 28, 2020
 * Author:     Khaled Magdy
 * -------------------------------------------
 * For More Information, Tutorials, etc.
 * Visit Website: www.DeepBlueMbedded.com
 *
 */

#include "FIR.h"


void AVG_FIR_LPF(FILTER_DATA_TYPE IN, FILTER_DATA_TYPE* OUT, FIR_Filter_cfg* FILTER_OBJ)
{
	FILTER_DATA_TYPE SUM = 0;
    uint16_t i = 0;
    
    // Push The New Input To The History Buffer
    FILTER_OBJ->Data_Buffer[FILTER_OBJ->Buffer_Index] = IN;
    FILTER_OBJ->Buffer_Index++;
    if(FILTER_OBJ->Buffer_Index == FILTER_OBJ->Filter_Order+1)
    {
        FILTER_OBJ->Buffer_Index = 0;
    }
    
    // Calculate The Average For The Data In The Buffer
    for(i=0; i < FILTER_OBJ->Filter_Order+1; i++)
    {
        SUM += FILTER_OBJ->Data_Buffer[i];
    }
    
    *OUT = SUM / (FILTER_OBJ->Filter_Order+1);
}

void SUM_BUFFER(FILTER_DATA_TYPE IN, FILTER_DATA_TYPE* SUM, FIR_Filter_cfg* FILTER_OBJ)
{
    int32_t temp_sum = 0;
    uint16_t i = 0;
    
    // Push The New Input To The History Buffer
	FILTER_OBJ->Data_Buffer[FILTER_OBJ->Buffer_Index] = IN;
	FILTER_OBJ->Buffer_Index++;
	
    if(FILTER_OBJ->Buffer_Index == FILTER_OBJ->Filter_Order+1)
    { FILTER_OBJ->Buffer_Index = 0; }
    
    // Calculate The Accumulated Sum For The Buffer Data
    for(i = 0; i < FILTER_OBJ->Filter_Order+1; i++)
    {
        temp_sum += FILTER_OBJ->Data_Buffer[i];
    }
    *SUM = (FILTER_DATA_TYPE)temp_sum;
}
