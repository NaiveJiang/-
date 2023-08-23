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
	SET_STATE,			//����ģʽ
	
	POWER_DENSITY,		//�����ܶ�
	MAX_POW,			//��������
	MANUAL_POWER,		//�ֶ��
	MAX_SPEED,			//�ٱ�ģʽ����ٶ�
	LOW_POWER,			//��С�趨����

	SPD_UP,				//�趨����
	ROLLER_DIAMETER_LOCAL,	//���ع���ֱ��
	ROLLER_DIAMETER_EXTERNAL,//�ⲿ����ֱ��
	ROLLER_WIDTH,		//������
	ROLLER_PULSE_LOCAL,	//���ع���ÿ��������
	ROLLER_PULSE_EXTERNAL,	//���ع���ÿ��������
	
	SET_SPD_DELAY,		//�趨����״̬�µ���ʱֹͣ�ŵ�ʱ��
	SET_PULSE_DELAY,	//�趨���崥��״̬�µ���ʱֹͣ�ŵ�ʱ��
	SET_REMAIN,			//�趨�������ʱ�ĵȴ�����ʱ��
	SET_LINE_DELAY,		//�趨�߿�״̬�µ���ʱֹͣ�ŵ�ʱ��
	SET_DELAY_LENGTH1,	//�趨�����ӳٷŵ糤��1
	SET_DELAY_LENGTH2,	//�趨�����ӳٷŵ糤��2
	
	C_SEC,	//��
	C_HOR,	//ʱ
	C_DAY1,	//��1
	C_DAY2,	//��2
	C_DAY3,	//��3
	
	PWD1,	//����1
	PWD2,	//����2
	PWD3,	//����3
	
	V_PWD,	//ȷ������
	
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
