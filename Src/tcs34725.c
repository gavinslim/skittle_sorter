/*
 * TCS34725.c
 *
 *  Created on: May 20, 2021
 *      Author: Gavin
 */

#include <tcs34725.h>

// TCS34725 Address
const uint8_t TCS_ADDR = 0x29;

// Register
const REG_CODE CMD_CODE			= 0x80;			// Command Register. b[7] = 1b to select command reg.

const REG_CODE ENABLE_REG 	= 0x00;    	// Enables states and interrupts
const REG_CODE ATIME_REG  	= 0x01;    	// RGBC time
const REG_CODE WTIME_REG  	= 0x03;    	// Wait time
const REG_CODE AILTL_REG  	= 0x04;    	// Clear interrupt low threshold low byte
const REG_CODE AILTH_REG  	= 0x05;    	// Clear interrupt low threshold high byte
const REG_CODE AIHTL_REG  	= 0x06;    	// Clear interrupt high threshold low byte
const REG_CODE AIHTH_REG  	= 0x07;    	// Clear interrupt high threshold high byte
const REG_CODE PERS_REG   	= 0x0C;    	// Interrupt persistence filter
const REG_CODE CONFIG_REG 	= 0x0D;    	// Configuration
const REG_CODE CONTROL_REG	= 0x0F;    	// Control
const REG_CODE ID_REG     	= 0x12;    	// Device ID
const REG_CODE STATUS_REG 	= 0x13;    	// Device status
const REG_CODE CDATAL_REG 	= 0x14;    	// Clear data low byte
const REG_CODE CDATAH_REG 	= 0x15;    	// Clear data high byte
const REG_CODE RDATAL_REG 	= 0x16;    	// Red data low byte
const REG_CODE RDATAH_REG 	= 0x17;    	// Red data high byte
const REG_CODE GDATAL_REG 	= 0x18;    	// Green data low byte
const REG_CODE GDATAH_REG 	= 0x19;    	// Green data high byte
const REG_CODE BDATAL_REG 	= 0x1A;    	// Blue data low byte
const REG_CODE BDATAH_REG 	= 0x1B;    	// Blue data high byte



RetVal readDataReg(I2C_HandleTypeDef handle, REG_CODE cmd, REG_CODE reg) {

	uint8_t buffer[5];
	RetVal ret;
	buffer[0] = (cmd | reg);

	// Send I2C Write
	ret.status = HAL_I2C_Master_Transmit(&handle, ((uint16_t)TCS_ADDR) << 1, buffer, 1, HAL_MAX_DELAY);
	if (ret.status != HAL_OK) {
		return ret;
	} else {

		// Send I2C Read
		ret.status = HAL_I2C_Master_Receive(&handle, ((uint16_t)TCS_ADDR) << 1, buffer, 1, HAL_MAX_DELAY);
		if (ret.status != HAL_OK) {
			return ret;
		} else {
			ret.value = (uint8_t)buffer[0];
		}
	}
	return ret;
}

RetVal checkStatus(I2C_HandleTypeDef handle) {
	return readDataReg(handle, CMD_CODE, ID_REG);
}

