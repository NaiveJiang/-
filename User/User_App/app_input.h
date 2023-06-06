#ifndef _APP_INPUT_H
#define _APP_INPUT_H

#include "util.h"

#define INPUT_TASK_PERIOD	5

typedef enum{
	INPUT_PRIORITY = 6,
	INPUT_STACK_SIZE = 512,
}appInput_e;

typedef enum{
	__STANDBY  = 0x0000,	//待机状态
	__FAN_ON,				//风机启动状态
	__CORONA,				//放电状态
	__STOP,					//停机状态
	
}controlState_e;


typedef struct{
	float rated_power;			//额定功率
	
	uint8_t control_mode;		//控制模式
	uint8_t power_mode;			//功率模式
	uint8_t speed_signal;		//速度信号
	
	uint8_t speed_up;			//达速标志
	
	uint16_t power_density;		//功率密度
	float speed_max;			//最大线速功率
	float speed_min;			//最低线速功率
	float manual_power;			//手动给定功率
	
	float speed_up_max;			//最大达速
	float speed_up_min;			//最小达速
	
	float roller_pulse_length;	//滚筒脉冲线长
	float local_speed;			//本地滚筒速度
	float external_speed;		//外部生产线速度
	
	float power;
	
	uint8_t dry_mode;			//湿启动使能
	
	uint32_t error_sta;			//错误报警
	
	uint8_t control_step;		//控制过程变量
	uint8_t wait_sw;
	uint16_t wait_time;
	
	
	//小机用
	uint8_t use_pulse_corona;		//允许脉冲放电
	uint8_t corona_in_low_sw;		//启动时的第一个脉冲触发放电
	uint32_t low_corona_time;		//脉冲放电不能连续，下降沿触发放电3s左右
}appControlStruct_t;


typedef struct{			//自由开关的脉冲控制
	uint8_t mode;
	uint8_t last_mode;
	uint8_t sw;
}app_pulseControlStruct_t;

void app_inputTaskInit(void);
appControlStruct_t *get_controlData(void);
void set_controlState(controlState_e setState);
controlState_e get_controlState(void);


#endif
