#include "app.h"


appStruct_t appSupervisior;
appSupStruct_t supervisiorData;

appSupStruct_t *get_supervisiorData(void){
	return &supervisiorData;
}

//LE故障代码
void app_sup_LE(void){
	uint8_t le_error;
	if(!LE3) return;	//LE3为1时有效
	else{
		le_error |= (uint8_t)LE0;
		le_error |= (uint8_t)(LE1 << 1);
		le_error |= (uint8_t)(LE2 << 2);
		get_controlData()->error_sta |= NEGATIVE_PRESSURE << le_error;
	}
}

//状态机监控
void app_supervisiorTask(void *Parameters){
	TickType_t xLastWakeTime = xTaskGetTickCount();
	while(1){
		vTaskDelayUntil(&xLastWakeTime, SUPER_TASK_PERIOD);
		
		//打开PBLE3时检测LE状态
		if(PBLE3) app_sup_LE();
		if(QSALARM) get_controlData()->error_sta |= QSALARM_ERROR;			//缺项检测
		if(DCVCHK) get_controlData()->error_sta |= DCVCHK_ERROR;			//低压电源检测
		if(JTJC) get_controlData()->error_sta |= EMERGENCY_STOP;			//急停报警
		if(IN_ALARM) get_controlData()->error_sta |= SYSTEM_ERROR;			//系统报警
		if(PVDD) get_controlData()->error_sta |= POWER_DOWN_ERROR;			//低压电源检测
		if(DHAL) get_controlData()->error_sta |= HIGH_VOLTAGE_SPARK_ERROR;	//高压打火报警
		if(HIAL) get_controlData()->error_sta |= HIGH_VOLTAGE_OVERCURRENT;	//高压放电过流
		if(IGBTBAL) get_controlData()->error_sta |= IGBTB_OVERCURRENT;		//IGBTB过流
		if(IGBTAAL) get_controlData()->error_sta |= IGBTA_OVERCURRENT;		//IGBTA过流
		if(TIAL) get_controlData()->error_sta |= SYSTEM_ERROR;				//原边电流过流
		if(IDCAL) get_controlData()->error_sta |= POWER_DOWN_ERROR;			//IDC过流
		
		//写入flash
		if(get_supervisiorData()->flash_sw){
			app_FlashWriteUdata();
			digitalLo(&get_supervisiorData()->flash_sw);
		}
		IWDG_Feed(); //喂狗
	}
}	


void app_supervisiorTaskInit(void){
	xTaskCreate(app_supervisiorTask,"SUPERVISIOR",SUPER_STACK_SIZE,NULL,SUPER_PRIORITY,&appSupervisior.xHandleTask);
}



