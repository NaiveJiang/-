#ifndef _APP_H
#define _APP_H
#include  <math.h>
#include <stdbool.h>
#include "driver.h"
//#include "GUI.h"
//#include "app_walk.h"
#include "app_input.h"
//#include "app_work.h"
#include "app_flash.h"
//#include "app_echo.h"
#include "app_findFire.h"
#include "app_mcrc16.h"
#include "app_serial.h"
#include "app_filter.h"
#include "app_adc.h"
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
	
	CONTROL_MODE,
	POWER_MODE,
	SPD_SIGNAL,
	
	NAME_OF_LIST,
}nameOfParameter_e;

typedef struct {
	TaskHandle_t xHandleTask;
	uint8_t dataInitFlag;
	uint32_t loops;
}appStruct_t;

//extern outfireRobotState_t outfireRobotState;
//extern rescueRobotState_t rescueRobotState;
extern appStruct_t app;
extern uint32_t parameter[NAME_OF_LIST];
//extern robotSelectStruct_t robotSelect;

void app_ParameterInit(void);
void app_Init(void);
void app_FlashUdata(void);
#endif
