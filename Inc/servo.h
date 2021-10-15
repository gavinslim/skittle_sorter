#ifndef _SERVO_H
#define _SERVO_H

#include "stm32f4xx.h"
//#include "stm32f4xx_rcc.h"
//#include "stm32f4xx_gpio.h"
//#include "stm32f4xx_tim.h"

typedef struct {
	TIM_TypeDef* TIM;		// Pointer to timer
	float degrees;			// Servo position
} ServoVal;

void set_servo(ServoVal* ServoInfo);


#endif /* _SERVO_H */
