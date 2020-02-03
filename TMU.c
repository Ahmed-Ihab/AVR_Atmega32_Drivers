/*
 * TMU.c
 *
 *  Created on: Oct 12, 2019
 *      Author: PeterKleber and Ehab
 */

#include "TMU.h"

static uint8 Buffer_ptr = 0;

volatile uint16 Current_Tick_Count = 0;
volatile uint8 ISR_Generated_Flag = 0;

static void Null_func() {
}

//Call back function for the ISR to set the flag
void ISR_Generated_Flag_Setter() {
	ISR_Generated_Flag = 1;
}

TMU_Buffer_t TMU_Buffer[BUFFER_SIZE ];

EnmTMUError_t TMU_Init(const TMU_ConfigType *ConfigPtr) {

	//Initialize the buffer
	for (uint8 i = 0; i < BUFFER_SIZE ; i++) {
		TMU_Buffer[i].ptr = Null_func;
		TMU_Buffer[i].Start_time = 0;
		TMU_Buffer[i].Delay_time = 0;
		TMU_Buffer[i].Mode = 2; //Outside the enum values
		TMU_Buffer[i].Status = NOT_Active;
	}

	TIMER_ID_init(ConfigPtr->timer);
	//send callback function
	if((ConfigPtr->timer)==TIMER_0){
	 Timer0_Set_Callback ( ISR_Generated_Flag_Setter );
	}
	else if((ConfigPtr->timer)==TIMER_1){
	 Timer1_Set_Callback ( ISR_Generated_Flag_Setter );
	}
	else if((ConfigPtr->timer)==TIMER_2){
	 Timer2_Set_Callback ( ISR_Generated_Flag_Setter );
	}

	 Time_Delay ( (ConfigPtr->timer)  , 1  , ms);
	 TIMER_Start (ConfigPtr->timer);

	return TMU_OK;
}

EnmTMUError_t TMU_Start_Timer(uint16 Time_Delay, void (*Ptr)(void),Rotation_t mode) {
	if (Buffer_ptr < BUFFER_SIZE) {
		TMU_Buffer[Buffer_ptr].ptr = Ptr;
		TMU_Buffer[Buffer_ptr].Start_time = Current_Tick_Count;
		TMU_Buffer[Buffer_ptr].Mode = mode;
		TMU_Buffer[Buffer_ptr].Delay_time = Time_Delay ;
		TMU_Buffer[Buffer_ptr].Status = Active;

		Buffer_ptr++;
	} else {
		return TMU_NOK; //no place in the buffer
	}


	return TMU_OK;
}

EnmTMUError_t TMU_Stop_Timer(void (*Ptr)(void)) {

	for (uint8 i = 0; i < Buffer_ptr; i++) {

		if (Ptr == TMU_Buffer[i].ptr) { //Search for the function in the buffer
			TMU_Buffer[i].Status = NOT_Active; //if it exist change it's state to not active
		}
	}

	return TMU_OK;
}

EnmTMUError_t TMU_DeInit(void) {
	//Clear the Buffer
	for (uint8 i = 0; i < Buffer_ptr; i++) {
		TMU_Buffer[i].ptr = Null_func;
		TMU_Buffer[i].Start_time = 0;
		TMU_Buffer[i].Delay_time = 0;
		TMU_Buffer[i].Start_time = 0;
		TMU_Buffer[i].Mode = 2; //Outside the enum values
		TMU_Buffer[i].Status = NOT_Active;
	}

	Buffer_ptr = 0; //return Buffer_ptr to zero

	return TMU_OK;
}

EnmTMUError_t TMU_Dispatch() {

	if (ISR_Generated_Flag == 1) {

		Current_Tick_Count++; //when an ISR happens increment the tick counter
		//if(Current_Tick_Count > 1001){Current_Tick_Count=0;}

		for (uint8 i = 0; i < Buffer_ptr; i++) {

			if (((Current_Tick_Count - TMU_Buffer[i].Start_time)% TMU_Buffer[i].Delay_time) == 0) {

				if (TMU_Buffer[i].Status == Active) {
					void (*Ptr_to_excute)(void) = TMU_Buffer[i].ptr;

					Ptr_to_excute();

					if (TMU_Buffer[i].Mode == ONE_SHOT) {
						TMU_Buffer[i].Status = NOT_Active;
					}
					else if (TMU_Buffer[i].Mode == PERIODIC) {
						TMU_Buffer[i].Start_time = Current_Tick_Count;
					}
					else {
						return TMU_NOK;
					}
					//break; //breaks from the for loop // excute only 1 function per loop
				}
			}
		}
	}
	ISR_Generated_Flag = 0 ;
	return TMU_OK;
}
