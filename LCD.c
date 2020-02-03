/*
 * LCD_FUNCTIONS.c
 *
 *
 *  Author: HOBA
 */
//library

#include "LCD.h"

#define DELAY_PERIOD_AFTER_ENABLE_HIGH 20
#define DELAY_PERIOD_AFTER_ENABLE_LOW  20

//--------------------------------------------------------------------------------------------------------------------------//
/*init indication*/
uint8 LCD_Init_Flag = 0;
uint8 Timer ;
uint8 Char_G;


#if LCD_MODE

void LCD_4Bits_Initialization (void)
{	
		LCD_Init_Flag=1;
		//LCD_4Bits_DDR = 0xFF; 						//LCD port is output
		LCD_4Bits_PORT &= ~ (1<<LCD_EN) ;			//LCD_EN = 0

		LCD_4Bits_Write_Command (0x33) ;			//$33 for 4-bit mode
		LCD_4Bits_Write_Command (0x32);				//$32 for 4-bit mode
		LCD_4Bits_Write_Command (0x28) ; 			//$28 for 4-bits, LCD 2 line , 5x7 matrix
		LCD_4Bits_Write_Command (0x0E);				//display on, cursor on
		LCD_4Bits_Write_Command (0x01);				//Clear LCD
		_delay_us (2000);							//long wait as clear command takes a long time
		LCD_4Bits_Write_Command (0x06) ; 			//shift cursor right
	}
//--------------------------------------------------------------------------------------------------------------------------//



void LCD_4Bits_Write_Command ( unsigned char Command )				//Working by OS
{

	if(	LCD_Init_Flag == 1)
	{
		//Send the Highest Nibble in Command to the outPort
		DIO_Write_PORT(LCD_4Bits_PORT_,(LCD_4Bits_PORT & 0x0F) | (Command & 0xF0));

		DIO_Write_Pin(LCD_4Bits_PORT_,LCD_RS,0);

		DIO_Write_Pin(LCD_4Bits_PORT_,LCD_RW,0);

		DIO_Write_Pin(LCD_4Bits_PORT_,LCD_EN,1);
		//DELAY_PERIOD_AFTER_ENABLE_HIGH
		LCD_Delay();

		DIO_Write_Pin(LCD_4Bits_PORT_,LCD_EN,0);
		//DELAY_PERIOD_AFTER_ENABLE_LOW
		LCD_Delay();
		//  Shift the least nibble by 4 to send the Highest Nibble in   Command  to the outPort
		DIO_Write_PORT(LCD_4Bits_PORT_, (LCD_4Bits_PORT & 0x0F) | (Command <<4) );
		//EN = 1 for High-to-Low
		DIO_Write_Pin(LCD_4Bits_PORT_,LCD_EN,1);

		LCD_Delay();
		//EN = 0 for High-to-Low
		DIO_Write_Pin(LCD_4Bits_PORT_,LCD_EN,0);

		LCD_Delay();
	}

}




void LCD_4Bits_Write_Data ( unsigned char Data )				//Working with OS
{
  if(LCD_Init_Flag == 1)
  {
	//Send the Highest Nibble in Command to the outPort
	DIO_Write_PORT(LCD_4Bits_PORT_, (LCD_4Bits_PORT & 0x0F) | (Data & 0xF0));

	//RS = 1 for data
	DIO_Write_Pin(LCD_4Bits_PORT_,LCD_RS,1);

	//RW = 0 for write
	DIO_Write_Pin(LCD_4Bits_PORT_,LCD_RW,0);

	//EN = 1    for High-to-Low
	DIO_Write_Pin(LCD_4Bits_PORT_,LCD_EN,1);

	 //DELAY_PERIOD_AFTER_ENABLE_HIGH) ;
	 LCD_Delay();

	//EN = 0   for High-to-Low
	DIO_Write_Pin(LCD_4Bits_PORT_,LCD_EN,0);

	//DELAY_PERIOD_AFTER_ENABLE_LOW);
	LCD_Delay();

	// shift the least nibble by 4 to send the Highest Nibble in data  to the outPort.

	DIO_Write_PORT(LCD_4Bits_PORT_, (LCD_4Bits_PORT & 0x0F) | (Data <<4) );

	//EN = 1    for High-to-Low
	DIO_Write_Pin(LCD_4Bits_PORT_,LCD_EN,1);

	// DELAY_PERIOD_AFTER_ENABLE_HIGH ;
	 LCD_Delay();

	//EN = 0   for High-to-Low
	DIO_Write_Pin(LCD_4Bits_PORT_,LCD_EN,0);

	//DELAY_PERIOD_AFTER_ENABLE_LOW);
	LCD_Delay();

	}

}

//--------------------------------------------------------------------------------------------------------------------------//


//--------------------------------------------------------------------------------------------------------------------------//

void LCD_4Bits_Cursor_Position( unsigned char y , unsigned char x)   		// define Cursor Position in the LCD
{
unsigned char firstCharAdr [ ] ={0x80,0xC0,0x94,0xD4} ;			// See the Table of Cursor Position adjustment

LCD_4Bits_Write_Command (  firstCharAdr [y-1] + x - 1);
_delay_us(100);
 }
//--------------------------------------------------------------------------------------------------------------------------//

void LCD_4Bits_Clear (void)
{
    LCD_4Bits_Write_Command (0x01);			 //Clear LCD
    _delay_us (2000);			 //long wait as clear command takes a long time
	LCD_4Bits_Cursor_Position ( 1 , 1) ;
}
//--------------------------------------------------------------------------------------------------------------------------//

void LCD_4Bits_Shift_Right(void)
{
	LCD_4Bits_Write_Command(0x0C);
}
//--------------------------------------------------------------------------------------------------------------------------//

void LCD_4Bits_Shift_Left(void)
{
	LCD_4Bits_Write_Command(0x08);
}
//--------------------------------------------------------------------------------------------------------------------------//

void LCD_4Bits_Print_Character( unsigned char row , unsigned char column , unsigned char ch )
{
	LCD_4Bits_Cursor_Position(row,column);

	LCD_4Bits_Write_Data (ch );

}
//--------------------------------------------------------------------------------------------------------------------------//

void LCD_4Bits_Print_String( unsigned char row , unsigned char column , unsigned char *str )
{

	LCD_4Bits_Cursor_Position(row,column);
	unsigned char i = 0;

	while (str [i] != '\0')                 // till reaches its end (null Terminator)
		{
		LCD_4Bits_Write_Data (str[i] );
		i++ ;
		}
}
//--------------------------------------------------------------------------------------------------------------------------//

void LCD_4Bits_Print_Number(unsigned char row ,unsigned char column ,long num)
{

	LCD_4Bits_Cursor_Position(row,column);

	if (0<=num)
	{

		if (num<=9) LCD_4Bits_Write_Data(num+'0');

		else if(num<=99)
		{
			LCD_4Bits_Write_Data((num/10)+'0');
			LCD_4Bits_Write_Data((num%10)+'0');
		}
		else if(num<=999)
		{
			LCD_4Bits_Write_Data((num/100)+'0');
			LCD_4Bits_Write_Data(((num/10)%10)+'0');
			LCD_4Bits_Write_Data((num%10)+'0');
		}

		else if(num<=99999)
		{
			LCD_4Bits_Write_Data( ( (num/10000) ) +'0');
			LCD_4Bits_Write_Data( ( (num/1000) %10 )+'0');
			LCD_4Bits_Write_Data( ( (num/100) %10 ) +'0');
			LCD_4Bits_Write_Data( ( (num/10) %10 )+'0');
			LCD_4Bits_Write_Data((num%10)+'0');
		}
	}

	if (0>num)
	{
		num = -1*num;

		if (num<=9)
		{
			LCD_4Bits_Write_Data('-');
			LCD_4Bits_Write_Data(num+'0');
		}
		else if(num<=99)
		{
			LCD_4Bits_Write_Data('-');
			LCD_4Bits_Write_Data((num/10)+'0');
			LCD_4Bits_Write_Data((num%10)+'0');
		}
		else if(num<=999)
		{
			LCD_4Bits_Write_Data('-');
			LCD_4Bits_Write_Data((num/100)+'0');
			LCD_4Bits_Write_Data(((num/10)%10)+'0');
			LCD_4Bits_Write_Data((num%10)+'0');
		}

	}

}
//--------------------------------------------------------------------------------------------------------------------------//

#else


void LCD_8Bits_Initialization (void )
{
	//LCD_8Bits_DATA_DDR = 0xFF;			         //make the Data Port an Outport.
	//LCD_8Bits_COMMAND_DDR = 0xFF; 				 //make the Command Port an Outport.
	LCD_8Bits_COMMAND_PORT &= ~ (1<<LCD_EN) ;    //LCD_EN = 0
	_delay_us(2000); 							 //wait for initialization and Stable Power.
	LCD_8Bits_Write_Command (0x38) ; 			 //8-bits, LCD 2 line , 5x7 matrix
	LCD_8Bits_Write_Command (0x0E);  			 //display on, cursor on
	LCD_8Bits_Write_Command (0x01) ;	 		 //Clear LCD
	_delay_us(2000); 							 //long wait since clear command takes long time
	LCD_8Bits_Write_Command (0x06);				 //shift cursor right
}
//--------------------------------------------------------------------------------------------------------------------------//

void LCD_8Bits_Write_Command ( unsigned char Command )
{
	LCD_8Bits_DATA_PORT = Command;  				 //send Command to data port
	LCD_8Bits_COMMAND_PORT &= ~ (1 << LCD_RS);  	//RS = 0 for command
	LCD_8Bits_COMMAND_PORT &= ~ (1 << LCD_RW); 		//RW = 0 for write
	LCD_8Bits_COMMAND_PORT |= (1 << LCD_EN); 		//EN = 1 for High-to-Low pulse
	_delay_us(1); 									//wait to make enable wide
	LCD_8Bits_COMMAND_PORT &= ~ (1<<LCD_EN) ;		//EN = 0 for High-to-Low pulse
	_delay_us (100);								//wait to make enable wide
}
//--------------------------------------------------------------------------------------------------------------------------//

void LCD_8Bits_Write_Data ( unsigned char Data )
{
	//static unsigned char X_axis = 1 , Y_axis = 1;

	LCD_8Bits_DATA_PORT = Data; 				//send Data to Data port
	LCD_8Bits_COMMAND_PORT |= (1<<LCD_RS) ;                           //RS = 1 for data
	LCD_8Bits_COMMAND_PORT &= ~ (1<<LCD_RW);  		//RW = 0 for write
	LCD_8Bits_COMMAND_PORT |= (1 << LCD_EN); 		//EN = 1 for High-to-Low pulse
	_delay_us(1); 						//wait to make enable wide
	LCD_8Bits_COMMAND_PORT &= ~ (1<<LCD_EN) ;		//EN = 0 for High-to-Low pulse
	_delay_us (100);						//wait to make enable wide


}
//--------------------------------------------------------------------------------------------------------------------------//

void LCD_8Bits_Cursor_Position( unsigned char y , unsigned char x)   		// define Cursor Position in the LCD
{
	unsigned char firstCharAdr [ ] ={0x80,0xC0,0x94,0xD4} ;			// See the Table of Cursor Position adjustment

	LCD_8Bits_Write_Command (  firstCharAdr [y-1] + x - 1);
	_delay_ms(100);
}
//--------------------------------------------------------------------------------------------------------------------------//

void LCD_8Bits_Clear (void)
{
	LCD_8Bits_Write_Command (0x01);			 //Clear LCD
	_delay_us (2000);			 //long wait as clear command takes a long time
	LCD_8Bits_Cursor_Position ( 1 , 1) ;
}
//--------------------------------------------------------------------------------------------------------------------------//

void LCD_8Bits_Shift_Right(void)
{
	LCD_8Bits_Write_Command(0x0C);
}
//--------------------------------------------------------------------------------------------------------------------------//

void LCD_8Bits_Shift_Left(void)
{
	LCD_8Bits_Write_Command(0x08);
}
//--------------------------------------------------------------------------------------------------------------------------//

void LCD_8Bits_Print_Character( unsigned char row , unsigned char column , unsigned char ch )
{
	 LCD_8Bits_Cursor_Position(row,column);
	 LCD_8Bits_Write_Data ( ch );

}
//--------------------------------------------------------------------------------------------------------------------------//

void LCD_8Bits_Print_String(unsigned  char row , unsigned char column , unsigned char *str )
{
	LCD_8Bits_Cursor_Position(row,column);
	unsigned char i = 0;

	while (str [i] != '\0')                 // till reaches its end (null Terminator)
	{
		LCD_8Bits_Write_Data (str[i] );
		i++ ;
	}
}
//--------------------------------------------------------------------------------------------------------------------------//

void LCD_8Bits_Print_Number( unsigned char row , unsigned char column ,long num)
{
	LCD_8Bits_Cursor_Position(row,column);

	if (0<=num)
	{

		if (num<=9) LCD_8Bits_Write_Data(num+'0');
		else if(num<=99)
		{
			LCD_8Bits_Write_Data((num/10)+'0');
			LCD_8Bits_Write_Data((num%10)+'0');
		}
		else if(num<=999)
		{
			LCD_8Bits_Write_Data((num/100)+'0');
			LCD_8Bits_Write_Data(((num/10)%10)+'0');
			LCD_8Bits_Write_Data((num%10)+'0');
		}

		else if(num<=99999)
		{
			LCD_8Bits_Write_Data( ( (num/10000) ) +'0');
			LCD_8Bits_Write_Data( ( (num/1000) %10 )+'0');
			LCD_8Bits_Write_Data( ( (num/100) %10 ) +'0');
			LCD_8Bits_Write_Data( ( (num/10) %10 )+'0');
			LCD_8Bits_Write_Data((num%10)+'0');
		}
	}

	if (0>num)
	{
		num = -1*num;

		if (num<=9)
		{
			LCD_8Bits_Write_Data('-');
			LCD_8Bits_Write_Data(num+'0');
		}
		else if(num<=99)
		{
			LCD_8Bits_Write_Data('-');
			LCD_8Bits_Write_Data((num/10)+'0');
			LCD_8Bits_Write_Data((num%10)+'0');
		}
		else if(num<=999)
		{
			LCD_8Bits_Write_Data(0x2d);
			LCD_8Bits_Write_Data((num/100)+'0');
			LCD_8Bits_Write_Data(((num/10)%10)+'0');
			LCD_8Bits_Write_Data((num%10)+'0');
		}

	}

}

#endif
//--------------------------------------------------------------------------------------------------------------------------//


/**********************  LCD with OS Schedule ****************************/





uint8 LCD_Data_G[LCD_LINES][LCD_CHARACTERS+1];



Function_Process_t LCD_4Bits_Initialization_OS (void)		//Working with OS
{
	 Function_Process_t Function_Process=NOT_FINISHED;
	static NO_of_Visit_t LCD_Init_Visit = FIRST_VISIT;
	static uint8 Visit_Counter = 0;

	switch (LCD_Init_Visit)
	{
		case FIRST_VISIT:
		{

			LCD_Init_Flag = 1;
			DIO_Write_Pin(LCD_4Bits_PORT_,LCD_EN,0);

			LCD_4Bits_Write_Command (0x33) ;			//$33 for 4-bit mode
			LCD_4Bits_Write_Command (0x32);				//$32 for 4-bit mode
			LCD_4Bits_Write_Command (0x28) ; 			//$28 for 4-bits, LCD 2 line , 5x7 matrix
			LCD_4Bits_Write_Command (0x0E);				//display on, cursor on

			LCD_4Bits_Write_Command (0x01);				//Clear LCD

			LCD_Init_Visit = WAITING;

			break;
		}
		/*long wait as clear command takes a long time*/
		case WAITING:
		{
			Visit_Counter++;

			if(Visit_Counter == 3)
			{
				LCD_Init_Visit = SECOND_VISIT;
			}

			break;
		}

		case SECOND_VISIT:
		{

			LCD_Init_Visit = DONE;
			LCD_4Bits_Write_Command (0x06) ; 			//shift cursor right

			break;
		}

		case DONE:
		{
			 Function_Process=FINISHED ;
			break;
		}

		default:
		{
			break;
		}
	}

	return Function_Process;
}

//--------------------------------------------------------------------------------------------------------------------------//


// This Function updates one character in the LCD panel.
// This Function Execution Time Duration <= 0.1 ms
// Schedule roughly every 25 ms to Force one Complete Display update every Second
void LCD_Update (void)
{
	static uint8 Line;
	static uint8 Character;

	uint8 Tests, Data;
	uint8 Update_required;

	//Find next Character to be updated
	Tests = LCD_CHARACTERS * LCD_LINES;

	do
	{
		Character++;

		if(Character == LCD_CHARACTERS)
		{
			Character = 0;

			Line++;

			if(Line == LCD_LINES)
			{
				Line=0;
			}
		}

	//Array Contents set to \0 after data are written to LCD

	Update_required = (LCD_Data_G[Line][Character] != '\0');	//The value of Update required will be one if the Current Character doesn't match with null terminator otherwise it will be 0

	} while ((Tests-- > 0 ) && (!Update_required));

	if(!Update_required)
	{
		return;			//No Data in LCD requires Updating
	}

	// Set DDRAM address which character is to be written to

	if(Line == 0)
	{
		LCD_SetDDRAM(0x80 + Character);		//First Line
	}

	else
	{
		LCD_SetDDRAM(0xC0 + Character);		//Second Line
	}

	//This is the Data for Updating

	Data = LCD_Data_G[Line][Character];

	LCD_Send_Byte(Data,1);			//Send Single Data Byte

	//Once Data has been written to LCD
	 LCD_Data_G[Line][Character] = '\0';
}


void LCD_Send_Byte(const uint8 Data , const uint8 Data_Flag)
{
	
	GPIO_WritePortPin(LCD_4Bits_PORT, PIN4, 0);
	GPIO_WritePortPin(LCD_4Bits_PORT, PIN5, 0);
	GPIO_WritePortPin(LCD_4Bits_PORT, PIN6, 0);
	GPIO_WritePortPin(LCD_4Bits_PORT, PIN7, 0);
	
	/*
	DIO_Write_Pin(LCD_4Bits_PORT_ , PIN4 , 0);
	DIO_Write_Pin(LCD_4Bits_PORT_ , PIN5 , 0);
	DIO_Write_Pin(LCD_4Bits_PORT_ , PIN6 , 0);
	DIO_Write_Pin(LCD_4Bits_PORT_ , PIN7 , 0);
	*/
	
	
	GPIO_WritePortPin(LCD_4Bits_PORT, LCD_RS, Data_Flag);
	GPIO_WritePortPin(LCD_4Bits_PORT, LCD_RW, 0);
	GPIO_WritePortPin(LCD_4Bits_PORT, LCD_EN, 0);
	
	/*
	DIO_Write_Pin(LCD_4Bits_PORT_ , LCD_RS , Data_Flag);
	DIO_Write_Pin(LCD_4Bits_PORT_ , LCD_RW , 0);
	DIO_Write_Pin(LCD_4Bits_PORT_ , LCD_EN , 0);
	*/
	
	LCD_Delay();

	// Write the Data (High nybble)
	
	GPIO_WritePortPin(LCD_4Bits_PORT, PIN4,( (Data & 0x10) == 0x10 ) );
	GPIO_WritePortPin(LCD_4Bits_PORT, PIN5,( (Data & 0x20) == 0x20 ) );
	GPIO_WritePortPin(LCD_4Bits_PORT, PIN6,( (Data & 0x40) == 0x40 ) );
	GPIO_WritePortPin(LCD_4Bits_PORT, PIN7,( (Data & 0x80) == 0x80 ) );
	
	/*
	DIO_Write_Pin(LCD_4Bits_PORT_ , PIN4 , ( (Data & 0x10) == 0x10 ) );
	DIO_Write_Pin(LCD_4Bits_PORT_ , PIN4 , ( (Data & 0x20) == 0x20 ) );
	DIO_Write_Pin(LCD_4Bits_PORT_ , PIN4 , ( (Data & 0x40) == 0x40 ) );
	DIO_Write_Pin(LCD_4Bits_PORT_ , PIN4 , ( (Data & 0x80) == 0x80 ) );
	*/

	LCD_Delay();

	GPIO_WritePortPin(LCD_4Bits_PORT, LCD_EN,1 );
	//DIO_Write_Pin(LCD_4Bits_PORT_ , LCD_EN , 1);
	
	//Latch the High nybble

	LCD_Delay();

	GPIO_WritePortPin(LCD_4Bits_PORT, LCD_EN,0 );
	//DIO_Write_Pin(LCD_4Bits_PORT_ , LCD_EN , 0);

	LCD_Delay();
	
	
	GPIO_WritePortPin(LCD_4Bits_PORT, PIN4, 0);
	GPIO_WritePortPin(LCD_4Bits_PORT, PIN5, 0);
	GPIO_WritePortPin(LCD_4Bits_PORT, PIN6, 0);
	GPIO_WritePortPin(LCD_4Bits_PORT, PIN7, 0);
	
	/*
	DIO_Write_Pin(LCD_4Bits_PORT_ , PIN4 , 0);
	DIO_Write_Pin(LCD_4Bits_PORT_ , PIN5 , 0);
	DIO_Write_Pin(LCD_4Bits_PORT_ , PIN6 , 0);
	DIO_Write_Pin(LCD_4Bits_PORT_ , PIN7 , 0);
	*/
	
	GPIO_WritePortPin(LCD_4Bits_PORT, LCD_RS, Data_Flag);
	GPIO_WritePortPin(LCD_4Bits_PORT, LCD_RW, 0);
	GPIO_WritePortPin(LCD_4Bits_PORT, LCD_EN, 0);
	
	/*
	DIO_Write_Pin(LCD_4Bits_PORT_ , LCD_RS , Data_Flag);
	DIO_Write_Pin(LCD_4Bits_PORT_ , LCD_RW , 0);
	DIO_Write_Pin(LCD_4Bits_PORT_ , LCD_EN , 0);
	*/

	LCD_Delay();

	// Write the Data (LOW Nybble)
	
	GPIO_WritePortPin(LCD_4Bits_PORT, PIN4,( (Data & 0x01) == 0x01 ) );
	GPIO_WritePortPin(LCD_4Bits_PORT, PIN5,( (Data & 0x02) == 0x02 ) );
	GPIO_WritePortPin(LCD_4Bits_PORT, PIN6,( (Data & 0x04) == 0x04 ) );
	GPIO_WritePortPin(LCD_4Bits_PORT, PIN7,( (Data & 0x08) == 0x08 ) );
	
	
	/*
	DIO_Write_Pin(LCD_4Bits_PORT_ , PIN4 , ( (Data & 0x01) == 0x01 ) );
	DIO_Write_Pin(LCD_4Bits_PORT_ , PIN4 , ( (Data & 0x02) == 0x02 ) );
	DIO_Write_Pin(LCD_4Bits_PORT_ , PIN4 , ( (Data & 0x04) == 0x04 ) );
	DIO_Write_Pin(LCD_4Bits_PORT_ , PIN4 , ( (Data & 0x08) == 0x08 ) );
	*/

	LCD_Delay();

	GPIO_WritePortPin(LCD_4Bits_PORT, LCD_EN, 1);
	//DIO_Write_Pin(LCD_4Bits_PORT_ , LCD_EN , 1);
	//Latch the Low nybble

	LCD_Delay();

	GPIO_WritePortPin(LCD_4Bits_PORT, LCD_EN, 0);
	//DIO_Write_Pin(LCD_4Bits_PORT_ , LCD_EN , 0);

	LCD_Delay();

}

void LCD_Control_Cursor (const uint8 VISIBLE , const uint8 BLINKING, const uint8 ADDRESS)
{
	// Cursor / Blinking appears at current DDRAM address

	// Use SetDDRAM() at alter the sursor position

	uint8 Command = 0x0C;

	if(VISIBLE)
	{
		Command |= 0x02;
	}

	if(BLINKING)
	{
		Command |= 0x01;
	}

	LCD_Send_Byte(Command,0);

	LCD_SetDDRAM(ADDRESS);
}


// Set the DDRAM to a particular address.
// Choose the DDRAM address we wish to write to.

void LCD_SetDDRAM (uint8 ADDRESS)
{

	ADDRESS &= 0x7f;

	ADDRESS |= 0x80;

	LCD_Send_Byte(ADDRESS,0);
}

//UDC means User Defined Character
void  LCD_Create_Character(const uint8 UDC_ID , const uint8 *const P_UDC)
{
	uint8 Row;
	uint8 Address;

	//Select CG RAM, appropriate address

	Address = 0x40 + (UDC_ID <<3);

	LCD_Send_Byte(Address,0);

	//Now Write the Data

	for(Row = 0 ; Row < 8 ; Row++)
	{
		LCD_Send_Byte(P_UDC[Row] ,1);
	}

	//Select DD RAM - address 0
	Address = 0x80;

	// Ensure that next data write is to DDRAM

	LCD_Send_Byte(Address,0);
}


// This Function Provides a short delay for the LCD
void LCD_Delay (void)
{
	_delay_us(15);
	
	/*
	OCR0=30;
	TIMER_Start(TIMER0);

	while( (TIFR & (1<<OCF0)) == 0);
	TIFR |= 1<<OCF0 ;		//Clear the OCF0 By Writing Logic High (one) in TIRF

	TIMER_Stop(TIMER0);
	*/
}





/**************************/
	/* thumb up icon

	1	1	0	0	0		ROW[0]= 0x18
	1	1	0	0	0		ROW[1]= 0x18
	1	1	1	1	1		ROW[2]= 0x1F
	1	1	1	1	1		ROW[3]= 0x1F
	1	1	1	1	1		ROW[4]= 0x1F
	1	1	1	1	0		ROW[5]= 0x1E
	1	1	1	0	0		ROW[6]= 0x1C

	uint8 Thumb_UP [7] = { 0x18 , 0x18 , 0x1F , 0x1F , 0x1F , 0x1E , 0x1C};
	*/
 /************************/

 /**************************/
	/* thumb up icon

	0	0	1	1	1		ROW[0]= 0x07
	0	1	1	1	1		ROW[1]= 0x0F
	1	1	1	1	1		ROW[2]= 0x1F
	1	1	1	1	1		ROW[3]= 0x1F
	1	1	1	1	1		ROW[4]= 0x1F
	0	0	0	1	1		ROW[5]= 0x03
	0	0	0	1	1		ROW[6]= 0x03

	uint8 Thumb_Down [7] = { 0x07 , 0x0F , 0x1F , 0x1F , 0x1F , 0x03 , 0x03};
	*/
 /************************/

