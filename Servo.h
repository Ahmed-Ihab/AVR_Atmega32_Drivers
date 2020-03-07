/*
 * Servo.h
 *      Author: Ehab
 */

#ifndef	SERVO_SERVO_H_
#define SERVO_SERVO_H_

#include "std_types.h"

#include "DIO.h"
#include "Timer.h"
#include "Servo_cfg.h"

void SERVO_init(TIMER_t Timer);
Std_Func_t SERVO_setAngle(uint8 Angle);

#endif /* SERVO_SERVO_H_ */
