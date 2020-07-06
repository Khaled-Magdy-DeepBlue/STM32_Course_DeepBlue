/*
 * File: MATH.h
 * Driver Name: [[ MATH Functions ]]
 * SW Layer:   ECUAL
 * Created on: Jun 28, 2020
 * Author:     Khaled Magdy
 * -------------------------------------------
 * For More Information, Tutorials, etc.
 * Visit Website: www.DeepBlueMbedded.com
 *
 */

#ifndef MATH_H_
#define MATH_H_

#include<stdint.h>

typedef uint32_t MATH_DataType;

uint32_t MAP(uint32_t au32_IN, uint32_t au32_INmin, uint32_t au32_INmax, uint32_t au32_OUTmin, uint32_t au32_OUTmax);

uint32_t Constrain(uint32_t au32_IN, uint32_t au32_MIN, uint32_t au32_MAX);

MATH_DataType MIN(MATH_DataType* IN_Arr, uint32_t au32_LEN);

MATH_DataType MAX(MATH_DataType* IN_Arr, uint32_t au32_LEN);

#endif /* MATH_H_ */
