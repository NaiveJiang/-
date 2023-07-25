#ifndef __APP_ROLL_CHANGING_H
#define __APP_ROLL_CHANGING_H

#include "util.h"

typedef struct{
	uint8_t state;	//�ź�״̬
	uint8_t delay_sw;		//�ӳٳ��ȼ��㴥��
	float speed;
	float length;	//�ӳٳ���
	uint8_t step;	//�����źŶ���
	float set_delay_length1; //�趨�ӳٷŵ糤��
	float set_delay_length2; //�趨�缫�պ��ӳٳ���
	
	
}roll_changingStruct_t;


roll_changingStruct_t *get_rcCtrlData(void);
void app_roll_changing(void);



#endif
