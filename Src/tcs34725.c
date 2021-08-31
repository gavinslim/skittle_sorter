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

WORD map(WORD x, WORD in_min, WORD in_max, WORD out_min, WORD out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Red: 110/71/66
// Orange: 133/60/40
// Convert raw data to RGB 888 format
DWORD convert_RGB888(RGBC rgbc) {
	WORD red = ((float)rgbc.R / (float)rgbc.C) * (float)255;
	WORD green = ((float)rgbc.G / (float)rgbc.C) * (float)255;
	WORD blue = ((float)rgbc.B / (float)rgbc.C) * (float)255;

	if (red > 255) { red = 255; }
	if (green > 255) { green = 255; }
	if (blue > 255) { blue = 255; }

	//red = red / (red + green + blue);
	//blue = blue / (red + green + blue);
	//green = green / (red + green + blue);

	return ((red << 16) | (green << 8) | blue);
}

// https://shop.m5stack.com/products/color-unit
coordinate convert_RGB8882(RGBC rgbc) {
	float x = 2.7688*((float)(rgbc.R)) + 1.7517*((float)(rgbc.G)) + 1.1301*((float)(rgbc.B));
	float y = 1.0000*((float)(rgbc.R)) + 4.5906*((float)(rgbc.G)) + 0.0601*((float)(rgbc.B));
	float z = 0*((float)(rgbc.R)) + 0.0565*((float)(rgbc.G)) + 5.5942*((float)(rgbc.B));

	float x_axis = (float)(x / (x + y + z));
	float y_axis = (float)(y / (x + y + z));

	coordinate output;
	output.x = x;
	output.y = y;
	output.z = z;
	output.x_axis = (x_axis * 100);
	output.y_axis = (y_axis * 100);

	return output;
}

Eflavour check_colour(DWORD rgb_data) {
	BYTE red = rgb_data >> 16;
	BYTE green = (rgb_data >> 8) & 0xFF;
	BYTE blue = rgb_data & 0xFF;

	if 	(((red >= 61) && (red <= 76)) &&
			((green >= 112) && (green <= 131)) &&
			((blue >= 38) && (blue <= 52))) {
			return GREEN_APPLE;
	} else if (((red >= 106) && (red <= 115)) &&
			((green >= 90) && (green <= 97)) &&
			((blue >= 29) && (blue <= 39))) {
			return LEMON;
	} else if (((red >= 135) && (red <= 156)) &&
			((green >= 54) && (green <= 61)) &&
			((blue >= 31) && (blue <= 43))) {
			return ORANGE;
	} else if (((red >= 114) && (red <= 145)) &&
			((green >= 36) && (green <= 68)) &&
			((blue >= 49) && (blue <= 65))) {
			return STRAWBERRY;
	} else if (((red >= 80) && (red <= 107)) &&
			((green >= 76) && (green <= 87)) &&
			((blue >= 62) && (blue <= 75))) {
			return GRAPE;
	}
	return UNKNOWN;
}

// Using convert_RGB888
Eflavour check_colour_side(DWORD rgb_data) {
	BYTE red = rgb_data >> 16;
	BYTE green = (rgb_data >> 8) & 0xFF;
	BYTE blue = rgb_data & 0xFF;

	if 	(((red >= 72) && (red <= 82)) &&
			((green >= 112) && (green <= 120)) &&
			((blue >= 39) && (blue <= 45))) {
			return GREEN_APPLE;
	} else if (((red >= 99) && (red <= 119)) &&
			((green >= 89) && (green <= 95)) &&
			((blue >= 29) && (blue <= 34))) {
			return LEMON;
	} else if (((red >= 131) && (red <= 159)) &&
			((green >= 48) && (green <= 60)) &&
			((blue >= 31) && (blue <= 37))) {
			return ORANGE;
	} else if (((red >= 128) && (red <= 153)) &&
			((green >= 54) && (green <= 64)) &&
			((blue >= 43) && (blue <= 53))) {
			return STRAWBERRY;
	} else if (((red >= 95) && (red <= 108)) &&
			((green >= 76) && (green <= 87)) &&
			((blue >= 59) && (blue <= 66))) {
			return GRAPE;
	}
	return UNKNOWN;
}

DWORD convert_RGB8881(RGBC rgbc) {
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

DWORD read_sensor(I2C_HandleTypeDef hi2c1, ByteStruct atime, int read_delay, int order_delay) {
	RGBC raw_color;
	RGBC ave_color;
	RGBC sum_color;
	sum_color.R = 0;
	sum_color.G = 0;
	sum_color.B = 0;
	sum_color.C = 0;
	//BYTE red, blue, green;
	DWORD color;

	// Turn LED on
	HAL_GPIO_WritePin(LED_EN_PORT, LED_EN_PIN, GPIO_PIN_SET);
	HAL_Delay(order_delay);
	for (int i = 0; i < MAX_SENSOR_READS; i++) {
		raw_color = read_RGBC(hi2c1, atime.val);
		sum_color.R = raw_color.R + sum_color.R;
		sum_color.G = raw_color.G + sum_color.G;
		sum_color.B = raw_color.B + sum_color.B;
		sum_color.C = raw_color.C + sum_color.C;

		HAL_Delay(read_delay);
	}

	// Turn LED off
	HAL_GPIO_WritePin(LED_EN_PORT, LED_EN_PIN, GPIO_PIN_RESET);

	ave_color.R = (float)sum_color.R / (float)MAX_SENSOR_READS;
	ave_color.G = (float)sum_color.G / (float)MAX_SENSOR_READS;
	ave_color.B = (float)sum_color.B / (float)MAX_SENSOR_READS;
	ave_color.C = (float)sum_color.C / (float)MAX_SENSOR_READS;

	color = convert_RGB888(ave_color);
	//red = color >> 16;
	//green = (color >> 8) & 0xFF;
	//blue = color & 0xFF;

	return color;
}
