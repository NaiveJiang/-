#include "app.h"

//NTC adc±í
const uint16_t ntc_adc[NTC_NUM] = {4095,3887,3692,3508,3334,3169,3014,2867,2729,2598,	//0~9
								   2474,2357,2246,2140,2041,1947,1857,1772,1692,1616,	//10~19
								   1543,1475,1410,1348,1289,1233,1180,1129,1081,1035,	//20~29
								   991,950,910,873,837,803,770,739,709,681,				//30~39
								   654,628,604,580,558,536,516,496,477,459,				//40~49
								   442,426,410,395,380,367,353,341,328,317,				//50~59
								   306,295,285,275,265,256,247,239,231,223,				//60~69
								   216,208,202,195,189,182,176,171,165,160,				//70~79
								   155,150,145,141,137,132,128,124,121,117,				//80~89
								   113,110,107,104,101,98,95,92,89,87,84};				//90~100

//²é±í					  
uint8_t ntc_look_up(const uint16_t *list,uint16_t data){
	uint8_t middle = 0;
	uint8_t index_L = 0;
	uint8_t index_R = NTC_NUM - 1;
	if(data >= *(list + 0)) return 0;
	if(data <= *(list + NTC_NUM - 1)) return NTC_NUM - 1;
	
	while(index_R - index_L > 1){
		middle = (index_L + index_R) >> 1;
		if(data == *(list + middle)) return middle;
		else if(data > *(list + middle)) index_R = middle;
		else if(data < *(list + middle)) index_L = middle;
	}
	return index_L;
}


float get_ntc_temp(const uint16_t *list,uint16_t adc_val,float base_temp){
	uint8_t index = 0;
	uint8_t deta = 0;
	float t = 0;
	float temp = 0;
	
	index = ntc_look_up(list,adc_val);
	if(adc_val > list[0]) return base_temp;
	
	if(adc_val <= *(list + NTC_NUM - 1)){
		temp = ((NTC_NUM - 1) * 10 + base_temp);
		return temp;
	}
	deta = list[index] - list[index + 1];
	t = 10 * (list[index] - adc_val) / deta;
	temp = (base_temp + index * 10 + t);
	return temp;
}



















