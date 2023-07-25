#include "app.h"

__IO powSet_pageStruct powSetData;

//��ȡpowerdata������
__IO powSet_pageStruct* get_powSetData(void){
	return &powSetData;
}
//�����õ�����״̬ ��Ҫ��ȡ��״̬��0:1����ģʽ��2����ģʽ��3�ٶ��ź�
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




