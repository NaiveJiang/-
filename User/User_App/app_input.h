#ifndef _APP_INPUT_H
#define _APP_INPUT_H

#include "util.h"

#define INPUT_TASK_PERIOD	5
#define START_INIT_TIME 600
typedef enum{
	INPUT_PRIORITY = 6,
	INPUT_STACK_SIZE = 512,
}appInput_e;

typedef enum{
	__STANDBY  = 0x0000,	//����״̬
	__FAN_ON,				//�������״̬
	__CORONA,				//�ŵ�״̬
	__DRY,					//ʪ����״̬
	__ROLL_CHANGING,		//����״̬
	__STOP,					//ͣ��״̬
	
}controlState_e;


typedef struct{
	uint8_t start_init;			//������ʼ��
	uint32_t start_time;		//������ʼ��ʱ��
	uint8_t page_num;			//��Ļ��ҳ�����
	float temp;					//�����¶�
	float rated_power;			//�����
	
	uint8_t control_mode;		//����ģʽ
	uint8_t power_mode;			//����ģʽ
	uint8_t speed_signal;		//�ٶ��ź�
	
	uint8_t speed_up;			//���ٱ�־
	uint8_t fy_ok;				//��ѹ��־
	uint8_t corona_on;		//��ʼ�ŵ��־
	
	uint16_t power_density;		//�����ܶ�
	float speed_max;			//������ٹ���
	float speed_min;			//������ٹ���
	float manual_power;			//�ֶ���������
	
	float set_speed_up;			//�趨����
	float set_spd_max;			//�������
	
	float power;
	
	uint8_t dry_mode;			//ʪ����ʹ��
	uint8_t manual_mode;		//�ֶ�����ʹ��
	
	uint32_t error_sta;			//���󱨾�
	
	uint8_t control_step;		//���ƹ��̱���
	uint8_t last_control_step;
	uint8_t wait_sw;
	uint16_t wait_time;
	
	uint8_t line_control;			//�����߿��Ʊ�־
	uint8_t line_suspend;			//��������ͣ
	uint8_t line_suspend_delay_sw;	//��������ʱ�ŵ��־
	uint32_t line_suspend_time;		//��������ʱ�ŵ���ʱ
	uint32_t line_set_suspend_time;	//��������ʱ�ŵ��趨��ʱ
	
	
	//С����
	uint8_t use_pulse_corona;		//��������ŵ�
	uint8_t corona_in_low_sw;		//����ʱ�ĵ�һ�����崥���ŵ�
	uint32_t low_corona_time;		//����ŵ粻���������½��ش����ŵ�3s����
	
	//���ϸ�λ
	uint8_t error_reset;
	
	//RTC��ʱ
	uint32_t rtc_sec;	//��
	uint32_t rtc_hour;	//Сʱ
	uint32_t last_hour;
	uint32_t rtc_day1;	//����1
	uint32_t rtc_day2;	//����2
	uint32_t rtc_day3;	//����3
	uint8_t remain_day;
	
	//����
	uint8_t lock;
	uint8_t lock_tamp;	//����ʱ�ϵ���
	uint32_t password[3]; //�洢�ڻ��ڵ�����
	uint32_t verify_password; //��Ҫ��֤������
	uint8_t pwd_ok;	//���������Ƿ���ȷ
	
	
	
}appControlStruct_t;


typedef struct{			//���ɿ��ص��������
	uint8_t mode;
	uint8_t last_mode;
	uint8_t sw;
}app_pulseControlStruct_t;

void app_inputTaskInit(void);
appControlStruct_t *get_controlData(void);
void set_controlState(controlState_e setState,uint8_t step);
controlState_e get_controlState(void);
controlState_e get_last_controlState(void);
extern controlState_e last_controlState;

#endif
