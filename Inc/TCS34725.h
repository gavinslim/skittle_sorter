#ifndef __TCS34_H
#define __TCS34_H

#include "stm32f4xx_hal.h"
#include "main.h"

/* TCS34725 Address */
#define TCS_ADDR    0x29

/* Register */
#define CMD_CODE    0x80	// Command Register. b[7] = 1b to select command reg.

#define ENABLE_REG  0x00    // Enables states and interrupts
#define ATIME_REG   0x01    // RGBC time
#define WTIME_REG   0x03    // Wait time
#define AILTL_REG   0x04    // Clear interrupt low threshold low byte
#define AILTH_REG   0x05    // Clear interrupt low threshold high byte
#define AIHTL_REG   0x06    // Clear interrupt high threshold low byte
#define AIHTH_REG   0x07    // Clear interrupt high threshold high byte
#define PERS_REG    0x0C    // Interrupt persistence filter
#define CONFIG_REG  0x0D    // Configuration
#define CONTROL_REG 0x0F    // Control     
#define ID_REG      0x12    // Device ID
#define STATUS_REG  0x13    // Device status
#define CDATAL_REG  0x14    // Clear data low byte
#define CDATAH_REG  0x15    // Clear data high byte
#define RDATAL_REG  0x16    // Red data low byte
#define RDATAH_REG  0x17    // Red data high byte
#define GDATAL_REG  0x18    // Green data low byte
#define GDATAH_REG  0x19    // Green data high byte
#define BDATAL_REG  0x1A    // Blue data low byte
#define BDATAH_REG  0x1B    // Blue data high byte




/* Functions */
RetVal foo(I2C_HandleTypeDef handle);

#endif /* __TCS34_H */
