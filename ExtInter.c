#include "ExtInter.h"
//#include <avr/interrupt.h>


// DDR Direct Register
// PIN  Input register
// port output register

//registers addresses for Port A
#define DDRA (*(( volatile uint8*)0x3A))
#define PINA (*(( volatile uint8*)0x39))
#define PORTA (*(( volatile uint8*)0x3B))


//registers addresses for port B
#define DDRB (*(( volatile uint8*)0x37))
#define PORTB (*((volatile uint8*)0x38))
#define PINB (*((volatile uint8*)0x36))


//registers addresses port C
#define DDRC (*((volatile uint8*)0x34))
#define PORTC (*((volatile uint8*)0x35))
#define PINC (*((volatile uint8*)0x33))


//registers addresses for port D
#define DDRD (*((volatile uint8*)0x31))
#define PORTD (*((volatile uint8*)0x32))
#define PIND (*((volatile uint8*)0x30))


//register address for SREG Status Register
#define SREG (*((volatile uint8*)0x5F))
//to enable the Global Interrupt Flag pin
#define SREG_I 7


//ADDRESS OF GICR "GLOBAL INTERRUPT CONTROL REGISTER"
#define GICR_ADDRESS 0x5B
#define GICR (*(volatile uint8 *) GICR_ADDRESS)
#define GIFR_ADDRESS 0x5A
#define GIFR  (*(volatile uint8 *) GIFR_ADDRESS)

//ADDRESS OF MCUCR "MCU CONTROL REGISTER"
#define MCUCR_ADDRESS 0x55
#define MCUCR (*(volatile uint8 *) MCUCR_ADDRESS)

//ADDRESS OF MCUCSR "MCU CONTROL SENSE REGISTER"
#define MCUCSR_ADDRESS 0x54
#define MCUCSR (*(volatile uint8 *) MCUCSR_ADDRESS)


void (*Ptr_INT0) (void) = NULL_PTR;
void (*Ptr_INT1) (void) = NULL_PTR;
void (*Ptr_INT2) (void) = NULL_PTR;

//ISR DEFINITION AND ATTRIBUTE
#define ISR(vector, ...)\
    void vector (void) __attribute__ ((signal,__INTR_ATTRS)) __VA_ARGS__;\
    void vector (void)

//VECTOR NAMING
#define _VECTOR(N) __vector_ ## N

#define INT0_vect	_VECTOR(1)
#define INT1_vect	_VECTOR(2)
#define INT2_vect	_VECTOR(3)



Std_Func_t INTP_vidInit(void)
{
    uint8 Local_Loop;
    Std_Func_t StateVar = OK;
    if ( Num_Of_Interrupt <= 3){

        for(Local_Loop=0; Local_Loop<Num_Of_Interrupt; Local_Loop++){

            if (Interrupt_Array[Local_Loop].Name == INT0){
                // Pull Up Resistor
                DDRD &= ~(1<<INT0_PIN);
				//PORTD |= (1<<INT0_PIN);
                // Configuration
                switch(Interrupt_Array[Local_Loop].Trigger_Mode){
                    case INT0_LowLevel:
                    MCUCR &= (~(1<<ISC00)) & (~(1<<ISC01));
                    break;
                    case INT0_LogicalChange:
                    MCUCR |= (1<<ISC00);
                    break;
                    case INT0_FallingEdge:
                    MCUCR |= (1<<ISC01);
                    break;
                    case INT0_RisingEdge:
                    MCUCR |= (1<<ISC01);
                    MCUCR |= (1<<ISC00);
                    break;
                    default:
                    StateVar = NOK;
                    break;
                }
                // Enable Interrupt
                switch(Interrupt_Array[Local_Loop].State){
                    case Active:
                    GICR |= (1<<INT0_GICRBIT);
                    SREG |= (1<<SREG_I);
                    break;
                    case In_Active:

                    break;
                    default:
                    StateVar = NOK;
                    break;
                }
            }
            else if (Interrupt_Array[Local_Loop].Name == INT1){
                // Pull Up Resistor
				DDRD &= ~(1<<INT1_PIN);
                //PORTD |= (1<<INT1_PIN);
                // Configuration
                switch(Interrupt_Array[Local_Loop].Trigger_Mode){
                    case INT1_LowLevel:
                    MCUCR &= (~(1<<ISC10)) & (~(1<<ISC11));
                    break;
                    case INT1_LogicalChange:
                    MCUCR |= (1<<ISC10);
                    break;
                    case INT1_FallingEdge:
                    MCUCR |= (1<<ISC11);
                    break;
                    case INT1_RisingEdge:
                    MCUCR |= (1<<ISC11);
                    MCUCR |= (1<<ISC10);
                    break;
                    default:
                    StateVar = NOK;
                    break;
                }
                // Enable Interrupt
                switch(Interrupt_Array[Local_Loop].State){
                    case Active:
                    GICR |= (1<<INT1_GICRBIT);
                    SREG |= (1<<SREG_I);
                    break;
                    case In_Active:

                    break;
                    default:
                    StateVar = NOK;
                    break;
                }
            }
            else if (Interrupt_Array[Local_Loop].Name == INT2) {
                // Pull Up Resistor
				DDRB &= ~(1<<INT2_PIN);
                //PORTB |= (1<<INT2_PIN);
                // Configuration
                switch(Interrupt_Array[Local_Loop].Trigger_Mode){
                    case INT2_FallingEdge:
                    MCUCSR &= (~(1<<ISC2));
                    break;
                    case INT2_RisingEdge:
                    MCUCSR |= (1<<ISC2);
                    break;
                    default:
                    StateVar = NOK;
                    break;
                }
                // Enable Interrupt
                switch(Interrupt_Array[Local_Loop].State){
                    case Active:
                    GICR |= (1<<INT2_GICRBIT);
                    SREG |= (1<<SREG_I);
                    break;
                    case In_Active:

                    break;
                    default:
                    StateVar = NOK;
                    break;
                }
            }
        }
    }

    else {
        StateVar = NOK;
    }

    return StateVar;
}


void INTP0_vidEnabled(void){
    uint8 Local_loop;
    for(Local_loop=0; Local_loop<Num_Of_Interrupt; Local_loop++){
        if (Interrupt_Array[Local_loop].Name == INT0){
            Interrupt_Array[Local_loop].State = Active;
        }
    }
    // Enable Global Interrupt control register and global interrupt
    GICR |= (1<<INT0_GICRBIT);
    SREG |= (1<<SREG_I);
}

void INTP1_vidEnabled(void){
    uint8 Local_loop;
    for(Local_loop=0; Local_loop<Num_Of_Interrupt; Local_loop++){
        if (Interrupt_Array[Local_loop].Name == INT1){
            Interrupt_Array[Local_loop].State = Active;
        }
    }
    // Enable Global Interrupt control register and global interrupt
    GICR |= (1<<INT1_GICRBIT);
    SREG |= (1<<SREG_I);
}

void INTP2_vidEnabled(void){
    uint8 Local_loop;
    for(Local_loop=0; Local_loop<Num_Of_Interrupt; Local_loop++){
        if (Interrupt_Array[Local_loop].Name == INT2){
            Interrupt_Array[Local_loop].State = Active;
        }
    }
    // Enable Global Interrupt control register and global interrupt
    GICR |= (1<<INT2_GICRBIT);
    SREG |= (1<<SREG_I);
}

void INTP0_vidDisabled(void){
    uint8 Local_loop;
    for(Local_loop=0; Local_loop<Num_Of_Interrupt; Local_loop++){
        if (Interrupt_Array[Local_loop].Name == INT0){
            Interrupt_Array[Local_loop].State = In_Active;
        }
    }
    // Enable Global Interrupt control register and global interrupt
    GICR &= (~(1<<INT0_GICRBIT));
}
void INTP1_vidDisabled(void){
    uint8 Local_loop;
    for(Local_loop=0; Local_loop<Num_Of_Interrupt; Local_loop++){
        if (Interrupt_Array[Local_loop].Name == INT1){
            Interrupt_Array[Local_loop].State = In_Active;
        }
    }
    // Enable Global Interrupt control register and global interrupt
    GICR &= (~(1<<INT1_GICRBIT));
}
void INTP2_vidDisabled(void){
    uint8 Local_loop;
    for(Local_loop=0; Local_loop<Num_Of_Interrupt; Local_loop++){
        if (Interrupt_Array[Local_loop].Name == INT2){
            Interrupt_Array[Local_loop].State = In_Active;
        }
    }
    // Enable Global Interrupt control register and global interrupt
    GICR &= (~(1<<INT2_GICRBIT));
}

void INTP_VidDisableGlobal(void){
    SREG &= (~(1<<SREG_I));
}


void INTP0_VidSelectEvent(uint8 COPY_uint8EventState){
    uint8 Local_loop;
    for(Local_loop=0; Local_loop<Num_Of_Interrupt; Local_loop++){
        if (Interrupt_Array[Local_loop].Name == INT0){
            switch(COPY_uint8EventState){
                    case INT0_LowLevel:
                    MCUCR &= (~(1<<ISC00)) & (~(1<<ISC01));
                    break;
                    case INT0_LogicalChange:
					MCUCR &= (~(1<<ISC00)) & (~(1<<ISC01));
                    MCUCR |= (1<<ISC00);
                    break;
                    case INT0_FallingEdge:
					MCUCR &= (~(1<<ISC00)) & (~(1<<ISC01));
                    MCUCR |= (1<<ISC01);
                    break;
                    case INT0_RisingEdge:
					MCUCR &= (~(1<<ISC00)) & (~(1<<ISC01));
                    MCUCR |= (1<<ISC01);
                    MCUCR |= (1<<ISC00);
                    break;
                    default:
                    break;
                }
        }
    }
}
void INTP1_VidSelectEvent(uint8 COPY_uint8EventState){
    uint8 Local_loop;
    for(Local_loop=0; Local_loop<Num_Of_Interrupt; Local_loop++){
        if (Interrupt_Array[Local_loop].Name == INT1){

            switch(COPY_uint8EventState){
                    case INT1_LowLevel:
                    MCUCR &= (~(1<<ISC10)) & (~(1<<ISC11));
                    break;
                    case INT1_LogicalChange:
                    MCUCR |= (1<<ISC10);
                    break;
                    case INT1_FallingEdge:
                    MCUCR |= (1<<ISC11);
                    break;
                    case INT1_RisingEdge:
                    MCUCR |= (1<<ISC11);
                    MCUCR |= (1<<ISC10);
                    break;
                    default:
                    break;
            }
        }
    }
}

void INTP2_VidSelectEvent(uint8 COPY_uint8EventState){
    uint8 Local_loop;
    for(Local_loop=0; Local_loop<Num_Of_Interrupt; Local_loop++){
        if (Interrupt_Array[Local_loop].Name == INT2){
            switch(COPY_uint8EventState){
                    case INT2_FallingEdge:
                    MCUCSR &= (~(1<<ISC2));
                    break;
                    case INT2_RisingEdge:
                    MCUCSR |= (1<<ISC2);
                    break;
                    default:
                    break;
                }
        }
    }
}

void INTP0_vidSetCallBack(void(*COPY_pvidCallBackFunction)(void))
{
    Ptr_INT0 = COPY_pvidCallBackFunction;
}
void INTP1_vidSetCallBack(void(*COPY_pvidCallBackFunction)(void))
{
    Ptr_INT1 = COPY_pvidCallBackFunction;
}
void INTP2_vidSetCallBack(void(*COPY_pvidCallBackFunction)(void))
{
    Ptr_INT2 = COPY_pvidCallBackFunction;
}


ISR(INT0_vect){
    Ptr_INT0();
}

ISR(INT1_vect){
    Ptr_INT1();
}

ISR(INT2_vect){
    Ptr_INT2();
}
