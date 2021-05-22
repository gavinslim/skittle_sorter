/*
 * TCS34725.c
 *
 *  Created on: May 20, 2021
 *      Author: Gavin
 */

#include "TCS34725.h"

RetVal foo(I2C_HandleTypeDef handle) {

	uint8_t buffer[5];

	RetVal ret_val;

	buffer[0] = (CMD_CODE | ID_REG);

	// Send I2C Write
	ret_val.status = HAL_I2C_Master_Transmit(&handle, ((uint16_t)TCS_ADDR) << 1, buffer, 1, HAL_MAX_DELAY);
	if (ret_val.status != HAL_OK) {
		return ret_val;
	} else {

		// Send I2C Read
		ret_val.status = HAL_I2C_Master_Receive(&handle, ((uint16_t)TCS_ADDR) << 1, buffer, 1, HAL_MAX_DELAY);
		if (ret_val.status != HAL_OK) {
			return ret_val;
		} else {
			ret_val.value = (uint8_t)buffer[0];
		}
	}
	return ret_val;
}
