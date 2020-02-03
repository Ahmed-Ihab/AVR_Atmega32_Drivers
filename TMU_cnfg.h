/*
 * TMU_cnfg.h
 *
 *  Created on: Oct 12, 2019
 *      Author: PeterKleber and Ehab
 */

#ifndef TMU_CNFG_H_
#define TMU_CNFG_H_

#include"std_types.h"
#include"Timer.h"

typedef enum {
	TIMER_0, TIMER_1, TIMER_2
} Timer_ID;

typedef struct {
	Timer_ID timer;
	uint8 Resolution;
} TMU_ConfigType ;

extern TMU_ConfigType TMU_cnfg ;

#endif /* TMU_CNFG_H_ */
