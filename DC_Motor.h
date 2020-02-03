

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

//-------------------  File_Inculsion  -------------------------------//

#include "../MCAL/DIO/DIO.h"
#include "../MCAL/PWM/PWM.h"
//-------------------  Enum  -------------------------------//

typedef enum {MOTOR_1,MOTOR_2}Motor_Select_t;

//#define ANTICLOCKWISE 0
//#define CLOCKWISE	    1
typedef enum {ANTICLOCKWISE = 0 ,CLOCKWISE = 1 , FORWARD = 0 , BACKWARD = 1}Set_Direction_t;


//-------------------  Macro_Constants -------------------------------//

//-------------------  Public_Functions-------------------------------//

void MOTORS_Initialization (void);
Std_Func_t MOTOR_Direction ( Motor_Select_t Motor_Select , Set_Direction_t Set_Direction);
Std_Func_t MOTOR_Stop (Motor_Select_t Motor_Select);

//-------------------  Public_Global_Variables -----------------------//


#endif