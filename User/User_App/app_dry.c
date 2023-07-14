#include "app.h"

dryStruct_t dryCtrlData;
dryStruct_t* get_dryCtrlData(void){
	return &dryCtrlData;
}

//打火报警位初始化
void dhal_init(void){
	BSP_GPIO_EXTI_Init(DHAL_PORT,EXTI_Trigger_Rising,DHAL_PREEMPTIONPRIORITY,DHAL_SUBPRIORITY);
}

//湿启动控制
void app_dryCtrl(void){
	
	//每30s上升额定功率的5%，直到湿启动额定功率的50%
	dryCtrlData.inc_power = get_controlData()->rated_power * 0.05f; 
	
	if(dryCtrlData.spark_wait){	//产生较大打火,降低到当前功率的50%
		digitalLo(&dryCtrlData.spark_wait);
		pulse_outputLow(&RESET_DH,1);	//打火复位
	}
	
	//最大输出为额定功率的50%
	if(dryCtrlData.dry_power > dryCtrlData.max_power){
		dryCtrlData.dry_power = dryCtrlData.max_power;
	}
	
	//输出
	dac_ch1_voltageOut(dryCtrlData.dry_power * SAMP_MAX_VOLTAGE / get_controlData()->rated_power);
	dac_ch2_voltageOut(dryCtrlData.dry_power * SAMP_MAX_VOLTAGE / get_controlData()->rated_power);
	
	
}









