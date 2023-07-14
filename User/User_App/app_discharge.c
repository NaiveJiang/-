#include "app.h"

dischargeStruct_t dischargeCtrl;
pulse_dischargeStruct_t pulseCtrl;
spd_dischargeStruct_t spdCtrl;
dischargeStruct_t* get_dischargeCtrlData(void){
	return &dischargeCtrl;
}
pulse_dischargeStruct_t* get_pulseDischargeData(void){
	return &pulseCtrl;
}
spd_dischargeStruct_t* get_spdDischargeData(void){
	return &spdCtrl;
}
void pulse_dischargeUpdate(void);
void spd_dischargeUpdate(void);
float get_speed(uint8_t speedMode);
float get_line_set_power(uint8_t line_power_mode);
void spd_discharge_delay(void);

void app_discharge(void){
	switch(get_dischargeCtrlData()->mode){
		//线速模式
		case 0:{
			switch(get_dischargeCtrlData()->step){
				case 0:{		//状态4，线速比较达速
					//检测达速，未达速不放电
					if(!(get_controlData()->error_sta & SPEED_UP_ERROR)){
						//使能3875移相输出
						STANDBY = 0;
						digitalIncreasing(&get_dischargeCtrlData()->step);
					}
				}break;
				case 1:{
					//如果速度降低到达速以下，延迟放电(22状态),或者触发线控暂停(线控状态无关达速与否，直接开始延时)  手动达速下只能通过停止电晕来停止放电
					if((get_controlData()->error_sta & SPEED_UP_ERROR || get_controlData()->line_suspend) && !get_rcCtrlData()->state){  //没有触发换卷信号
						if(!get_controlData()->use_pulse_corona){			//不用脉冲放电模式
							digitalHi(&get_spdDischargeData()->delay_sw);	//开启延时触发
							digitalIncreasing(&get_dischargeCtrlData()->step);
						}
						else{
							//回到脉冲触发模式
							digitalHi(&get_dischargeCtrlData()->mode);
							digitalClan(&get_dischargeCtrlData()->step);
						}
					}
					//正常线速放电
					else{
						spd_dischargeUpdate();
					}
				}break;
				case 2:{	//状态22
					//如果是线控状态下重新达速还要确认LRUN是否重新启动,才跳转到step1
					if(get_controlData()->line_control){
						if(get_controlData()->line_suspend){	//线控暂停必须暂停放电，无论是否达速
							if(get_spdDischargeData()->delay_time >= get_spdDischargeData()->set_line_delay_time){
								digitalIncreasing(&get_dischargeCtrlData()->step);
								digitalLo(&get_spdDischargeData()->delay_sw);	//关闭延时触发
							}
						}
						else{		//没有线控暂停时，可能是未达速引起的延时放电
							if(!(get_controlData()->error_sta & SPEED_UP_ERROR)){  //达速
								digitalLo(&get_spdDischargeData()->delay_sw);	//关闭延时触发
								digitalClan(&get_spdDischargeData()->delay_time);	//清除延时时间
								get_dischargeCtrlData()->step = 1;	//状态跳转
							}
							else{	//未达速延时放电
								if(get_spdDischargeData()->delay_time >= get_spdDischargeData()->set_line_delay_time){
									digitalIncreasing(&get_dischargeCtrlData()->step);
									digitalLo(&get_spdDischargeData()->delay_sw);	//关闭延时触发
								}
							}
						}
					}
					else{	//非线控状态下
						//如果此时又达速,非线控状态下回到step1
						if(!(get_controlData()->error_sta & SPEED_UP_ERROR) && !get_controlData()->line_control){
							digitalLo(&get_spdDischargeData()->delay_sw);	//关闭延时触发
							digitalClan(&get_spdDischargeData()->delay_time);	//清除延时时间
							get_dischargeCtrlData()->step = 1;	//状态跳转
						}
						else{
							//延时放电
							if(get_spdDischargeData()->delay_time >= get_spdDischargeData()->set_delay_time){
								digitalIncreasing(&get_dischargeCtrlData()->step);
								digitalLo(&get_spdDischargeData()->delay_sw);	//关闭延时触发
							}
						}
					}
					if(get_spdDischargeData()->delay_sw)
						spd_discharge_delay();	//延迟放电操作
				}break;
				case 3:{
					//如果当前输出功率小于10kw，可以直接关闭电晕
					if(get_dischargeCtrlData()->current_power <= 10.0f){
						digitalIncreasing(&get_dischargeCtrlData()->step);
					}
					else{	//超出10kw则每隔0.1s降低50%
						get_spdDischargeData()->discharge_power *= 0.5f;
						dac_ch1_voltageOut(get_spdDischargeData()->discharge_power);
						dac_ch2_voltageOut(get_spdDischargeData()->discharge_power);
						vTaskDelay(100);	//延时0.1s
					}
				}break;
				case 4:{
					//停止dac输出
					digitalClan(&get_spdDischargeData()->discharge_power);
					dac_ch1_voltageOut(get_spdDischargeData()->discharge_power);
					dac_ch2_voltageOut(get_spdDischargeData()->discharge_power);
					
					//关闭3875移相输出
					STANDBY = 1;
					
					digitalClan(&get_dischargeCtrlData()->step); //回到线速比较达速
					//如果为线控状态，需要等待生产线重启
					//进入暂停状态
					if(get_controlData()->line_suspend)
						set_controlState(__CORONA,5);
				}break;
			}
			
		}break;
		//脉冲触发模式
		case 1:{
			//未达速按照脉冲触发
			if(get_controlData()->error_sta & SPEED_UP_ERROR){
				pulse_dischargeUpdate();	//脉冲放电
			}
			else{	//达速进入线速控制(手动达速自动进入线速状态)
				digitalLo(&get_dischargeCtrlData()->mode);
				digitalClan(&get_dischargeCtrlData()->step);
			}
		}break;
		default:break;
	}
}


//脉冲触发放电
void pulse_dischargeUpdate(void){
	//计算功率增量的电压
	get_pulseDischargeData()->inc_power = INCREASE_DISCHARGE_MAX100MS * SAMP_MAX_VOLTAGE / get_controlData()->rated_power;
	//脉冲触发标志触发，放电
	if(!get_pulseDischargeData()->discharge_sw){
		//清空增量时间
		digitalClan(&get_dischargeCtrlData()->inc_time);
		//如果当前输出功率小于10kw，可以直接关闭电晕
		if(get_dischargeCtrlData()->current_power <= 10.0f){
			
			//停止放电
			digitalClan(&get_pulseDischargeData()->discharge_power);
			dac_ch1_voltageOut(get_pulseDischargeData()->discharge_power);
			dac_ch2_voltageOut(get_pulseDischargeData()->discharge_power);
			
			//关闭3875移相输出
			STANDBY = 1;
			
			//如果为线控状态，需要等待生产线重启
			//进入暂停状态
			if(get_controlData()->line_suspend)
				set_controlState(__CORONA,5);	
		}
		else{	//超出10kw则每隔0.1s降低50%
			get_pulseDischargeData()->discharge_power *= 0.5f;
			dac_ch1_voltageOut(get_pulseDischargeData()->discharge_power);
			dac_ch2_voltageOut(get_pulseDischargeData()->discharge_power);
			vTaskDelay(100);	//延时0.1s
		}
	}
	else{
		//使能3875移相输出
		STANDBY = 0;
		//输出
		dac_ch1_voltageOut(get_pulseDischargeData()->discharge_power);
		dac_ch2_voltageOut(get_pulseDischargeData()->discharge_power);
	}
}


void spd_dischargeUpdate(void){
	//功率控制模式更新
	get_dischargeCtrlData()->power_ctrlState = get_ctrlSetData(get_ctrlSetdata()->ctrlSet_rev_data,POWERMODE);
	//根据功率控制模式工作
	switch(get_dischargeCtrlData()->power_ctrlState){
		case MANUAL_MODE:{	//手动模式，以100ms 2kw速率增加到最大功率
			//计算功率增量的电压
			get_spdDischargeData()->inc_power = INCREASE_DISCHARGE_MAX100MS * SAMP_MAX_VOLTAGE / get_controlData()->rated_power;
			
			//功率限幅
			if(!get_controlData()->line_control){		//非线控模式下使用屏幕设置的手动功率dac限幅
				if(get_spdDischargeData()->discharge_power >= get_dischargeCtrlData()->manual_power){
					get_spdDischargeData()->discharge_power = get_dischargeCtrlData()->manual_power;
				}
			}
			else{	//线控模式下使用生产线给定功率dac输出限幅 = 生产线功率给定adc采样(百分比) * 额定功率(满偏电压)
				get_dischargeCtrlData()->line_set_power = get_line_set_power(get_dischargeCtrlData()->line_power_mode);
				if(get_spdDischargeData()->discharge_power >= get_dischargeCtrlData()->line_set_power){
					get_spdDischargeData()->discharge_power = get_dischargeCtrlData()->line_set_power;
				}
			}
			
		}break;
		
		case POWER_DENSITY_MODE:{	//功率密度模式
			//根据速度模式选择获取当前线速度	本地脉冲/外部脉冲/生产线电压/生产线电流
			get_spdDischargeData()->speed = get_speed(get_spdDischargeData()->speed_signal);
			//计算输出功率=给定功率密度*当前线速度*滚轴宽度
			get_spdDischargeData()->power_density = (float)get_powSetData()->power_density;	//获取给定功率密度
			get_spdDischargeData()->discharge_power = get_spdDischargeData()->power_density * get_spdDischargeData()->speed * get_spdDischargeData()->roller_width; //计算得到W/m2
			//输出功率转为dac输出
			get_spdDischargeData()->discharge_power *= SAMP_MAX_VOLTAGE / (get_controlData()->rated_power * 1000);	//kw换算成w所以要/1000
		}break;
		
		case SPEED_MODE:{	//速比模式
			//根据速度模式选择获取当前线速度	本地脉冲/外部脉冲/生产线电压/生产线电流
			get_spdDischargeData()->speed = get_speed(get_spdDischargeData()->speed_signal);
			//计算比例系数
			get_spdDischargeData()->scale = get_spdDischargeData()->spd_max_pow / get_spdDischargeData()->max_spd;
			//根据比例系数计算出当前的输出功率
			get_spdDischargeData()->discharge_power = get_spdDischargeData()->scale * get_spdDischargeData()->speed;
			//如果当前速度下算出的功率小于最小输出功率，则按照最小输出功率输出
			if(get_spdDischargeData()->discharge_power < get_dischargeCtrlData()->low_power)
				get_spdDischargeData()->discharge_power = get_dischargeCtrlData()->low_power;
			if(get_spdDischargeData()->discharge_power > get_spdDischargeData()->spd_max_pow)
				get_spdDischargeData()->discharge_power = get_spdDischargeData()->spd_max_pow;
			//输出功率转为dac输出
			get_spdDischargeData()->discharge_power *= SAMP_MAX_VOLTAGE / get_controlData()->rated_power;
		}break;
	}
	
	//输出
	dac_ch1_voltageOut(get_spdDischargeData()->discharge_power);
	dac_ch2_voltageOut(get_spdDischargeData()->discharge_power);
}

//线速延迟放电操作
void spd_discharge_delay(void){
	//功率控制模式更新
	get_dischargeCtrlData()->power_ctrlState = get_ctrlSetData(get_ctrlSetdata()->ctrlSet_rev_data,POWERMODE);
	//根据功率控制模式工作
	switch(get_dischargeCtrlData()->power_ctrlState){
		case POWER_DENSITY_MODE:{	//功率密度模式下如果输出小于最小功率按照最小功率放电
			//根据速度模式选择获取当前线速度	本地脉冲/外部脉冲/生产线电压/生产线电流
			get_spdDischargeData()->speed = get_speed(get_spdDischargeData()->speed_signal);
			//计算输出功率=给定功率密度*当前线速度*滚轴宽度
			get_spdDischargeData()->power_density = (float)get_powSetData()->power_density;	//获取给定功率密度
			get_spdDischargeData()->discharge_power = get_spdDischargeData()->power_density * get_spdDischargeData()->speed * get_spdDischargeData()->roller_width;
			if(get_spdDischargeData()->discharge_power < get_dischargeCtrlData()->low_power)
				get_spdDischargeData()->discharge_power = get_dischargeCtrlData()->low_power;
			//输出功率转为dac输出
			get_spdDischargeData()->discharge_power *= SAMP_MAX_VOLTAGE / get_controlData()->rated_power;
		}break;
		case SPEED_MODE:{
			//根据速度模式选择获取当前线速度	本地脉冲/外部脉冲/生产线电压/生产线电流
			get_spdDischargeData()->speed = get_speed(get_spdDischargeData()->speed_signal);
			//计算比例系数
			get_spdDischargeData()->scale = get_spdDischargeData()->spd_max_pow / get_spdDischargeData()->max_spd;			
			//根据比例系数计算出当前的输出功率
			get_spdDischargeData()->discharge_power = get_spdDischargeData()->scale * get_spdDischargeData()->speed;
			//如果当前速度下算出的功率小于最小输出功率，则按照最小输出功率输出
			if(get_spdDischargeData()->discharge_power < get_spdDischargeData()->spd_min_pow)
				get_spdDischargeData()->discharge_power = get_spdDischargeData()->spd_min_pow;
			if(get_spdDischargeData()->discharge_power > get_spdDischargeData()->spd_max_pow)
				get_spdDischargeData()->discharge_power = get_spdDischargeData()->spd_max_pow;
			//输出功率转为dac输出
			get_spdDischargeData()->discharge_power *= SAMP_MAX_VOLTAGE / get_controlData()->rated_power;
		}break;
		default: break; //手动模式保持原功率
	}
	//输出
	dac_ch1_voltageOut(get_spdDischargeData()->discharge_power);
	dac_ch2_voltageOut(get_spdDischargeData()->discharge_power);
}


//速度信号选择
float get_speed(uint8_t speedMode){
	float speed;
	switch(speedMode){
		case 0:{		//本地脉冲
			speed = get_spdDischargeData()->local_speed;
		}break;
		case 1:{		//外部脉冲
			speed = get_spdDischargeData()->external_speed;
		}break;
		case 2:{		//生产线电压
			//计算当前速度=生产线电压百分比 * 最大设定线速
			get_spdDischargeData()->line_voltage_speed = adc_filter_LSPV3V3 * get_controlData()->set_spd_max;
			speed = get_spdDischargeData()->line_voltage_speed;
		}break;
		case 3:{		//生产线电流
			//计算当前速度=生产线电流百分比 * 最大设定线速
			get_spdDischargeData()->line_current_speed = adc_filter_LSPI3V3 * get_controlData()->set_spd_max;
			speed = get_spdDischargeData()->line_current_speed;
		}break;
		default:break;
	}
	return speed;
}

float get_line_set_power(uint8_t line_power_mode){
	float power;
	switch(line_power_mode){
		case 0:{
			power = adc_filter_LPV3V3 * SAMP_MAX_VOLTAGE;
		}break;
		case 1:{
			power = adc_filter_LPIMA3V3 * SAMP_MAX_VOLTAGE;
		}break;
	}
	return power;
}

