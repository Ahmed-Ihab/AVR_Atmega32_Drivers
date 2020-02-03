/*
 * DIO.c

 *      Author: HOBA
 */


#include "DIO.h"



ACK DIO_init_PORT(uint8 PORT , uint8 DIRECTION, uint8 PULL_RESISTOR)
{
	ACK STATE = AK;
	switch (PORT)
	{
		case PORT_A:
		
		if (DIRECTION == OUTPUT)
		{
			DDRA = 0xFF;
		}
		else if (DIRECTION == INPUT)
		{
			DDRA = 0x00;
			
			if ( PULL_RESISTOR == PULLDOWN)
			{
				PORTA = 0x00;
			}
			else if ( PULL_RESISTOR == PULLUP)
			{
				PORTA = 0xFF;
			}

		}
		break;
		case PORT_B:
		
		if (DIRECTION == OUTPUT)
		{
			DDRB = 0xFF;
		}
		else if (DIRECTION == INPUT)
		{
			DDRB = 0x00;
			if ( PULL_RESISTOR ==PULLDOWN)
			{
				
				PORTB = 0x00;
			}
			else if ( PULL_RESISTOR ==PULLUP)
			{
				
				PORTB = 0xFF;
			}
		}
		break;
		
		case PORT_C:
		if (DIRECTION == OUTPUT)
		{
			DDRC = 0xFF;
		}
		else if (DIRECTION == INPUT)
		{
			DDRC = 0x00;
			if ( PULL_RESISTOR ==PULLDOWN)
			{
				PORTC = 0x00;
			}
			else if ( PULL_RESISTOR == PULLUP)
			{
				
				PORTC =0xFF;
			}
		}
		break;
		
		case PORT_D:
		
		if (DIRECTION == OUTPUT)
		{
			DDRD =0xFF;;
		}
		else if ( DIRECTION == INPUT)
		{
			DDRD = 0x00;
			if ( PULL_RESISTOR ==PULLDOWN)
			{
				PORTD = 0x00;
			}
			else if ( PULL_RESISTOR ==PULLUP)
			{
				PORTD = 0xFF;
			}
		}
		break;

		default:
		STATE=NAK;
		break;
	}

	return STATE ;

}


ACK DIO_init_PIN(uint8 PORT ,uint8 PIN, uint8 DIRECTION, uint8 PULL_RESISTOR)
{
	ACK STATE = AK;
	 switch (PORT)
		  				{
		                case PORT_A:
						
							if (DIRECTION == OUTPUT)
							{
								DDRA |=(1u<<PIN);
								PORTA &= ~(1u<<PIN);
							}
							else if (DIRECTION == INPUT)
							{							
								DDRA &= ~(1u<<PIN);
								
								if ( PULL_RESISTOR ==PULLDOWN)
								{
									PORTA &= ~(1u<<PIN);
								}
								else if ( PULL_RESISTOR == PULLUP)
								{
									PORTA |= (1u<<PIN);
								}

							}
						break;
						case PORT_B:
						
							if (DIRECTION == OUTPUT)
							{
								DDRB |=(1u<<PIN);
								PORTB &= ~(1u<<PIN);
							}
							else if (DIRECTION == INPUT)
							{
								DDRB &=~(1u<<PIN);
								if ( PULL_RESISTOR ==PULLDOWN)
								{
										
									PORTB &= ~(1u<<PIN);
								}
								else if ( PULL_RESISTOR ==PULLUP)
								{	
								
									PORTB |= (1u<<PIN);
								}
							}
						break;
						case PORT_C:
							if (DIRECTION == OUTPUT)
							{
								DDRC |=(1u<<PIN);
								PORTC &= ~(1u<<PIN);
							}
							else if (DIRECTION == INPUT)
							{
								DDRC &= ~(1u<<PIN);
								if ( PULL_RESISTOR ==PULLDOWN)
								{
									PORTC &= ~(1u<<PIN);
								}
								else if ( PULL_RESISTOR == PULLUP)
								{
									
									PORTC |= (1u<<PIN);
								}
							}
						break;
						case PORT_D:
					
							if (DIRECTION == OUTPUT)
							{
								DDRD |=(1u<<PIN);
								PORTD &= ~(1u<<PIN);
							}
							else if ( DIRECTION == INPUT)
							{
								DDRD &=~(1u<<PIN);
								if ( PULL_RESISTOR ==PULLDOWN)
								{
									PORTD &= ~(1u<<PIN);
								}
								else if ( PULL_RESISTOR ==PULLUP)
								{
									PORTD |= (1u<<PIN);
								}
							}
						break;

						default:
							STATE=NAK;
							break;
		  				}

	return STATE ;
}



ACK DIO_init(void){
	ACK STATE = AK;
			uint8 loop_index ;
			if (NUM_OF_PINS>MAX_NUMBER_OF_PINS){  STATE = NAK; }
			else {
				for (loop_index=0 ; loop_index < NUM_OF_PINS ; loop_index++)
				{
			DIO_cnfg_arr[loop_index].IS_init=INITIALIZED;
					switch(DIO_cnfg_arr[loop_index].PORT)
					{

					case PORT_A:
						if (DIO_cnfg_arr[loop_index].DIR==OUTPUT)
					{
						DDRA |=(1u<<DIO_cnfg_arr[loop_index].PIN);
						if (DIO_cnfg_arr[loop_index].VALUE==HIGH)
						{
							PORTA |=(1u<<DIO_cnfg_arr[loop_index].PIN);
						}
						else if (DIO_cnfg_arr[loop_index].VALUE==LOW)
						{
							PORTA &= ~(1u<<DIO_cnfg_arr[loop_index].PIN);
						}
						
						else if (DIO_cnfg_arr[loop_index].VALUE==NA)
						{
							PORTA &= ~(1u<<DIO_cnfg_arr[loop_index].PIN);
						}
							
						else
						{
							DIO_cnfg_arr[loop_index].IS_init=NOT_INITIALIZED;
							STATE=NAK;
						}
					}
					else if (DIO_cnfg_arr[loop_index].DIR==INPUT)
					{
						DDRA &= ~(1u<<DIO_cnfg_arr[loop_index].PIN);
						if (DIO_cnfg_arr[loop_index].PULL==PULLUP)
						{
							PORTA |=(1u<<DIO_cnfg_arr[loop_index].PIN);

						}
						else if (DIO_cnfg_arr[loop_index].PULL==PULLDOWN)
						{
							PORTA &= ~(1u<<DIO_cnfg_arr[loop_index].PIN);
						}
						else
						{
							DIO_cnfg_arr[loop_index].IS_init=NOT_INITIALIZED;
							STATE=NAK;
						}
					}
					else
					{
						DIO_cnfg_arr[loop_index].IS_init=NOT_INITIALIZED;
						STATE=NAK;
					}
					break;

					case PORT_B:
							if (DIO_cnfg_arr[loop_index].DIR==OUTPUT)
							{
								DDRB |=(1u<<DIO_cnfg_arr[loop_index].PIN);
								if (DIO_cnfg_arr[loop_index].VALUE==HIGH)
								{
									PORTB |=(1u<<DIO_cnfg_arr[loop_index].PIN);
								}
								else if (DIO_cnfg_arr[loop_index].VALUE==LOW)
								{
									PORTB &= ~(1u<<DIO_cnfg_arr[loop_index].PIN);
								}
								
								else if (DIO_cnfg_arr[loop_index].VALUE==NA)
								{
									PORTB &= ~(1u<<DIO_cnfg_arr[loop_index].PIN);
								}
								else
								{
									DIO_cnfg_arr[loop_index].IS_init=NOT_INITIALIZED;
									STATE=NAK;
								}
							}
							else if (DIO_cnfg_arr[loop_index].DIR==INPUT)
							{
								DDRB &= ~(1u<<DIO_cnfg_arr[loop_index].PIN);
								if (DIO_cnfg_arr[loop_index].PULL==PULLUP)
								{
									PORTB |=(1u<<DIO_cnfg_arr[loop_index].PIN);

								}
								else if (DIO_cnfg_arr[loop_index].PULL==PULLDOWN)
								{
									PORTB &= ~(1u<<DIO_cnfg_arr[loop_index].PIN);
								}
								else
								{
									DIO_cnfg_arr[loop_index].IS_init=NOT_INITIALIZED;
															STATE=NAK;
								}
							}
							else
							{
								DIO_cnfg_arr[loop_index].IS_init=NOT_INITIALIZED;
								STATE=NAK;
							}
							break;

					case PORT_C:
								if (DIO_cnfg_arr[loop_index].DIR==OUTPUT)
							{
								DDRC |=(1u<<DIO_cnfg_arr[loop_index].PIN);
								if (DIO_cnfg_arr[loop_index].VALUE==HIGH)
								{
									PORTC |=(1u<<DIO_cnfg_arr[loop_index].PIN);
								}
								else if (DIO_cnfg_arr[loop_index].VALUE==LOW)
								{
									PORTC &= ~(1u<<DIO_cnfg_arr[loop_index].PIN);
								}
								
								else if (DIO_cnfg_arr[loop_index].VALUE==NA)
								{
									PORTC &= ~(1u<<DIO_cnfg_arr[loop_index].PIN);
								}
								
								else
								{
									DIO_cnfg_arr[loop_index].IS_init=NOT_INITIALIZED;
									STATE=NAK;
								}
							}
							else if (DIO_cnfg_arr[loop_index].DIR==INPUT)
							{
								DDRC &= ~(1u<<DIO_cnfg_arr[loop_index].PIN);
								if (DIO_cnfg_arr[loop_index].PULL==PULLUP)
								{
									PORTC |=(1u<<DIO_cnfg_arr[loop_index].PIN);

								}
								else if (DIO_cnfg_arr[loop_index].PULL==PULLDOWN)
								{
									PORTC &= ~(1u<<DIO_cnfg_arr[loop_index].PIN);
								}
								else
								{
									DIO_cnfg_arr[loop_index].IS_init=NOT_INITIALIZED;
									STATE=NAK;
								}
							}
							else
							{
								DIO_cnfg_arr[loop_index].IS_init=NOT_INITIALIZED;
								STATE=NAK;
							}
							break;

					case PORT_D:
								if (DIO_cnfg_arr[loop_index].DIR==OUTPUT)
							{
								DDRD |=(1u<<DIO_cnfg_arr[loop_index].PIN);
								if (DIO_cnfg_arr[loop_index].VALUE==HIGH)
								{
									PORTD |=(1u<<DIO_cnfg_arr[loop_index].PIN);
								}
								else if (DIO_cnfg_arr[loop_index].VALUE==LOW)
								{
									PORTD &= ~(1u<<DIO_cnfg_arr[loop_index].PIN);
								}
								
								else if (DIO_cnfg_arr[loop_index].VALUE==NA)
								{
									PORTD &= ~(1u<<DIO_cnfg_arr[loop_index].PIN);
								}
								else
								{
									DIO_cnfg_arr[loop_index].IS_init=NOT_INITIALIZED;
									STATE=NAK;
								}
							}
							else if (DIO_cnfg_arr[loop_index].DIR==INPUT)
							{
								DDRD &= ~(1u<<DIO_cnfg_arr[loop_index].PIN);
								if (DIO_cnfg_arr[loop_index].PULL==PULLUP)
								{
									PORTD |=(1u<<DIO_cnfg_arr[loop_index].PIN);

								}
								else if (DIO_cnfg_arr[loop_index].PULL==PULLDOWN)
								{
									PORTD &= ~(1u<<DIO_cnfg_arr[loop_index].PIN);
								}
								else
								{
									DIO_cnfg_arr[loop_index].IS_init=NOT_INITIALIZED;
															STATE=NAK;
								}
							}
							else
							{
								DIO_cnfg_arr[loop_index].IS_init=NOT_INITIALIZED;
								STATE=NAK;
							}
							break;

					}

				}

    }
return STATE ;
}




ACK DIO_read(uint8 PORT ,uint8 PIN,uint8 *VALUE)
{
	PORTD |= (1<<PD0);
	ACK STATE = AK;
		uint8 loop_index ;
		if (NUM_OF_PINS>MAX_NUMBER_OF_PINS){  STATE = NAK; }
		else {
			for (loop_index=0 ; loop_index < NUM_OF_PINS ; loop_index++)
			{
				STATE = AK;
				 if( (DIO_cnfg_arr[loop_index].PORT==PORT) && (DIO_cnfg_arr[loop_index].PIN==PIN) && (DIO_cnfg_arr[loop_index].IS_init==INITIALIZED ) && (DIO_cnfg_arr[loop_index].DIR==INPUT) )
				  {
					 switch (PORT)
					 				{
					 					case PORT_A:
					 						*VALUE = (1u&(PINA>>PIN));
					 					break;
					 					case PORT_B:
					 					    *VALUE = (1u&(PINB>>PIN));
					 					break;
					 					case PORT_C:
					 						*VALUE = (1u&(PINC>>PIN));
					 					break;
					 					case PORT_D:
					 						*VALUE = (1u&(PIND>>PIN));
					 					break;
					 				}

				  }
				  else
				  {
					  STATE = NAK;
				  }
			}
	loop_index = MAX_NUMBER_OF_PINS+1;
		}



		return STATE ;
}



ACK DIO_read_Pin(uint8 PORT ,uint8 PIN,uint8 *VALUE)
{
	ACK STATE = AK;
	
	if(VALUE == NULL_PTR)
	{
		STATE = NAK ;
		return STATE;
	}
	
		if( (PIN<8) || (PIN>=0) )
		{
				switch (PORT)
				{
					case PORT_A:
					*VALUE = (1u&(PINA>>PIN));
					break;
					case PORT_B:
					*VALUE = (1u&(PINB>>PIN));
					break;
					case PORT_C:
					*VALUE = (1u&(PINC>>PIN));
					break;
					case PORT_D:
					*VALUE = (1u&(PIND>>PIN));
					break;
					default:
					STATE = NAK;
					break;
				}
		}
	
		// if the condition  ((PIN<8) || (PIN>0) )  is not true
		else
		{
			STATE = NAK ;
		}

	
	return STATE ;

}


ACK DIO_write(uint8 PORT ,uint8 PIN,uint8 VALUE){

	ACK STATE = AK;
	uint8 loop_index ;
	
	if (NUM_OF_PINS > MAX_NUMBER_OF_PINS)
	{ 
		 STATE = NAK; 
	}
	
	else {
	for (loop_index=0 ; loop_index < NUM_OF_PINS ; loop_index++)
	{
		
		STATE = AK;
		if( (DIO_cnfg_arr[loop_index].PORT==PORT) && (DIO_cnfg_arr[loop_index].PIN==PIN) && (DIO_cnfg_arr[loop_index].IS_init==INITIALIZED ) && (DIO_cnfg_arr[loop_index].DIR==OUTPUT) && ((DIO_cnfg_arr[loop_index].VALUE==HIGH ) || (DIO_cnfg_arr[loop_index].VALUE==LOW ) ) )
  {
	  switch (PORT)
	  				{
	                case PORT_A:
						if (VALUE == HIGH)
						{
							PORTA |=(1u<<PIN);
						}
						else
						{
							PORTA &=~(1u<<PIN);
						}
					break;
					case PORT_B:
						if (VALUE == HIGH)
						{
							PORTB |=(1u<<PIN);
						}
						else
						{
							PORTB &=~(1u<<PIN);
						}
					break;
					case PORT_C:
						if (VALUE == HIGH)
						{
							PORTC |=(1u<<PIN);
						}
						else
						{
							PORTC &=~(1u<<PIN);
						}
					break;
					case PORT_D:
						if (VALUE == HIGH)
						{
							PORTD |= (1<<PD2);
							PORTD |=(1u<<PIN);
						}
						else
						{
							PORTD &=~(1u<<PIN);
						}
					break;
				}
	  loop_index = MAX_NUMBER_OF_PINS+1;
  }
  else
  {

	  STATE = NAK;
  }

	}

	}
return STATE ;
}



ACK DIO_Write_Pin(uint8 PORT ,uint8 PIN,uint8 VALUE)
{

	ACK STATE = AK;
	
		if(  (PIN>=0) && (PIN<8))
		{
			
					if((VALUE == HIGH) || (VALUE == LOW))
					{
							switch (PORT)
							{
								case PORT_A:
								if (VALUE == HIGH)
								{
									PORTA |=(1u<<PIN);
								}
								else
								{
									PORTA &=~(1u<<PIN);
								}
								break;
								case PORT_B:
								if (VALUE == HIGH)
								{
									PORTB |=(1u<<PIN);
								}
								else
								{
									PORTB &=~(1u<<PIN);
								}
								break;
								case PORT_C:
								if (VALUE == HIGH)
								{
									PORTC |=(1u<<PIN);
								}
								else
								{
									PORTC &=~(1u<<PIN);
								}
								break;
								
								case PORT_D:
								if (VALUE == HIGH)
								{
									PORTD |=(1u<<PIN);
								}
								else
								{
									PORTD &=~(1u<<PIN);
								}
								break;
								
								default:
								STATE = NAK;
								break;
							}
					}
					// if the condition (VALUE == HIGH) || (VALUE == LOW) is not true
					else
					{
						STATE = NAK ;
					}
			
			}
			
			// if the condition  ((PIN<8) || (PIN>0) )  is not true 
			else
			{
				STATE = NAK ;
			}

			
	return STATE ;
}



ACK DIO_Write_PORT(uint8 PORT,uint8 VALUE)
{

	ACK STATE = AK;
	
			switch (PORT)
			{
				case PORT_A:

					PORTA=VALUE;
				
				break;
				
				case PORT_B:

					PORTB = VALUE;

				break;
				case PORT_C:

					PORTC = VALUE;

				break;
				
				case PORT_D:

					PORTD = VALUE;
				break;
				
				default:
				STATE = NAK;
				break;
			}

	
	return STATE ;
}
