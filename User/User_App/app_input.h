#ifndef _APP_INPUT_H
#define _APP_INPUT_H

#include "util.h"

#define INPUT_TASK_PERIOD	5

typedef enum{
	INPUT_PRIORITY = 6,
	INPUT_STACK_SIZE = 512,
}appInput_e;

typedef enum{
	CORONA_WAIT = 0x0000,			//电晕准备，等待抽风机
	CORONA_STATE,			//电晕
	CORONA_STOP,			//电晕停止
	
}controlState_e;


typedef struct{
	uint8_t control_mode;		//控制模式
	uint8_t power_mode;			//功率模式
	uint8_t speed_signal;		//速度信号
	
	uint16_t power_density;		//功率密度
	float speed_max;			//最大线速功率
	float speed_min;			//最低线速功率
	float manual_power;			//手动给定功率
	uint32_t error_sta;
	
	uint8_t control_step;		//控制过程变量
	uint8_t wait_sw;
	uint16_t wait_time;
}appControlStruct_t;


void app_inputTaskInit(void);
appControlStruct_t *get_controlData(void);
void set_controlState(controlState_e setState);
controlState_e get_controlState(void);


#endif
