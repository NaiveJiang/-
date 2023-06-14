#ifndef __APP_DISCHARGE_H
#define __APP_DISCHARGE_H

#include "util.h"

#define INCREASE_DISCHARGE_MAX100MS 2	//2Kw增速 100ms
#define INCLEASE_TIME 10	//10*10=100ms

//脉冲放电
typedef struct{
	uint8_t discharge_sw;
	uint8_t mode;				//间歇模式，1开启，屏蔽无脉冲报警
	uint32_t set_delay_time;	//设定的延时时间，超出该时间未有脉冲触发
	uint32_t discharge_time;
	float discharge_power;		//dac输出
	//手动模式
	float inc_power;			//功率增量
}pulse_dischargeStruct_t;

//达速放电
typedef struct{
	float roller_pulse_length;		//滚筒脉冲线长
	float speed;
	float local_speed;				//本地滚筒速度
	float external_speed;			//外部生产线速度
	float line_voltage_speed;		//生产线速度电压信号
	float line_current_speed;		//生产线速度电流信号
	float discharge_power;			//dac输出
	uint8_t speed_signal;			//速度信号 0本地脉冲 1外部脉冲 2生产线电压 3生产线电流
	//手动模式
	float inc_power;				//功率增量
	//功率密度模式
	float power_density;			//给定的功率密度
	float roller_width;				//滚轴宽度
	//速比模式
	float speed_scale[2];
	float power_scale[2];
	float scale;					//比例功率控制系数
	//未达速时的延迟放电控制
	uint8_t delay_sw;
	uint32_t set_delay_time;		//设定的延时时间，超出该时间暂停放电
	uint32_t set_line_delay_time;	//设定的生产线延时时间，超出该时间暂停放电
	uint32_t delay_time;
}spd_dischargeStruct_t;

//放电控制
typedef struct{
	pulse_dischargeStruct_t *pulseCtrl;
	spd_dischargeStruct_t	*spdCtrl;
	float discharge_power;
	uint8_t power_ctrlState;
	uint8_t mode;				//放电模式  0达速模式  1脉冲触发模式
	uint8_t line_power_mode;	//生产线功率给定模式	0生产线功率给定电压采样  1生产线功率给定电流采样
	float line_set_power;		//设定的生产线功率给定
	float manual_power;			//手动设定的最大功率
	float low_power;			//设定的最小功率，在脉冲触发状态下达速后按照线速控制，不达速按照该功率(手动除外)/速比模式下，计算的速比输出小于该功率，则按照该功率输出
	float current_power;		//当前功率
	uint8_t step;
	uint32_t inc_time;
}dischargeStruct_t;

dischargeStruct_t* get_dischargeCtrlData(void);
void app_discharge(void);
void pulse_dischargeUpdate(void);
void spd_dischargeUpdate(void);
float get_speed(uint8_t speedMode);
float get_line_set_power(uint8_t line_power_mode);
#endif
