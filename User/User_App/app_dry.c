#include "app.h"

dryStruct_t dryCtrlData;
dryStruct_t* get_dryCtrlData(void){
	return &dryCtrlData;
}
//ʪ��������
void app_dryMode(void){
	
	//��2.5KW/s�����������
	//����2.5kw�Ĺ����������ƫ3v�������ѹ=0.25k*3/�����
	dryCtrlData.inc_power = INCREASE_POWER * SAMP_MAX_VOLTAGE / get_controlData()->rated_power; 
	
	if(DHAL){	//�����ϴ���
		digitalHi(&get_dryCtrlData()->spark_wait);
		digitalClan(&get_dryCtrlData()->dry_power);	//���������ʣ���Ҫ�ȴ�0.2s�����������ӹ���
		pulse_outputLow(&RESET_DH,1);	//���λ
	}
	
	//���
	dac_ch1_voltageOut(dryCtrlData.dry_power);
	dac_ch2_voltageOut(dryCtrlData.dry_power);
	
	
}









