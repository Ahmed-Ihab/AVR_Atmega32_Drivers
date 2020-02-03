
#include "Keypad.h"

// ----- Private Constant ----------

#define KEYPAD_RECV_BUFFER_LENGTH (6)
#define KEYPAD_NO_NEW_DATA        (uint8) '_'

// ----- Private Global Variables ----------
static uint8 Keypad_Recv_Buffer [KEYPAD_RECV_BUFFER_LENGTH+1][2]; // first Column to Store the values of Key and Second Column to store the values of Function Key
static uint8 Keypad_in_Read_index=0;                                // Data in the Buffer that has been read
static uint8 Keypad_in_waiting_index=0;                             // Data in the Buffer not yet read
static uint8 Last_Valid_Key = KEYPAD_NO_NEW_DATA;

// ----- Private Functions ------------------
static uint8 Keypad_Scan ( uint8* const );

// ----- Implementation of Functions ----------

static uint8 Keypad_Init_Flag=0 ; 


void Keypad_Init (void)
{	
	if(Keypad_Init_Flag == 0)
	{
	  Keypad_in_Read_index = 0;
	  Keypad_in_waiting_index = 0;
	  Keypad_Init_Flag=1;
	}
  
}


// the Keypad_Update must be Scheduled  approx every 50 to 200 ms
void keypad_Update (void)
{
	if(Keypad_Init_Flag == 1)
	{
		uint8 Key = 0;

		//Call the Keypad_Scan Function
		if (Keypad_Scan( &Key ) == 0)
		{
			//No New Key Data just return
	    	return ;
		}


		// want to read into index 0, if Old Data has been read
		// Circular Buffer
		if(Keypad_in_waiting_index == Keypad_in_Read_index)
		{
			Keypad_in_Read_index = 0;
			Keypad_in_waiting_index = 0;
		}
		// Load Keypad Data into the Buffer

		Keypad_Recv_Buffer[Keypad_in_waiting_index][0]= Key;

		if ( Keypad_in_waiting_index <  KEYPAD_RECV_BUFFER_LENGTH )
		{
			//Increment without Overflowing Buffer
			Keypad_in_waiting_index ++ ;
		}
	}
   

}


uint8 Keypad_Scan ( uint8* const P_Key )
{
    static uint8 Old_Key;

	uint8 Keypad_input_col_1;
	uint8 Keypad_input_col_2;
	uint8 Keypad_input_col_3;
	
    uint8 Key= KEYPAD_NO_NEW_DATA;

    //Scanning Column 0
   // CLEAR_ROW_0 ;
	DIO_Write_Pin(KEYPAD_PORT , PIN2 , 0 );
	
	DIO_read_Pin(KEYPAD_PORT,PIN5,&Keypad_input_col_1);
	DIO_read_Pin(KEYPAD_PORT,PIN6,&Keypad_input_col_2);
	DIO_read_Pin(KEYPAD_PORT,PIN7,&Keypad_input_col_3);
		
	if ( Keypad_input_col_1 == 0 ) {Key='1';}
	if ( Keypad_input_col_2 == 0 )  Key='2';
	if ( Keypad_input_col_3 == 0 )  Key='3';
	
	/*
    if ( READ_COLUMN_0 == 0 )  Key='7';
    if ( READ_COLUMN_1 == 0 )  Key='4';
    if ( READ_COLUMN_2 == 0 )  Key='1';
    if ( READ_COLUMN_3 == 0 )  Key='4';
	*/
   // SET_ROW_0 ;
	DIO_Write_Pin(KEYPAD_PORT , PIN2 , 1 );
	
   //Scanning Column 1
   // CLEAR_ROW_1 ;
	DIO_Write_Pin(KEYPAD_PORT , PIN3 , 0 );
	
	DIO_read_Pin(KEYPAD_PORT,PIN5,&Keypad_input_col_1);
	DIO_read_Pin(KEYPAD_PORT,PIN6,&Keypad_input_col_2);
	DIO_read_Pin(KEYPAD_PORT,PIN7,&Keypad_input_col_3);
	
	if ( Keypad_input_col_1 == 0 )  Key='4';
	if ( Keypad_input_col_2 == 0 )  Key='5';
	if ( Keypad_input_col_3 == 0 )  Key='6';
   
   	/*
    if ( READ_COLUMN_0 == 0 )  Key='7';
    if ( READ_COLUMN_1 == 0 )  Key='4';
    if ( READ_COLUMN_2 == 0 )  Key='1';
    if ( READ_COLUMN_3 == 0 )  Key='4';
	*/

	//SET_ROW_1 ;
	DIO_Write_Pin(KEYPAD_PORT , PIN3 , 1 );
		
   //Scanning Column 2
   //CLEAR_ROW_2 ;
    DIO_Write_Pin(KEYPAD_PORT , PIN4 , 0 );
   
	DIO_read_Pin(KEYPAD_PORT,PIN5,&Keypad_input_col_1);
	DIO_read_Pin(KEYPAD_PORT,PIN6,&Keypad_input_col_2);
	DIO_read_Pin(KEYPAD_PORT,PIN7,&Keypad_input_col_3);
		
	if ( Keypad_input_col_1 == 0 )  Key='7';
	if ( Keypad_input_col_2 == 0 )  Key='8';
	if ( Keypad_input_col_3 == 0 )  Key='9';
	   
   /*
   if ( READ_COLUMN_0 == 0 )  Key='7';
   if ( READ_COLUMN_1 == 0 )  Key='4';
   if ( READ_COLUMN_2 == 0 )  Key='1';
   if ( READ_COLUMN_3 == 0 )  Key='4';
	*/
   // SET_ROW_2 ;
    DIO_Write_Pin(KEYPAD_PORT , PIN4, 1 );
   
   
   /*
    //Scanning Column 3
    CLEAR_ROW_3 ;

   if ( READ_COLUMN_0 == 0 )  Key='7';
   if ( READ_COLUMN_1 == 0 )  Key='4';
   if ( READ_COLUMN_2 == 0 )  Key='1';
   if ( READ_COLUMN_3 == 0 )  Key='4';

    SET_ROW_3 ;
	*/
   
    //NO Key is Pressed or ( just a function key is pressed only )
    if(Key == KEYPAD_NO_NEW_DATA)
    {
        // No Key is Pressed
        Old_Key = KEYPAD_NO_NEW_DATA;
        Last_Valid_Key = KEYPAD_NO_NEW_DATA;

        return 0; // No NEW Data
    }

        // A key has been pressed : debounce by checking the pressed key twice
        if( Key == Old_Key)
        {
               // A Valid (Debounce) key press has been detected.

               // Must be a new key to be valid - no "Auto Repeated"
              if(Key != Last_Valid_Key)
              {
                    *P_Key = Key;           //New Key value
                    Last_Valid_Key = Key;   // update the Last_Valid_Key for feedback checking

                return 1;
             }
       }

    // No new Data
    Old_Key = Key ;

    return 0;

}


uint8 Keypad_Get_Data_from_Buffer (uint8* const P_Key )
{
	if(Keypad_Init_Flag == 1)
	{
		//if there is new data in the Buffer
		if( Keypad_in_Read_index < Keypad_in_waiting_index )
		{
			*P_Key = Keypad_Recv_Buffer[Keypad_in_Read_index][0];

			Keypad_in_Read_index++;

			return 1;
		}

		return 0;
		
	}
	
	return 0;

}


void Keypad_Clear_Buffer(void)
{
	if(Keypad_Init_Flag == 1)
	{
		Keypad_in_Read_index = 0;
		Keypad_in_waiting_index = 0;
	}
}



//-------------------------------------------------------- Section of Keypad without OS and it can use Timeout mechanism ---------------------------------------------------------------//


void KEYPAD_Initialization(void);
char KEYPAD_Press(void);
char KEYPAD_Button(unsigned );
void Timer0_Timeout_Handling (void);

//static uint16 Timeout_Counter = 0;
static short Number=0 ;
//static char j=0;
//static char Timeout_Flag = 0;

void KEYPAD_Initialization(void)
{

	if((N_ROW == 3) && (N_COL == 3) )
	{
		KEYPAD_PORT_DDR |= 0x1C;	//0b00011100  // high significant nibble is input which is indicating to columns and Least significant nibble is output which is indicating to Rows
		KEYPAD_PORT_DDR &= 0x1F;	//0b00011111
		KEYPAD_PORT_OUT |= 0xFC;	//0b11111100  // Write ones to the all pins in the port to activate pull-up resistors in the input nibble
	}

	else if((N_ROW == 4) && (N_COL == 4) )
	{
		KEYPAD_PORT_DDR = 0x0F ;              // high significant nibble is input which is indicating to columns and Least significant nibble is output which is indicating to Rows
		KEYPAD_PORT_OUT= 0xFF;	              // Write ones to the all pins in the port to activate pull-up resistors in the input nibble
	}
}


char KEYPAD_Press(void)
{
	unsigned char row,col, key;

	if((N_ROW == 3) && (N_COL == 3) )
	{
	//	Time_Delay(TIMER0,2,sec);
		
		//while( Timeout_Flag == 0)
		while(1)
		{
			for(row=0;row<N_ROW;row++)  // rows for the output pins ( LSB of the port)
			{
				KEYPAD_PORT_OUT = (~(0b00000100<<row));			//clear this row pin and set all pins

				for(col=0;col<N_COL;col++) // this loop reads the status of each column in that row.
				{

					if ( ! ( KEYPAD_PIN_IN & (0b00100000<<col) ) )		//Column Detected means that there is a single 0 in that row.
					{
						key = KEYPAD_Button ((row*N_COL)+col+1) ;
						Number = Number*10;
						//j++;
						return  (key);

					}
				}
			}

		}
		
		//Timeout_Flag=0;
		//TIMER_Stop(TIMER0);
		//return TIMEROUT_VALUE ;
		
	}

	else if((N_ROW == 4) && (N_COL == 4) )
	{
		//Time_Delay(TIMER0,2,sec);
		
		//while( Timeout_Flag == 0)
		while(1)
		{
			for(row=0;row<N_ROW;row++)  // rows for the output pins ( LSB of the port)
			{
				KEYPAD_PORT_OUT = (~(0b00000001<<row));			//clear this row pin and set all pins

				for(col=0;col<N_COL;col++) // this loop reads the status of each column in that row.
				{

					if ( ! ( KEYPAD_PIN_IN & (0b00010000<<col) ) )		//Column Detected means that there is a single 0 in that row.
					{
						key = KEYPAD_Button ((row*N_COL)+col+1) ;
						Number = Number*10;
					//	j++;
						return  (key);

					}
				}
			}

		}
		
		//Timeout_Flag=0;
		//TIMER_Stop(TIMER0);
		//return TIMEROUT_VALUE ;
	}

}


short  KEYPAD_INPUT_Number(void)
{
	
	KEYPAD_Initialization();
	//Timer0_OVF_Set_Callback (Timer0_Timeout_Handling);
	
	short ret_Number=0 ;
	char Digit = 0;
	unsigned char i=0 ;

	for ( i = 0; i < DIGIT_SIZE ; i++)
	{
		Digit = KEYPAD_Press();
		/*
		if(Digit == TIMEROUT_VALUE)
		{
			continue;
		}*/
		
		//LCD_4Bits_Print_Number( 1, 8+(j-1) ,Digit);
		
		Number = Number+Digit;
		//Timeout_Counter = 0;
		
		_delay_ms(200);
	}
	

	if((N_ROW == 3) && (N_COL == 3) )
	{
	KEYPAD_PORT_OUT |= 0xFC;	//make the Keypad to its idle status all pins high
	}
	
	if((N_ROW == 4) && (N_COL == 4) )
	{
		KEYPAD_PORT_OUT |= 0xFF;	//make the Keypad to its idle status all pins high
	}
	
	//TIMER_Stop(TIMER0);
	ret_Number=Number;
	return ret_Number ;
}

/*
void Timer0_Timeout_Handling (void)
{

	if ( (TIMER_cnfg_arr[TIMER0].IS_init == INITIALIZED) && (TIMER_cnfg_arr[TIMER0].interrupt == INTERRUPT))
	{
		Timeout_Counter++;

		if(Timeout_Counter == OVF_Counter_Loop[TIMER0])
		{
			Timeout_Flag=1;
			Timeout_Counter=0;
		}
	}
}
*/
 char *  KEYPAD_INPUT_String(void)
{
	char static String[SIZE+1];
	unsigned char i=0 ;

	for ( i = 0; i<DIGIT_SIZE ; i++)
	{
		do
		{
			String[i]= KEYPAD_Press();
		}while ( String[i] == '+' || String[i] == '-' || String[i] == '*' || String[i] == '/' || String[i] == '=' ) ;

		_delay_ms(500);
	}
	String [i] = '\0' ;

	return String ;
}




 char KEYPAD_Button (unsigned button_number)
{
	#if(PRINT_NUMBERS_From_KEYPAD)
	{
		switch(button_number)
		{
			case 1: return 7 ; break;		 // position of (1) is located at top left of the keypad which is corresponding to button no. 7
			case 2: return 2 ; break;
			case 3: return 3 ; break;
			case 4: return 1 ; break;
			case 5: return 5 ; break;
			case 6: return 6 ; break;
			case 7: return 4 ; break;
			case 8: return 8 ; break;
			case 9: return 9 ; break;
			case 10: return 4 ; break;
			case 11: return 8 ; break;
			case 12: return 9 ; break;
			case 13: return -1 ; break;
			case 14: return 0 ; break;
			case 15: return -1; break;
			case 16: return -1 ; break;
			default: return button_number;
		}
	}

	// or to print the numbers in terms of ascii value

	#elif(! PRINT_NUMBERS_From_KEYPAD )
	{

		switch(button_number)
		{
			case 1: return '1' ; break;			 // position of (1) is located at top left of the keypad which is corresponding to button no. 7
			case 2: return '2' ; break;
			case 3: return '3' ; break;
			case 4: return '4' ; break;
			case 5: return '5' ; break;
			case 6: return '6' ; break;
			case 7: return '7' ; break;
			case 8: return '8' ; break;
			case 9: return  '9' ; break;
			case 10: return 'A' ; break;
			case 11: return 'B' ; break;
			case 12: return 'C' ; break;
			case 13: return 'D' ; break;
			case 14: return '0' ; break;
			case 15: return 'E'; break;
			case 16: return 'F' ; break;
			default: return button_number;
		}
	}

	#endif
}

//it's used for Keypad 4x4 like Calculator
/*
 char KEYPAD_Button (unsigned button_number)
{
	#if(PRINT_NUMBERS_From_KEYPAD)
	{
		switch(button_number)
		{
			case 1: return 7 ; break;		 // position of (1) is located at top left of the keypad which is corresponding to button no. 7
			case 2: return 8 ; break;
			case 3: return 9 ; break;
			case 4: return 10 ; break;
			case 5: return 4 ; break;
			case 6: return 5 ; break;
			case 7: return 6 ; break;
			case 8: return 11 ; break;
			case 9: return 1 ; break;
			case 10: return 2 ; break;
			case 11: return 3 ; break;
			case 12: return 12 ; break;
			case 13: return 13 ; break;
			case 14: return 0 ; break;
			case 15: return 14; break;
			case 16: return 15 ; break;
			default: return button_number;
		}
	}

	// or to print the numbers in terms of ascii value

	#elif(! PRINT_NUMBERS_From_KEYPAD )
	{

		switch(button_number)
		{
			case 1: return '7' ; break;			 // position of (1) is located at top left of the keypad which is corresponding to button no. 7
			case 2: return '8' ; break;
			case 3: return '9' ; break;
			case 4: return 'A' ; break;
			case 5: return '4' ; break;
			case 6: return '5' ; break;
			case 7: return '6' ; break;
			case 8: return 'B' ; break;
			case 9: return '1' ; break;
			case 10: return '2' ; break;
			case 11: return '3' ; break;
			case 12: return 'C' ; break;
			case 13: return 'D' ; break;
			case 14: return '0' ; break;
			case 15: return 'E'; break;
			case 16: return 'F' ; break;
			default: return button_number;
		}
	}

	#endif
}
*/


//------------------------------------------------------------------------------------------------------------------------------------------//
/*
void main (void)
{
    uint8 key;
    uint8 Func_Key;
    Keypad_Init();
    keypad_Update();
    Keypad_Get_Data_from_Buffer( &key , &Func_Key);
}
*/
