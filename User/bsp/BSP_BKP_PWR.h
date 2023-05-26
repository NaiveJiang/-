#ifndef __BSP_BKP_PWR_H
#define __BSP_BKP_PWR_H


#include "stm32f10x.h"

#define PVD_PREEMPTIONPRIORITY 8
#define PVD_SUBPRIORITY 0

#define RTC_BKPREG BKP_DR1				//RTC����У��Ĵ����������ظ�����
#define ACTIVITE_BKPREG BKP_DR2			//��������⣬ȷ������״̬
#define CONTROL_STATE_BKPREG BKP_DR3	//����״̬,ÿ��0.1sд��BKP
//���Ʋ����Ĵ�����ÿ��0.1sд��BKP

#define RTC_BACKUP_DATA 0x5050   //BKP�󱸼Ĵ����������ݣ�������BKP_DR1

typedef struct{
	uint16_t bkp_reg;
	uint16_t bkp_data;
}BKPdataStruct_t;


void bsp_BKP_PWRInit(void);
void bsp_PVDInit(void);
uint32_t bkp_hot_activite(void);


#endif

