#ifndef _STEPPER_H
#define _STEPPER_H

#include "stm32f4xx_hal.h"
#include "global_var.h"
#include "tcs34725.h"

#define STEP_ANGLE 	1.8		// Step angle
#define STEP_SIZE		4			// Step size based on A4988 Stepper Motor Driver Carrier
#define MAX_STEPS		(360 / STEP_ANGLE)

// Gear Ratio (17HS4023)
#define DRIVER_TEETH 24
#define DRIVEN_TEETH 48

// Revolver motor (PA10)
#define REVOLVER_PORT 		GPIOA
#define REVOLVER_PIN 			GPIO_PIN_10

// Orderly motor (PA8)
#define ORDERLY_PORT 			GPIOA
#define ORDERLY_PIN 			GPIO_PIN_8

// Distributor motor (PB5)
#define DISTRIBUTOR_PORT 	GPIOB
#define DISTRIBUTOR_PIN 	GPIO_PIN_5
#define DIRECTION_PORT		GPIOB
#define DIRECTION_PIN			GPIO_PIN_4

float gear_ratio;

typedef enum {
	CLOCKWISE,
	COUNTER_CLOCKWISE,
	IDLE
} StepperDir;

typedef struct {
	int abs_deg;
	StepperDir dir;
} StepperMove;

// Rotate stepper
void rotate_step_deg(uint16_t degrees, uint8_t delay_ms, GPIO_TypeDef* GPIO_Port, uint16_t GPIO_Pin);
StepperMove find_deg_diff(int curr_deg, int dest_deg);

// Revolver motor
void rotate_revolver(int delay);
void rotate_orderly(int delay);
int rotate_distributor(Eflavour flavour, int curr_degrees, int delay);
#endif /* _STEPPER_H */
