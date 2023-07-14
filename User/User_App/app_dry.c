#include "app.h"

dryStruct_t dryCtrlData;
dryStruct_t* get_dryCtrlData(void){
	return &dryCtrlData;
}

//��𱨾�λ��ʼ��
void dhal_init(void){
	BSP_GPIO_EXTI_Init(DHAL_PORT,EXTI_Trigger_Rising,DHAL_PREEMPTIONPRIORITY,DHAL_SUBPRIORITY);
}

//ʪ��������
void app_dryCtrl(void){
	
	//ÿ30s��������ʵ�5%��ֱ��ʪ��������ʵ�50%
	dryCtrlData.inc_power = get_controlData()->rated_power * 0.05f; 
	
	if(dryCtrlData.spark_wait){	//�����ϴ���,���͵���ǰ���ʵ�50%
		digitalLo(&dryCtrlData.spark_wait);
		pulse_outputLow(&RESET_DH,1);	//���λ
	}
	
	//������Ϊ����ʵ�50%
	if(dryCtrlData.dry_power > dryCtrlData.max_power){
		dryCtrlData.dry_power = dryCtrlData.max_power;
	}
	
	//���
	dac_ch1_voltageOut(dryCtrlData.dry_power * SAMP_MAX_VOLTAGE / get_controlData()->rated_power);
	dac_ch2_voltageOut(dryCtrlData.dry_power * SAMP_MAX_VOLTAGE / get_controlData()->rated_power);
	
	
}









