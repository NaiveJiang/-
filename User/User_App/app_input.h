#ifndef _APP_INPUT_H
#define _APP_INPUT_H

#include "util.h"

#define INPUT_TASK_PERIOD	5

typedef enum{
	INPUT_PRIORITY = 6,
	INPUT_STACK_SIZE = 512,
}appInput_e;

typedef enum{
	__STANDBY  = 0x0000,	//����״̬
	__FAN_ON,				//�������״̬
	__CORONA,				//�ŵ�״̬
	__STOP,					//ͣ��״̬
	
}controlState_e;


typedef struct{
	float rated_power;			//�����
	
	uint8_t control_mode;		//����ģʽ
	uint8_t power_mode;			//����ģʽ
	uint8_t speed_signal;		//�ٶ��ź�
	
	uint8_t speed_up;			//���ٱ�־
	
	uint16_t power_density;		//�����ܶ�
	float speed_max;			//������ٹ���
	float speed_min;			//������ٹ���
	float manual_power;			//�ֶ���������
	
	float speed_up_max;			//������
	float speed_up_min;			//��С����
	
	float roller_pulse_length;	//��Ͳ�����߳�
	float local_speed;			//���ع�Ͳ�ٶ�
	float external_speed;		//�ⲿ�������ٶ�
	
	float power;
	
	uint8_t dry_mode;			//ʪ����ʹ��
	
	uint32_t error_sta;			//���󱨾�
	
	uint8_t control_step;		//���ƹ��̱���
	uint8_t wait_sw;
	uint16_t wait_time;
	
	
	//С����
	uint8_t use_pulse_corona;		//��������ŵ�
	uint8_t corona_in_low_sw;		//����ʱ�ĵ�һ�����崥���ŵ�
	uint32_t low_corona_time;		//����ŵ粻���������½��ش����ŵ�3s����
}appControlStruct_t;


typedef struct{			//���ɿ��ص��������
	uint8_t mode;
	uint8_t last_mode;
	uint8_t sw;
}app_pulseControlStruct_t;

void app_inputTaskInit(void);
appControlStruct_t *get_controlData(void);
void set_controlState(controlState_e setState);
controlState_e get_controlState(void);


#endif
