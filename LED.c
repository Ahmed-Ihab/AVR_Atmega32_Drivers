
#include "LED.h"


void LED_TOGGLE (const uint8 Led_port , const uint8 Led_pin)
{
	static uint8 value = 0;
	DIO_Write_Pin(Led_port,Led_pin,value);
	value ^=1;
}

void LED_OFF  (const uint8 Led_port , const uint8 Led_pin)
{
	 uint8 value = 0;
	DIO_Write_Pin(Led_port,Led_pin,value);
}

void LED_ON (const uint8 Led_port , const uint8 Led_pin)
{
	 uint8 value = 1;
	DIO_Write_Pin(Led_port,Led_pin,value);
}
