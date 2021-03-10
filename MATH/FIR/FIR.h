/*
 * File: FIR.h
 * Driver Name: [[ FIR Digital Filters ]]
 * SW Layer:   ECUAL
 * Created on: Jun 28, 2020
 * Ver: 1.1
 * Author:     Khaled Magdy
 * -------------------------------------------
 * For More Information, Tutorials, etc.
 * Visit Website: www.DeepBlueMbedded.com
 *
 */

#ifndef FIR_H
#define	FIR_H

#include <stdint.h>

#define FILTER_DATA_TYPE    float


typedef struct
{
    uint16_t Filter_Order;
    uint16_t Buffer_Index;
    FILTER_DATA_TYPE* Data_Buffer;
    float* Filter_Coeffecients;
}FIR_Filter_cfg;


void AVG_FIR_LPF(FILTER_DATA_TYPE IN, FILTER_DATA_TYPE* OUT, FIR_Filter_cfg* FILTER_OBJ);

void SUM_BUFFER(FILTER_DATA_TYPE IN, FILTER_DATA_TYPE* SUM, FIR_Filter_cfg* FILTER_OBJ);


#endif	/* FIR_H */

