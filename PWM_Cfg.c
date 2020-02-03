
/*
 * PWM.c
 *
 *  Created on: Sep 24, 2019
 *      Author: Hoba
 */


#include "PWM_Cfg.h"


ST_PWM_Cfg_t PWM_Cfg_arr[NUM_OF_PWM]=
{
	// ---------------- Timer_ID ------------- // // ---------------- PWM_type ------------- // // ---------------- Initialized ------------- //
	{                     TIMER0                 ,                      Non_Inverting_PWM       ,                         NOT_INITIALIZED    },
	{                     TIMER1                 ,                      Non_Inverting_PWM       ,                          INITIALIZED       },
	{                     TIMER2                 ,                      Non_Inverting_PWM       ,                          NOT_INITIALIZED	 }
};