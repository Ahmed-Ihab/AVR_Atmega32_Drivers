
/*
 * PWM.c
 *
 *  Created on: Sep 24, 2019
 *      Author: Hoba
 */


#ifndef _PWM_Cfg_H

#include "../MCAL/Timer/Timer.h"
#include "std_types.h"
#define _PWM_Cfg_H

#define MAX_NUM_OF_PWM	3

typedef enum {Timer0_PWM , Timer1_PWM , Timer2_PWM , NUM_OF_PWM}Timer_PWM_t;
typedef enum { Timer0_Channel , Timer1_Channel , Timer2_Channel}Timer_Channel_t;

typedef enum { Inverting_PWM=34 , Non_Inverting_PWM =35 }PWM_type_t;

typedef struct
{
	Timer_PWM_t Timer_PWM_ID ;
	PWM_type_t PWM_type;
	uint8 IS_init  ;
}ST_PWM_Cfg_t;

extern ST_PWM_Cfg_t PWM_Cfg_arr[MAX_NUM_OF_TIMERS];

#endif