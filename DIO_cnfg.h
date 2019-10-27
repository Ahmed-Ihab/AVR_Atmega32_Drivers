/*
 * DIO_cnfg.h
 *
 *  Created on: Sep 22, 2019
 *      Author: HOBA
 */

#ifndef DIO_CNFG_H_
#define DIO_CNFG_H_

#include "std_types.h"
#include <avr/io.h>

typedef enum {Switch_inc=0,Switch_Dec,NUM_OF_PINS=20}PINS_t; // types

#define PORT_A              'A'
#define PORT_B              'B'
#define PORT_C              'C'
#define PORT_D              'D'


#define MAX_NUMBER_OF_PINS (uint8)32


#define PIN0                0
#define PIN1                1
#define PIN2                2
#define PIN3                3
#define PIN4                4
#define PIN5                5
#define PIN6                6
#define PIN7                7

#define OUTPUT  	        1
#define INPUT		        0

#define PULLUP	            1
#define PULLDOWN            0


#define INITIALIZED			1
#define NOT_INITIALIZED		0


#define NA			        0xff


#define KEYPAD_PORT_	PORT_C
#define LCD_4Bits_PORT_ PORT_A


typedef struct
{
	uint8 PORT;
	uint8 PIN;
	uint8 DIR;
	uint8 VALUE;
	uint8 PULL;
	uint8 IS_init;
}DIO_cnfg_t;



extern DIO_cnfg_t DIO_cnfg_arr[NUM_OF_PINS];


#endif /* DIO_CNFG_H_ */
