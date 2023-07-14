#ifndef __APP_ROLL_CHANGING_H
#define __APP_ROLL_CHANGING_H

#include "util.h"

typedef struct{
	uint8_t state;	//信号状态
	uint8_t delay_sw;		//延迟长度计算触发
	float speed;
	uint16_t length;	//延迟长度
	uint8_t step;	//换卷信号动作
	uint16_t set_delay_length1; //设定延迟放电长度
	uint16_t set_delay_length2; //设定电极闭合延迟长度
	
	
}roll_changingStruct_t;


roll_changingStruct_t *get_rcCtrlData(void);
void app_roll_changing(void);



#endif
