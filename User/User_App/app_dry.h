#ifndef __APP_DRY_H
#define __APP_DRY_H


#include "util.h"

#define DHAL_PORT BSP_GPIOE4		//高压打火报警
#define DHAL PEin(4)
#define DHAL_PREEMPTIONPRIORITY 5
#define DHAL_SUBPRIORITY 0


#define INCREASE_POWER 0.25f	//2.5kw/s
#define DRY_INCREASE_TIME 300000	//30s
#define SPARK_WAIT_TIME 20		//0.2s


typedef struct{
	float dry_power;	//输出功率
	float inc_power;
	float max_power;
	
	uint32_t dry_time;	//30s的功率自增
	uint8_t spark_wait;	//打火等待标志
	uint8_t spark_wait_time;	//打火等待
}dryStruct_t;



void dhal_init(void);
void app_dryCtrl(void);
dryStruct_t* get_dryCtrlData(void);

#endif
