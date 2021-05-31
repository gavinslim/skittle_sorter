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


ByteStruct rd_I2C_byte(I2C_HandleTypeDef handle) {
	ByteStruct ret;
	BYTE buffer[1];

	ret.status = HAL_I2C_Master_Receive(&handle, (WORD)TCS_ADDR_RD, buffer, 1, HAL_MAX_DELAY);
	ret.val = buffer[0];
	return ret;
}

WordStruct rd_I2C_word(I2C_HandleTypeDef handle) {
	WordStruct ret;
	BYTE buffer[2];

	ret.status = HAL_I2C_Master_Receive(&handle, (WORD)TCS_ADDR_RD, buffer, 2, HAL_MAX_DELAY);
	ret.val = (buffer[0] | (buffer[1] << 8));
	return ret;
}

HAL_StatusTypeDef wr_I2C_byte(I2C_HandleTypeDef handle, BYTE reg_addr) {
	BYTE buffer[1];
	buffer[0] = (CMD_CODE | reg_addr);
	return (HAL_I2C_Master_Transmit(&handle, (WORD)TCS_ADDR_WR, buffer, 1, HAL_MAX_DELAY));
}

HAL_StatusTypeDef wr_I2C_word(I2C_HandleTypeDef handle, BYTE reg_addr, BYTE wr_data) {
	BYTE buffer[2];
	buffer[0] = (CMD_CODE | reg_addr);
	buffer[1] = wr_data;

	return (HAL_I2C_Master_Transmit(&handle, (WORD)TCS_ADDR_WR, buffer, 2, HAL_MAX_DELAY));
}

ByteStruct read_reg_byte(I2C_HandleTypeDef handle, BYTE reg_addr) {
	wr_I2C_byte(handle, reg_addr);
	return rd_I2C_byte(handle);
}

WordStruct read_reg_word(I2C_HandleTypeDef handle, BYTE reg_addr) {
	wr_I2C_byte(handle, reg_addr);
	return rd_I2C_word(handle);
}

HAL_StatusTypeDef write_reg_byte(I2C_HandleTypeDef handle, BYTE reg_addr) {
	return wr_I2C_byte(handle, reg_addr);
}

HAL_StatusTypeDef write_reg_word(I2C_HandleTypeDef handle, BYTE reg_addr, BYTE wr_data) {
	return wr_I2C_word(handle, reg_addr, wr_data);
}

HAL_StatusTypeDef power_on(I2C_HandleTypeDef handle) {
	ByteStruct ret;

	// Read ENABLE register
	ret = read_reg_byte(handle, ENABLE_REG);
	if (ret.status != HAL_OK) {
		return ret.status;
	}

	// Set PON bit, AEN bit and clear WEN bit. Write ENABLE register
	//ret.val = (ret.val | 0x3) & (~(1 << 3));
	ret.val = (ret.val | 0x1);
	ret.status = write_reg_word(handle, ENABLE_REG, ret.val);
	if (ret.status != HAL_OK) {
		return ret.status;
	}

	return ret.status;
}

HAL_StatusTypeDef power_off(I2C_HandleTypeDef handle) {
	ByteStruct ret;

	// Read ENABLE register
	ret = read_reg_byte(handle, ENABLE_REG);
	if (ret.status != HAL_OK) {
		return ret.status;
	}

	// Clear PON bit
	ret.val = (ret.val & ~(0x1));

	ret.status = write_reg_word(handle, ENABLE_REG, ret.val);
	if (ret.status != HAL_OK) {
		return ret.status;
	}

	return ret.status;
}

// Clear WEN bit and set AEN bit to enable RGBC mode
HAL_StatusTypeDef start(I2C_HandleTypeDef handle) {
	ByteStruct ret;

	ret = read_reg_byte(handle, ENABLE_REG);
	if (ret.status != HAL_OK) {
		return ret.status;
	}

	// Clear WEN bit and set AEN bit
	ret.val &= ~(0x1 << 3);
	ret.val |= (0x1 << 1);

	ret.status = write_reg_word(handle, ENABLE_REG, ret.val);
	if (ret.status != HAL_OK) {
		return ret.status;
	}

	return ret.status;
}

ByteStruct check_id(I2C_HandleTypeDef handle) {
	return read_reg_byte(handle, ID_REG);
}

ByteStruct check_status(I2C_HandleTypeDef handle) {
	return read_reg_byte(handle, STATUS_REG);
}

ByteStruct check_atime(I2C_HandleTypeDef handle) {
	return read_reg_byte(handle, ATIME_REG);
}

ByteStruct check_wtime(I2C_HandleTypeDef handle) {
	return read_reg_byte(handle, WTIME_REG);
}

void set_atime(I2C_HandleTypeDef handle, EIntegrationTime time) {
	write_reg_word(handle, ATIME_REG, time);
	return;
}

BYTE check_gain(I2C_HandleTypeDef handle) {
	ByteStruct ret = read_reg_byte(handle, CONTROL_REG);
	return (ret.val | 0x3);
}

WORD read_channel(I2C_HandleTypeDef handle, EColour colour) {
	WordStruct ret;

	switch (colour) {
	case RED:
		ret = read_reg_word(handle, RDATAL_REG);
		break;
	case GREEN:
		ret = read_reg_word(handle, GDATAL_REG);
		break;
	case BLUE:
		ret = read_reg_word(handle, BDATAL_REG);
		break;
	case CLEAR:
		ret = read_reg_word(handle, CDATAL_REG);
		break;
	default:
		ret.status = HAL_ERROR;
	}

	return ret.val;
}

RGBC read_RGBC(I2C_HandleTypeDef handle, EIntegrationTime atime) {
	RGBC color;

	color.R = read_channel(handle, RED);
	color.G = read_channel(handle, GREEN);
	color.B = read_channel(handle, BLUE);
	color.C = read_channel(handle, CLEAR);

	switch (atime) {
	case INTEGRATION_TIME_2_4MS:
		HAL_Delay(3);
		break;
	case INTEGRATION_TIME_24MS:
		HAL_Delay(24);
		break;
	case INTEGRATION_TIME_101MS:
		HAL_Delay(101);
		break;
	case INTEGRATION_TIME_154MS:
		HAL_Delay(154);
		break;
	case INTEGRATION_TIME_614MS:
		HAL_Delay(614);
		break;
	default:
		HAL_Delay(614);
	}

	return color;
}

DWORD convert_RGB888(RGBC rgbc) {
  float i=1;
  //Limit data range
  if(rgbc.R >= rgbc.G && rgbc.R >= rgbc.B){
      i = rgbc.R / 255 + 1;
  }
  else if(rgbc.G >= rgbc.R && rgbc.G >= rgbc.B){
      i = rgbc.G / 255 + 1;
  }
  else if(rgbc.B >=  rgbc.G && rgbc.B >= rgbc.R){
      i = rgbc.B / 255 + 1;
  }
  if(i!=0)
  {
      rgbc.R = (rgbc.R) / i;
      rgbc.G = (rgbc.G) / i;
      rgbc.B = (rgbc.B) / i;
  }
  //Amplify data differences
  /*Please don't try to make the data negative,
      unless you don't change the data type*/
  if(rgbc.R > 30)
      rgbc.R = rgbc.R - 30;
  if(rgbc.G > 30)
      rgbc.G = rgbc.G - 30;
  if(rgbc.B > 30)
      rgbc.B = rgbc.B - 30;
  rgbc.R = rgbc.R * 255 / 225;
  rgbc.G = rgbc.G * 255 / 225;
  rgbc.B = rgbc.B * 255 / 225;

  if(rgbc.R>255)
         rgbc.R = 255;
  if(rgbc.G>255)
         rgbc.G = 255;
  if(rgbc.B>255)
         rgbc.B = 255;
  return (rgbc.R << 16) | (rgbc.G << 8) | (rgbc.B);
}
