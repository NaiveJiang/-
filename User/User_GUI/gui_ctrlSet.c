#include "app.h"
#include "math.h"

__IO ctrlSet_pageStruct ctrlSetData;

//解析得到controlset页面的数据  controlset页面返回的数据   需要读取的状态，0~1功率模式，2控制模式，3速度信号
uint8_t get_ctrlSetData(uint8_t ctrlSet_data,ctrlSetData_e data_state){
	if(data_state == POWERMODE)
		return ctrlSet_data & data_state;
	else
		return (ctrlSet_data & data_state) >> ((uint8_t)sqrt(data_state));
}



