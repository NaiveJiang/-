#include "app.h"


appStruct_t appSupervisior;
appSupStruct_t supervisiorData;

appSupStruct_t *get_supervisiorData(void){
	return &supervisiorData;
}

//��������
void app_sup_LE(void){
	uint8_t le_error;
	if(!LE3) get_controlData()->error_sta &= 0xFFFFFF87;	//LE3Ϊ1ʱ��Ч
	else{
		le_error |= (uint8_t)LE0;
		le_error |= (uint8_t)(LE1 << 1);
		le_error |= (uint8_t)(LE2 << 2);
		get_controlData()->error_sta &= 0xFFFFFF87;
		get_controlData()->error_sta |= (le_error + 1) << 3;
	}
}

//״̬�����
void app_supervisiorTask(void *Parameters){
	TickType_t xLastWakeTime = xTaskGetTickCount();
	while(1){
		vTaskDelayUntil(&xLastWakeTime, SUPER_TASK_PERIOD);
		
		//��PBLE3ʱ��⻥������״̬
		if(PBLE3) app_sup_LE();							//��������
		error_set(&QSALARM,QSALARM_ERROR,1);				//ȱ����
		error_set(&DCVCHK,DCVCHK_ERROR,1);				//��ѹ��Դ���
		error_set(&JTJC,EMERGENCY_STOP,1);				//��ͣ����
		error_set(&IN_ALARM,SYSTEM_ERROR,1);				//ϵͳ����
		error_set(&PVDD,POWER_DOWN_ERROR,1);				//��ѹ��Դ���
		error_set(&DHAL,HIGH_VOLTAGE_SPARK_ERROR,1);		//��ѹ��𱨾�		
		error_set(&HIAL,HIGH_VOLTAGE_OVERCURRENT,1);		//��ѹ�ŵ����
		error_set(&IGBTBAL,IGBTB_OVERCURRENT,1);			//IGBTB����
		error_set(&IGBTAAL,IGBTA_OVERCURRENT,1);			//IGBTA����
		error_set(&TIAL,PRIMARY_OVERCURRENT,1);			//ԭ�ߵ�������
		
		if(adc_filter_VDC_ADC < LOW_VDC)	//VDC����470
			get_controlData()->error_sta |= VDC_LOW_ERROR;	//�ⲿ����������;���
		else
			get_controlData()->error_sta &= ~VDC_LOW_ERROR;	//������������
		
		//���ټ��
		if(get_dischargeCtrlData()->spdCtrl->local_speed < get_controlData()->set_speed_up)	//���ڵ�����С�趨�ٶȼ�����
			get_controlData()->error_sta |= SPEED_UP_ERROR;	//δ����
		else
			get_controlData()->error_sta &= ~SPEED_UP_ERROR;	//����
		
		//�߿�״̬
		if(LKEN) {	//�߿��ź���Ч
			if(LRUN){
				digitalHi(&get_controlData()->line_control);	//����������
				digitalLo(&get_controlData()->line_suspend);
			}
			else{
				if(get_controlData()->line_control)
					digitalHi(&get_controlData()->line_suspend);	//��������ͣ
			}
			
		}
		else{	//�߿��ź���Ч
			digitalLo(&get_controlData()->line_control);
			digitalLo(&get_controlData()->line_suspend);
		}
		
		
		//�ֶ�����תΪ��ѹ
		get_dischargeCtrlData()->manual_power = get_controlData()->manual_power * SAMP_MAX_VOLTAGE / get_controlData()->rated_power;
		//�ɼ���ѹadcתΪ�������
		get_dischargeCtrlData()->current_power = adc_filter_POV3V3 * get_controlData()->rated_power / MAX_POWER_ADC;
		
		//����ͣ��
		
		
		//д��flash
		if(get_supervisiorData()->flash_sw){
			app_FlashWriteUdata();
			digitalLo(&get_supervisiorData()->flash_sw);
		}
		IWDG_Feed(); //ι��
	}
}	


void app_supervisiorTaskInit(void){
	xTaskCreate(app_supervisiorTask,"SUPERVISIOR",SUPER_STACK_SIZE,NULL,SUPER_PRIORITY,&appSupervisior.xHandleTask);
}



