#include "app.h"

__IO powSet_pageStruct powSetData;

//获取powerdata的数据
__IO powSet_pageStruct* get_powSetData(void){
	return &powSetData;
}
//解析得到工作状态 需要读取的状态，0:1功率模式，2控制模式，3速度信号
uint8_t get_setStateData(uint8_t ctrlSet_data,ctrlSetData_e data_state){
	uint8_t data;
	switch(data_state){
		case POWERMODE:{
			data = ctrlSet_data & data_state;
		}break;
		case CONTROLMODE:{
			data = (ctrlSet_data & data_state) >> 2;
		}break;
		case SPEEDSIGNAL:{
			data = (ctrlSet_data & data_state) >> 3;
		}break;
		default:break;
	}
	return data;
}




