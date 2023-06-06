#ifndef __APP_DISCHARGE_H
#define __APP_DISCHARGE_H

#include "util.h"

//����ŵ�
typedef struct{
	uint8_t discharge_sw;
	uint8_t mode;				//��Ъģʽ��1���������������屨��
	uint32_t discharge_time;
	float discharge_power;
}pulse_dischargeStruct_t;

//���ٷŵ�
typedef struct{
	float scale;	//�������ʿ���ϵ��
	
}spd_dischargeStruct_t;

//�ŵ����
typedef struct{
	pulse_dischargeStruct_t *pulseCtrl;
	spd_dischargeStruct_t	*spdCtrl;
	float manual_power;
}dischargeStruct_t;

dischargeStruct_t* get_dischargeCtrlData(void);

#endif
