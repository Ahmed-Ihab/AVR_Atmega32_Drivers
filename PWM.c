
/*
 * PWM.c
 *
 *  Created on: Sep 24, 2019
 *      Author: Hoba
 */


#include "PWM.h"

//-------------------  Private_Global_Varibles -------------------------------//
static uint16 ICR_Value=0;

//-------------------  Private_Function --------------------------------------//
static Std_Func_t Fill_OCR_Value( Timer1_Channel_t Channel_Select , float Required_duty_cycle);
static Std_Func_t Fill_ICR_Value (double Required_Frequency);

//-------------------  Public_Global_Variables -------------------------------//

uint8 PWM_Value = 0;

//-------------------  Implementation of Functions -------------------------------//


Std_Func_t PWM_Init (void)
{
	Std_Func_t Error_t = OK;
	uint8 Loop_index=0;
	
	if (NUM_OF_PWM > MAX_NUM_OF_PWM)
	{
		Error_t = NOK;
	}
	
	for(Loop_index = 0; Loop_index < NUM_OF_PWM ; Loop_index++)
	{
		if( (TIMER_cnfg_arr[PWM_Cfg_arr[Loop_index].Timer_PWM_ID].IS_init ==  INITIALIZED ) && (PWM_Cfg_arr[Loop_index].IS_init == INITIALIZED) )
		{
			switch (TIMER_cnfg_arr[PWM_Cfg_arr[Loop_index].Timer_PWM_ID].timer)
			{
				case Timer0_PWM:
				{	
					if (PWM_Flag_mode[TIMER_cnfg_arr[PWM_Cfg_arr[Loop_index].Timer_PWM_ID].timer] == PWM_MODE) 
					{
						TIMER_ID_init(Timer0_PWM);
						TCNT0=0;
						OCR0=0;
						DDRB |= (1u<<PB3);  // OCO PIN OUTPUT
						if(TIMER_cnfg_arr[PWM_Cfg_arr[Loop_index].Timer_PWM_ID].WGM_mode == FAST_PWM_mode)
						{
							TCCR0 |= ((1u << WGM01) | (1u << WGM00)); // FAST PWM MODE WGM01=1 & WGM00=1
							
							if(PWM_Cfg_arr[Loop_index].PWM_type == Inverting_PWM)
							{
								OCR0=0xff;
								TCCR0 |= (1u << COM01); // INVERTING COM01=1
								TCCR0 &= (~(1u << COM00));  // INVERTING COM00=0						
							}
							
							else if (PWM_Cfg_arr[Loop_index].PWM_type == NON_INVERTING_PWM)
							{
								TCCR0 |= ((1u << COM01) | (1u << COM00)); // NON_INVERTING COM01=1 & COM00=1
							}
							
							else
							{
								Error_t = NOK;
								//Shouldn't be here
							}
						}
						
						else if(TIMER_cnfg_arr[PWM_Cfg_arr[Loop_index].Timer_PWM_ID].WGM_mode == PHASE_CORRECT_mode)
						{
							TCCR0 &= (~(1u << WGM01)); // PHASE CORRECT MODE  WGM01=0
							TCCR0 |= (1u << WGM00);  // PHASE CORRECT MODE  WGM00=1
							
							
							if(PWM_Cfg_arr[Loop_index].PWM_type == Inverting_PWM)
							{
								OCR0=0xff;
								TCCR0 |= (1u << COM01); // INVERTING COM01=1
								TCCR0 &= (~(1u << COM00));  // INVERTING COM00=0
								
							}
							
							else if (PWM_Cfg_arr[Loop_index].PWM_type == NON_INVERTING_PWM)
							{
								TCCR0 |= ((1u << COM01) | (1u << COM00)); // NON_INVERTING COM01=1 & COM00=1
								
							}
							
							else
							{
								Error_t = NOK;
								PWM_Cfg_arr[Loop_index].IS_init = NOT_INITIALIZED;
								//Shouldn't be here
							}
							
						}
						
						else
						{
							Error_t = NOK;
							PWM_Cfg_arr[Loop_index].IS_init = NOT_INITIALIZED;
							//Shouldn't be here
						}
					}
					
					else if (PWM_Flag_mode[TIMER_cnfg_arr[PWM_Cfg_arr[Loop_index].Timer_PWM_ID].timer] == NON_PWM_MODE)
					{
						PWM_Cfg_arr[Loop_index].IS_init = NOT_INITIALIZED;
					}
					
					else
					{
						PWM_Cfg_arr[Loop_index].IS_init = NOT_INITIALIZED;
						//Shouldn't be here
					}
					
					break;
				}
				
				case Timer1_PWM:
				{
					
					if (PWM_Flag_mode[TIMER_cnfg_arr[PWM_Cfg_arr[Loop_index].Timer_PWM_ID].timer] == PWM_MODE)
					{
						TIMER_ID_init(Timer1_PWM);
						TCNT1=0;
						OCR1A=0;
						OCR1B=0;
						DDR_TIMER_1 |= (1<<OC1A);
						DDR_TIMER_1 |= (1<<OC1B);
						
						if(TIMER_cnfg_arr[PWM_Cfg_arr[Loop_index].Timer_PWM_ID].WGM_mode == FAST_PWM_mode)
						{
							FAST_PWM_8_BIT();
							
							if(PWM_Cfg_arr[Loop_index].PWM_type == Inverting_PWM)
							{
								OCR1A=0xffff;
								OCR1B=0xffff;
								COM_1A_PWM_INVERTED();
								COM_1B_PWM_INVERTED();
							}
							
							else if (PWM_Cfg_arr[Loop_index].PWM_type == NON_INVERTING_PWM)
							{
								COM_1A_PWM_NON_INVERTED();
								COM_1B_PWM_NON_INVERTED();
								
							}
							
							else
							{
								Error_t = NOK;
								PWM_Cfg_arr[Loop_index].IS_init = NOT_INITIALIZED;
								//Shouldn't be here
							}
						}
						
						else if(TIMER_cnfg_arr[PWM_Cfg_arr[Loop_index].Timer_PWM_ID].WGM_mode == PHASE_CORRECT_mode)
						{
							PWM_PHASE_CORRECT_8_BIT();
							
							if(PWM_Cfg_arr[Loop_index].PWM_type == Inverting_PWM)
							{
								OCR1A=0xffff;
								OCR1B=0xffff;
								COM_1A_PWM_INVERTED();
								COM_1B_PWM_INVERTED();	
							}
							
							else if (PWM_Cfg_arr[Loop_index].PWM_type == NON_INVERTING_PWM)
							{
								COM_1A_PWM_NON_INVERTED();
								COM_1B_PWM_NON_INVERTED();								
							}
							
							else
							{
								Error_t = NOK;
								PWM_Cfg_arr[Loop_index].IS_init = NOT_INITIALIZED;
								//Shouldn't be here
							}
							
						}
						
						else
						{
							Error_t = NOK;
							PWM_Cfg_arr[Loop_index].IS_init = NOT_INITIALIZED;
							//Shouldn't be here
						}
					}
					
					else if (PWM_Flag_mode[TIMER_cnfg_arr[PWM_Cfg_arr[Loop_index].Timer_PWM_ID].timer] == NON_PWM_MODE)
					{
						
						PWM_Cfg_arr[Loop_index].IS_init = NOT_INITIALIZED;
					}
					
					else
					{
						Error_t = NOK;
						PWM_Cfg_arr[Loop_index].IS_init = NOT_INITIALIZED;
						//Shouldn't be here
					}
					
					break;
				}
				
				case Timer2_PWM:
				{
					if (PWM_Flag_mode[TIMER_cnfg_arr[PWM_Cfg_arr[Loop_index].Timer_PWM_ID].timer] == PWM_MODE)
					{
						TIMER_ID_init(Timer2_PWM);
						TCNT2=0;
						OCR2=0;
						DDRD |= (1u<<PD7);
						if(TIMER_cnfg_arr[PWM_Cfg_arr[Loop_index].Timer_PWM_ID].WGM_mode == FAST_PWM_mode)
						{
							TCCR2 |= ((1u << WGM21) | (1u << WGM20)); // FAST PWM MODE WGM21=1 & WGM20=1
							
							
							if(PWM_Cfg_arr[Loop_index].PWM_type == Inverting_PWM)
							{
								TCCR2 |= (1u << COM21);		// INVERTING COM21=1
								TCCR2 &= (~(1u << COM20));  // INVERTING COM20=0
								OCR2=0xFF;
							}
							
							else if (PWM_Cfg_arr[Loop_index].PWM_type == NON_INVERTING_PWM)
							{
								TCCR2 |= ((1u << COM21) | (1u << COM20)); // NON_INVERTING COM21=1 & COM20=1
							}
							
							else
							{
								Error_t = NOK;
								PWM_Cfg_arr[Loop_index].IS_init = NOT_INITIALIZED;
								//Shouldn't be here
							}
						}
						
						else if(TIMER_cnfg_arr[PWM_Cfg_arr[Loop_index].Timer_PWM_ID].WGM_mode == PHASE_CORRECT_mode)
						{
							TCCR2 &= (~(1u << WGM21)); // PHASE CORRECT MODE  WGM21=0
							TCCR2 |= (1u << WGM20);  // PHASE CORRECT MODE  WGM20=1
							
							if(PWM_Cfg_arr[Loop_index].PWM_type == Inverting_PWM)
							{
								TCCR2 |= (1u << COM21);		// INVERTING COM21=1
								TCCR2 &= (~(1u << COM20));  // INVERTING COM20=0
							}
							
							else if (PWM_Cfg_arr[Loop_index].PWM_type == NON_INVERTING_PWM)
							{
								TCCR2 |= ((1u << COM21) | (1u << COM20)); // NON_INVERTING COM21=1 & COM20=1
							}
							
							else
							{
								Error_t = NOK;
								PWM_Cfg_arr[Loop_index].IS_init = NOT_INITIALIZED;
								//Shouldn't be here
							}
							
						}
						
						else
						{
							Error_t = NOK;
							PWM_Cfg_arr[Loop_index].IS_init = NOT_INITIALIZED;
							//Shouldn't be here
						}
					}
					
					else if (PWM_Flag_mode[TIMER_cnfg_arr[PWM_Cfg_arr[Loop_index].Timer_PWM_ID].timer] == NON_PWM_MODE)
					{
						PWM_Cfg_arr[Loop_index].IS_init = NOT_INITIALIZED;
					}
					
					else
					{
						PWM_Cfg_arr[Loop_index].IS_init = NOT_INITIALIZED;
						//Do Nothing
					}
					
					break;
				}
				
				
				default:
				{
					Error_t = NOK;
					break;
				}
			}
			
			
		}
		
		else if( TIMER_cnfg_arr[PWM_Cfg_arr[Loop_index].Timer_PWM_ID].IS_init ==  NOT_INITIALIZED )
		{
			continue;
		}
		
		else
		{
			Error_t = NOK;
			PWM_Cfg_arr[Loop_index].IS_init = NOT_INITIALIZED;
			//Shouldn't be here
		}
	}
	
	return Error_t;
}

Std_Func_t PWM_Timer1_Generate (float Required_duty_cycle , double Required_Frequency ,  Timer1_Channel_t Channel_Select , PWM_type_t PWM_type )
{
	//TIMER_ID_init(TIMER1);
	
	Std_Func_t Error_t = OK; 
	Std_Func_t State = OK;
	
		
		if ( (TIMER_cnfg_arr[TIMER1].IS_init == INITIALIZED ) )
		{
			if(PWM_Flag_mode[TIMER1] == PWM_MODE)
			{
				if(TIMER_cnfg_arr[TIMER1].WGM_mode == FAST_PWM_MODE )
				{
					FAST_PWM_ICR1();					//Force the WGM of Timer 1 to Mode 14
					State = Fill_ICR_Value(Required_Frequency);
				}
				
				else if(TIMER_cnfg_arr[TIMER1].WGM_mode == PHASE_CORRECT_MODE )
				{	
					PWM_PHASE_CORRECT_ICR1();			//Force the WGM of Timer 1 to Mode 10
					State = Fill_ICR_Value(Required_Frequency*2);
				}
				
				if(State == OK)	
				{
					Fill_OCR_Value(Channel_Select,Required_duty_cycle);
					
					if(PWM_type == NON_INVERTING_PWM )
					{
						switch (Channel_Select)
						{
							case Channel_A:
							{
								COM_1A_PWM_NON_INVERTED();
								break;
							}
							
							case Channel_B:
							{
								COM_1B_PWM_NON_INVERTED();
								break;
							}
							
							case CHANNEL_A_B:
							{
								COM_1A_PWM_NON_INVERTED();
								COM_1B_PWM_NON_INVERTED();
								break;
							}
							
							default:
							{
								Error_t = NOK;
								break;
							}
							
						}
					}  // End of else if (PWM_type == NON_INVERTING_PWM)
					
					else if (PWM_type == INVERTING_PWM)
					{
						switch (Channel_Select)
						{
							case Channel_A:
							{
								COM_1A_PWM_INVERTED();
								break;
							}
							
							case Channel_B:
							{
								COM_1B_PWM_INVERTED();
								break;
							}
							
							case CHANNEL_A_B:
							{
								COM_1A_PWM_INVERTED();
								COM_1B_PWM_INVERTED();
								break;
							}
							
							default:
							{
								Error_t = NOK;
								break;
							}
						}
					} // End of else if (PWM_type == INVERTING_PWM)
					
					else
					{
						Error_t = NOK;
					}
				} // End of if(State == OK) 
				
			} // End of if(PWM_Flag_mode[TIMER1] == PWM_MODE)
				
			else // else if (PWM_Flag_mode[TIMER1] != PWM_MODE)
			{
				Error_t = NOK;
			}						
			
			TIMER_Start(TIMER1);
		} // End of else if (TIMER_cnfg_arr[TIMER1].IS_init == INITIALIZED ) && (TIMER_cnfg_arr[TIMER1].timer == TIMER1 )
		
//	}	//End of For Loop
	
	return Error_t;	
}


Std_Func_t Fill_ICR_Value (double Required_Frequency)
{
	Std_Func_t Error_t = OK;
	double Frequency_of_Timer1 = 0;
	double Required_Period = 0;
	Frequency_of_Timer1 =   (F_CPU / Prescalar_Factor[TIMER_cnfg_arr[TIMER1].prescalar]);
	Required_Period = 1 / Required_Frequency;
	ICR_Value =  Required_Period * Frequency_of_Timer1;
	
	if( ICR_Value < 65535)
	{
		ICR1= ICR_Value;
	}
	else
	{
		Error_t = NOK;
	}
	
	return Error_t;
}

Std_Func_t Fill_OCR_Value( Timer1_Channel_t  Channel_Select , float Required_duty_cycle)
{
	uint16 Top;
	Std_Func_t Error_t = OK;
	Top=ICR_Value;

switch (Channel_Select)
{
	case CHANNEL_A :
	{
		OCR1A= (uint16) ( Top * (float)(Required_duty_cycle/100) );
		break;
	}

	case CHANNEL_B :
	{
	
		OCR1B= (uint16) ( Top * (float)(Required_duty_cycle/100) );
		break;
	}

	case CHANNEL_A_B :
	{
		OCR1A= (uint16) ( Top * (float)(Required_duty_cycle/100) );
		OCR1B= (uint16) ( Top * (float)(Required_duty_cycle/100) );
		break;
	}

	default:
	{
		Error_t = NOK;
		break;
	}
}
	return Error_t;

}


ACK PWM_generate ( TIMER_t TIMER_Select , float Required_duty_cycle , PWM_type_t PWM_type )
{

	ACK STATE = AK ;
	uint16 Top;

	if((TIMER_cnfg_arr[TIMER_Select].timer == TIMER_Select) && (TIMER_cnfg_arr[TIMER_Select].IS_init==INITIALIZED) )
	{
		switch(TIMER_Select)
		{
			case TIMER0:
			{
				OCR0 = (255 * (float)(Required_duty_cycle/100));

				break;
			}

			case TIMER1:
			{

				Top=0xFFFF;

				OCR1A= (uint16) ( Top * (float)(Required_duty_cycle/100) );
				break;
			}

			case TIMER2:
			{

				Top=255;
				OCR2 = (Top*(float)(Required_duty_cycle/100));

				break;
			}

			default:
			{
				// Should not be here
				STATE=NAK;
				break;
			}
		}

		switch(PWM_Flag_mode[TIMER_Select])
		{
			case NON_PWM_MODE:
			{
				if (TIMER_cnfg_arr[TIMER_Select].interrupt==INTERRUPT)
				{
					STATE= interrupt_PWM (TIMER_Select);

					if (PWM_type == INVERTING)
					{
						PWM_Value=0;
						//PWM_PORT &= ~(1<<PWM_PIN);


					}
					else if(PWM_type == NON_INVERTING)
					{
						PWM_Value=1;
						// PWM_PORT |= (1<<PWM_PIN);
					}
					
					break;
				}


				else
				{
					STATE=NAK;
					break;
				}

			}

			case PWM_MODE:
			{
				break;
			}

			default:
			{
				STATE=NAK;
				break;
			}
		}

	}

	else
	{
		STATE=NAK;
	}


	TIMER_Start(TIMER_Select);
	return STATE;
}

