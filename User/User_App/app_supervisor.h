#ifndef __APP_SUPERVISOR_H
#define __APP_SUPERVISOR_H

#include "util.h"

#define SUPER_TASK_PERIOD	2

typedef enum{
	SUPER_PRIORITY = 5,
	SUPER_STACK_SIZE = 256,
}appSupervisor_e;

enum{
	QSALARM_ERROR 				= 0x00000001,	//ȱ��
	DCVCHK_ERROR 				= 0x00000002,	//��ѹ��Դ����
	IN_HOT_ACTIVE 				= 0x00000004,	//������
	NEGATIVE_PRESSURE 			= 0x00000008,	//�޸�ѹ				LE=0
	ELECTRODE_SW2_CUTOFF 		= 0x00000010,	//�缫��λ����2�Ͽ�	LE=1
	ELECTRODE_SW1_CUTOFF 		= 0x00000020,	//�缫��λ����1�Ͽ�	LE=2
	SAFETY_SW4_CUTOFF 			= 0x00000040,	//��ȫ����4�Ͽ�		LE=3
	SAFETY_SW3_CUTOFF 			= 0x00000080,	//��ȫ����3�Ͽ�		LE=4
	SAFETY_SW2_CUTOFF 			= 0x00000100,	//��ȫ����2�Ͽ�		LE=5
	SAFETY_SW1_CUTOFF 			= 0x00000200,	//��ȫ����1�Ͽ�		LE=6
	TEMP_SW_CUTOFF 				= 0x00000400,	//�¶ȶϿ�			LE=7
	EMERGENCY_STOP				= 0x00000800,	//��ͣ����
	SYSTEM_ERROR				= 0x00001000,	//ϵͳ����
	POWER_DOWN_ERROR			= 0x00002000,	//���籨��
	HIGH_VOLTAGE_SPARK_ERROR 	= 0x00004000,	//��ѹ��𱨾�
	HIGH_VOLTAGE_OVERCURRENT 	= 0x00008000,	//��ѹ�ŵ����
	IGBTB_OVERCURRENT			= 0x00010000,	//IGBTB����
	IGBTA_OVERCURRENT 			= 0x00020000,	//IGBTA����
	PRIMARY_OVERCURRENT 		= 0x00040000,	//ԭ�ߵ�������
	IDC_OVERCURRENT				= 0x00080000,	//IDC����
};

typedef struct{
	uint8_t machine_active;	//������־
	formatTrans32Struct_t adc_output;
	formatTrans32Struct_t dac_output;
	uint8_t flash_sw;
	uint8_t write_bkp_time;	//д��󱸼Ĵ�������λ
}appSupStruct_t;


void app_supervisiorTaskInit(void);
appSupStruct_t *get_supervisiorData(void);


#endif
