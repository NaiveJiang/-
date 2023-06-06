#ifndef __GUI_MAIN_H
#define __GUI_MAIN_H

#include "util.h"

typedef enum{
	CORONA = 0x01,
	FAN = 0x02,
	DRY = 0x03,
}mainData_e;

typedef struct{
	//������Ϣ
	float power;
	float speed;
	uint8_t speed_up;
	//������Ϣ
	uint8_t main_rev_data;
}main_pageStruct;

__IO main_pageStruct *get_maindata(void);
uint8_t get_mainData(uint8_t main_data,mainData_e data_state);
#endif
