#include "app.h"
#include "math.h"

__IO ctrlSet_pageStruct ctrlSetData;

//�����õ�controlsetҳ�������  controlsetҳ�淵�ص�����   ��Ҫ��ȡ��״̬��0~1����ģʽ��2����ģʽ��3�ٶ��ź�
uint8_t get_ctrlSetData(uint8_t ctrlSet_data,ctrlSetData_e data_state){
	if(data_state == POWERMODE)
		return ctrlSet_data & data_state;
	else
		return (ctrlSet_data & data_state) >> ((uint8_t)sqrt(data_state));
}



