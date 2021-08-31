/*
 * servo.c
 *
 *  Created on: Jun 22, 2021
 *      Author: Gavin
 */

#include <servo.h>

// PWM Frequency = 50Hz. Period = 0.02s = 20ms
// APB1 = 84 MHz
// Timer Frequency = Period(us) * PWM Frequency
//                 = 20000 * 50
//								 = 1 MHz
// To achieve 1 MHz, prescaler must be 84 MHz / 1 MHz = 84

void set_servo(ServoVal* ServoInfo) {
	ServoInfo->TIM->CCR1 = ServoInfo->degrees;
	HAL_Delay(1000);
}
