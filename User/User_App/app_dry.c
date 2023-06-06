#include "app.h"

dryStruct_t dryData;
dryStruct_t* get_dryData(void){
	return &dryData;
}
//湿启动控制
void app_dryMode(void){
	
	//以2.5KW/s上升到最大功率
	//计算2.5kw的功率输出，满偏3v，输出电压=0.25k*3/额定功率
	dryData.inc_power = INCREASE_POWER * SAMP_MAX_VOLTAGE / get_controlData()->rated_power; 
	
	if(DHAL){	//产生较大打火
		digitalHi(&get_dryData()->spark_wait);
		digitalClan(&get_dryData()->dry_power);	//清空输出功率，需要等待0.2s后重新再增加功率
		pulse_outputLow(&RESET_DH,1);	//打火复位
	}
	
	//输出
	dac_ch1_voltageOut(dryData.dry_power);
	dac_ch2_voltageOut(dryData.dry_power);
	
	
}









