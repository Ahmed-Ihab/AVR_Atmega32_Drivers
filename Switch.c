
#include "Switch.h"


ACK Switch_Init(void)
{
    ACK STATE = AK;
	uint8 loop_index=0;
	
    if (NUM_OF_PINS>MAX_NUMBER_OF_PINS)
        {  STATE = NAK; }
    else
    {

        for(loop_index=0;loop_index<NUMBER_OF_SWITCHES; loop_index++)
        {
            if ( (Switch_Cnfg_arr[loop_index].DIR == INPUT) && (Switch_Cnfg_arr[loop_index].IS_init == INITIALIZED ) )
            {
                DIO_init_PIN(Switch_Cnfg_arr[loop_index].PORT , Switch_Cnfg_arr[loop_index].PIN, Switch_Cnfg_arr[loop_index].DIR, Switch_Cnfg_arr[loop_index].PULL_RESISTOR);
            }
            else
            {
				
                STATE = NAK;
            }
        }
    }

    return STATE;

}
uint8 Switch_GetState(SWITCH_Select_t Switch_Select)
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
