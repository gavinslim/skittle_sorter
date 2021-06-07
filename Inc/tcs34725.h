#ifndef _TCS34_H
#define _TCS34_H

#include "stm32f4xx_hal.h"
#include "global_var.h"

#define MAX_RD_BYTES 5

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

typedef enum {
	INTEGRATION_TIME_2_4MS	= 0xFF,
	INTEGRATION_TIME_24MS 	= 0xF6,
	INTEGRATION_TIME_101MS	= 0xD5,
	INTEGRATION_TIME_154MS	= 0xC0,
	INTEGRATION_TIME_614MS	= 0x00
} EIntegrationTime;

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
WORD map(WORD x, WORD in_min, WORD in_max, WORD out_min, WORD out_max);
DWORD convert_RGB888(RGBC rgbc);
DWORD convert_RGB8881(RGBC rgbc);
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
