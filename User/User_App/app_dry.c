#include "app.h"

dryStruct_t dryCtrlData;
dryStruct_t* get_dryCtrlData(void){
	return &dryCtrlData;
}
//湿启动控制
void app_dryMode(void){
	
	//以2.5KW/s上升到最大功率
	//计算2.5kw的功率输出，满偏3v，输出电压=0.25k*3/额定功率
	dryCtrlData.inc_power = INCREASE_POWER * SAMP_MAX_VOLTAGE / get_controlData()->rated_power; 
	
	if(DHAL){	//产生较大打火
		digitalHi(&get_dryCtrlData()->spark_wait);
		digitalClan(&get_dryCtrlData()->dry_power);	//清空输出功率，需要等待0.2s后重新再增加功率
		pulse_outputLow(&RESET_DH,1);	//打火复位
	}
	
	//输出
	dac_ch1_voltageOut(dryCtrlData.dry_power);
	dac_ch2_voltageOut(dryCtrlData.dry_power);
	
	
}









