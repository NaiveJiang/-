#ifndef __GUI_POWSET_H
#define __GUI_POWSET_H

#include "util.h"

typedef enum{
	POWERMODE = 0x03,
	CONTROLMODE = 0x04,
	SPEEDSIGNAL = 0x18,
}ctrlSetData_e;

typedef enum{
	MANUAL_MODE = 0,			//手动
	SPEED_MODE,					//速比
	POWER_DENSITY_MODE,			//功率密度
}powerMode_e;

typedef enum{
	LOCAL_CTRL = 0,				//本地
	COMBINE_CTRL,				//联动
}ctrlMode_e;


typedef struct{
	//接收信息
	uint8_t set_state;		//工作模式 0:1为功率模式 2为控制模式 3为速度信号
}powSet_pageStruct;

__IO powSet_pageStruct* get_powSetData(void);
uint8_t get_setStateData(uint8_t ctrlSet_data,ctrlSetData_e data_state);
#endif
