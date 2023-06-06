#include "app.h"

appStruct_t appInput;
//测试用变量
float test_adc1[6];
float test_adc3[6];
float set_vdc;
float set_speed;
uint8_t flash_sw;
appControlStruct_t controlData;
app_pulseControlStruct_t djCtrlData;
controlState_e controlState;
void set_controlState(controlState_e setState){
	controlState = setState;
}
controlState_e get_controlState(void){
	return controlState;
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

void app_inputUpdata(void){
	driverKeyNowStateUpdate();
	dac_ch1_voltageOut(set_vdc);
	dac_ch2_voltageOut(set_speed);
	appInput.loops += INPUT_TASK_PERIOD;
}

void app_djControl(void){
	get_djCtrlData()->mode = get_djCtrlData()->sw;	//获取电极控制状态
	if(get_djCtrlData()->mode != get_djCtrlData()->last_mode){
		if(get_djCtrlData()->mode) pulse_outputHigh(&DJOPEN,100);
		else pulse_outputHigh(&DJCLOSE,100);
	}
	get_djCtrlData()->last_mode = get_djCtrlData()->mode;	//状态更新
}

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
			
			case __STOP:{
				
			}break;
		}
		
		//放电架控制
		app_djControl();
		
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
			//等待按下风机启动或电晕启动/湿启动(湿启动为独立页面)
			if(get_mainData(get_maindata()->main_rev_data,CORONA) || get_mainData(get_maindata()->main_rev_data,DRY) || get_mainData(get_maindata()->main_rev_data,FAN))
				get_controlData()->control_step = 99;
		}break;
		case 99:{
			set_controlState(__FAN_ON);		//进入风机启动模式
			digitalClan(&get_controlData()->control_step);
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
			//负压开关屏蔽解除后，产生负压，没有无负压报警则进入风机启动待机状态
			if(!FYKL && !(get_controlData()->error_sta & NEGATIVE_PRESSURE))
				digitalIncreasing(&get_controlData()->control_step);
		}break;
		case 4:{		//风机启动时的待机状态(状态1)
			//如果关闭风机,返回到0状态
			if(!get_mainData(get_maindata()->main_rev_data,FAN)){
				pulse_outputHigh(&STOPCJ3,100);		//关闭风机
				FYKL = 1; //屏蔽负压开关,互锁开关不屏蔽
				pulse_outputHigh(&STOP_P,100);	//关闭直流功率电源
				set_controlState(__STANDBY);	//回到状态0待机
				digitalClan(&get_controlData()->control_step);
			}
			//检测是否存在缺相/低压/外部交流输入过低，如果存在不允许启动电晕
			if(!(get_controlData()->error_sta & QSALARM_ERROR) && !(get_controlData()->error_sta & DCVCHK_ERROR) && !(adc_filter_VDC_ADC < LOW_VDC)){
				//如果按下了电晕启动键或者湿启动，且CJ3确定已经闭合，启动电晕
				if((get_mainData(get_maindata()->main_rev_data,CORONA) || get_mainData(get_maindata()->main_rev_data,DRY)) && CJ3OK)	
					get_controlData()->control_step = 99;
			}
		}break;
		case 99:{
			set_controlState(__CORONA);	//进入电晕模式
			digitalClan(&get_controlData()->control_step);
		}break;
			
	}
}

void app_coronaMode(void){
	switch(get_controlData()->control_step){
		case 0:{	//准备阶段
			if(adc_filter_VDC_ADC >= LOW_VDC){	//等待VDC检测到达470V,继电器闭合
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
				digitalIncreasing(&get_controlData()->control_step);
			}
			else{	//50ms后未能闭合CJ2，发出停机报警
				get_controlData()->control_step = 99;
			}
		}break;
		case 3:{
			//启动功率电源
			STOPCS = 0;		//放开STOPCS
			pulse_outputHigh(&STARTS,100);
			//使能3875输出
			
//			vTaskDelay(100);
			STANDBY = 0;	//允许3875输出
			//DAC给定为0
			dac_ch1_voltageOut(0.0f);
			dac_ch2_voltageOut(0.0f);
			digitalIncreasing(&get_controlData()->control_step);
		}break;
		case 4:{	
			if(get_mainData(get_maindata()->main_rev_data,DRY)){	//如果使能湿启动
				digitalHi(&get_controlData()->dry_mode);
				//屏蔽线控信号
				LKEN = 0;
				//湿启动放电
				app_dryMode();
			}
			//如果湿启动时按下停机键，需要停机进入风机启动待机状态(状态1)
			if(!get_mainData(get_maindata()->main_rev_data,DRY) && get_controlData()->dry_mode){
				digitalLo(&get_controlData()->dry_mode);
				//3875禁止输出
				STANDBY = 1;
				//关闭功率电源
				pulse_outputHigh(&STOP_P,100);
				//清空DAC给定
				digitalClan(&get_dryData()->dry_power);
				dac_ch1_voltageOut(0.0f);
				dac_ch2_voltageOut(0.0f);
				//跳转状态
				set_controlState(__FAN_ON);
				get_controlData()->control_step = 4;
			}
//			else{
//				//恢复线控信号
//				LKEN = 1;
//				digitalIncreasing(&get_controlData()->control_step);
//			}
		}break;
		case 5:{
			//功率放电模式
			
		}break;
		case 99:{
			
		}break;
	}
}



//启动电晕状态
void app_corona_state(void){
	switch(get_controlData()->control_step){
		case 0:{
			//只有这些错误解决了，才能进入继电器闭合
			if(!(get_controlData()->error_sta & 0xFFFFFFFE))
				digitalIncreasing(&get_controlData()->control_step);
		}break;
		case 1:{
			//等待VDC检测到达80%,继电器闭合
			if(adc_filter_VDC_ADC >= LOW_VDC){
				//闭合继电器CJ2
				pulse_outputHigh(&UPEDCJ2,50);
				digitalIncreasing(&get_controlData()->control_step);
			}
		}break;
		case 2:{
			//等待50ms，如果这期间CJ2无法闭合，则产生停机报警
			digitalHi(&get_controlData()->wait_sw);
			if(get_controlData()->wait_time >= 5){
				digitalLo(&get_controlData()->wait_sw);
				error_set(&CJ12OK,CJ12_ERROR,0);	//若CJ2无法闭合，则产生停机报警	
				if(CJ12OK){		//继电器闭合
					digitalIncreasing(&get_controlData()->control_step);
				}
				else{	//50ms后未能闭合CJ2，发出停机报警
					get_controlData()->control_step = 99;
				}
			}
		}break;
		case 3:{
			//使能3875输出
			STOPCS = 0;		//放开STOPCS
			vTaskDelay(100);
			STANDBY = 0;	//允许3875输出
			//DAC给定为0
			dac_ch1_voltageOut(0.0f);
			dac_ch2_voltageOut(0.0f);
			digitalIncreasing(&get_controlData()->control_step);
		}break;
		case 4:{
			//检测达速  大机
//			if(get_controlData()->speed_up){
//				
//			}
//			else{	//没达速就停机
//				
//			}
//			//按照控制模式放电
			//如果是小机，未达速前触发最小功率放电，达速后就按照模式选择功率放电
//			if(get_controlData()->corona_in_low_sw){
//				//未达速，最小功率放电
//				if(!get_controlData()->speed_up){
//					//放电
//				}
//				else{
//					
//				}
//			}
//			else{
//				//不放电
//				
//			}
			//如果关闭电晕
			if(!get_mainData(get_maindata()->main_rev_data,CORONA)){
				get_controlData()->control_step = 99;
			}
		}break;
		case 99:{
//			set_controlState(CORONA_STOP);		//进入停止放电状态
			digitalClan(&get_controlData()->control_step);
		}break;
	}
}

