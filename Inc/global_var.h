#ifndef _GLOBAL_VAR_H
#define _GLOBAL_VAR_H

#define PASS 1
#define FAIL 0

#define DWORD uint32_t
#define WORD uint16_t
#define BYTE uint8_t

typedef struct RetVal_s {
	HAL_StatusTypeDef status;
	uint8_t val;
	//BYTE byte_val;
	//WORD word_val;
	uint8_t data[5];
} RetVal;

typedef struct BYTE_s {
	HAL_StatusTypeDef status;
	BYTE val;
} ByteStruct;

typedef struct WORD_s {
	HAL_StatusTypeDef status;
	WORD val;
} WordStruct;

#endif /* _GLOBAL_VAR_H */
