/*
 * I2C.c
 *
 * Created: 2/23/2020 3:15:15 PM
 *  Author: Ahmed Ehab
 */ 

#include "I2C.h"

#define  HEXA_DECIMAL_SIZE	3
//--------------------------------- Private Enum -----------------------------------//
typedef enum {
	IDLE,
	SENDING_START_CONDITION,
	SENDING_SLAVEADDRESS_WRITE,
	SENDING_BYTE,
	SENDING_BYTE_COMPLETE,
	SENDING_REPEATED_START,
	SLAVE_SEND_COMPLETE,
	SENDING_SLAVEADDRESS_READ,
	RECEIVING_BYTE,
	RECEIVING_BYTE_COMPLETE,
	SLAVE_RECEIVE_COMPLETE,
	SENDING_STOP_CONDITION,
	TRANSMISSION_FINISHED
}I2C_State_t;

typedef enum
{
	//---------------------MASTER_DEVICE_Transmitting_Mode------------------------//
	
	MASTER_START_CONDITION_TRANSMITTED =								0x08 ,
	
	MASTER_SLAVEADDRESS_WRITE_OPERATION_TRANSMITTED_ACK_RECEIVED =		0x18 ,
	MASTER_SLAVEADDRESS_WRITE_OPERATION_TRANSMITTED_NACK_RECEIVED =		0x20 ,
	
	MASTER_SLAVEADDRESS_READ_OPERATION_TRANSMITTED_ACK_RECEIVED =		0x40 ,
	MASTER_SLAVEADDRESS_READ_OPERATION_TRANSMITTED_NACK_RECEIVED =		0x48 ,
	
	MASTER_DATA_TRANSMITTED_ACK_RECEIVED  =								0x28 ,
	MASTER_DATA_TRANSMITTED_NACK_RECEIVED =								0x30 ,
	
	MASTER_REPEATED_START_TRANSMITTED =									0x10 ,
	
	//----------------------------------------------------------------------------//

	//---------------------MASTER_DEVICE_Receiving_Mode------------------------//

	MASTER_DATA_RECEIVED_ACK_TRANSMITTED =								0x50 ,
	MASTER_DATA_RECEIVED_NACK_TRANSMITTED =								0x58 ,

	//---------------------SLAVE_DEVICE_Receiving_Mode------------------------//
	
	SLAVE_SLAVEADDRESS_WRITE_OPERATION_RECEIVED_ACK_TRANSMITTED =		0x60 ,
	SLAVE_SLAVEADDRESS_WRITE_OPERATION_RECEIVED_BUT_ARBITRATION_LOST =	0x68 ,	//Arbitration Lost as Master and addressed as slave
	
	SLAVE_SLAVEADDRESS_READ_OPERATION_RECEIVED_ACK_TRANSMITTED =		0xA8 ,
	SLAVE_SLAVEADDRESS_READ_OPERATION_RECEIVED_BUT_ARBITRATION_LOST =	0xB0 ,		//Arbitration Lost as Master and addressed as slave

	SLAVE_DATA_RECEIVED_ACK_TRANSMITTED =								0x80 ,
	SLAVE_DATA_RECEIVED_NACK_TRANSMITTED =								0x88 ,
	
	SLAVE_STOP_OR_REPEATED_START_RECEIVED =								0xA0 ,
	SLAVE_GENERAL_CALL_RECEIVED_ACK_RECEIVED =							0x70 ,
	
	//---------------------SLAVE_DEVICE_Transmitting_Mode------------------------//
		
	SLAVE_DATA_TRANSMITTED_ACK_RECEIVED =								0xB8 ,
	SLAVE_DATA_TRANSMITTED_NACK_RECEIVED =								0xC0 ,
	
	//----------------------------------------------------------------------------//

	//-------------------------------OTHERS---------------------------------------//
	
	BUS_ERROR =															   0 ,
	Master_Arbitration_Lost =											0x38 ,
	NO_RELEVANT_STATE =												    0xF8 ,

}I2C_Status_t;

typedef enum {LOCKED,UNLOCKED}I2C_State_Lock_t;
	
typedef enum {WRITE,READ}I2C_Operation_t;

// --------------------------------------------------------------------------------//


//---------------------- Private Static Function APIs Section ----------------------//
static uint8 * Decimal_to_Hex (int Decimal_Number);
static  void I2C_Interrupt_Handler (void);
static  void I2C_vidGetStatus(uint8 * COPY_u8Data);
static  void I2C_vidStart(void);
static  void I2C_vidStop(void);
static  void I2C_vidSendSlaveAddress(uint8 COPY_u8Address,I2C_Operation_t COPY_u8Operation);
static  void I2C_vidSendByte(uint8 COPY_u8Data);
static  void I2C_vidReadWithAch(uint8 * COPY_pu8Data);
// --------------------------------------------------------------------------------//

//---------------- Private Global Variables and Pointers Section -----------------//

//static I2C_Status_t I2C_Status = NO_RELEVANT_STATE;
//static I2C_State_t I2C_State = IDLE;
//static I2C_State_Lock_t I2C_State_Lock = UNLOCKED;
static volatile uint8 Look_up_I2C_Status_Table [32]=
{
	0x08,	//MASTER_START_CONDITION_TRANSMITTED							[0]
	0x18,	//MASTER_SLAVEADDRESS_WRITE_OPERATION_TRANSMITTED_ACK_RECEIVED  [1]
	0x20,   //MASTER_SLAVEADDRESS_WRITE_OPERATION_TRANSMITTED_NACK_RECEIVED [2]
	0x40,	//MASTER_SLAVEADDRESS_READ_OPERATION_TRANSMITTED_ACK_RECEIVED   [3]
	0x48,	//MASTER_SLAVEADDRESS_READ_OPERATION_TRANSMITTED_NACK_RECEIVED  [4]
	0x28,	//MASTER_DATA_TRANSMITTED_ACK_RECEIVED							[5]
	0x30,	//MASTER_DATA_TRANSMITTED_NACK_RECEIVED							[6]
	0x10,	//MASTER_REPEATED_START_TRANSMITTED								[7]
	0x50 ,  //MASTER_DATA_RECEIVED_ACK_TRANSIMITTED							[8]
	0x58 ,	//MASTER_DATA_RECEIVED_NACK_TRANSIMITTED						[9]
	0x60,   //SLAVE_SLAVEADDRESS_WRITE_OPERATION_RECEIVED_ACK_TRANSIMITTED  [10]
	0x68,	//Arbitration Lost as Master and addressed as slave				[11]
	0xA8,	//SLAVE_SLAVEADDRESS_READ_OPERATION_RECEIVED_ACK_TRANSIMITTED	[12]
	0xB0,	//Arbitration Lost as Master and addressed as slave				[13]
	0x80,	//SLAVE_DATA_RECEIVED_ACK_TRANSMITTED							[14]
	0x88,	//SLAVE_DATA_RECEIVED_NACK_TRANSMITTED							[15]
	0xA0,	//SLAVE_STOP_OR_REPEATED_START_RECEIVED							[16]
	0x70,	//SLAVE_GENERAL_CALL_RECEIVED_ACK_RECEIVED						[17]
	0xB8,	//SLAVE_DATA_TRANSMITTED_ACK_RECEIVED							[18]
	0xC0,	//SLAVE_DATA_TRANSMITTED_NACK_RECEIVED							[19]
	0,		//BUS_ERROR														[20]
	0x38,	//Master_Arbitration_Lost										[21]
	0xF8,	//NO_RELEVANT_STATE												[22]
	
};

void (*g_callBackPtr_I2C)(void);
 St_I2C_Buffer * I2C_TX_Buffer = NULL_POINTER ;  
 St_I2C_Buffer * I2C_RX_Buffer = NULL_POINTER ;
 
 static I2C_Error_t I2C_ERROR_IN_INTERRUPT = FREE_ERROR;
 

// ------------------------------------------------------------------------------- //


static uint8 Repeated_Start_Flag = 0 ;


typedef enum{SLAVE_SENDING,SLAVE_RECEIVING,SLAVE_NOT_DEFINED}Slave_Transmission_Flag_t;

static Slave_Transmission_Flag_t Slave_Transmission_Flag = SLAVE_NOT_DEFINED;

static void I2C_Interrupt_Handler (void)
{
	
	I2C_Status_t  local_I2C_Status ;
	
	I2C_vidGetStatus (&local_I2C_Status);

	switch (I2C_Cfg.I2C_Mode)
	{
		
		case I2C_MASTER:
		{

			I2C_Status_t I2C_MASTER_TX_STATUS ;
			I2C_MASTER_TX_STATUS = local_I2C_Status;
			
			switch (I2C_MASTER_TX_STATUS)
			{
				
				case MASTER_START_CONDITION_TRANSMITTED:
				{
				
					if(Repeated_Start_Flag == 0)
					{
						if((I2C_Cfg.I2C_Sequence == TRANSMITTING) || (I2C_Cfg.I2C_Sequence == TRANSMITTING_THEN_RECEIVING))
						{
							
							I2C_vidSendSlaveAddress( I2C_Cfg.Destination_Address , WRITE);
									
						}
						else if ((I2C_Cfg.I2C_Sequence == RECEIVING) || (I2C_Cfg.I2C_Sequence == RECEIVING_THEN_TRANSMITTING))
						{
							I2C_vidSendSlaveAddress( I2C_Cfg.Destination_Address , READ);
								
						}
					}
					
					else if(Repeated_Start_Flag == 1)
					{
						if( (I2C_Cfg.I2C_Sequence == TRANSMITTING_THEN_RECEIVING))
						{
							I2C_vidSendSlaveAddress( I2C_Cfg.Destination_Address , READ);
							
						}
						else if ((I2C_Cfg.I2C_Sequence == RECEIVING_THEN_TRANSMITTING))
						{
							I2C_vidSendSlaveAddress( I2C_Cfg.Destination_Address , WRITE);
							
						}
					}
					
			
					
					break;
				}
				
				case MASTER_SLAVEADDRESS_WRITE_OPERATION_TRANSMITTED_ACK_RECEIVED:
				{	
					I2C_vidSendByte(I2C_TX_Buffer->Ptr_to_Data[I2C_TX_Buffer->Buffer_Index]);		//First Byte
					break;
				}
				
				case MASTER_SLAVEADDRESS_WRITE_OPERATION_TRANSMITTED_NACK_RECEIVED:
				{
					
					//Do Nothing
					break;
				}
				
				case MASTER_SLAVEADDRESS_READ_OPERATION_TRANSMITTED_ACK_RECEIVED:
				{
					//Do Nothing
					break;
				}
				
				case MASTER_SLAVEADDRESS_READ_OPERATION_TRANSMITTED_NACK_RECEIVED:
				{
					//Do Nothing
					break;
				}
				
				case MASTER_DATA_TRANSMITTED_ACK_RECEIVED:
				{
					if(I2C_TX_Buffer->Buffer_Index >= I2C_TX_Buffer->User_Size)
					{						
						if(I2C_Cfg.I2C_Sequence == TRANSMITTING_THEN_RECEIVING)
						{
							Repeated_Start_Flag = 1;
							
							I2C_vidStart();	
						}
						
						else if ( I2C_Cfg.I2C_Sequence == TRANSMITTING)
						{
							if(I2C_TX_Buffer->Function_Notification != NULL_POINTER)
							{
								I2C_TX_Buffer->Function_Notification();
								I2C_TX_Buffer->Buffer_Usage = NOT_USED;
							}
							
							I2C_vidStop();
						}				
						
					}
					
					else if(I2C_TX_Buffer->Buffer_Index < I2C_TX_Buffer->User_Size)
					{
						I2C_TX_Buffer->Buffer_Index++;
						I2C_vidSendByte(I2C_TX_Buffer->Ptr_to_Data[I2C_TX_Buffer->Buffer_Index]);		//Send Other Bytes
					}
					
					else
					{
						//Shouldn't be here
						//Do Nothing
					}
					
					break;
				}
				
				case MASTER_DATA_TRANSMITTED_NACK_RECEIVED:
				{
					//Do Nothing
					break;
				}
				
				
				case MASTER_REPEATED_START_TRANSMITTED:
				{
					 if(Repeated_Start_Flag == 1)
					{
						if( (I2C_Cfg.I2C_Sequence == TRANSMITTING_THEN_RECEIVING))
						{
							I2C_vidSendSlaveAddress( I2C_Cfg.Destination_Address , READ);
							
						}
						else if ((I2C_Cfg.I2C_Sequence == RECEIVING_THEN_TRANSMITTING))
						{
							I2C_vidSendSlaveAddress( I2C_Cfg.Destination_Address , WRITE);
							
						}
					}
				
					break;
				}
				

				case MASTER_DATA_RECEIVED_ACK_TRANSMITTED :
				{

					I2C_RX_Buffer->Ptr_to_Data[I2C_RX_Buffer->Buffer_Index] = TWDR;
									
					if(I2C_RX_Buffer->Buffer_Index >= I2C_RX_Buffer->User_Size)
					{
			
						if(I2C_Cfg.I2C_Sequence == RECEIVING_THEN_TRANSMITTING)
						{
							Repeated_Start_Flag=1;
							I2C_vidStart();
						}
									
						else if ( I2C_Cfg.I2C_Sequence == RECEIVING_ONLY)
						{
							if(I2C_RX_Buffer->Function_Notification != NULL_POINTER)
							{
								I2C_RX_Buffer->Function_Notification();
								I2C_RX_Buffer->Buffer_Usage = NOT_USED;
							}
							
							I2C_vidStop();
						}
									
						else
						{
							//Shouldn't be here
							//Do Nothing
						}			
					}
						
					else if(I2C_RX_Buffer->Buffer_Index < I2C_RX_Buffer->User_Size)
					{
						I2C_RX_Buffer->Buffer_Index++;
						
					}
						
					else
					{
						//Shouldn't be here
						//Do Nothing
					}
					break;
				}
				
				case MASTER_DATA_RECEIVED_NACK_TRANSMITTED :
				{
					// Do Nothing
					break;
				}
				
				default:
				{
					I2C_ERROR_IN_INTERRUPT = I2C_STATUS_ERROR;
					break;
				}
			}	
			
			break;
		}
		
		case I2C_SLAVE:
		{
			
			I2C_Status_t I2C_SLAVE_TX_STATUS ;
			I2C_SLAVE_TX_STATUS = local_I2C_Status;
			
			switch (I2C_SLAVE_TX_STATUS)
			{
				case SLAVE_SLAVEADDRESS_WRITE_OPERATION_RECEIVED_ACK_TRANSMITTED:
				{
					//Do Nothing
					break;
				}
					
				case  SLAVE_SLAVEADDRESS_WRITE_OPERATION_RECEIVED_BUT_ARBITRATION_LOST : //Arbitration Lost as Master and addressed as slave
				{
					//Do Nothing
					break;
				}

				case SLAVE_SLAVEADDRESS_READ_OPERATION_RECEIVED_ACK_TRANSMITTED:
				{
					I2C_vidSendByte(I2C_TX_Buffer->Ptr_to_Data[I2C_TX_Buffer->Buffer_Index]);		//send the first byte
					break;
				}
					
				case  SLAVE_SLAVEADDRESS_READ_OPERATION_RECEIVED_BUT_ARBITRATION_LOST : //Arbitration Lost as Master and addressed as slave
				{
					//Do Nothing
					break;
				}
					
				case SLAVE_DATA_TRANSMITTED_ACK_RECEIVED:
				{
					if(I2C_TX_Buffer->Buffer_Index >= I2C_TX_Buffer->User_Size)
					{
						if(I2C_TX_Buffer->Function_Notification != NULL_POINTER)
						{						
							I2C_TX_Buffer->Function_Notification();
							I2C_TX_Buffer->Buffer_Usage = NOT_USED;
						}
					}
					
					else if(I2C_TX_Buffer->Buffer_Index < I2C_TX_Buffer->User_Size)
					{
						Slave_Transmission_Flag = SLAVE_SENDING;
						I2C_TX_Buffer->Buffer_Index++;
						I2C_vidSendByte(I2C_TX_Buffer->Ptr_to_Data[I2C_TX_Buffer->Buffer_Index]);		//send the other byte
					}
					
					else
					{
						//Shouldn't be here
						//Do Nothing
					}
					
					break;
				}
				
				case SLAVE_DATA_TRANSMITTED_NACK_RECEIVED:
				{
					// Do Nothing
					break;
				}
								
				case SLAVE_DATA_RECEIVED_ACK_TRANSMITTED :
				{
					I2C_RX_Buffer->Ptr_to_Data[I2C_RX_Buffer->Buffer_Index] = TWDR;
							
					if(I2C_RX_Buffer->Buffer_Index >= I2C_RX_Buffer->User_Size)
					{
						if(I2C_RX_Buffer->Function_Notification != NULL_POINTER)
						{
							I2C_RX_Buffer->Function_Notification();
							I2C_RX_Buffer->Buffer_Usage = NOT_USED;
						}
		
					}
						
					else if(I2C_RX_Buffer->Buffer_Index < I2C_RX_Buffer->User_Size)
					{
						I2C_RX_Buffer->Buffer_Index++;
						Slave_Transmission_Flag = SLAVE_RECEIVING;
					}
						
					else
					{
						//Shouldn't be here
						//Do Nothing
					}
										
					break;
				}
				
				case SLAVE_DATA_RECEIVED_NACK_TRANSMITTED :
				{
					//Do Nothing
					break;
				}
				
				case SLAVE_STOP_OR_REPEATED_START_RECEIVED  :
				{
					if( (I2C_TX_Buffer->Function_Notification != NULL_POINTER) && (Slave_Transmission_Flag == SLAVE_SENDING) )
					{
						I2C_TX_Buffer->Function_Notification();
						I2C_TX_Buffer->Buffer_Usage=NOT_USED;
						
						if(I2C_Cfg.I2C_Sequence == RECEIVING_THEN_TRANSMITTING)
						{
							I2C_vidSendByte(I2C_TX_Buffer->Ptr_to_Data[I2C_TX_Buffer->Buffer_Index]);		//send the first byte
						}
						
					}
					
					if( (I2C_RX_Buffer->Function_Notification != NULL_POINTER) && (Slave_Transmission_Flag == SLAVE_RECEIVING) )
					{
						I2C_RX_Buffer->Function_Notification();
						I2C_RX_Buffer->Buffer_Usage=NOT_USED;
					}
					
					Slave_Transmission_Flag = SLAVE_NOT_DEFINED;
					break;
				}
				case SLAVE_GENERAL_CALL_RECEIVED_ACK_RECEIVED:
				{
					// Do Nothing
					break;
				}
				
				default:
				{
					I2C_ERROR_IN_INTERRUPT = I2C_STATUS_ERROR;
					break;
				}
			}		
	
			break;
		}
		
		default:
		{
			I2C_ERROR_IN_INTERRUPT= I2C_MODE_ERROR;
			break;
		}
			
	}
	TWCR = (1<<TWEA)|(1<<TWEN)|(1<<TWINT)|(1<<TWIE);	
}
 





 void I2C_vidGetStatus (uint8 * COPY_u8Data)
{
	*COPY_u8Data=TWSR&0xf8;
}

 void I2C_vidStart(void)
{
	TWCR |= (1<<TWEN)| (1<<TWSTA) |(1<<TWINT)|(1<<TWIE);
	
}

 void I2C_vidStop(void)
{
	TWCR |= (1<<TWEN)| (1<<TWSTO) |(1<<TWINT);
}

 void I2C_vidSendSlaveAddress(uint8 COPY_u8Address,I2C_Operation_t COPY_u8Operation)
{	
	
	TWDR=((COPY_u8Address<<1)|(COPY_u8Operation));
	
	//TWCR = (1<<TWEN)|(1<<TWIE)|(TWEA);

}

 void I2C_vidSendByte(uint8 COPY_u8Data)
{
	TWDR=(COPY_u8Data);
	//TWCR = (1<<TWEN)|(1<<TWEN)|(1<<TWIE);
}

 void I2C_vidReadWithAch(uint8 * COPY_pu8Data)
{
	TWCR=(1<<TWEN)|(1<<TWEA)| (1<<TWINT)|(1<<TWIE);
	I2C_POLL_ON_INTERRUPT_FLAG;
	*COPY_pu8Data=TWDR;
}



void I2C_Set_Callback ( void (*ptr)(void) )
{
	if(ptr != NULL_POINTER)
	{
		g_callBackPtr_I2C = ptr;
	}
}

ISR(TWI_vect)
{
	
	if(g_callBackPtr_I2C != NULL_POINTER)
	{
		g_callBackPtr_I2C();
	}
}

I2C_Error_t I2C_Init (void)
{
	I2C_Error_t I2C_Error = FREE_ERROR;
	I2C_Cfg.I2C_Initialize = Initialized;
	TWCR=0;
	I2C_Clear_Status_Register;		//need to check the initial value of Status Register
	I2C_CLEAR_NODE_ADDRESS;
	I2C_SET_NODE_ADDRESS((I2C_Cfg.Source_Address<<1));
	
	if(I2C_Cfg.I2C_Mode == I2C_MASTER)
	{
		switch (I2C_Cfg.I2C_Baudrate)
		{
			case _50KHZ:
			{
				TWSR |= I2C_PRESCALER_1;						// set prescale bits to zero
				TWBR=0x47;										//setting freq of SCL = 50khz
				break;
			}
			default:
			{
				I2C_Error = I2C_BAUDRATE_ERROR;
				I2C_Cfg.I2C_Initialize = NOT_Initialized;
				break;
			}
		}
	}
	else if(I2C_Cfg.I2C_Mode == I2C_SLAVE)
	{
		//Do Nothing
	}
	
	else
	{
		I2C_Error = I2C_MODE_ERROR;
		I2C_Cfg.I2C_Initialize = NOT_Initialized;
	}
	
	switch (I2C_Cfg.I2C_Interrupt)
	{
		case INTERRUPT:
		{
			I2C_ENABLE_INTERRUPT;
			GLOBAL_INTERRUPT_ENABLE;
			break;
		}
		
		case NO_INTERRUPT:
		{
			I2C_DISABLE_INTERRUPT;
			break;
		}
		
		default:
		{
			I2C_Error = I2C_INTERRUPT_ERROR;
			I2C_Cfg.I2C_Initialize = NOT_Initialized;
			break;
		}
	}
	
	TWDR = 0;
	I2C_Set_Callback (I2C_Interrupt_Handler);
	TWCR |= (1<<TWEN) | (1<<TWINT) | (1<<TWIE) | (1<<TWEA);
	return I2C_Error;
}

uint8 * Decimal_to_Hex (int Decimal_Number)
{
	// char array to store hexadecimal number
	static uint8 Hexa_Decimal_arr[HEXA_DECIMAL_SIZE];
	
	Hexa_Decimal_arr[HEXA_DECIMAL_SIZE-1]='\0';
	// counter for hexadecimal number array
	int i = HEXA_DECIMAL_SIZE-2;
	while(Decimal_Number!=0)
	{
		// temporary variable to store remainder
		int temp = 0;

		// storing remainder in temp variable.
		temp = Decimal_Number % 16;

		// check if temp < 10
		if(temp < 10)
		{
			Hexa_Decimal_arr[i] = temp + '0';        // we add the
			i--;
		}
		else // check if temp > 10
		{
			Hexa_Decimal_arr[i] = (temp-10) + 'A';  // we subtract the value in the temp by 10 in order to make the ground of temp begin with 'A' as well as to print the letter 'A' when number (10) is gotten
			i--;
		}

		Decimal_Number = Decimal_Number/16;
	}
	
	return 	Hexa_Decimal_arr;
}



I2C_Error_t I2C_Setup ( St_I2C_Buffer * I2C_Buffer_TX ,  St_I2C_Buffer * I2C_Buffer_RX , I2C_Sequence_t I2C_Sequence )
{
	I2C_Error_t I2C_Error = FREE_ERROR ;
	
	if( (I2C_Buffer_TX == NULL_POINTER) &&  (I2C_Buffer_RX == NULL_POINTER) )
	{
		I2C_Error = GENERAL_ERROR;
		
	}
	
	else
	{
		if(I2C_Cfg.I2C_Mode == I2C_MASTER)
		{
			if ( (I2C_Sequence == TRANSMITTING_ONLY) )
			{
				I2C_Cfg.I2C_Sequence = TRANSMITTING_ONLY ;
				
				I2C_TX_Buffer = I2C_Buffer_TX;
				
				I2C_TX_Buffer->Buffer_Usage = USED;
			}
			
			else if  ( I2C_Sequence == RECEIVING_ONLY )
			{
				I2C_Cfg.I2C_Sequence = RECEIVING ;
				
				I2C_RX_Buffer = I2C_Buffer_RX;
				
				I2C_RX_Buffer->Buffer_Usage = USED;
			}
						
			
			else if  ( I2C_Sequence == TRANSMITTING_THEN_RECEIVING )
			{
				I2C_Cfg.I2C_Sequence = TRANSMITTING_THEN_RECEIVING ;
				
				I2C_TX_Buffer = I2C_Buffer_TX;
				
				I2C_RX_Buffer = I2C_Buffer_RX;
				
				I2C_TX_Buffer->Buffer_Usage = USED;
				
				I2C_RX_Buffer->Buffer_Usage = USED;
			}
			

			
			I2C_vidStart();
		}
		
		else if(I2C_Cfg.I2C_Mode == I2C_SLAVE)
		{
			if ( (I2C_Sequence == RECEIVING_ONLY) )
			{
				I2C_RX_Buffer = I2C_Buffer_RX;
				I2C_RX_Buffer->Buffer_Usage = USED;
			}
			
			else if ( (I2C_Sequence == TRANSMITTING_ONLY) )
			{
				I2C_TX_Buffer = I2C_Buffer_TX;
				I2C_TX_Buffer->Buffer_Usage = USED;
			}
			
			else if  (I2C_Sequence == RECEIVING_THEN_TRANSMITTING)
			{
				I2C_Cfg.I2C_Sequence = RECEIVING_THEN_TRANSMITTING ;
				I2C_TX_Buffer = I2C_Buffer_TX;
				I2C_RX_Buffer = I2C_Buffer_RX;
				
				I2C_TX_Buffer->Buffer_Usage = USED;
				
				I2C_RX_Buffer->Buffer_Usage = USED;
			}
		}
	}
	return I2C_Error;
}
