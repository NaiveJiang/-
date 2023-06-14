#include "app.h"


appStruct_t appSupervisior;
appSupStruct_t supervisiorData;

appSupStruct_t *get_supervisiorData(void){
	return &supervisiorData;
}

//互锁开关
void app_sup_LE(void){
	uint8_t le_error;
	if(!LE3) get_controlData()->error_sta &= 0xFFFFFF87;	//LE3为1时有效
	else{
		le_error |= (uint8_t)LE0;
		le_error |= (uint8_t)(LE1 << 1);
		le_error |= (uint8_t)(LE2 << 2);
		get_controlData()->error_sta &= 0xFFFFFF87;
		get_controlData()->error_sta |= (le_error + 1) << 3;
	}
}

//状态机监控
void app_supervisiorTask(void *Parameters){
	TickType_t xLastWakeTime = xTaskGetTickCount();
	while(1){
		vTaskDelayUntil(&xLastWakeTime, SUPER_TASK_PERIOD);
		
		//打开PBLE3时检测互锁开关状态
		if(PBLE3) app_sup_LE();							//互锁开关
		error_set(&QSALARM,QSALARM_ERROR,1);				//缺项检测
		error_set(&DCVCHK,DCVCHK_ERROR,1);				//低压电源检测
		error_set(&JTJC,EMERGENCY_STOP,1);				//急停报警
		error_set(&IN_ALARM,SYSTEM_ERROR,1);				//系统报警
		error_set(&PVDD,POWER_DOWN_ERROR,1);				//低压电源检测
		error_set(&DHAL,HIGH_VOLTAGE_SPARK_ERROR,1);		//高压打火报警		
		error_set(&HIAL,HIGH_VOLTAGE_OVERCURRENT,1);		//高压放电过流
		error_set(&IGBTBAL,IGBTB_OVERCURRENT,1);			//IGBTB过流
		error_set(&IGBTAAL,IGBTA_OVERCURRENT,1);			//IGBTA过流
		error_set(&TIAL,PRIMARY_OVERCURRENT,1);			//原边电流过流
		
		if(adc_filter_VDC_ADC < LOW_VDC)	//VDC低于470
			get_controlData()->error_sta |= VDC_LOW_ERROR;	//外部交流输入过低警告
		else
			get_controlData()->error_sta &= ~VDC_LOW_ERROR;	//解决后清除报警
		
		//达速检测
		if(get_dischargeCtrlData()->spdCtrl->local_speed < get_controlData()->set_speed_up)	//大于等于最小设定速度即达速
			get_controlData()->error_sta |= SPEED_UP_ERROR;	//未达速
		else
			get_controlData()->error_sta &= ~SPEED_UP_ERROR;	//达速
		
		//线控状态
		if(LKEN) {	//线控信号有效
			if(LRUN){
				digitalHi(&get_controlData()->line_control);	//生产线运行
				digitalLo(&get_controlData()->line_suspend);
			}
			else{
				if(get_controlData()->line_control)
					digitalHi(&get_controlData()->line_suspend);	//生产线暂停
			}
			
		}
		else{	//线控信号无效
			digitalLo(&get_controlData()->line_control);
			digitalLo(&get_controlData()->line_suspend);
		}
		
		
		//手动功率转为电压
		get_dischargeCtrlData()->manual_power = get_controlData()->manual_power * SAMP_MAX_VOLTAGE / get_controlData()->rated_power;
		//采集电压adc转为输出功率
		get_dischargeCtrlData()->current_power = adc_filter_POV3V3 * get_controlData()->rated_power / MAX_POWER_ADC;
		
		//报警停机
		
		
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



