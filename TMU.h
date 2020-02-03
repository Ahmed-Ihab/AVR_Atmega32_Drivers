/*
 * TMU.h
 *
 *  Created on: Oct 12, 2019
 *      Author: PeterKleber and Ehab
 */

#ifndef TMU_H_
#define TMU_H_

#include"std_types.h"
#include"TMU_cnfg.h"
#include "Timer.h"


#define BUFFER_SIZE (uint8)3

typedef enum {NOT_Active,Active}Status_t;

typedef enum {
 PERIODIC, ONE_SHOT
 } Rotation_t;


typedef enum {
	TMU_NOK, TMU_OK
} EnmTMUError_t;


typedef struct{
	void (*ptr) ();
	uint16 Start_time ;
	uint16 Delay_time ;
	Rotation_t Mode ;
	Status_t Status ;
}TMU_Buffer_t;

EnmTMUError_t TMU_Start_Timer(uint16 Time_Delay, void (*Ptr)(void), Rotation_t mode);
EnmTMUError_t TMU_Stop_Timer(void (*Ptr)(void));
EnmTMUError_t TMU_Init(const TMU_ConfigType *ConfigPtr);
EnmTMUError_t TMU_DeInit(void);
EnmTMUError_t TMU_Dispatch();

void ISR_Generated_Flag_Setter();

#endif /* TMU_H_ */
