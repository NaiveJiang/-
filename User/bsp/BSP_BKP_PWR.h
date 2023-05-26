#ifndef __BSP_BKP_PWR_H
#define __BSP_BKP_PWR_H


#include "stm32f10x.h"

#define PVD_PREEMPTIONPRIORITY 8
#define PVD_SUBPRIORITY 0

#define RTC_BKPREG BKP_DR1				//RTC配置校验寄存器，避免重复配置
#define ACTIVITE_BKPREG BKP_DR2			//热启动检测，确认启动状态
#define CONTROL_STATE_BKPREG BKP_DR3	//工作状态,每隔0.1s写入BKP
//控制参数寄存器，每隔0.1s写入BKP

#define RTC_BACKUP_DATA 0x5050   //BKP后备寄存区保存数据，保存至BKP_DR1

typedef struct{
	uint16_t bkp_reg;
	uint16_t bkp_data;
}BKPdataStruct_t;


void bsp_BKP_PWRInit(void);
void bsp_PVDInit(void);
uint32_t bkp_hot_activite(void);


#endif

