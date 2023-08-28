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

void remain_day(void){
	if(get_controlData()->rtc_day1){	//期限1未到
		if(get_controlData()->rtc_day1 <= 15 && get_controlData()->rtc_day1 > 5){  //还剩下15天
			get_controlData()->remain_day = 1;
		}
		else if(get_controlData()->rtc_day1 <= 5){	//还剩下5天
			get_controlData()->remain_day = 2;
		}
		else{	//期限1大于15天
			digitalClan(&get_controlData()->remain_day); 
		}
	}
	else{	//期限1已到
		if(get_controlData()->rtc_day2){ //期限2未到
			if(get_controlData()->rtc_day2 <= 15 && get_controlData()->rtc_day2 > 5){  //还剩下15天
				get_controlData()->remain_day = 4;
			}
			else if(get_controlData()->rtc_day1 <= 5){	//还剩下5天
				get_controlData()->remain_day = 5;
			}
			else{ //期限2大于15天，日期1上锁时为3，日期1解锁时为0
				if(!get_controlData()->lock) //如果日期1已经解锁
					digitalClan(&get_controlData()->remain_day); 
			}
		}
		else{	//期限2已到
			if(get_controlData()->rtc_day3){ //期限3未到
				if(get_controlData()->rtc_day1 <= 15 && get_controlData()->rtc_day1 > 5){  //还剩下15天
					get_controlData()->remain_day = 7;
				}
				else if(get_controlData()->rtc_day1 <= 5){	//还剩下5天
					get_controlData()->remain_day = 8;
				}
				else{	//期限3大于15天，日期2上锁为6，日期2解锁为0
					if(!get_controlData()->lock) //如果日期2已经解锁
						digitalClan(&get_controlData()->remain_day);
				}
			}
			else{	//期限3已到
				if(get_controlData()->lock)  //上锁状态
					get_controlData()->remain_day = 9;
				else
					digitalClan(&get_controlData()->remain_day);
			}
		}
	}
}

void lock_to_unlock(void){
	if(!get_controlData()->rtc_day1){	//如果期限1已到
		if(get_controlData()->rtc_day2){	//期限2未到
			//输入第一把锁的密码正确解锁
			if(get_controlData()->verify_password == get_controlData()->password[0]){
				digitalLo(&get_controlData()->lock);	//机器解锁
				digitalHi(&get_controlData()->pwd_ok);
			}
			else{
				digitalHi(&get_controlData()->lock); //密码错误机器上锁
				digitalLo(&get_controlData()->pwd_ok);
				//如果期限2小于15则按照小于15天的方法处理
				if(get_controlData()->rtc_day2 > 15)
					get_controlData()->remain_day = 3; //日期1锁机
			}
		}
		else{	//期限2已到
			if(get_controlData()->rtc_day3){	//期限3未到
				//输入第二把锁的密码正确解锁
				if(get_controlData()->verify_password == get_controlData()->password[1]){
					digitalLo(&get_controlData()->lock);	//机器解锁
					digitalHi(&get_controlData()->pwd_ok);
				}
				else{
					digitalHi(&get_controlData()->lock); //密码错误机器上锁
					digitalLo(&get_controlData()->pwd_ok);
					//如果期限3小于15则按照小于15天的方法处理
					if(get_controlData()->rtc_day3 > 15)
						get_controlData()->remain_day = 6;	//日期2锁机
				}
			}
			else{	//期限3已到
				//输入第三把锁的密码正确解锁
				if(get_controlData()->verify_password == get_controlData()->password[2]){
					digitalLo(&get_controlData()->lock);	//机器解锁
					digitalHi(&get_controlData()->pwd_ok);
				}
				else{
					digitalHi(&get_controlData()->lock); //密码错误机器上锁
					digitalLo(&get_controlData()->pwd_ok);
					get_controlData()->remain_day = 9;	//日期3锁机
				}
			}
		}
	}
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
//		error_set(&DHAL,HIGH_VOLTAGE_SPARK_ERROR,1);		//高压打火报警		
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
		
//		//只有在电晕状态VDC的检测是正常的
		if(get_controlState() == __CORONA){
			//触发电晕时，需要等待一段时间使VDC稳定
			if(get_supervisiorData()->start_delay >= START_DELAY_TIME){
				if(adc_filter_VDC_ADC < LOW_VDC)	//VDC低于450
					get_controlData()->error_sta |= VDC_LOW_ERROR;	//外部交流输入过低警告
				else
					get_controlData()->error_sta &= ~VDC_LOW_ERROR;	//解决后清除报警
				
				if(adc_filter_VDC_ADC < WARN_VDC)	//VDC低于470
					get_controlData()->error_sta |= VDC_LOW_WARN;	//外部交流输入预警
				else
					get_controlData()->error_sta &= ~VDC_LOW_WARN;	//解决后清除预警
			}
		}
		
		
		//达速检测(如果手动达速，屏蔽达速检测)
		if(!get_controlData()->manual_mode){
			if(get_spdDischargeData()->local_speed < get_controlData()->set_speed_up){	//大于等于最小设定速度即达速
				digitalLo(&get_controlData()->speed_up);
				SPDUP = 0;
			}
			else{
				digitalHi(&get_controlData()->speed_up);
				SPDUP = 1;
				//达速后切换为线速状态
				digitalLo(&get_dischargeCtrlData()->mode);
//				if(get_dischargeCtrlData()->mode){	//如果是脉冲放电状态
//					//达速直接进入线速放电
//					digitalLo(&get_dischargeCtrlData()->mode);
//					digitalClan(&get_dischargeCtrlData()->step);
//				}
			}
		}
		else{
			digitalHi(&get_controlData()->speed_up);
			SPDUP = 1;
		}
		
		//负压检测
		if(!FYKL){ //屏蔽负压开关时候不检测
			if((get_controlData()->error_sta & 0x78) == NEGATIVE_PRESSURE){
				//无负压
				digitalLo(&get_controlData()->fy_ok);
			}
			else{
				//有负压
				digitalHi(&get_controlData()->fy_ok);
			}
		}
		else{
			//无负压
			digitalLo(&get_controlData()->fy_ok);
		}
		
		//使用脉冲触发
		if(get_controlData()->use_pulse_corona){	
			if(!get_controlData()->speed_up){
				//未达速处于脉冲触发模式
				digitalHi(&get_dischargeCtrlData()->mode);
				//拉高两个标志位防止切换线速状态清空
				digitalHi(&get_spdDischargeData()->remain_local_sw);
				digitalHi(&get_spdDischargeData()->remain_external_sw);
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
		
		//功率输出过低报警
		if(get_dischargeCtrlData()->current_power < get_supervisiorData()->pwr_lo_warn){
			PLBJ = 1;
		}
		//功率输出过高报警
		else if(get_dischargeCtrlData()->current_power > get_supervisiorData()->pwr_hi_warn){
			PHBJ = 1;
		}
		else{
			PLBJ = 0;
			PHBJ = 0;
		}
		
		//输出功率超过额定功率的1%，说明正在放电,驱动放电继电器
		if(get_dischargeCtrlData()->current_power >= get_controlData()->rated_power * 0.01f){
			FD = 1;
		}
		else{
			FD = 0;
		}
		
//		//线速状态下，如果输出和真实输出产生的误差大于4kw，说明输出不受控，一定时间后停机并报警
//		if((fabs(get_spdDischargeData()->discharge_power - get_dischargeCtrlData()->current_power) > 4.0f) && !get_dischargeCtrlData()->mode){
//			
//		}
//		
//		//脉冲状态下，如果输出和真实输出产生的误差大于4kw，说明输出不受控，一定时间后停机并报警
//		if((fabs(get_pulseDischargeData()->discharge_power - get_dischargeCtrlData()->current_power) > 4.0f) && get_dischargeCtrlData()->mode){
//			
//		}
		
		/************************************************************************************/
		//如果收到打火报警后，需要进行打火复位
		if((get_controlData()->error_sta & HIGH_VOLTAGE_SPARK_ERROR) && (get_supervisiorData()->spark_last_count != get_supervisiorData()->spark_count)){
			pulse_outputLow(&RESET_DH,50);	//打火复位
			digitalLo(&get_dryCtrlData()->spark_wait);
		}
		//防止重复触发
		get_supervisiorData()->spark_last_count = get_supervisiorData()->spark_count;
		
		/************************************************************************************/
		//报警停机(一些报警会使得机器停机)
		//开机1s系统稳定后在检测停机报警
		if(get_controlData()->start_init){
			//互锁开关停机
			if(!PBLE3){		//解除互锁开关屏蔽
				//触发互锁开关必定停机
				if(get_controlData()->error_sta & 0x78){
					digitalHi(&get_supervisiorData()->stop_alarm);	//停机故障
					if(get_controlState() != __STOP)
						set_controlState(__STOP,0);	//进入停机
				}
			}
			//其他报警停机
			if(get_controlData()->error_sta & 0x7F386){
				digitalHi(&get_supervisiorData()->stop_alarm);	//停机故障
				if(get_controlState() != __STOP)
					set_controlState(__STOP,0);	//进入停机
			}
		}
		/************************************************************************************/
		//获取温度
		get_controlData()->temp = get_ntc_temp(ntc_adc,(uint16_t)adc_filter_TEMP,BASE_TEMP) * 0.1f;		
		
		//如果输入的密码为日期3的密码，直接彻底解锁
		if(get_controlData()->verify_password != get_controlData()->password[2]){
			//上锁处理
			lock_to_unlock();
		
			//剩余时间处理
			remain_day();
		}
		else{
			digitalLo(&get_controlData()->lock); //解锁
			digitalHi(&get_controlData()->pwd_ok);
			digitalClan(&get_controlData()->remain_day); 
		}
		
		
		if(get_controlData()->lock || get_controlData()->lock_tamp){ //上锁了
			//如果已经处于停机状态，则不用再执行
			//如果此时还在放电状态，必须要按下停机才能进入锁机，否则无法停止放电
			if(get_controlState() != __CORONA && get_controlState() != __STOP) //如果当前不是放电状态和停机状态
				set_controlState(__STOP,0);	//强制停机，进入锁机状态
			
		}
		
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



