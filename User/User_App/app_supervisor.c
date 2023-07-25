#include "app.h"


appStruct_t appSupervisior;
appSupStruct_t supervisiorData;

appSupStruct_t *get_supervisiorData(void){
	return &supervisiorData;
}


uint8_t rst_error = 0;
uint8_t read_cj3 = 0;
//互锁开关
void app_sup_LE(void){
	uint8_t le_error;
	if(!LE3) get_controlData()->error_sta &= 0xFFFFFF87;	//LE3为1时有效
	else if(!(get_controlData()->error_sta & EMERGENCY_STOP)){ //如果没有急停报警
		le_error = (uint8_t)LE0 | (uint8_t)(LE1 << 1) | (uint8_t)(LE2 << 2);
		get_controlData()->error_sta &= 0xFFFFFF87;
		//屏蔽负压开关后屏蔽无负压检测
		if(FYKL && !le_error) get_controlData()->error_sta |= (le_error) << 3;
		else get_controlData()->error_sta |= (le_error + 1) << 3;
	}
	else get_controlData()->error_sta &= 0xFFFFFF87;
}

//状态机监控
void app_supervisiorTask(void *Parameters){
	TickType_t xLastWakeTime = xTaskGetTickCount();
	while(1){
		vTaskDelayUntil(&xLastWakeTime, SUPER_TASK_PERIOD);
		
		
		//打开PBLE3时检测互锁开关状态
		if(!PBLE3) app_sup_LE();							//互锁开关
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
		
//		if(get_rcCtrlData()->state){	//触发换卷信号
//			if(!get_rcCtrlData()->sw){
//				digitalHi(&get_rcCtrlData()->sw);
//				//记录上一次的状态
//				get_controlData()->last_control_step = get_controlData()->control_step;
//				last_controlState = get_controlState(); 
//			}
//			set_controlState(__ROLL_CHANGING,get_controlData()->control_step);
//		}
		
		
		//速度信号更新
		get_spdDischargeData()->speed_signal = get_setStateData(get_powSetData()->set_state,SPEEDSIGNAL);

		//联动信号
		LKEN = get_setStateData(get_powSetData()->set_state,CONTROLMODE);
		
		if(adc_filter_VDC_ADC < LOW_VDC)	//VDC低于470
			get_controlData()->error_sta |= VDC_LOW_ERROR;	//外部交流输入过低警告
		else
			get_controlData()->error_sta &= ~VDC_LOW_ERROR;	//解决后清除报警
		
		//达速检测(如果手动达速，屏蔽达速检测)
		if(!get_controlData()->manual_mode){
			if(get_spdDischargeData()->local_speed < get_controlData()->set_speed_up){	//大于等于最小设定速度即达速
				get_controlData()->error_sta |= SPEED_UP_ERROR;	//未达速
				digitalLo(&get_controlData()->speed_up);
			}
			else{
				get_controlData()->error_sta &= ~SPEED_UP_ERROR;	//达速
				digitalHi(&get_controlData()->speed_up);
			}
		}
		else{
			get_controlData()->error_sta &= ~SPEED_UP_ERROR;	//达速
			digitalHi(&get_controlData()->speed_up);
		}
		
		if(get_controlData()->use_pulse_corona){	//使用脉冲触发
			if(!get_controlData()->speed_up){
				//未达速处于脉冲触发模式
				digitalHi(&get_dischargeCtrlData()->mode);
			}
		}
		
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
		//采集电压adc转为输出功率 kw
		get_dischargeCtrlData()->current_power = adc_filter_POV3V3 * get_controlData()->rated_power / MAX_POWER_ADC;
		
		//根据速度模式选择获取当前线速度	本地脉冲/外部脉冲/生产线电压/生产线电流
		get_spdDischargeData()->speed = get_speed(get_spdDischargeData()->speed_signal);
		
//		//线速状态下，如果输出和真实输出产生的误差大于4kw，说明输出不受控，一定时间后停机并报警
//		if((fabs(get_spdDischargeData()->discharge_power - get_dischargeCtrlData()->current_power) > 4.0f) && !get_dischargeCtrlData()->mode){
//			
//		}
//		
//		//脉冲状态下，如果输出和真实输出产生的误差大于4kw，说明输出不受控，一定时间后停机并报警
//		if((fabs(get_pulseDischargeData()->discharge_power - get_dischargeCtrlData()->current_power) > 4.0f) && get_dischargeCtrlData()->mode){
//			
//		}
		
		//报警停机
		
		//获取温度
		get_controlData()->temp = get_ntc_temp(ntc_adc,(uint16_t)adc_filter_TEMP,BASE_TEMP) * 0.1f;		
		
		/***************************test******************************/
		if(rst_error){	//故障复位
			pulse_outputHigh(&RESET_SYS,10);
			digitalLo(&rst_error);
		}
		read_cj3 = CJ3OK;
		/*************************************************************/
		
		//写入flash
		if(get_supervisiorData()->flash_sw){
			app_flash_dataUpdate();
			app_FlashWriteUdata();
			digitalLo(&get_supervisiorData()->flash_sw);
		}
		IWDG_Feed(); //喂狗
	}
}	


void app_supervisiorTaskInit(void){
	xTaskCreate(app_supervisiorTask,"SUPERVISIOR",SUPER_STACK_SIZE,NULL,SUPER_PRIORITY,&appSupervisior.xHandleTask);
}



