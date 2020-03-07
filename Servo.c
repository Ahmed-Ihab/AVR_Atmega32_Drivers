/*
 * Servo.c
 *
 *      Author:Ehab
 */

#include "Servo.h"

#define T_OFF_PERIOD		0
#define T_ON_PERIOD			1
#define SERVO_PWM_PERIOD	10
#define _2_ms_DURATION_STEP	250


static void Servo_Handler (void);

static uint8 Ton_Steps  =  255 ;
static uint8 Toff_Steps =  255 ;

static TIMER_t Timer_ID = NA;

void SERVO_init(TIMER_t Timer )
{
	DIO_init_PIN ( SERVO_SIGNAL_PORT_ , SERVO_SIGNAL_PIN , OUTPUT , NA ) ;
	
	Timer_ID = Timer;
	
	TIMER_ID_init(Timer);
	
	if (Timer_ID == timer0)
	{
		Timer0_COMP_Set_Callback(Servo_Handler);
	}
	
	else if (Timer_ID == timer1)
	{
		Timer1_COMP_Set_Callback(Servo_Handler);
	}
	
	else if (Timer_ID == timer2)
	{
		Timer2_COMP_Set_Callback(Servo_Handler);
	}
	
	else
	{
		//Do Nothing 
		//Shouldn't be here
	}
	
}


void SERVO_setAngle(uint8 Angle)
{
	/*
	 * apply First Degree Linear equation
	 *
	 *
	 *Assume:-
	 *--------
	 * y:Angle		y2:SERVO_MAX_ANGLE_DEGREE   y1:SERVO_MIN_ANGLE_DEGREE
	 * x:Ton_Steps	x2:TON_MAX_STEPS			x1:TON_MIN_STEPS
	 *	
	 *
	 *			  y2-y1
	 * Slope(m) = -----   
	 *			  x2-x1
	 *
	 *
	 * y-y1   y2-y1 
	 * ---- = -----   -----> y-y1 = m(x-x1)
	 * x-x1   x2-x1
	 *
	 *
	 * (x-x1) =	(y-y1)/m
	 *
	 *
	 * x = (y-y1)/m + x1
	 *  
	 * */
	
	Ton_Steps = (
	
				( (Angle - SERVO_MIN_ANGLE_DEGREE)     *	     (TON_MAX_STEPS - TON_MIN_STEPS)				 )
			/*------------------------------------------------------------------------------------------*/
												       /  ( SERVO_MAX_ANGLE_DEGREE - SERVO_MIN_ANGLE_DEGREE		 )
				)
				+   TON_MIN_STEPS;
		
	//--------------------------------------------------------------------------------------------------------//
	
	/*
	
	At Angle = 0 -> TON = 125;
	
	Toff_Steps = 250 - 125/10;
	
	Toff_Steps = 238
	
	*/
				
	//Toff_Steps = SERVO_PWM_SIGNAL_DUARTION_STEPS_DIV_BY_FACTOR - Ton_Steps/SERVO_PWM_SIGNAL_FACTOR;
	
	//--------------------------------------------------------------------------------------------------------//
	
	/*
	
	At Angle = 90 -> TON = 187;
	
	Toff_Steps = 250 - 187 ;
	
	Toff_Steps = 63
	
	*/
	
	Toff_Steps = SERVO_PWM_SIGNAL_DUARTION_STEPS_DIV_BY_FACTOR - Ton_Steps;
	
	
	if (Timer_ID == timer0)
	{
		OCR0 = (uint8)Ton_Steps;
	}
			
	else if (Timer_ID == timer1)
	{
		OCR1A = (uint8)Ton_Steps;
	}
			
	else if (Timer_ID == timer2)
	{
		OCR2 = (uint8)Ton_Steps;
	}
			
	else
	{
		//Do Nothing
		//Shouldn't be here
	}
	
	TIMER_Start(Timer_ID);
	
	DIO_Write_Pin ( SERVO_SIGNAL_PORT_ , SERVO_SIGNAL_PIN , HIGH ) ;
	
	PORTB = OCR2;
}


// Periodicity of this Function (Servo_handler) is 2 ms since the maximum value of the Top (OCR) is 250 which is equivalent to 250 steps  while the Servo Motor should works on periodicity 20 ms 
// So Static Counter in this function (Servo_handler) should be limited to 0 <= Counter <= 10

/*
void Servo_Handler (void)
{
	static uint8 counter =0;
	
	if(counter == 0)
	{
		//TIMER2_setCounterSteps((uint8)Toff_Steps);
		
		if (Timer_ID == timer0)
		{
			OCR0 = (uint8)Toff_Steps;
		}
			
		else if (Timer_ID == timer1)
		{
			OCR1A = (uint8)Toff_Steps;
		}
			
		else if (Timer_ID == timer2)
		{
			OCR2 = (uint8)Toff_Steps;
		}
			
		else
		{
			//Do Nothing
			//Shouldn't be here
		}
			
		DIO_Write_Pin ( SERVO_SIGNAL_PORT_ , SERVO_SIGNAL_PIN , LOW ) ;
		
		counter++;
	}
	
	else if (counter == SERVO_PWM_SIGNAL_FACTOR+1)
	{
		//TIMER2_setCounterSteps((uint8)Ton_Steps);
		
		if (Timer_ID == timer0)
		{
			OCR0 = (uint8)Ton_Steps;
		}
		
		else if (Timer_ID == timer1)
		{
			OCR1A = (uint8)Ton_Steps;
		}
		
		else if (Timer_ID == timer2)
		{
			OCR2 = (uint8)Ton_Steps;
		}
		
		else
		{
			//Do Nothing
			//Shouldn't be here
		}
		
		DIO_Write_Pin ( SERVO_SIGNAL_PORT_ , SERVO_SIGNAL_PIN , HIGH ) ;
		counter = 0;
	}
	
	else
	{
		counter++;
	}
	
}

*/


void Servo_Handler (void)
{
	PORTC ^= (1<<PC1);	//test
	
	static uint8 counter = 0;
	
	if (counter == T_OFF_PERIOD)
	{

		if (Timer_ID == timer0)
		{
			OCR0 = (uint8)Toff_Steps;
		}
			
		else if (Timer_ID == timer1)
		{
			OCR1A = (uint8)Toff_Steps;
		}
			
		else if (Timer_ID == timer2)
		{
			OCR2 = (uint8)Toff_Steps;
		}
			
		else
		{
			//Do Nothing
			//Shouldn't be here
		}
			
		DIO_Write_Pin ( SERVO_SIGNAL_PORT_ , SERVO_SIGNAL_PIN , LOW ) ;
		
		counter++;
	}
	
	//else if( counter == (SERVO_PWM_PERIOD - 1) )
	else if(counter == SERVO_PWM_PERIOD + 1 )
	{
		
		if (Timer_ID == timer0)
		{
			OCR0 = (uint8)Ton_Steps;
		}
		
		else if (Timer_ID == timer1)
		{
			OCR1A = (uint8)Ton_Steps;
		}
		
		else if (Timer_ID == timer2)
		{
			OCR2 = (uint8)Ton_Steps;
		}
		
		else
		{
			//Do Nothing
			//Shouldn't be here
		}
		
		DIO_Write_Pin ( SERVO_SIGNAL_PORT_ , SERVO_SIGNAL_PIN , HIGH ) ;
		
		counter = T_OFF_PERIOD ;
	}
		
	else
	{	
		DIO_Write_Pin ( SERVO_SIGNAL_PORT_ , SERVO_SIGNAL_PIN , LOW ) ;
			
		if (Timer_ID == timer0)
		{
			OCR0 = _2_ms_DURATION_STEP;
		}
			
		else if (Timer_ID == timer1)
		{
			OCR1A =_2_ms_DURATION_STEP;
		}
			
		else if (Timer_ID == timer2)
		{
			OCR2 =_2_ms_DURATION_STEP;
		}
			
		else
		{
			//Do Nothing
			//Shouldn't be here
		}
		
		counter++;
	}
}
