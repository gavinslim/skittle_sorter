#ifndef _TCS34_H
#define _TCS34_H

#include "stm32f4xx_hal.h"
#include "global_var.h"

// LED enable pin
#define LED_EN_PORT	GPIOB
#define LED_EN_PIN 	GPIO_PIN_13

#define MAX_RD_BYTES 5
#define MAX_SENSOR_READS 3

// TCS34725 Address
extern const uint8_t TCS_ADDR;

// Register
typedef uint8_t REG_CODE;
const REG_CODE CMD_CODE;				// Command Register. b[7] = 1b to select command reg.
const REG_CODE CMD_AUTO_INCR;

const REG_CODE ENABLE_REG;    	// Enables states and interrupts
const REG_CODE ATIME_REG;    		// RGBC time
const REG_CODE WTIME_REG;    		// Wait time
const REG_CODE AILTL_REG;    		// Clear interrupt low threshold low byte
const REG_CODE AILTH_REG;    		// Clear interrupt low threshold high byte
const REG_CODE AIHTL_REG;    		// Clear interrupt high threshold low byte
const REG_CODE AIHTH_REG;    		// Clear interrupt high threshold high byte
const REG_CODE PERS_REG;    		// Interrupt persistence filter
const REG_CODE CONFIG_REG;    	// Configuration
const REG_CODE CONTROL_REG;    	// Control
const REG_CODE ID_REG;    			// Device ID
const REG_CODE STATUS_REG;    	// Device status
const REG_CODE CDATAL_REG;    	// Clear data low byte
const REG_CODE CDATAH_REG;    	// Clear data high byte
const REG_CODE RDATAL_REG;    	// Red data low byte
const REG_CODE RDATAH_REG;    	// Red data high byte
const REG_CODE GDATAL_REG;    	// Green data low byte
const REG_CODE GDATAH_REG;    	// Green data high byte
const REG_CODE BDATAL_REG;    	// Blue data low byte
const REG_CODE BDATAH_REG;    	// Blue data high byte


typedef enum {
	RED, GREEN, BLUE, CLEAR
} EColour;

typedef struct {
	WORD R;
	WORD G;
	WORD B;
	WORD C;
} RGBC;

typedef struct {
	float x;
	float y;
	float z;
	float x_axis;
	float y_axis;
} coordinate;

typedef enum {
	INTEGRATION_TIME_2_4MS	= 0xFF,
	INTEGRATION_TIME_24MS 	= 0xF6,
	INTEGRATION_TIME_101MS	= 0xD5,
	INTEGRATION_TIME_154MS	= 0xC0,
	INTEGRATION_TIME_614MS	= 0x00
} EIntegrationTime;

typedef enum {
	LEMON,
	ORANGE,
	GREEN_APPLE,
	STRAWBERRY,
	GRAPE,
	UNKNOWN
} Eflavour;

// Functions
RetVal read_reg(I2C_HandleTypeDef handle, REG_CODE cmd, REG_CODE reg, uint16_t num_bytes);
RetVal write_reg(I2C_HandleTypeDef handle, REG_CODE cmd, REG_CODE reg, uint16_t num_bytes, uint8_t* buffer);

//RetVal power_on(I2C_HandleTypeDef handle);
//RetVal power_off(I2C_HandleTypeDef handle);
RetVal enableRGBC(I2C_HandleTypeDef handle);
//RetVal check_status(I2C_HandleTypeDef handle);
//RetVal check_id(I2C_HandleTypeDef handle);

//RetVal read_channel(I2C_HandleTypeDef handle, EColour colour);
RetVal read_all_channels(I2C_HandleTypeDef handle);
RetVal read_timing(I2C_HandleTypeDef handle);
RetVal read_ctrl_reg(I2C_HandleTypeDef handle);

//RGBC read_RGB(I2C_HandleTypeDef handle);

ByteStruct rd_I2C_byte(I2C_HandleTypeDef handle);
WordStruct rd_I2C_word(I2C_HandleTypeDef handle);
HAL_StatusTypeDef wr_I2C_byte(I2C_HandleTypeDef handle, BYTE reg_addr);
HAL_StatusTypeDef wr_I2C_word(I2C_HandleTypeDef handle, BYTE reg_addr, BYTE wr_data);

ByteStruct read_reg_byte(I2C_HandleTypeDef handle, BYTE reg_addr);
WordStruct read_reg_word(I2C_HandleTypeDef handle, BYTE reg_addr);
HAL_StatusTypeDef write_reg_byte(I2C_HandleTypeDef handle, BYTE reg_addr);
HAL_StatusTypeDef write_reg_word(I2C_HandleTypeDef handle, BYTE reg_addr, BYTE wr_data);

HAL_StatusTypeDef power_on(I2C_HandleTypeDef handle);
HAL_StatusTypeDef power_off(I2C_HandleTypeDef handle);
HAL_StatusTypeDef start(I2C_HandleTypeDef handle);
ByteStruct check_id(I2C_HandleTypeDef handle);
ByteStruct check_status(I2C_HandleTypeDef handle);
ByteStruct check_atime(I2C_HandleTypeDef handle);
ByteStruct check_wtime(I2C_HandleTypeDef handle);
BYTE check_gain(I2C_HandleTypeDef handle);
void set_atime(I2C_HandleTypeDef handle, BYTE time);

WORD read_channel(I2C_HandleTypeDef handle, EColour colour);
RGBC read_RGBC(I2C_HandleTypeDef handle, EIntegrationTime atime);
DWORD convert_RGB888(RGBC rgbc);
Eflavour check_colour_side(DWORD rgb_data);
DWORD convert_RGB8881(RGBC rgbc);
coordinate convert_RGB8882(RGBC rgbc);
BYTE range (BYTE colour, BYTE low, BYTE high);
BYTE matrix_check (BYTE matrix[][2], BYTE red, BYTE blue, BYTE green);
DWORD read_sensor(I2C_HandleTypeDef hi2c1, ByteStruct atime, int read_delay, int order_delay);
DWORD read_sensor1(I2C_HandleTypeDef hi2c1, ByteStruct atime, int read_delay, int order_delay);

/*
RetVal readClearLowByte(I2C_HandleTypeDef handle);
RetVal readClearHighByte(I2C_HandleTypeDef handle);
RetVal readRedLowByte(I2C_HandleTypeDef handle);
RetVal readRedHighByte(I2C_HandleTypeDef handle);
RetVal readGreenLowByte(I2C_HandleTypeDef handle);
RetVal readGreenHighByte(I2C_HandleTypeDef handle);
RetVal readBlueLowByte(I2C_HandleTypeDef handle);
RetVal readBlueHighByte(I2C_HandleTypeDef handle);
*/
#endif /* TCS34_H */
