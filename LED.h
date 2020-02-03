#ifndef LED_H_
#define LED_H_

#include "std_types.h"
#include "DIO.h"


void LED_ON (const uint8 Led_port , const uint8 Led_pin);
void LED_OFF (const uint8 Led_port , const uint8 Led_pin);
void LED_TOGGLE (const uint8 Led_port , const uint8 Led_pin);


#endif /* LED_H_ */