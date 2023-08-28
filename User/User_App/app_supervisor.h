#ifndef __APP_SUPERVISOR_H
#define __APP_SUPERVISOR_H

#include "util.h"

#define SUPER_TASK_PERIOD	2

#define LOW_VDC 450
#define WARN_VDC 470
#define MAX_POWER_ADC 3722.727272727272f

#define START_DELAY_TIME 200	//2s

typedef enum{
	SUPER_PRIORITY = 5,
	SUPER_STACK_SIZE = 256,
}appSupervisor_e;

enum{
	IN_HOT_ACTIVE 				= 0x00000001,	//热启动
	QSALARM_ERROR 				= 0x00000002,	//缺相							//停机
	DCVCHK_ERROR 				= 0x00000004,	//低压电源错误					//停机
	NEGATIVE_PRESSURE 			= 0x00000008,	//无负压			LE=0		//停机
	ELECTRODE_SW2_CUTOFF 		= 0x00000010,	//电极到位开关2断开	LE=1		//停机
	ELECTRODE_SW1_CUTOFF 		= 0x00000018,	//电极到位开关1断开	LE=2		//停机
	SAFETY_SW4_CUTOFF 			= 0x00000020,	//安全开关4断开		LE=3		//停机
	SAFETY_SW3_CUTOFF 			= 0x00000028,	//安全开关3断开		LE=4		//停机
	SAFETY_SW2_CUTOFF 			= 0x00000030,	//安全开关2断开		LE=5		//停机
	SAFETY_SW1_CUTOFF 			= 0x00000038,	//安全开关1断开		LE=6		//停机
	TEMP_SW_CUTOFF 				= 0x00000040,	//温度断开			LE=7		//停机
	EMERGENCY_STOP				= 0x00000080,	//急停报警						//停机
	SYSTEM_ERROR				= 0x00000100,	//系统报警						//停机
	POWER_DOWN_ERROR			= 0x00000200,	//掉电报警						//停机
	HIGH_VOLTAGE_SPARK_ERROR 	= 0x00000400,	//高压打火报警					//一分钟之内打火5次停机,一分钟内小于5次清零，直到再次发生打火再次计时
	HIGH_VOLTAGE_OVERCURRENT 	= 0x00000800,	//高压放电过流
	IGBTB_OVERCURRENT			= 0x00001000,	//IGBTB过流						//停机
	IGBTA_OVERCURRENT 			= 0x00002000,	//IGBTA过流						//停机
	PRIMARY_OVERCURRENT 		= 0x00004000,	//原边电流过流					//停机
	IDC_OVERCURRENT				= 0x00008000,	//IDC过流						//停机
	CJ3_ERROR					= 0x00010000,	//CJ3损坏						//停机
	CJ12_ERROR					= 0x00020000,	//CJ12损坏						//停机
	VDC_LOW_ERROR				= 0x00040000,	//外部交流输入过低				//低于450停机
	VDC_LOW_WARN				= 0x00080000,	//外部交流输入低预警			//低于470预警
};

typedef struct{
	uint8_t machine_active;	//启动标志
	formatTrans32Struct_t adc_output;
	formatTrans32Struct_t dac_output;
	
	uint8_t start_delay;	//开机延时
	uint8_t start_delay_sw;
	
	uint8_t flash_sw;
	uint8_t write_bkp_time;	//写入后备寄存区计数位
	uint8_t stop_alarm;
	//打火停机
	uint8_t spark_tim_sw;	//打火计数定时器触发
	uint8_t spark_count;	//打火次数
	uint32_t spark_tim;
	uint8_t spark_last_count;
	
	//功率过高警告
	float pwr_hi_warn;
	//功率过低警告
	float pwr_lo_warn;
	
}appSupStruct_t;


void app_supervisiorTaskInit(void);
appSupStruct_t *get_supervisiorData(void);


#endif
