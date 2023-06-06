#include "app.h"

dryStruct_t dryData;
dryStruct_t* get_dryData(void){
	return &dryData;
}
//ʪ��������
void app_dryMode(void){
	
	//��2.5KW/s�����������
	//����2.5kw�Ĺ����������ƫ3v�������ѹ=0.25k*3/�����
	dryData.inc_power = INCREASE_POWER * SAMP_MAX_VOLTAGE / get_controlData()->rated_power; 
	
	if(DHAL){	//�����ϴ���
		digitalHi(&get_dryData()->spark_wait);
		digitalClan(&get_dryData()->dry_power);	//���������ʣ���Ҫ�ȴ�0.2s�����������ӹ���
		pulse_outputLow(&RESET_DH,1);	//���λ
	}
	
	//���
	dac_ch1_voltageOut(dryData.dry_power);
	dac_ch2_voltageOut(dryData.dry_power);
	
	
}









