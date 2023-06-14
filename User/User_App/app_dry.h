#ifndef __APP_DRY_H
#define __APP_DRY_H


#include "util.h"

#define INCREASE_POWER 0.25f	//2.5kw/s
#define DRY_INCREASE_TIME 100	//1s
#define SPARK_WAIT_TIME 20		//0.2s


typedef struct{
	float dry_power;
	float inc_power;
	float max_power;
	
	uint8_t dry_time;	//1s�Ĺ�������
	
	uint8_t spark_wait;	//���ȴ���־
	uint8_t spark_wait_time;	//���ȴ�
}dryStruct_t;




void app_dryMode(void);
dryStruct_t* get_dryCtrlData(void);

#endif
