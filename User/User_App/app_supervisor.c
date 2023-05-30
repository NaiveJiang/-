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
		error_set(&QSALARM,QSALARM_ERROR);				//ȱ����
		error_set(&DCVCHK,DCVCHK_ERROR);				//��ѹ��Դ���
		error_set(&JTJC,EMERGENCY_STOP);				//��ͣ����
		error_set(&IN_ALARM,SYSTEM_ERROR);				//ϵͳ����
		error_set(&PVDD,POWER_DOWN_ERROR);				//��ѹ��Դ���
		error_set(&DHAL,HIGH_VOLTAGE_SPARK_ERROR);		//��ѹ��𱨾�		
		error_set(&HIAL,HIGH_VOLTAGE_OVERCURRENT);		//��ѹ�ŵ����
		error_set(&IGBTBAL,IGBTB_OVERCURRENT);			//IGBTB����
		error_set(&IGBTAAL,IGBTA_OVERCURRENT);			//IGBTA����
		error_set(&TIAL,PRIMARY_OVERCURRENT);			//ԭ�ߵ�������
		
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



