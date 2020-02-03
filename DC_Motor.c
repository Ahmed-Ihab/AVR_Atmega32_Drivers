
#include "DC_Motor.h"

//-------------------  Private_Global_Varibles -------------------------------//

//-------------------  Private_Function --------------------------------------//


//-------------------  Public_Global_Variables -------------------------------//


//-------------------  Implementation of Functions -------------------------------//


void MOTORS_Initialization (void)
{
	DIO_init_PIN(PORT_MOTOR_1_,M1_IN1,OUTPUT,NA);	// enable the IN1 of the Half Bridge as an output
	DIO_init_PIN(PORT_MOTOR_1_,M1_IN2,OUTPUT,NA);	// enable the IN2 of the Half Bridge as an output
	DIO_init_PIN(PORT_MOTOR_1_,M1_EN1,OUTPUT,NA);	// enable the EN1 of the Half Bridge as an output
	DIO_init_PIN(PORT_MOTOR_2_,M2_IN3,OUTPUT,NA);	// enable the IN3 of the Half Bridge as an output
	DIO_init_PIN(PORT_MOTOR_2_,M2_IN4,OUTPUT,NA);	// enable the IN4 of the Half Bridge as an output
	DIO_init_PIN(PORT_MOTOR_2_,M2_EN2,OUTPUT,NA);	// enable the EN2 of the Half Bridge as an output
	
	/*
	DDR_MOTOR_1 |= (1<<M1_IN1) ;  // enable the IN1 of the Half Bridge as an output
	DDR_MOTOR_1 |= (1<<M1_IN2) ;  // enable the IN2 of the Half Bridge as an output
	DDR_MOTOR_1 |= (1<<M1_EN1) ;  // enable the EN1 of the Half Bridge as an output
	DDR_MOTOR_2 |= (1<<M2_IN3) ;  // enable the IN3 of the Half Bridge as an output
	DDR_MOTOR_2 |= (1<<M2_IN4) ;  // enable the IN4 of the Half Bridge as an output
	DDR_MOTOR_2 |= (1<<M2_EN2) ;  // enable the EN2 of the Half Bridge as an output
	*/
}


Std_Func_t MOTOR_Direction ( Motor_Select_t Motor_Select , Set_Direction_t Set_Direction)
{
	Std_Func_t Error_t = OK;
	
	if(Motor_Select == MOTOR_1)
	{
			if( Set_Direction == ANTICLOCKWISE)  // Default Direction
			{
				DIO_Write_Pin(PORT_MOTOR_1_, M1_IN1,LOW);   // Put logic one on the IN1 to the half bridge (L298)
				DIO_Write_Pin(PORT_MOTOR_1_, M1_IN2,HIGH);  // Put logic zero on the IN2 to the half bridge (L298)
				/*
				PORT_MOTOR_1 &= ~ (1<<M1_IN1) ;				// Put logic one on the IN1 to the half bridge (L298)
				PORT_MOTOR_1 |= (1<<M1_IN2) ;				// Put logic zero on the IN2 to the half bridge (L298)
				*/			
			}
			
			else if( Set_Direction == CLOCKWISE)
			{
				
				DIO_Write_Pin(PORT_MOTOR_1_, M1_IN1,HIGH); // Put logic one on the IN1 to the half bridge (L298)
				DIO_Write_Pin(PORT_MOTOR_1_, M1_IN2,LOW);  // Put logic zero on the IN2 to the half bridge (L298)
				/*			
				PORT_MOTOR_1 |= (1<<M1_IN1) ;			   // Put logic one on the IN1 to the half bridge (L298)
				PORT_MOTOR_1 &= ~(1<<M1_IN2) ;             // Put logic zero on the IN2 to the half bridge (L298)
				*/
			}
			
			else
			{
				Error_t = NOK;
				//Do Nothing	
			}
	}
	
	else if(Motor_Select == MOTOR_2) 
	{
		if( Set_Direction == ANTICLOCKWISE)  // Default Direction
		{
			DIO_Write_Pin(PORT_MOTOR_2_, M2_IN3,LOW);   // Put logic one on the IN3 to the half bridge (L298)
			DIO_Write_Pin(PORT_MOTOR_2_, M2_IN4,HIGH);  // Put logic zero on the IN4 to the half bridge (L298)
			/*
			PORT_MOTOR_2 &= ~(1<<M2_IN3) ;				// Put logic one on the IN3 to the half bridge (L298)
			PORT_MOTOR_2 |= (1<<M2_IN4) ;				// Put logic zero on the IN4 to the half bridge (L298)
			*/
		}
		
		else if( Set_Direction == CLOCKWISE)
		{
			DIO_Write_Pin(PORT_MOTOR_2_, M2_IN3,HIGH);   // Put logic one on the IN3 to the half bridge (L298)
			DIO_Write_Pin(PORT_MOTOR_2_, M2_IN4,LOW);    // Put logic zero on the IN4 to the half bridge (L298)
			/*
			PORT_MOTOR_2 |= (1<<M2_IN3) ;				 // Put logic one on the IN3 to the half bridge (L298)
			PORT_MOTOR_2 &= ~(1<<M2_IN4) ;				 // Put logic zero on the IN4 to the half bridge (L298)
			*/
		}
		
		else
		{
			Error_t = NOK;
			//Do Nothing
		}
	}
	
	else 
	{
		Error_t = NOK;
		//Do Nothing
	}

return Error_t;
	
}

Std_Func_t MOTOR_Stop (Motor_Select_t Motor_Select)
{
	Std_Func_t Error_t = OK;
	
	if(Motor_Select == MOTOR_1)
	{
		DIO_Write_Pin(PORT_MOTOR_1_, M1_IN1,LOW);  // Put logic one on the IN1 to the half bridge (L298)
		DIO_Write_Pin(PORT_MOTOR_1_, M1_IN2,LOW);  // Put logic zero on the IN2 to the half bridge (L298)
		/*
		PORT_MOTOR_1 &= ~ (1<<M1_IN1) ;				// Put logic one on the IN1 to the half bridge (L298)
		PORT_MOTOR_1 |= (1<<M1_IN2) ;				// Put logic zero on the IN2 to the half bridge (L298)
		*/
	}
	
	else if(Motor_Select == MOTOR_2)
	{
		DIO_Write_Pin(PORT_MOTOR_2_, M2_IN3,LOW); // Put logic one on the IN3 to the half bridge (L298)
		DIO_Write_Pin(PORT_MOTOR_2_, M2_IN4,LOW); // Put logic zero on the IN4 to the half bridge (L298)
		/*
		PORT_MOTOR_1 &= ~ (1<<M1_IN1) ;			  // Put logic one on the IN1 to the half bridge (L298)
		PORT_MOTOR_1 |= (1<<M1_IN2) ;			  // Put logic zero on the IN2 to the half bridge (L298)
		*/
	}
	
	else
	{
		Error_t = NOK;
		// Do Nothing 
	}
	return Error_t;
}