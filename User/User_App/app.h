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

///*************灭火机器人结构体****************/
//typedef struct{
//	bool fireBassarl;
//	moveWays_e moveWays;
//	moveWays_e lastMoveWays;
//	uint8_t workStep;
//	testTarget_e testTarget;
//	step_e step;
//	beginFlag_e beginFlag;
//	robotTaskstep_e robotTaskstep;
//	workMode_e workMode;
//	turnDirection_e useDir;
//	uint8_t fireArray[10] ;
//	uint8_t returnFlag[10];
//	uint16_t ADCDataLeft;
//	uint16_t ADCDataRight;
//	uint32_t caliTimeCalc;
//}outfireRobotState_t;
///*************救援机器人结构体****************/
//typedef struct{
//	moveWays_e moveWays;
//	moveWays_e lastMoveWays;
//	uint8_t workStep;
//	testTarget_e testTarget;
//	step_e step;
//	beginFlag_e beginFlag;
//	robotTaskstep_e robotTaskstep;
//	workMode_e workMode;
//	turnDirection_e useDir;
//	rescueSelect_e rescuePosition;
//	rescueTaskstep_e rescueTaskstep;
//	uint8_t returnFlag[4];
//}rescueRobotState_t;

//typedef struct{
//	robotType_e robotSelect;
//}robotSelectStruct_t;

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
