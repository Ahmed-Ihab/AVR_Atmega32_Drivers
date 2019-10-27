/*
 * DIO_cnfg.h
 *
 *  Created on: Sep 22, 2019
 *      Author: Mohammed
 */

#ifndef _Switch_CNFG_H_
#define _Switch_CNFG_H_

#include "std_types.h"
#include <avr/io.h>
#include <util/delay.h>
#include "DIO.h"

/*
typedef enum
{
	 SWITCH_MINUS,
	 SWITCH_PLUS

} SWITCH_Select_t;


typedef enum
{
    SW_RELEASED,
    SW_PRE_PRESSED,
    SW_PRESSED,
    SW_PRE_RELEASED
} SWITCH_State_t;

*/


#define MAX_NUMBER_OF_SWITCHES (uint8)32u

#define SWITCH_PLUS_PORT_DR     (PORT_C)
#define SWITCH_PLUS_PORT_CR     (DDRC)
#define SWITCH_PLUS_PIN         (PIN4)

#define SWITCH_PLUS_PORT_DR_MIRROR (PORT_D)
#define SWITCH_PLUS_PORT_CR_MIRROR (DDRD)
#define SWITCH_PLUS_PIN_MIRROR	   (PIN3)

#define SWITCH_MINUS_PORT_DR    (PORT_B)
#define SWITCH_MINUS_PORT_CR    (DDRB)
#define SWITCH_MINUS_PIN        (PIN2)


typedef enum {SWITCH_MINUS=0,SWITCH_PLUS=1,NUMBER_OF_SWITCHES=3}SWITCH_Select_t; // types


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
