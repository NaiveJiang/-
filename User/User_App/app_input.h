#ifndef _APP_INPUT_H
#define _APP_INPUT_H

#include "util.h"

#define INPUT_TASK_PERIOD	5

typedef enum{
	INPUT_PRIORITY = 6,
	INPUT_STACK_SIZE = 512,
}appInput_e;

typedef enum{
	CORONA_WAIT = 0x0000,			//����׼�����ȴ�����
	CORONA_STATE,			//����
	CORONA_STOP,			//����ֹͣ
	
}controlState_e;


typedef struct{
	uint8_t control_mode;		//����ģʽ
	uint8_t power_mode;			//����ģʽ
	uint8_t speed_signal;		//�ٶ��ź�
	
	uint16_t power_density;		//�����ܶ�
	float speed_max;			//������ٹ���
	float speed_min;			//������ٹ���
	float manual_power;			//�ֶ���������
	uint32_t error_sta;
	
	uint8_t control_step;		//���ƹ��̱���
	uint8_t wait_sw;
	uint16_t wait_time;
}appControlStruct_t;


void app_inputTaskInit(void);
appControlStruct_t *get_controlData(void);
void set_controlState(controlState_e setState);
controlState_e get_controlState(void);


#endif
