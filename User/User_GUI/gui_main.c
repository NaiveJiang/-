#include "app.h"
#include "math.h"

__IO main_pageStruct mainData;
__IO main_pageStruct *get_maindata(void){
	return &mainData;
}
//解析得到main页面的数据  main页面返回的数据   需要读取的状态，0位电晕开关状态，1位抽风开关状态
uint8_t get_mainData(uint8_t main_data,mainData_e data_state){
//	return main_data & data_state;
	return (main_data & data_state);
}




