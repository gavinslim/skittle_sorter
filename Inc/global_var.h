#ifndef _GLOBAL_VAR_H
#define _GLOBAL_VAR_H

#define PASS 1
#define FAIL 0

typedef struct RetVal_s {
	HAL_StatusTypeDef status;
	uint8_t val;
	uint8_t data[5];
} RetVal;

#endif /* _GLOBAL_VAR_H */
