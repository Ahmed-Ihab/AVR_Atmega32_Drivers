/*
 * I2C_Cfg.h
 *
 * Created: 2/23/2020 3:16:02 PM
 *  Author: Ahmed Ehab
 */ 


#ifndef I2C_CFG_H_
#define I2C_CFG_H_

//----------------- Inclusion of Main Header Files Section -----------------//

#include "std_types.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// ----------------------------------------------------------------------- //

#define MAX_I2C_BUFFER_DAtA_SIZE 100



//--------------------- Declaration of ENUM Section ----------------------//

typedef void (*ptrToFunc)(void);



typedef enum {I2C_MASTER,I2C_SLAVE} I2C_Mode_t;
typedef enum {_50KHZ,} I2C_Baudrate_t;
typedef enum {WRITING_ONLY=0 , TRANSMITTING_ONLY=0 ,TRANSMITTING=0 , WRITING=0 , READIN_ONLY=1 , RECEIVING_ONLY=1 , RECEIVING=1 , READING=1 , WRITING_THEN_READING=2 , TRANSMITTING_THEN_RECEIVING = 2,READING_THEN_WRITING=3,RECEIVING_THEN_TRANSMITTING=3,NOT_VALID_SEQUENCE=4} I2C_Sequence_t;
typedef enum {USED,NOT_USED} Buffer_Usage_t;
//typedef enum {WRITING_ONLY=0 , TRANSMITTING_ONLY=0 ,TRANSMITTING=0 , WRITING=0 , READIN_ONLY=1 , RECEIVING_ONLY=1 , RECEIVING=1 , READING=1 ,NOT_VALID_SEQUENCE=4} I2C_Sequence_t;
typedef enum {Initialized,NOT_Initialized}I2C_Initialize_t;
typedef enum {INTERRUPT,NO_INTERRUPT=1,POLLING=1}I2C_Interrupt_t;

typedef enum
{
	FREE_ERROR,
	I2C_MODE_ERROR,
	I2C_NODE_ADDRESS_ERROR,
	I2C_BAUDRATE_ERROR,
	I2C_INTERRUPT_ERROR,
	I2C_SEQUENCE_ERROR,
	I2C_INITIALIZE_ERROR,
	I2C_BUFFER_ERROR,
	I2C_STATUS_ERROR,
	I2C_STATE_ERROR,
	GENERAL_ERROR
}I2C_Error_t;

typedef enum {I2C_NO_REQUEST=0,I2C_FINISHED_REQUEST=0,I2C_REQUEST}I2C_Request_t;
// ----------------------------------------------------------------------- //


//----------------- Declaration of Structure Section -----------------//

//------------- Data ------------- || ------------- Buffer_Index ------------- || ------------- User_Size ------------- || ------------- Buffer_Usage ------------- ||------------- Function_Notification ------------- //

typedef struct
{
	//uint8 Data[MAX_I2C_BUFFER_DAtA_SIZE];
	uint8 * Ptr_to_Data;
	uint8 Buffer_Index;
	uint8 User_Size;
	Buffer_Usage_t Buffer_Usage ;
	ptrToFunc Function_Notification ;
	
}St_I2C_Buffer;


//------------- I2C_Mode ------------- || ------------- Source_Address ------------- || ------------- Destination_Address ------------- || ------------- I2C_Baudrate -------------- || ------------- I2C_Interrupt ------------- || ------------- I2C_Sequence ------------- ||-------------  I2C_Request -------------- || ------------- I2C_Initialize ------------- //

typedef struct
{
	I2C_Mode_t I2C_Mode;
	uint8 Source_Address;
	uint8 Destination_Address;
	I2C_Baudrate_t I2C_Baudrate;
	I2C_Interrupt_t I2C_Interrupt;
	I2C_Sequence_t I2C_Sequence;
	I2C_Request_t I2C_Request ;
	I2C_Initialize_t I2C_Initialize;
	
}St_I2C_Cfg;

// ----------------------------------------------------------------------- //





extern St_I2C_Cfg I2C_Cfg ; 

#endif /* I2C_CFG_H_ */