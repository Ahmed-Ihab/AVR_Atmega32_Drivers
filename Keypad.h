
#ifndef __Keypad_H__
#define __Keypad_H__

#include <avr/io.h>
#include "std_types.h"
#include "DIO.h"
#include "LCD.h"

// Determine your Keypad Dimension
#define N_COL  3
#define N_ROW  3


// Locate your keypad at which port does it exist
#define KEYPAD_PORT_OUT  PORTC
#define KEYPAD_PORT_DDR  DDRC
#define KEYPAD_PIN_IN    PINC

// Locate your keypad at which port does it exist
#define KEYPAD_PORT		  PORT_C
#define KEYPAD_PORT_DDR_  PORT_C
#define KEYPAD_PIN_IN_    PINC


//------- Configure the Keypad------------//
//----------------------------------------//
//#define KEYPAD_PORT     PORTC

#define GPIO_WritePortPin(PORT, PIN, DATA)          ((PORT) = (PORT & (~(1 << PIN)))|(DATA << PIN))

#define GPIO_ReadPortPin(PORT, PIN)    (((PORT) & (1 << PIN)) >> (PIN))

#define CLEAR_COLUMN_0		GPIO_WritePortPin(KEYPAD_PORT, 5, LOW)
#define CLEAR_COLUMN_1		GPIO_WritePortPin(KEYPAD_PORT, 6, LOW)
#define CLEAR_COLUMN_2		GPIO_WritePortPin(KEYPAD_PORT, 7, LOW)
#define CLEAR_COLUMN_3		GPIO_WritePortPin(KEYPAD_PORT, 0, LOW)

#define SET_COLUMN_0		GPIO_WritePortPin(KEYPAD_PORT, 5, HIGH)
#define SET_COLUMN_1		GPIO_WritePortPin(KEYPAD_PORT, 6, HIGH)
#define SET_COLUMN_2		GPIO_WritePortPin(KEYPAD_PORT, 7, HIGH)
#define SET_COLUMN_3		GPIO_WritePortPin(KEYPAD_PORT, 0, HIGH)

#define READ_COLUMN_0		GPIO_ReadPortPin(KEYPAD_PORT, 5)
#define READ_COLUMN_1		GPIO_ReadPortPin(KEYPAD_PORT, 6)
#define READ_COLUMN_2		GPIO_ReadPortPin(KEYPAD_PORT, 7)
#define READ_COLUMN_3		GPIO_ReadPortPin(KEYPAD_PORT, 0)


#define CLEAR_ROW_0		GPIO_WritePortPin(KEYPAD_PORT, 2, LOW)
#define CLEAR_ROW_1		GPIO_WritePortPin(KEYPAD_PORT, 3, LOW)
#define CLEAR_ROW_2		GPIO_WritePortPin(KEYPAD_PORT, 4, LOW)
#define CLEAR_ROW_3		GPIO_WritePortPin(KEYPAD_PORT, 0, LOW)

#define SET_ROW_0		GPIO_WritePortPin(KEYPAD_PORT, 2, HIGH)
#define SET_ROW_1		GPIO_WritePortPin(KEYPAD_PORT, 3, HIGH)
#define SET_ROW_2		GPIO_WritePortPin(KEYPAD_PORT, 4, HIGH)
#define SET_ROW_3		GPIO_WritePortPin(KEYPAD_PORT, 0, HIGH)

#define READ_ROW_0		GPIO_ReadPortPin(KEYPAD_PORT, 2)
#define READ_ROW_1		GPIO_ReadPortPin(KEYPAD_PORT, 3)
#define READ_ROW_2		GPIO_ReadPortPin(KEYPAD_PORT, 4)
#define READ_ROW_3		GPIO_ReadPortPin(KEYPAD_PORT, 0)



// ----- Public Functions ------------------//
void Keypad_Init (void);
void keypad_Update (void);
uint8 Keypad_Get_Data_from_Buffer (uint8* const );
void Keypad_Clear_Buffer(void);


//-------------------------------------------------------- Section of Keypad without OS and it can use Timeout mechanism ---------------------------------------------------------------//

// Determine your Keypad Dimension
#define N_COL  3
#define N_ROW  3

// Determine your Digit Size for your input
#define DIGIT_SIZE 3

#define SIZE   6
#define YES	   1
#define NO     0

#define TIMEROUT_VALUE 18

// Determine whether you need to return Integer Numbers from keypad or Ascii Value (Characters) from KeyPad
#define PRINT_NUMBERS_From_KEYPAD YES


//----------------------- Section of Public Function Prototype of Keypad API's -----------------//
short  KEYPAD_INPUT_Number(void);
char *  KEYPAD_INPUT_String(void);



#endif // __Keypad_H__