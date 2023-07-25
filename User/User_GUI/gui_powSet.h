#ifndef __GUI_POWSET_H
#define __GUI_POWSET_H

#include "util.h"

typedef enum{
	POWERMODE = 0x03,
	CONTROLMODE = 0x04,
	SPEEDSIGNAL = 0x18,
}ctrlSetData_e;

typedef enum{
	MANUAL_MODE = 0,			//�ֶ�
	SPEED_MODE,					//�ٱ�
	POWER_DENSITY_MODE,			//�����ܶ�
}powerMode_e;

typedef enum{
	LOCAL_CTRL = 0,				//����
	COMBINE_CTRL,				//����
}ctrlMode_e;


typedef struct{
	//������Ϣ
	uint8_t set_state;		//����ģʽ 0:1Ϊ����ģʽ 2Ϊ����ģʽ 3Ϊ�ٶ��ź�
}powSet_pageStruct;

__IO powSet_pageStruct* get_powSetData(void);
uint8_t get_setStateData(uint8_t ctrlSet_data,ctrlSetData_e data_state);
#endif
