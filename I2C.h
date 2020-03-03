/*
 * I2C.h
 *
 * Created: 2/23/2020 3:15:46 PM
 *  Author: Ahmed Ehab
 */ 

//----------------- Constant Macros and Function Macros Section -----------------//
#ifndef I2C_H_
#define I2C_H_


#define I2C_PORT PORTC
#define SDA			1
#define SCL			0

#define I2C_Clear_Status_Register   TWSR = 0x00

#define I2C_ENABLE_ACK_SEND			TWCR |= (1<<TWEA)
#define I2C_DISABLE_ACK_SEND	    TWCR &= ~(1<<TWEA)		//means send negative Acknowledge

#define I2C_ENABLE_PERIPHERAL	    TWCR |= (1<<TWEN)
#define I2C_CLEAR_INTERRUPT_FLAG    TWCR |= (1<<TWINT)
#define I2C_ENABLE_INTERRUPT		TWCR |= (1 << TWIE) | (1 << TWEN)
#define I2C_DISABLE_INTERRUPT		TWCR &= ~(1<<TWIE)

#define GLOBAL_INTERRUPT_ENABLE		SREG |= (1<<7)
#define GLOBAL_INTERRUPT_DISABLE	SREG &= ~(1<<7)

#define I2C_SEND_START_CONDITION	TWCR |= (1<<TWSTA)
#define I2C_SEND_STOP_CONDITION		TWCR |= (1<<TWSTO)

#define I2C_CLEAR_NODE_ADDRESS		TWAR = 0
#define I2C_SET_NODE_ADDRESS(SLAVEADDRESS)	(TWAR = (SLAVEADDRESS) )

#define I2C_POLL_ON_INTERRUPT_FLAG	while( ( TWCR & (1<<TWINT) ) == 0)

#define I2C_GET_STATUS(X) (X= TWSR & 0xF8)

#define I2C_LOAD_DATA(Y)  (TWDR=Y)
#define I2C_GET_DATA(Z)   (Z=TWDR)

#define I2C_PRESCALER_CLEAR		 0x11111100
#define I2C_PRESCALER_1          0x00000000
#define I2C_PRESCALER_4			 0x00000001
#define I2C_PRESCALER_16         0x00000010
#define I2C_PRESCALER_64         0x00000011


// ------------------------------------------------------------------- //


//--------- Inclusion of Header Files that depends on previous declaration Section --------------//

#include "I2C_Cfg.h"

// ----------------------------------------------------------------------- //


//---------------------- Public Function APIs Section ----------------------//

extern I2C_Error_t I2C_Init (void);

extern I2C_Error_t I2C_Setup ( St_I2C_Buffer * I2C_Buffer_TX ,  St_I2C_Buffer * I2C_Buffer_RX , I2C_Sequence_t I2C_Sequence );

extern void I2C_Set_Callback ( void (*ptr)(void) );

// ----------------------------------------------------------------------- //

//----------------- Global Variables and Pointers Section -----------------//
extern void (*g_callBackPtr_I2C)(void)   ;

// ----------------------------------------------------------------------- //





#endif /* I2C_H_ */