#include "Switch.h"



/* Number of samples per SW */
#define N_SAMPLES       (2)
#define N_SWITCHES      (2)

/* SW voltage level */
#define SWITCH_PRESSED_LEVEL        (1)
#define SWITCH_RELEASED_LEVEL       (0)


uint8 Switch_Duration_Flag = 0;

/* Switch info: samples, state */

//Declaration and Definition of Structure
typedef struct
{
    uint8 SWITCH_samples[N_SAMPLES];
    type_Enum_SWITCH_State SWITCH_state;
}type_Struct_Switch_Info;


/* Switch info */

//Static Global Variables Sections
static type_Struct_Switch_Info Switch_Info[N_SWITCHES];

//Definition and body of Functions

ACK Switch_Init(void)
{
	ACK STATE = AK;
	uint8 loop_index=0;
	
	if (NUMBER_OF_SWITCHES>MAX_NUMBER_OF_SWITCHES)
	{  STATE = NAK; }
		
	else
	{
		for(loop_index=0;loop_index<NUMBER_OF_SWITCHES; loop_index++)
		{
			if ( (Switch_Cnfg_arr[loop_index].DIR == INPUT) && (Switch_Cnfg_arr[loop_index].IS_init == INITIALIZED ) )
			{
				// Initialize switches as inputs 
				// Make all the Switches Pin as input Pins
				DIO_init_PIN(Switch_Cnfg_arr[loop_index].PORT , Switch_Cnfg_arr[loop_index].PIN, Switch_Cnfg_arr[loop_index].DIR, Switch_Cnfg_arr[loop_index].PULL_RESISTOR);
				
			    // initialize All the Switches States in the beginning as Switch Released
				// Initialize switches info by initialize switch samples and Switch state 
				// Initialize switch samples 
				Switch_Info[loop_index].SWITCH_samples[0] = SWITCH_RELEASED_LEVEL;
				Switch_Info[loop_index].SWITCH_samples[1] = SWITCH_RELEASED_LEVEL;

				// Initialize Switch state 
				Switch_Info[loop_index].SWITCH_state = SWITCH_RELEASED;
			}
			
			else
			{
				
				STATE = NAK;
			}
		}
	}

	return STATE;

}




type_Enum_SWITCH_State SWITCH_GetState(type_Enum_SWITCH_Selection Switch)
{
    /* return SWITCH state */
    return Switch_Info[Switch].SWITCH_state;
}


void SWITCH_Update(void)
{
	uint8 input_Value=4;	//any value except 0 or 1
	uint8 index = 0;
    /* Update switches states */
   for (index = SWITCH_1; index < NUMBER_OF_SWITCHES; index++)
   {
	  
	   /* Update and take Switch samples */
	   Switch_Info[index].SWITCH_samples[0] = Switch_Info[index].SWITCH_samples[1];


	   if (index == SWITCH_1)
	   {   
		   DIO_read_Pin(SWITCH_1_PORT_, SWITCH_1_PIN , &input_Value);
		   Switch_Info[index].SWITCH_samples[1] = input_Value;

		   //Switch_Info[index].SWITCH_samples[1] = GPIO_ReadPortPin(SWITCH_1_PORT_DR, SWITCH_1_PIN);
	   }
	   
	   else	if (index == SWITCH_2)
	   	{
		   DIO_read_Pin(SWITCH_2_PORT_, SWITCH_2_PIN , &input_Value);
		   Switch_Info[index].SWITCH_samples[1] = input_Value;

		   //Switch_Info[index].SWITCH_samples[1] = GPIO_ReadPortPin(SWITCH_2_PORT_DR, SWITCH_2_PIN);
	   	}
			  
	   else
	   {
		   /* Do nothing should not be here !! */
	   }

	   /* After Taking Switch Samples, Update Switch state */

	   switch(Switch_Info[index].SWITCH_state)
	   {
		   case SWITCH_RELEASED:
			   /* Go to pre-pressed state if needed */
			   if ((Switch_Info[index].SWITCH_samples[0] == SWITCH_PRESSED_LEVEL) &&
			   (Switch_Info[index].SWITCH_samples[1] == SWITCH_PRESSED_LEVEL))
			   {
				   Switch_Info[index].SWITCH_state = SWITCH_PRE_PRESSED;
			   }
			   else
			   {
				   /* Do nothing */
			   }
		   break;
		   
		   case SWITCH_PRE_PRESSED:
			   /* Go to pressed state if needed  */
			   if (Switch_Info[index].SWITCH_samples[1] == SWITCH_PRESSED_LEVEL)
			   {
				   Switch_Info[index].SWITCH_state = SWITCH_PRESSED;
			   }

			   else
			   {
				   /* Do nothing */
			   }
		   break;
		   
		   case SWITCH_PRESSED:
			   /* Go to pre-released state if needed */
			   if ((Switch_Info[index].SWITCH_samples[0] == SWITCH_RELEASED_LEVEL) &&
			   (Switch_Info[index].SWITCH_samples[1] == SWITCH_RELEASED_LEVEL))
			   {
				   Switch_Info[index].SWITCH_state = SWITCH_PRE_RELEASED;
			   }
			   else
			   {
				   /* Do nothing */
			   }
		   break;
		   
		   case SWITCH_PRE_RELEASED:
			   /* Go to released state if needed */
			   if (Switch_Info[index].SWITCH_samples[1] == SWITCH_RELEASED_LEVEL)
			   {
				   Switch_Info[index].SWITCH_state = SWITCH_RELEASED;
			   } else
			   {
				   /* Do nothing */
			   }
		   break;
		   
		   /* Should not be here */
		   default:
		   break;
	   }
   }
       
   

      //  Switch_Cal_time (SWITCH);
}


 void Switch_Calculate_Pressed_time (type_Enum_SWITCH_Selection Switch)       //Switch_Calculate_Press_time_Function
{
    static uint16 Switch_Duration_Cal=0;					    //Switch_Duration_Cal = Switch_Duration_Calculate_Pressed_Time

    if (SWITCH_GetState(Switch) == SWITCH_PRESSED)
    {
        Switch_Duration_Cal += SWITCH_PERIOD_MS;

        //if( ( Switch_Duration == SWITCH_PRESSED_TIME_PERIOD_MS ) && (Switch_Duration_Flag == 0) )
        if( Switch_Duration_Cal == SWITCH_PRESSED_TIME_PERIOD_MS )
        {
            Switch_Duration_Flag = 1 ;
            Switch_Duration_Cal = 0 ;
            return ;
        }

        return ;
    }

    else
        {
            Switch_Duration_Cal = 0 ;
            return ;
        }
}


//------------------------------------------------- Section of Switch without using OS ------------------------------------------------------------------------------//

uint8 Switch_Getvalue(SWITCH_Select_t Switch_Select)
{
	
	uint8 Switch_State = 0;
	uint8 Switch_Active_FLag=0;
	uint8 Switch_Get_value = HIGH ;  //idle value for Switch when it's pull up is high

	DIO_read_Pin (Switch_Cnfg_arr[Switch_Select].PORT,Switch_Cnfg_arr[Switch_Select].PIN, &Switch_Get_value);


	if (Switch_Get_value == LOW )
	{
		
		_delay_ms(30);

		DIO_read_Pin (Switch_Cnfg_arr[Switch_Select].PORT,Switch_Cnfg_arr[Switch_Select].PIN, &Switch_Get_value) ; //second check due to switch de-bouncing
		
		if (Switch_Get_value == LOW )
		{
			if (Switch_State == 0)
			{
				
				Switch_State = 1;
				Switch_Active_FLag=1;
			}
		}
	}
	else
	{
		
		Switch_State = 0;
		Switch_Active_FLag=0;
	}
	
	return Switch_Active_FLag;
}
