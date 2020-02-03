
/*
 * PWM.c
 *
 *  Created on: Sep 24, 2019
 *      Author: Hoba
 */


#ifndef PWM_H_
#define PWM_H_

//-------------------  File_Inculsion  -------------------------------//
#include "PWM_Cfg.h"
//-------------------  Enum  -------------------------------//

//-------------------  Macro_Constants -------------------------------//

//-------------------  Public_Functions-------------------------------//
Std_Func_t PWM_Init (void);
Std_Func_t PWM_Timer1_Generate (float Required_duty_cycle , double Required_Frequency , Timer1_Channel_t Channel_Select , PWM_type_t PWM_type );
ACK PWM_generate ( TIMER_t TIMER_Select , float Required_duty_cycle , PWM_type_t PWM_type );

//-------------------  Public_Global_Variables -----------------------//
extern uint8 PWM_Value;

#endif