#include "app.h"


appStruct_t appSupervisior;
appSupStruct_t supervisiorData;

appSupStruct_t *get_supervisiorData(void){
	return &supervisiorData;
}

//LE���ϴ���
void app_sup_LE(void){
	uint8_t le_error;
	if(!LE3) return;	//LE3Ϊ1ʱ��Ч
	else{
		le_error |= (uint8_t)LE0;
		le_error |= (uint8_t)(LE1 << 1);
		le_error |= (uint8_t)(LE2 << 2);
		get_controlData()->error_sta |= NEGATIVE_PRESSURE << le_error;
	}
}

//״̬�����
void app_supervisiorTask(void *Parameters){
	TickType_t xLastWakeTime = xTaskGetTickCount();
	while(1){
		vTaskDelayUntil(&xLastWakeTime, SUPER_TASK_PERIOD);
		
		//��PBLE3ʱ���LE״̬
		if(PBLE3) app_sup_LE();
		if(QSALARM) get_controlData()->error_sta |= QSALARM_ERROR;			//ȱ����
		if(DCVCHK) get_controlData()->error_sta |= DCVCHK_ERROR;			//��ѹ��Դ���
		if(JTJC) get_controlData()->error_sta |= EMERGENCY_STOP;			//��ͣ����
		if(IN_ALARM) get_controlData()->error_sta |= SYSTEM_ERROR;			//ϵͳ����
		if(PVDD) get_controlData()->error_sta |= POWER_DOWN_ERROR;			//��ѹ��Դ���
		if(DHAL) get_controlData()->error_sta |= HIGH_VOLTAGE_SPARK_ERROR;	//��ѹ��𱨾�
		if(HIAL) get_controlData()->error_sta |= HIGH_VOLTAGE_OVERCURRENT;	//��ѹ�ŵ����
		if(IGBTBAL) get_controlData()->error_sta |= IGBTB_OVERCURRENT;		//IGBTB����
		if(IGBTAAL) get_controlData()->error_sta |= IGBTA_OVERCURRENT;		//IGBTA����
		if(TIAL) get_controlData()->error_sta |= SYSTEM_ERROR;				//ԭ�ߵ�������
		if(IDCAL) get_controlData()->error_sta |= POWER_DOWN_ERROR;			//IDC����
		
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



