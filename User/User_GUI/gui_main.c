#include "app.h"
#include "math.h"

__IO main_pageStruct mainData;
__IO main_pageStruct *get_maindata(void){
	return &mainData;
}
//�����õ�mainҳ�������  mainҳ�淵�ص�����   ��Ҫ��ȡ��״̬��0λ���ο���״̬��1λ��翪��״̬
uint8_t get_mainData(uint8_t main_data,mainData_e data_state){
//	return main_data & data_state;
	return (main_data & data_state);
}




