/*
 * File: MATH.c
 * Driver Name: [[ MATH Functions ]]
 * SW Layer:   ECUAL
 * Created on: Jun 28, 2020
 * Author:     Khaled Magdy
 * -------------------------------------------
 * For More Information, Tutorials, etc.
 * Visit Website: www.DeepBlueMbedded.com
 *
 */

#include "MATH.h"

uint32_t MAP(uint32_t au32_IN, uint32_t au32_INmin, uint32_t au32_INmax, uint32_t au32_OUTmin, uint32_t au32_OUTmax)
{
	return ((((au32_IN - au32_INmin)*(au32_OUTmax - au32_OUTmin))/(au32_INmax - au32_INmin)) + au32_OUTmin);
}

uint32_t Constrain(uint32_t au32_IN, uint32_t au32_MIN, uint32_t au32_MAX)
{
	if(au32_IN < au32_MIN)
	{
		return au32_MIN;
	}
	else if (au32_IN > au32_MAX)
	{
		return au32_MAX;
	}
	else
	{
		return au32_IN;
	}
}

MATH_DataType MIN(MATH_DataType* IN_Arr, uint32_t au32_LEN)
{
	uint32_t i = 0;
	MATH_DataType MIN = 0;

	for(i=0; i<au32_LEN; i++)
	{
		if(IN_Arr[i] < MIN)
		{
			MIN = IN_Arr[i];
		}
	}
	return MIN;
}

MATH_DataType MAX(MATH_DataType* IN_Arr, uint32_t au32_LEN)
{
	uint32_t i = 0;
	MATH_DataType MAX = 0;

	for(i=0; i<au32_LEN; i++)
	{
		if(IN_Arr[i] > MAX)
		{
			MAX = IN_Arr[i];
		}
	}
	return MAX;
}
