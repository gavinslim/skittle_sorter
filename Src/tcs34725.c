/*
 * tcs34725.c
 *
 *  Created on: May 20, 2021
 *      Author: Gavin
 */

#include <tcs34725.h>

// TCS34725 Address
const uint8_t TCS_ADDR = 0x29;
const uint8_t TCS_ADDR_WR = 0x29 << 1;
const uint8_t TCS_ADDR_RD = (0x29 << 1) | 1;

// Register
const REG_CODE CMD_CODE			= 0x80;			// Command Register. b[7] = 1b to select command reg.
const REG_CODE CMD_AUTO_INCR = 0xA0;

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



RetVal read_reg(I2C_HandleTypeDef handle, REG_CODE cmd, REG_CODE reg, uint16_t num_bytes) {
	RetVal ret;
	ret.status = HAL_ERROR;
	ret.val = 0;

	// Ensure num_bytes is non-zero
	if ((num_bytes < 1) | (num_bytes > MAX_RD_BYTES)) {
		return ret;
	}

	uint8_t buffer[num_bytes];
	buffer[0] = (cmd | reg);

	// Send I2C Write
	ret.status = HAL_I2C_Master_Transmit(&handle, (uint16_t)TCS_ADDR_WR, buffer, 1, HAL_MAX_DELAY);
	if (ret.status != HAL_OK) {
		return ret;
	} else {

		// Send I2C Read
		ret.status = HAL_I2C_Master_Receive(&handle, ((uint16_t)TCS_ADDR) << 1, buffer, num_bytes, HAL_MAX_DELAY);
		if (ret.status != HAL_OK) {
			return ret;
		} else {
			for (uint8_t i = 0; i < num_bytes; i++){
				ret.data[i] = buffer[i];
			}
		}
	}
	return ret;
}

// Note: array passed as function argument or allocated in heap can not be determined in size using 'sizeof'
RetVal write_reg(I2C_HandleTypeDef handle, REG_CODE cmd, REG_CODE reg, uint16_t num_bytes, uint8_t* data) {
	RetVal ret;
	ret.val = 0;
	ret.status = HAL_ERROR;

	// Combine write data with command code into buffer array
	uint8_t buffer[num_bytes + 1];
	buffer[0] = (cmd | reg);
	for (uint8_t i = 0; i < num_bytes; i++) {
		buffer[i + 1] = data[i];
	}

	// Send I2C Write
	ret.status = HAL_I2C_Master_Transmit(&handle, (uint16_t)TCS_ADDR_WR, buffer, (num_bytes + 1), HAL_MAX_DELAY);

	return ret;
}

RetVal checkID(I2C_HandleTypeDef handle) {
	return read_reg(handle, CMD_CODE, ID_REG, 1);
}

RetVal checkStatus(I2C_HandleTypeDef handle) {
	return read_reg(handle, CMD_CODE, STATUS_REG, 1);
}

RetVal powerOnSensor(I2C_HandleTypeDef handle) {
	RetVal ret;
	uint8_t data[2];

	// Read ENABLE register
	ret = read_reg(handle, CMD_CODE, ENABLE_REG, 1);
	if (ret.status != HAL_OK) {
		return ret;
	}

	// Set PON bit, AEN bit and clear WEN bit. Write ENABLE register
	data[0] = (ret.val | 0x3) & (~(1 << 3));
	ret = write_reg(handle, CMD_CODE, ENABLE_REG, 1, data);

	return ret;
}

RetVal powerOffSensor(I2C_HandleTypeDef handle) {
	RetVal ret;
	uint8_t data[2];

	// Read ENABLE register
	ret = read_reg(handle, CMD_CODE, ENABLE_REG, 1);
	if (ret.status != HAL_OK) {
		return ret;
	}

	// Clear PON bit. Write ENABLE register
	data[0] = (ret.val & ~(0x1));
	ret = write_reg(handle, CMD_CODE, ENABLE_REG, 1, data);

	return ret;
}

// Read RGBC Channel Data Register
RetVal read_channel(I2C_HandleTypeDef handle, EColour colour) {
	RetVal ret;
	ret.status = HAL_OK;

	switch (colour) {
	case RED:
		ret = read_reg(handle, CMD_AUTO_INCR, RDATAL_REG, 2);
		break;
	case GREEN:
		break;
	case BLUE:
		break;
	case CLEAR:
		break;
	default:
		ret.status = HAL_ERROR;
	}

	return ret;
}
