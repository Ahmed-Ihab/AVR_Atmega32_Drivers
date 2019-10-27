
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
		
	if ( Keypad_input_col_1 == 0 )  
	{
		Key='1';
	}
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

			Keypad_in_Read_index++ ;

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
