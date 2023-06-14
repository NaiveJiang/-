#include "app.h"
#include "math.h"

__IO dry_pageStruct dryData;
__IO dry_pageStruct *get_drydata(void){
	return &dryData;
}

//�����õ�ʪ����ҳ�������  ʪ����ҳ�淵�ص�����   ��Ҫ��ȡ��״̬��0λʪ����״̬λ
uint8_t get_dryData(uint8_t dry_data,dryData_e data_state){
	return (dry_data & data_state) >> ((uint8_t)sqrt(data_state));
}

