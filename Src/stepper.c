
#include <stepper.h>

float gear_ratio = DRIVEN_TEETH / DRIVER_TEETH;

void rotate_step_deg(uint16_t degrees, uint8_t delay_ms, GPIO_TypeDef* GPIO_Port, uint16_t GPIO_Pin) {
	uint32_t num_steps = ((float)MAX_STEPS / (360.0 / (float)degrees)) * STEP_SIZE;

	for (int i = 0; i < num_steps; i++) {
		HAL_GPIO_WritePin(GPIO_Port, GPIO_Pin, GPIO_PIN_SET);
		HAL_Delay(delay_ms);
		HAL_GPIO_WritePin(GPIO_Port, GPIO_Pin, GPIO_PIN_RESET);
	}

	return ;
}

// Get absolute difference in degrees between two inputs
StepperMove find_deg_diff(int curr_deg, int dest_deg) {
	StepperMove out;

	if (curr_deg < dest_deg) {
		out.abs_deg = dest_deg - curr_deg;
		out.dir = COUNTER_CLOCKWISE;
	} else if (curr_deg > dest_deg) {
		out.abs_deg = curr_deg - dest_deg;
		out.dir = CLOCKWISE;
	} else {
		out.abs_deg = 0;
		out.dir = IDLE;
	}

	return out;
}

void rotate_revolver(int delay) {
	uint16_t rotate_deg = 60;
	uint16_t intv_degrees = rotate_deg * gear_ratio;
	rotate_step_deg(intv_degrees, 1, REVOLVER_PORT, REVOLVER_PIN);
	HAL_Delay(delay);

	return;
}

void rotate_orderly(int delay) {
	rotate_step_deg(90, 2, ORDERLY_PORT, ORDERLY_PIN);
	HAL_Delay(delay);
}

int rotate_distributor(Eflavour flavour, int curr_degrees, int delay) {
	StepperMove degrees;

	// Rotate distributor based on flavour
	switch (flavour) {
	case UNKNOWN:
		// Unchanged
		degrees = find_deg_diff(curr_degrees, 90);
		curr_degrees = 90;
		break;

	case ORANGE:
		// 0 degrees
		degrees = find_deg_diff(curr_degrees, 0);
		curr_degrees = 0;
		break;

	case LEMON:
		// 45 degrees
		degrees = find_deg_diff(curr_degrees, 45);
		curr_degrees = 45;
		break;

	case GREEN_APPLE:
		// 90 degrees
		degrees = find_deg_diff(curr_degrees, 90);
		curr_degrees = 90;
		break;

	case STRAWBERRY:
		// 135 degrees
		degrees = find_deg_diff(curr_degrees, 135);
		curr_degrees = 135;
		break;

	case GRAPE:
		// 180 degrees
		degrees = find_deg_diff(curr_degrees, 180);
		curr_degrees = 180;
		break;

	default:
		// Unchanged
		degrees = find_deg_diff(curr_degrees, 90);
		curr_degrees = 90;
		break;
	}

	if (degrees.dir == CLOCKWISE) {
		HAL_GPIO_WritePin(DIRECTION_PORT, DIRECTION_PIN, GPIO_PIN_RESET);
	} else if (degrees.dir == COUNTER_CLOCKWISE) {
		HAL_GPIO_WritePin(DIRECTION_PORT, DIRECTION_PIN, GPIO_PIN_SET);
	}

	rotate_step_deg(degrees.abs_deg, 1, DISTRIBUTOR_PORT, DISTRIBUTOR_PIN);
	HAL_Delay(delay);

	return curr_degrees;
}
