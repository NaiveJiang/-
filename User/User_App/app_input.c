#include "app.h"

appStruct_t appInput;

appControlStruct_t controlData;
app_pulseControlStruct_t djCtrlData;
controlState_e controlState;
controlState_e last_controlState;
void set_controlState(controlState_e setState,uint8_t step){
	controlState = setState;
	get_controlData()->control_step = step;
}
controlState_e get_controlState(void){
	return controlState;
}
controlState_e get_last_controlState(void){
	return last_controlState;
}
appControlStruct_t *get_controlData(void){
	return &controlData;
}
app_pulseControlStruct_t *get_djCtrlData(void){
	return &djCtrlData;
}

void app_standby(void);
void app_fanMode(void);
void app_coronaMode(void);
void app_dryMode(void);
void app_stopMode(void);
void app_inputUpdata(void){
	driverKeyNowStateUpdate();
//	dac_ch1_voltageOut(set_vdc);
//	dac_ch2_voltageOut(set_speed);
	appInput.loops += INPUT_TASK_PERIOD;
}

//void app_djControl(void){
//	get_djCtrlData()->mode = get_djCtrlData()->sw;	//获取电极控制状态
//	if(get_djCtrlData()->mode != get_djCtrlData()->last_mode){
//		if(get_djCtrlData()->mode) pulse_outputHigh(&DJOPEN,100);
//		else pulse_outputHigh(&DJCLOSE,100);
//	}
//	get_djCtrlData()->last_mode = get_djCtrlData()->mode;	//状态更新
//}

void app_inputTask(void *Parameters){
	TickType_t xLastWakeTime = xTaskGetTickCount();
	
	while(1){
		vTaskDelayUntil(&xLastWakeTime, INPUT_TASK_PERIOD);
		//工作状态
		switch(get_controlState()){
			case __STANDBY:{		//待机0状态
				app_standby();
			}break;
			
			case __FAN_ON:{			//风机启动1状态
				app_fanMode();
			}break;
			
			case __CORONA:{
				app_coronaMode();
			}break;
			
			case __DRY:{
				app_dryMode();
			}break;
			
			case __ROLL_CHANGING:{
				app_roll_changing();
			}break;
			
			case __STOP:{
				app_stopMode();
			}break;
		}
		
//		//放电架控制
//		app_djControl();
		
		app_inputUpdata();
//		can_Send(USE_CANx,&can_id20A);
//		rs485_send_data(RS_485,sendata,RS485_TEST_LEN);
	}
}

void app_inputTaskInit(void){
	xTaskCreate(app_inputTask,"INPUT",INPUT_STACK_SIZE,NULL,INPUT_PRIORITY,&appInput.xHandleTask);
}

//待机0状态
void app_standby(void){
	switch(get_controlData()->control_step){
		case 0:{
			//存在缺相和低压不能启动风机,VDC检测在外部监控机,报警都在监控机
			if(!(get_controlData()->error_sta & QSALARM_ERROR) && !(get_controlData()->error_sta & DCVCHK_ERROR))
				digitalIncreasing(&get_controlData()->control_step);
		}break;
		case 1:{
			//等待按下风机启动或电晕启动/湿启动(湿启动为独立页面) 或者线控信号接入
			if(get_mainData(get_maindata()->main_rev_data,CORONA) || get_mainData(get_maindata()->main_rev_data,FAN) || get_controlData()->dry_mode || get_controlData()->line_control)
				set_controlState(__STANDBY,99);
		}break;
		case 99:{
			set_controlState(__FAN_ON,0);		//进入风机启动模式
		}break;
	}
}


void app_fanMode(void){
	switch(get_controlData()->control_step){
		case 0:{
			pulse_outputHigh(&O2CJ3,100);		//开启风机
			digitalHi(&get_controlData()->wait_sw);
			digitalIncreasing(&get_controlData()->control_step);
		}break;
		case 1:{
			if(get_controlData()->wait_time > 20){	//等待0.2s后检测风机是否启动
				digitalLo(&get_controlData()->wait_sw);
				digitalIncreasing(&get_controlData()->control_step);
			}
		}break;
		case 2:{
			error_set(&CJ3OK,CJ3_ERROR,0);		//若CJ3未闭合，输出错误
			//CJ3OK=1 风机启动
			if(CJ3OK){
				digitalHi(&get_controlData()->wait_sw);
				digitalIncreasing(&get_controlData()->control_step);
			}
		}break;
		case 3:{
			if(get_controlData()->wait_time > 500){	//等待5s建立负压
				digitalLo(&get_controlData()->wait_sw);
				PBLE3 = 0; 	//解除互锁开关屏蔽
				FYKL = 0;	//解除负压开关屏蔽
			}
			//如果关闭风机,返回到0状态
			if(!get_mainData(get_maindata()->main_rev_data,FAN) && !get_controlData()->line_control){
				digitalLo(&get_controlData()->wait_sw);
				FYKL = 1; //屏蔽负压开关,互锁开关不屏蔽
				pulse_outputHigh(&STOPCJ3,100);		//关闭风机
				pulse_outputHigh(&STOP_P,100);	//关闭直流功率电源
				set_controlState(__STANDBY,0);	//回到状态0待机
			}
			//负压开关屏蔽解除后，产生负压，没有无负压报警则进入风机启动待机状态
			if(!FYKL && !(get_controlData()->error_sta & NEGATIVE_PRESSURE)){
				digitalIncreasing(&get_controlData()->control_step);
			}
		}break;
		case 4:{		
			//风机启动时的待机状态(状态1)
			//速度信号更新，仅在该状态可以改变(补充)
			
			//如果关闭风机,返回到0状态
			if(!get_mainData(get_maindata()->main_rev_data,FAN) && !get_controlData()->line_control){
				FYKL = 1; //屏蔽负压开关,互锁开关不屏蔽
				pulse_outputHigh(&STOPCJ3,100);		//关闭风机
				pulse_outputHigh(&STOP_P,100);	//关闭直流功率电源
				set_controlState(__STANDBY,0);	//回到状态0待机
			}
			//检测是否存在缺相/低压/外部交流输入过低，如果存在不允许启动电晕
			if(!(get_controlData()->error_sta & QSALARM_ERROR) && !(get_controlData()->error_sta & DCVCHK_ERROR)){
				//如果按下了电晕启动键或者湿启动，且CJ3确定已经闭合，启动电晕
				if((get_mainData(get_maindata()->main_rev_data,CORONA) || get_controlData()->dry_mode || get_controlData()->line_control) && CJ3OK){	
					digitalHi(&get_supervisiorData()->start_delay_sw);	//开启计时等待VDC稳定
					digitalClan(&get_supervisiorData()->start_delay); //清空重新计数
					set_controlState(__FAN_ON,99);
				}
			}
		}break;
		case 99:{
			if(get_controlData()->dry_mode)
				set_controlState(__DRY,0);		//进入湿启动模式
			else
				set_controlState(__CORONA,0);	//进入电晕模式
		}break;
		
	}
}

void app_coronaMode(void){
	switch(get_controlData()->control_step){
		case 0:{	//准备阶段
			//启动功率电源
			STOPCS = 0;		//放开STOPCS
			pulse_outputHigh(&STARTS,100);
			if(adc_filter_VDC_ADC >= 420){	//等待VDC检测到达420V,继电器CJ2闭合
				//闭合继电器CJ2
				pulse_outputHigh(&UPEDCJ2,50);
				digitalHi(&get_controlData()->wait_sw);
				digitalIncreasing(&get_controlData()->control_step);
			}
		}break;
		case 1:{
			//等待50ms，如果这期间CJ2无法闭合，则产生停机报警
			if(get_controlData()->wait_time >= 5){
				digitalLo(&get_controlData()->wait_sw);
				digitalIncreasing(&get_controlData()->control_step);
			}
		}break;
		case 2:{
			error_set(&CJ12OK,CJ12_ERROR,0);	//若CJ2无法闭合，则产生停机报警	
			if(CJ12OK){		//继电器闭合
				if(adc_filter_VDC_ADC >= LOW_VDC) //等待VDC检测到达470V才能放电
					digitalIncreasing(&get_controlData()->control_step);
			}
			else{	//50ms后未能闭合CJ2，发出停机报警
				set_controlState(__CORONA,99);
			}
		}break;
		case 3:{
			//使能3875输出
//			vTaskDelay(100);
			//DAC给定为0
			dac_ch1_voltageOut(0.0f);
			dac_ch2_voltageOut(0.0f);
			digitalIncreasing(&get_controlData()->control_step);
		}break;
		case 4:{
			//功率放电模式
			app_discharge();
			//非线控模式下如果按下停机键或者关闭电晕，进入停机,按下停止键后会回到待机状态(状态0)，关闭电晕会回到风机启动状态（状态1）
			if((get_mainData(get_maindata()->main_rev_data,STOP_CORONA) || !get_mainData(get_maindata()->main_rev_data,CORONA)) && !get_controlData()->line_control){
				set_controlState(__CORONA,99);
			}
		}break;
		case 5:{	//生产线暂停状态
			//等待生产线重启
			if(!get_controlData()->line_suspend && get_controlData()->line_control){
				//返回放电状态
				set_controlState(__CORONA,4);
			}
			
			if(get_mainData(get_maindata()->main_rev_data,STOP_CORONA)){
				set_controlState(__CORONA,99);
			}
		}break;
		case 99:{
			//复位一些状态变量
			digitalLo(&get_controlData()->line_control);
			digitalLo(&get_controlData()->line_suspend);
			//进入停机状态
			set_controlState(__STOP,0);
		}break;
	}
}

void app_dryMode(void){
	switch(get_controlData()->control_step){
		case 0:{	//准备阶段
			//屏蔽线控信号
			LKEN = 0;
			//启动功率电源
			STOPCS = 0;		//放开STOPCS
			pulse_outputHigh(&STARTS,100);
			if(adc_filter_VDC_ADC >= 420){	//等待VDC检测到达420V,继电器CJ2闭合
				//闭合继电器CJ2
				pulse_outputHigh(&UPEDCJ2,50);
				digitalHi(&get_controlData()->wait_sw);
				digitalIncreasing(&get_controlData()->control_step);
			}
		}break;
		case 1:{
			//等待50ms，如果这期间CJ2无法闭合，则产生停机报警
			if(get_controlData()->wait_time >= 5){
				digitalLo(&get_controlData()->wait_sw);
				digitalIncreasing(&get_controlData()->control_step);
			}
		}break;
		case 2:{
			error_set(&CJ12OK,CJ12_ERROR,0);	//若CJ2无法闭合，则产生停机报警	
			if(CJ12OK){		//继电器闭合
				if(adc_filter_VDC_ADC >= LOW_VDC) //等待VDC检测到达470V才能放电
					digitalIncreasing(&get_controlData()->control_step);
			}
			else{	//50ms后未能闭合CJ2，发出停机报警
				set_controlState(__DRY,0);
			}
		}break;
		case 3:{
			//DAC给定为0
			dac_ch1_voltageOut(0.0f);
			dac_ch2_voltageOut(0.0f);
			STANDBY = 0;	//允许3875输出
			digitalIncreasing(&get_controlData()->control_step);
		}break;
		case 4:{
			if(!get_controlData()->dry_mode){	//如果关闭了湿启动
				digitalIncreasing(&get_controlData()->control_step);
			}
			else{
				//湿启动输出
				app_dryCtrl();
			}
		}break;
		case 5:{
			if(get_dischargeCtrlData()->current_power <= 10.0f){
				digitalIncreasing(&get_controlData()->control_step);
			}
			else{
				get_dryCtrlData()->dry_power *= 0.5;
				dac_ch1_voltageOut(get_dryCtrlData()->dry_power);
				dac_ch2_voltageOut(get_dryCtrlData()->dry_power);
				vTaskDelay(100);
			}
		}break;
		case 6:{
			digitalClan(&get_dryCtrlData()->dry_power);
			//清空DAC给定
			dac_ch1_voltageOut(0.0f);
			dac_ch2_voltageOut(0.0f);
			//禁止3875移相输出
			STANDBY = 1;
			//关闭功率电源
			pulse_outputHigh(&STOP_P,100);
			//恢复线控信号
			LKEN = 1;
			
			set_controlState(__DRY,99);
		}break;
		case 99:{
			set_controlState(__FAN_ON,4);
		}break;
	}
}



void app_stopMode(void){
	switch(get_controlData()->control_step){
		case 0:{
			//产生停机报警后，输出使硬件停机
			if(get_supervisiorData()->stop_alarm) ALARM = 1;
			//进入停机自动清除电晕状态
			get_maindata()->main_rev_data &= ~CORONA;
			if(get_dischargeCtrlData()->current_power <= 10.0f){
				digitalIncreasing(&get_controlData()->control_step);
			}
			else{
				if(get_dischargeCtrlData()->mode){
					//超出10kw则每隔0.1s降低50%
					get_pulseDischargeData()->discharge_power *= 0.5f;
					dac_ch1_voltageOut(get_pulseDischargeData()->discharge_power);
					dac_ch2_voltageOut(get_pulseDischargeData()->discharge_power);
				}
				else{
					//超出10kw则每隔0.1s降低50%
					get_spdDischargeData()->discharge_power *= 0.5f;
					dac_ch1_voltageOut(get_spdDischargeData()->discharge_power);
					dac_ch2_voltageOut(get_spdDischargeData()->discharge_power);
				}
				vTaskDelay(100);	//延时0.1s
			}
		}break;
		case 1:{
			//停止放电
			digitalClan(&get_pulseDischargeData()->discharge_power);
			digitalClan(&get_spdDischargeData()->discharge_power);
			digitalClan(&get_dischargeCtrlData()->step);
			dac_ch1_voltageOut(0.0f);
			dac_ch2_voltageOut(0.0f);
			
			//关闭3875移相输出
			STANDBY = 1;
			
			//关闭直流功率电源
			pulse_outputHigh(&STOP_P,100);
			
			
			digitalIncreasing(&get_controlData()->control_step);
		}break;
		case 2:{
			vTaskDelay(1000);  //延时1s检测CJ1/CJ2是否断开
			//检查CJ1/CJ2
			if(!CJ12OK){	//CJ1/CJ2正常
				if(get_supervisiorData()->stop_alarm){	//存在其他停机错误报警
					digitalIncreasing(&get_controlData()->control_step);	//进入故障停机状态
				}
				else{
					//如果没有锁机
					if(!get_controlData()->lock && !get_controlData()->lock_tamp)
						set_controlState(__STOP,99);	//退出停机模式
					else
						set_controlState(__STOP,4);//进入上锁状态
				}
			}
			else{ //CJ1/CJ2无法断开
				if(PUPOK){
					ALARM = 1;
					digitalHi(&get_supervisiorData()->stop_alarm);	//产生停机报警
					digitalIncreasing(&get_controlData()->control_step);	//进入故障停机状态
				}
			}
		}break;
		case 3:{	//等待故障复位
			//如果收到故障复位命令(屏上确认)（补充）
			STOP_P = 1; 
			if(get_controlData()->error_reset){
				digitalLo(&get_controlData()->error_reset);
				//使能故障复位
				pulse_outputHigh(&RESET_SYS,10);
				//硬件停机解除
				ALARM = 0;
				STOP_P = 0;
				//清除停机报警
				digitalLo(&get_supervisiorData()->stop_alarm);
				//清除打火报警
				get_controlData()->error_sta &= ~HIGH_VOLTAGE_SPARK_ERROR;	//清空打火报警
				get_controlData()->error_sta &= ~VDC_LOW_ERROR;	//解决后清除报警
				get_controlData()->error_sta &= ~VDC_LOW_WARN;	//解决后清除报警
				digitalClan(&get_supervisiorData()->spark_count);
				digitalClan(&get_supervisiorData()->spark_last_count);
				
				if(!get_controlData()->lock && !get_controlData()->lock_tamp)	//没有锁机
					set_controlState(__STOP,99);
				else
					digitalIncreasing(&get_controlData()->control_step); //锁机状态
			}
		}break;
		case 4:{	//锁机状态
			if(!get_controlData()->lock && !get_controlData()->lock_tamp){	//解锁
				//如果存在停机故障，需要返回故障复位
				if(get_supervisiorData()->stop_alarm)
					set_controlState(__STOP,3);
				else
					set_controlState(__STOP,99);	//进入待机
			}
		}break;
		case 99:{
			//停机清除速度
			
			//根据CJ3回到相关状态
			if(CJ3OK)	//风机还在运行，回到状态1
				set_controlState(__FAN_ON,4);
			else		//风机停机进入状态0
				set_controlState(__STANDBY,0);
		}break;
	}
}



