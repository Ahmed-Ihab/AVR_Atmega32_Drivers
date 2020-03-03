/*
 * I2C_Cfg.c
 *
 * Created: 2/23/2020 3:16:19 PM
 *  Author: Ahmed Ehab
 */ 

#include "I2C_Cfg.h"

//------------- I2C_Mode ------------- || ------------- Source_Address ------------- || ------------- Destination_Address ------------- || ------------- I2C_Baudrate -------------- || ------------- I2C_Interrupt ------------- || ------------- I2C_Sequence ------------- ||-------------  I2C_Request -------------- || ------------- I2C_Initialize ------------- //


St_I2C_Cfg I2C_Cfg = {I2C_MASTER		,					0b00000001		         ,					0b00000010							,					_50KHZ					,					INTERRUPT				 ,					TRANSMITTING			 ,						I2C_REQUEST			,					NOT_DEFINED			 };


//St_I2C_Cfg I2C_Cfg = {I2C_SLAVE		,					0b00000010		         ,					0b00000001							,					_50KHZ					,					INTERRUPT				 ,					RECEIVING			 ,						I2C_REQUEST			,					NOT_DEFINED			 };
