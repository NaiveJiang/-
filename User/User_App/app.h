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
/*****************************/
#include "gui_data.h"
#include "gui_main.h"
#include "gui_ctrlSet.h"
#include "gui_powSet.h"

#define VERSION  2

typedef enum{
	INIT_PRIORITY = 1,
	INIT_STACK_SIZE = 1600,
}appInit_e;

typedef enum{
	
	NAME_VERSION = 0,
	
	POWER,
	CONTROL_MODE,		//控制模式
	POWER_MODE,			//功率模式
	SPD_SIGNAL,			//速度信号
	
	POWER_DENSITY,		//功率密度
	SPD_MAX,			//速比最大
	SPD_MIN,			//速比最小
	MANUAL_POWER,		//手动额定		
	
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
