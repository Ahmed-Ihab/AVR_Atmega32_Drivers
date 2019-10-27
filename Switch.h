#ifndef __SWITCH_H__
#define __SWITCH_H__

#include "Switch_cnfg.h"




ACK Switch_Init(void);
uint8 Switch_GetState(SWITCH_Select_t Switch_Select);
void Switch_Update(void);


#endif // __SWITCH_H__
