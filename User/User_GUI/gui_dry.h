#ifndef __GUI_DRY_H
#define __GUI_DRY_H


#include "util.h"

typedef enum{
	DRY_ON = 0x01,
}dryData_e;

typedef struct{
	//接收信息
	uint8_t dry_rev_data;
}dry_pageStruct;

__IO dry_pageStruct *get_drydata(void);
uint8_t get_dryData(uint8_t dry_data,dryData_e data_state);







#endif
