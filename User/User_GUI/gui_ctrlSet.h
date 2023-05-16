#ifndef __GUI_CTRLSET_H
#define __GUI_CTRLSET_H

#include "util.h"

typedef enum{
	POWERMODE = 0x03,
	CONTROLMODE = 0x04,
	SPEEDSIGNAL = 0x08,
}ctrlSetData_e;

typedef struct{
	//接收信息
	uint8_t ctrlSet_rev_data;
}ctrlSet_pageStruct;

extern __IO ctrlSet_pageStruct ctrlSetData;
uint8_t get_ctrlSetData(uint8_t ctrlSet_data,ctrlSetData_e data_state);

#endif
