#ifndef EXTINTER_CONF_H_
#define EXTINTER_CONF_H_

#include "std_types.h"

typedef enum { Interrupt_0, Interrupt_1, Interrupt_2, Num_Of_Interrupt} Interrupt_t;

// INTERRUPT 0
#ifndef INT0
#define INT0 		        0
#endif
#define INT0_PIN            2

#define INT0_LowLevel       0 		
#define INT0_LogicalChange  1
#define INT0_FallingEdge 	2
#define INT0_RisingEdge 	3

// INTERRUPT 1
#ifndef INT1
#define INT1 		        1
#endif

#define INT1_PIN            3

#define INT1_LowLevel       0 		
#define INT1_LogicalChange  1
#define INT1_FallingEdge 	2
#define INT1_RisingEdge 	3

// INTERRUPT 2
#ifndef INT2
#define INT2		        2
#endif

#define INT2_PIN            2

#define INT2_FallingEdge 	2
#define INT2_RisingEdge 	3

//INTERRUPT NUMBERS BITS IN GICR
#define INT1_GICRBIT    7
#define INT0_GICRBIT    6
#define INT2_GICRBIT    5

//CONTROL AND SENSE BITS IN MCUCR AND MCUCSR
//FOR THE INTERRUPT 0 IN MCUCR
#define ISC00 	0
#define ISC01 	1

//FOR THE INTERRUPT 1 IN MCUCR
#define ISC10 	2
#define ISC11 	3

//FOR THE INTERRUPT 2 IN MCUCSR
#define ISC2 	6

#define In_Active   0
#define Active      1


typedef struct {
    uint8 Name;
    uint8 Trigger_Mode;
    uint8 State;

} Interrupt_Conf_t;

extern Interrupt_Conf_t Interrupt_Array[Num_Of_Interrupt] ;



#endif