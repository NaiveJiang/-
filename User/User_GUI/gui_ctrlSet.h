#ifndef __GUI_CTRLSET_H
#define __GUI_CTRLSET_H

#include "util.h"

typedef enum{
	POWERMODE = 0x03,
	CONTROLMODE = 0x04,
	SPEEDSIGNAL = 0x08,
}ctrlSetData_e;

typedef enum{
	MANUAL_MODE = 0,
	POWER_DENSITY_MODE,
	SPEED_MODE,
}powerMode_e;

typedef struct{
	//接收信息
	uint8_t ctrlSet_rev_data;
}ctrlSet_pageStruct;

__IO ctrlSet_pageStruct *get_ctrlSetdata(void);
uint8_t get_ctrlSetData(uint8_t ctrlSet_data,ctrlSetData_e data_state);

#endif
