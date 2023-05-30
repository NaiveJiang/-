#ifndef __APP_SUPERVISOR_H
#define __APP_SUPERVISOR_H

#include "util.h"

#define SUPER_TASK_PERIOD	2

typedef enum{
	SUPER_PRIORITY = 5,
	SUPER_STACK_SIZE = 256,
}appSupervisor_e;

enum{
	IN_HOT_ACTIVE 				= 0x00000001,	//热启动
	QSALARM_ERROR 				= 0x00000002,	//缺相
	DCVCHK_ERROR 				= 0x00000004,	//低压电源错误
	NEGATIVE_PRESSURE 			= 0x00000008,	//无负压				LE=0
	ELECTRODE_SW2_CUTOFF 		= 0x00000010,	//电极到位开关2断开	LE=1
	ELECTRODE_SW1_CUTOFF 		= 0x00000018,	//电极到位开关1断开	LE=2
	SAFETY_SW4_CUTOFF 			= 0x00000020,	//安全开关4断开		LE=3
	SAFETY_SW3_CUTOFF 			= 0x00000028,	//安全开关3断开		LE=4
	SAFETY_SW2_CUTOFF 			= 0x00000030,	//安全开关2断开		LE=5
	SAFETY_SW1_CUTOFF 			= 0x00000038,	//安全开关1断开		LE=6
	TEMP_SW_CUTOFF 				= 0x00000040,	//温度断开			LE=7
	EMERGENCY_STOP				= 0x00000080,	//急停报警
	SYSTEM_ERROR				= 0x00000100,	//系统报警
	POWER_DOWN_ERROR			= 0x00000200,	//掉电报警
	HIGH_VOLTAGE_SPARK_ERROR 	= 0x00000400,	//高压打火报警
	HIGH_VOLTAGE_OVERCURRENT 	= 0x00000800,	//高压放电过流
	IGBTB_OVERCURRENT			= 0x00001000,	//IGBTB过流
	IGBTA_OVERCURRENT 			= 0x00002000,	//IGBTA过流
	PRIMARY_OVERCURRENT 		= 0x00004000,	//原边电流过流
	IDC_OVERCURRENT				= 0x00008000,	//IDC过流
};

typedef struct{
	uint8_t machine_active;	//启动标志
	formatTrans32Struct_t adc_output;
	formatTrans32Struct_t dac_output;
	uint8_t flash_sw;
	uint8_t write_bkp_time;	//写入后备寄存区计数位
}appSupStruct_t;


void app_supervisiorTaskInit(void);
appSupStruct_t *get_supervisiorData(void);


#endif
