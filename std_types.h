

#ifndef STD_TYPES_H_
#define STD_TYPES_H_


#ifndef F_CPU
#define F_CPU   8000000ul
#endif


/* Boolean Data Type */
typedef unsigned char bool;

/* Boolean Values */
#ifndef FALSE
#define FALSE       (0u)
#endif
#ifndef TRUE
#define TRUE        (1u)
#endif

#ifndef INITIALIZED
#define INITIALIZED			(1u)
#endif

#ifndef NOT_INITIALIZED
#define NOT_INITIALIZED		(0u)
#endif


#ifndef HIGH
#define HIGH		(1u)
#endif

#ifndef LOW
#define LOW			(0u)
#endif

#define NULL_PTR		((void*)0)

#define NULL_POINTER    ((void*)0)


typedef unsigned char         uint8;          /*           0 .. 255             */
typedef signed char           sint8;          /*        -128 .. +127            */
typedef unsigned short        uint16;         /*           0 .. 65535           */
typedef signed short          sint16;         /*      -32768 .. +32767          */
typedef unsigned long         uint32;         /*           0 .. 4294967295      */
typedef signed long           sint32;         /* -2147483648 .. +2147483647     */
typedef unsigned long long    uint64;         /*       0..18446744073709551615  */
typedef signed long long      sint64;
typedef float                 float32;
typedef double                float64;


#define GPIO_InitPortPin(CONTROL, PIN, DIRECTION)   ((CONTROL) = (CONTROL & (~(1 << PIN)))|(DIRECTION << PIN))
#define GPIO_WritePortPin(PORT, PIN, DATA)   ((PORT) = (PORT & (~(1 << PIN)))|(DATA << PIN))
#define GPIO_ReadPortPin(PORT, PIN)    (((PORT) & (1 << PIN)) >> (PIN))

typedef enum {
	NOK, OK
} Std_Func_t;

typedef enum {AK,NAK} ACK;

typedef enum {FIRST_VISIT=0,SECOND_VISIT=1, WAITING=2 , PENDING=2 ,DONE=3}NO_of_Visit_t;

typedef enum {NOT_FINISHED,FINISHED}Function_Process_t;


#endif /* STD_TYPE_H_ */
