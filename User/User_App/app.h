#ifndef _APP_H
#define _APP_H
#include  <math.h>
#include <stdbool.h>
#include "parameters.h"
#include "driver.h"
#include "app_input.h"
#include "app_flash.h"
#include "app_findFire.h"
#include "app_mcrc16.h"
#include "app_serial.h"
#include "app_filter.h"
#include "app_adc.h"
#include "app_supervisor.h"
#include "app_dry.h"
#include "app_discharge.h"
#include "app_ntc.h"
#include "app_roll_changing.h"
/*****************************/
#include "gui_data.h"
#include "gui_main.h"
#include "gui_powSet.h"

#define VERSION  2

typedef enum{
	INIT_PRIORITY = 8,
	INIT_STACK_SIZE = 1600,
}appInit_e;

typedef enum{
	
	NAME_VERSION = 0,
	
	POWER,
	SET_STATE,			//工作模式
	
	POWER_DENSITY,		//功率密度
	MAX_POW,			//输出最大功率
	MANUAL_POWER,		//手动额定
	MAX_SPEED,			//速比模式最大速度
	LOW_POWER,			//最小设定功率

	SPD_UP,				//设定达速
	ROLLER_DIAMETER_LOCAL,	//本地滚轴直径
	ROLLER_DIAMETER_EXTERNAL,//外部滚轴直径
	ROLLER_WIDTH,		//滚轴宽度
	ROLLER_PULSE_LOCAL,	//本地滚轴每周脉冲数
	ROLLER_PULSE_EXTERNAL,	//本地滚轴每周脉冲数
	
	SET_SPD_DELAY,		//设定线速状态下的延时停止放电时间
	SET_PULSE_DELAY,	//设定脉冲触发状态下的延时停止放电时间
	SET_REMAIN,			//设定脉冲采样时的等待清零时间
	SET_LINE_DELAY,		//设定线控状态下的延时停止放电时间
	SET_DELAY_LENGTH1,	//设定换卷延迟放电长度1
	SET_DELAY_LENGTH2,	//设定换卷延迟放电长度2
	
	C_SEC,	//秒
	C_HOR,	//时
	C_DAY1,	//日1
	C_DAY2,	//日2
	C_DAY3,	//日3
	
	PWD1,	//密码1
	PWD2,	//密码2
	PWD3,	//密码3
	
	V_PWD,	//确认密码
	
	PULSE_ON,
	
	NAME_OF_LIST,
}nameOfParameter_e;

typedef struct {
	TaskHandle_t xHandleTask;
	uint8_t dataInitFlag;
	uint32_t loops;
}appStruct_t;

extern appStruct_t app;
extern uint32_t parameter[NAME_OF_LIST];

void app_ParameterInit(void);
void app_ControlParameterLoad(void);
void app_Init(void);
void app_FlashUdata(void);
#endif
