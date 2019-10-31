/*
 * LCD_HEADER.h
 *
 *
 *  Author: HOBA
 */


#ifndef LCD_HEADER_H_
#define LCD_HEADER_H_

#ifndef F_CPU
#define F_CPU	8000000ul
#endif

//define libraries

#include <avr/io.h>
#include <util/delay.h>
#include "DIO.h"
#include "Timer.h"



#define _4_BITS		1
#define _8_BITS		0

#define LCD_MODE  _4_BITS

//define LCD 4Bits MACROS

#define LCD_4Bits_PORT_  	 PORT_A	//LCD DATA PORT from PD4 to PD7 // we can’t take PD3 as it is existed in least nibble.
#define LCD_4Bits_DDR_   	 PORT_A	//LCD DATA DDR from PD4 to PD7

#define LCD_4Bits_PORT 		 PORTA	//LCD DATA PORT from PD4 to PD7 // we can’t take PD3 as it is existed in least nibble.
#define LCD_4Bits_DDR    	 DDRA	//LCD DATA DDR from PD4 to PD7

#define LCD_4Bits_PIN		 PINA	//LCD DATA PIN from PD4 to PD7 (not used)

//define LCD 8Bits MACROS
#define	LCD_8Bits_DATA_PORT		PORTD		    //LCD	DATA PORT 	from PA0 to PA7
#define	LCD_8Bits_DATA_DDR 		DDRD			//LCD	DATA DDR
#define	LCD_8Bits_DPIN	        PIND			//LCD	DATA PIN	(not used)
#define	LCD_8Bits_COMMAND_PORT  PORTC	        //LCD	COMMANDS	from PB0 to PB2
#define	LCD_8Bits_COMMAND_DDR 	DDRC			//LCD	COMMANDS	DDR
#define	LCD_CPIN				PINC			//LCD	COMMANDS	PIN (not used)


//define LCD Control Pins MACROS
#define	LCD_RS           		 1				//LCD	RS	P0
#define	LCD_RW    				 2				//LCD	RW	P1
#define	LCD_EN     			     3				//LCD	EN	P2



//---------------- Section of LCD with OS APIs --------------//

#define LCD_LINES 2
#define LCD_CHARACTERS 16

extern uint8 LCD_Init_Flag;
extern uint8 LCD_Data_G[LCD_LINES][LCD_CHARACTERS+1];
extern uint8 Char_G ;


Function_Process_t LCD_4Bits_Initialization_OS (void);
void LCD_Update (void);
void LCD_Create_Character(const uint8 UDC_ID , const uint8 *const P_UDC);
void LCD_Send_Byte(const uint8 Data , const uint8 Data_Flag);
void LCD_Control_Cursor (const uint8 VISIBLE , const uint8 BLINKING, const uint8 ADDRESS);
void LCD_Delay (void);
void LCD_SetDDRAM (uint8 ADDRESS);

/*
void LCD_KEYPAD_Init (void);
void LCD_Keypad_Update (void);
*/


//---------------------------------------------------------------------------------------//


#if LCD_MODE


//LCD_4Bits_FUNCTIONS PROTOTYPE

void LCD_4Bits_Initialization (void);
void LCD_4Bits_Write_Command ( unsigned char Command );
void LCD_4Bits_Write_Data ( unsigned char Data );
void LCD_4Bits_Cursor_Position( unsigned char x , unsigned char y);   		// define Cursor Position in the LCD
void LCD_4Bits_Clear (void);
void LCD_4Bits_Shift_Right(void);
void LCD_4Bits_Shift_Left(void);
void LCD_4Bits_Print_Character( unsigned char row ,unsigned char column , unsigned char ch );
void LCD_4Bits_Print_String( unsigned char row , unsigned char column , unsigned char *str );
void LCD_4Bits_Print_Number(unsigned char row , unsigned char column , long num);



#else


//LCD_8Bits_FUNCTIONS PROTOTYPE
void LCD_8Bits_Initialization (void);
void LCD_8Bits_Write_Command ( unsigned char Command );
void LCD_8Bits_Write_Data ( unsigned char Data );
void LCD_8Bits_Cursor_Position( unsigned char x , unsigned char y);   		// define Cursor Position in the LCD
void LCD_8Bits_Clear (void);
void LCD_8Bits_Shift_Right(void);
void LCD_8Bits_Shift_Left(void);
void LCD_8Bits_Print_Character( unsigned char row ,unsigned char column , unsigned char ch );
void LCD_8Bits_Print_String( unsigned char row ,unsigned char column , unsigned char *str );
void LCD_8Bits_Print_Number(unsigned char row ,unsigned char column , long num);


#endif


#endif
