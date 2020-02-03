#ifndef __SW_H__
#define __SW_H__


#include "Switch_cnfg.h"


#define SWITCH_PRESSED_TIME_PERIOD_MS (500)

/* SWITCH_Update period */
#define SWITCH_PERIOD_MS   (50)

typedef enum
{
    SWITCH_1=0,
	SWITCH_2
}type_Enum_SWITCH_Selection;

typedef enum
{
    SWITCH_RELEASED,
    SWITCH_PRE_PRESSED,
    SWITCH_PRESSED,
    SWITCH_PRE_RELEASED
}type_Enum_SWITCH_State;


ACK Switch_Init(void);
type_Enum_SWITCH_State SWITCH_GetState(type_Enum_SWITCH_Selection );
void SWITCH_Update(void);
void Switch_Calculate_Pressed_time (type_Enum_SWITCH_Selection Switch);      
 
//------------------------------------------------- Section of Switch without using OS ------------------------------------------------------------------------------//


uint8 Switch_Getvalue(SWITCH_Select_t Switch_Select);

#endif // __SW_H__
