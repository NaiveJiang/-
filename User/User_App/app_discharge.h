#ifndef __APP_DISCHARGE_H
#define __APP_DISCHARGE_H

#include "util.h"

#define INCREASE_DISCHARGE_MAX100MS 2	//2Kw���� 100ms
#define INCLEASE_TIME 10	//10*10=100ms


//����ŵ�
typedef struct{
	uint8_t discharge_sw;
	uint8_t mode;				//��Ъģʽ��1���������������屨��
	uint32_t set_delay_time;	//�趨����ʱʱ�䣬������ʱ��δ�����崥��
	uint32_t discharge_time;
	float discharge_power;		//dac���
	//�ֶ�ģʽ
	float inc_power;			//��������
}pulse_dischargeStruct_t;

//���ٷŵ�
typedef struct{
	float roller_pulse_length;		//���ع�Ͳ�����߳�
	float external_pulase_length;	//�ⲿ��Ͳ�����߳�
	float speed;
	float local_speed;				//���ع�Ͳ�ٶ�
	float external_speed;			//�ⲿ�������ٶ�
	float line_voltage_speed;		//�������ٶȵ�ѹ�ź�
	float line_current_speed;		//�������ٶȵ����ź�
	float discharge_power;			//dac���
	float max_pow;					//�ٱ�ģʽ�������
	uint8_t speed_signal;			//�ٶ��ź� 0�������� 1�ⲿ���� 2�����ߵ�ѹ 3�����ߵ���
	//�ֶ�ģʽ
	float inc_power;				//��������
	//�����ܶ�ģʽ
	float power_density;			//�����Ĺ����ܶ�
	float roller_width;				//������
	//�ٱ�ģʽ
	float max_spd;					//�������
	float scale;					//�������ʿ���ϵ��
	//�����źŴ���
	uint8_t remain_local_sw;		//ֹͣ���������ź�����ʱ��Ҫ�����������
	uint32_t remain_local_time;		//�����ȴ�ʱ������ٶ�
	uint8_t remain_external_sw;		//ֹͣ�ⲿ�����ź�����ʱ��Ҫ�����������
	uint32_t remain_external_time;	//�����ȴ�ʱ������ٶ�
	uint32_t set_remain_time;		//�趨�ĵȴ�ʱ��
	//δ����ʱ���ӳٷŵ����
	uint8_t delay_sw;
	uint32_t set_delay_time;		//�趨����ʱʱ�䣬������ʱ����ͣ�ŵ�
	uint32_t set_line_delay_time;	//�趨����������ʱʱ�䣬������ʱ����ͣ�ŵ�
	uint32_t delay_time;
}spd_dischargeStruct_t;

//�ŵ����
typedef struct{
	float discharge_power;
	uint8_t power_ctrlState;
	uint8_t mode;				//�ŵ�ģʽ  0����ģʽ  1���崥��ģʽ
	uint8_t line_power_mode;	//�����߹��ʸ���ģʽ	0�����߹��ʸ�����ѹ����  1�����߹��ʸ�����������
	float line_set_power;		//�趨�������߹��ʸ���
	float manual_power;			//�ֶ��趨�������
	float power_density;		//�趨�Ĺ����ܶ�
	float low_power;			//�趨����С���ʣ������崥��״̬�´��ٺ������ٿ��ƣ������ٰ��ոù���(�ֶ�����)/�ٱ�ģʽ�£�������ٱ����С�ڸù��ʣ����ոù������
	float current_power;		//��ǰ����
	uint8_t step;
	uint32_t inc_time;
}dischargeStruct_t;

dischargeStruct_t* get_dischargeCtrlData(void);
pulse_dischargeStruct_t* get_pulseDischargeData(void);
spd_dischargeStruct_t* get_spdDischargeData(void);
void app_discharge(void);
void pulse_dischargeUpdate(void);
void spd_dischargeUpdate(void);
void spd_discharge_delay(void);
float get_speed(uint8_t speedMode);
float get_line_set_power(uint8_t line_power_mode);
#endif
