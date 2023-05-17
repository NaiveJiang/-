#ifndef __GUI_POWSET_H
#define __GUI_POWSET_H

#include "util.h"

typedef struct{
	//接收信息
	uint16_t power_density;
	float spd_max;
	float spd_min;
	float manual_power;
}powSet_pageStruct;

__IO powSet_pageStruct* get_powSetData(void);

#endif
