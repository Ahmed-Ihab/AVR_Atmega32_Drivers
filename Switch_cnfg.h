/*
 * Switch_Cnfg.c
 *      Author: Ehab
 */

#ifndef _Switch_CNFG_H_
#define _Switch_CNFG_H_

#include "std_types.h"
#include "DIO.h"



#define MAX_NUMBER_OF_SWITCHES (uint8)32u



typedef enum {_SWITCH_1_,_SWITCH_2_,NUMBER_OF_SWITCHES}SWITCH_Select_t; // types


typedef struct
{
	uint8 PORT;
	uint8 PIN;
	uint8 DIR;
    uint8 PULL_RESISTOR;
	uint8 IS_init;
	
}Switch_Cnfg_t;


extern Switch_Cnfg_t Switch_Cnfg_arr[NUMBER_OF_SWITCHES];

#endif  //_Switch_CNFG_H_
