#ifndef __APP_DISCHARGE_H
#define __APP_DISCHARGE_H

#include "util.h"

//脉冲放电
typedef struct{
	uint8_t discharge_sw;
	uint8_t mode;				//间歇模式，1开启，屏蔽无脉冲报警
	uint32_t discharge_time;
	float discharge_power;
}pulse_dischargeStruct_t;

//达速放电
typedef struct{
	float scale;	//比例功率控制系数
	
}spd_dischargeStruct_t;

//放电控制
typedef struct{
	pulse_dischargeStruct_t *pulseCtrl;
	spd_dischargeStruct_t	*spdCtrl;
	float manual_power;
}dischargeStruct_t;

dischargeStruct_t* get_dischargeCtrlData(void);

#endif
