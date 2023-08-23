#include "app.h"

void TIM2_IRQHandler(void){
	
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET){
		if(get_controlData()->wait_sw){
			digitalIncreasing(&get_controlData()->wait_time);
		}
		else{
			digitalClan(&get_controlData()->wait_time);
		}
		if(get_supervisiorData()->write_bkp_time >= 10){
			//写入备份数据到BKP
			BKP_WriteBackupRegister(ACTIVITE_BKPREG,(uint16_t)(get_supervisiorData()->machine_active));
			BKP_WriteBackupRegister(CONTROL_STATE_BKPREG,get_controlState());
			BKP_WriteBackupRegister(CONTROL_STEP_BKPREG,(uint16_t)(get_controlData()->control_step));
			
			digitalClan(&get_supervisiorData()->write_bkp_time);
		}else{
			digitalIncreasing(&get_supervisiorData()->write_bkp_time);
		}
		/**********************************************************************/
		if(get_supervisiorData()->start_delay_sw){
			digitalIncreasing(&get_supervisiorData()->start_delay);	//触发计数
			if(get_supervisiorData()->start_delay >= START_DELAY_TIME)
				digitalLo(&get_supervisiorData()->start_delay_sw); //停止触发
		}
		/**********************************************************************/
		//清空相关变量
		if(!get_spdDischargeData()->remain_local_sw && !get_dischargeCtrlData()->mode){
			digitalClan(&get_spdDischargeData()->local_speed);
			digitalClan(&BSPSI_CALC.get_cnt1);
			digitalClan(&BSPSI_CALC.get_cnt2);
			digitalClan(&BSPSI_CALC.pulse_get);
			digitalClan(&BSPSI_CALC.freq);
			digitalClan(&BSPSI_CALC.difference_cnt);
		}
		if(!get_spdDischargeData()->remain_external_sw && !get_dischargeCtrlData()->mode){
			digitalClan(&get_spdDischargeData()->external_speed);
			digitalClan(&LSPSI_CALC.get_cnt1);
			digitalClan(&LSPSI_CALC.get_cnt2);
			digitalClan(&LSPSI_CALC.pulse_get);
			digitalClan(&LSPSI_CALC.freq);
			digitalClan(&LSPSI_CALC.difference_cnt);
		}
		/**********************************************************************/
		//仅在线控模式下进行清空
		if(get_spdDischargeData()->remain_local_sw && !get_dischargeCtrlData()->mode){
			digitalDecline(&get_spdDischargeData()->remain_local_time);
			if(!get_spdDischargeData()->remain_local_time)
				digitalLo(&get_spdDischargeData()->remain_local_sw);
		}
		if(get_spdDischargeData()->remain_external_sw && !get_dischargeCtrlData()->mode){
			digitalDecline(&get_spdDischargeData()->remain_external_time);
			if(!get_spdDischargeData()->remain_external_time)
				digitalLo(&get_spdDischargeData()->remain_external_sw);
		}
		/**********************************************************************/
		//触发换卷信号的延迟放电后，开始采样当前速度 m/min
		if(get_rcCtrlData()->delay_sw){
			get_rcCtrlData()->speed = get_speed(get_spdDischargeData()->speed_signal);
		}
		/**********************************************************************/
		//打火计时
		if(get_supervisiorData()->spark_tim_sw){
			digitalIncreasing(&get_supervisiorData()->spark_tim);//10ms自增
			if(get_supervisiorData()->spark_tim >= 6000){	//超过1min
				digitalClan(&get_supervisiorData()->spark_tim);	//清空时间
				digitalLo(&get_supervisiorData()->spark_tim_sw);	//结束计时
				if(get_supervisiorData()->spark_count >= 5){	//如果1min之内超过5次打火
					//触发停机报警
					digitalHi(&get_supervisiorData()->stop_alarm);
					if(get_controlState() != __STOP)
					set_controlState(__STOP,0);	//进入停机
				}
				else{
					//清空打火累计
					digitalClan(&get_supervisiorData()->spark_count);
					get_controlData()->error_sta &= ~HIGH_VOLTAGE_SPARK_ERROR;	//清空打火报警
				}
				
			}
		}
		/**********************************************************************/
		//初始化计时
		if(!get_controlData()->start_init){
			digitalIncreasing(&get_controlData()->start_time);
			if(get_controlData()->start_time >= START_INIT_TIME){
				digitalClan(&get_controlData()->start_time);
				digitalHi(&get_controlData()->start_init);	//初始化完成标志
			}
		}
		/**********************************************************************/
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//清除中断
	}
	
}

void TIM4_IRQHandler(void){
	
	//CNT溢出中断
	if(TIM_GetITStatus(TIM4,TIM_IT_Update) != RESET){
		//如果在采集到脉冲时溢出，做溢出处理
		if(BSPSI_CALC.pulse_get) digitalIncreasing(&BSPSI_CALC.pulse_get);
		if(LSPSI_CALC.pulse_get) digitalIncreasing(&LSPSI_CALC.pulse_get);
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);//清除中断
	}
	//BSPSI本地速度脉冲，低电平触发，用于达速检测和速比模式速度
	if(TIM_GetITStatus(TIM4,TIM_IT_CC1) != RESET){
		if(!get_dischargeCtrlData()->mode){ //线速模式下需要清空
			digitalHi(&get_spdDischargeData()->remain_local_sw);
			get_spdDischargeData()->remain_local_time = get_spdDischargeData()->set_remain_time;	//重新装填时间
		}
		if(BSPSI_CALC.pulse_get){
			BSPSI_CALC.get_cnt2 = TIM4->CCR1; //得到第二个上升沿的CNT值
			//计算CNT差值
			BSPSI_CALC.difference_cnt = ICAP1_TIMER_PEROID * (BSPSI_CALC.pulse_get - 1) + BSPSI_CALC.get_cnt2 - BSPSI_CALC.get_cnt1;
			BSPSI_CALC.freq = ((float)(ICAP1_FREQ)) / ((float)(BSPSI_CALC.difference_cnt)); //计算频率
			get_spdDischargeData()->local_speed = BSPSI_CALC.freq * get_spdDischargeData()->roller_pulse_length * 60.0f;	//计算得到本地滚筒速度m/min
			digitalClan(&BSPSI_CALC.pulse_get); //清空倍数
			TIM4->CCR1 = 0;
		}
		else{
			BSPSI_CALC.get_cnt1 = TIM4->CCR1; //得到第一个上升沿的CNT值
			digitalHi(&BSPSI_CALC.pulse_get);
		}
		/********************************************************************************************************************************/
		//脉冲触发模式
		if(get_dischargeCtrlData()->mode){
//			//如果达速，就退出脉冲状态
//			if(get_spdDischargeData()->local_speed > get_controlData()->set_speed_up){
//				digitalLo(&get_dischargeCtrlData()->mode);
//			}
//			else{
				//置位脉冲触发标志
				digitalHi(&get_pulseDischargeData()->discharge_sw);
				get_pulseDischargeData()->discharge_time = get_pulseDischargeData()->set_delay_time;	//重新装填时间
//			}
		}
		/********************************************************************************************************************************/
		TIM_ClearITPendingBit(TIM4,TIM_IT_CC1);//清除中断
	}
	//LSPSI外部生产线速度脉冲,用于速比模式的速度
	if(TIM_GetITStatus(TIM4,TIM_IT_CC2) != RESET){
		if(!get_dischargeCtrlData()->mode){ //线速模式下需要清空
			digitalHi(&get_spdDischargeData()->remain_external_sw);
			get_spdDischargeData()->remain_external_time = get_spdDischargeData()->set_remain_time; //重新装填时间
		}
		if(LSPSI_CALC.pulse_get){
			LSPSI_CALC.get_cnt2 = TIM4->CCR2; //得到第二个上升沿的CNT值
			//计算CNT差值
			LSPSI_CALC.difference_cnt = ICAP1_TIMER_PEROID * (LSPSI_CALC.pulse_get - 1) + LSPSI_CALC.get_cnt2 - LSPSI_CALC.get_cnt1;
			LSPSI_CALC.freq = ((float)(ICAP1_FREQ)) / ((float)(LSPSI_CALC.difference_cnt)); //计算频率
			get_spdDischargeData()->external_speed = LSPSI_CALC.freq * get_spdDischargeData()->external_pulase_length * 60.0f;	//计算得到滚筒速度
			digitalClan(&LSPSI_CALC.pulse_get); //清空倍数
			TIM4->CCR2 = 0;
		}
		else{
			TIM4->CCR2 = 0;
			LSPSI_CALC.get_cnt1 = TIM4->CCR2; //得到第一个上升沿的CNT值
			digitalHi(&LSPSI_CALC.pulse_get);
		}
		TIM_ClearITPendingBit(TIM4,TIM_IT_CC2);//清除中断
	}
	
}

void TIM5_IRQHandler(void){
	
	//CNT溢出中断
	if(TIM_GetITStatus(TIM5,TIM_IT_Update) != RESET){
		/*********************************************************************************/
		if(get_controlState() == __DRY){	//如果为湿启动
			if(get_controlData()->dry_mode){	
				if(!get_dryCtrlData()->spark_wait){	//没有打火
					if(get_dryCtrlData()->dry_time >= DRY_INCREASE_TIME){	//30s一次上升功率
						digitalClan(&get_dryCtrlData()->dry_time);
						get_dryCtrlData()->dry_power += get_dryCtrlData()->inc_power;
					}
					else{
						digitalIncreasing(&get_dryCtrlData()->dry_time);	//计数
					}
				}
			}
			else{
				digitalClan(&get_dryCtrlData()->dry_time);
			}
		}
		else if(get_controlState() == __CORONA){	//电晕
			/*********************************************************************************/
			//脉冲触发方式
			if(get_dischargeCtrlData()->mode){
				
				if(get_pulseDischargeData()->discharge_sw){	//有脉冲触发才有自减，防止重复进入
					
					if(!get_pulseDischargeData()->discharge_time){		//超过设定时间没有触发脉冲进入
						digitalLo(&get_pulseDischargeData()->discharge_sw);	//复位脉冲触发标志,停止放电
					}
					else{
						digitalDecline(&get_pulseDischargeData()->discharge_time);	//自减
					}
					
					//脉冲触发情况下,每隔100ms 2kw增加输出功率
					if(get_dischargeCtrlData()->inc_time >= INCLEASE_TIME){
						digitalClan(&get_dischargeCtrlData()->inc_time);
						get_pulseDischargeData()->discharge_power += get_pulseDischargeData()->inc_power;
						
						//如果处于手动模式，按照100ms 2kw的增速达到最大手动设置功率
						if(get_setStateData(get_powSetData()->set_state,POWERMODE) == MANUAL_MODE){
							if(!get_controlData()->line_control){
								if(get_pulseDischargeData()->discharge_power > get_dischargeCtrlData()->manual_power)
									get_pulseDischargeData()->discharge_power = get_dischargeCtrlData()->manual_power;
							}
							else{
								get_dischargeCtrlData()->line_set_power = get_line_set_power(get_dischargeCtrlData()->line_power_mode);
								if(get_pulseDischargeData()->discharge_power >= get_dischargeCtrlData()->line_set_power)
									get_pulseDischargeData()->discharge_power = get_dischargeCtrlData()->line_set_power;
							}
						}
						//其他模式按照100ms 2kw的增速达到最小设定功率
						else{
							if(get_pulseDischargeData()->discharge_power > get_dischargeCtrlData()->low_power)
								get_pulseDischargeData()->discharge_power = get_dischargeCtrlData()->low_power;
						}
						
					}
					else{
						digitalIncreasing(&get_dischargeCtrlData()->inc_time);	//时间更新
					}
				}
			}
			else{
				//手动模式下
				if(get_setStateData(get_powSetData()->set_state,POWERMODE) == MANUAL_MODE){
					if(get_dischargeCtrlData()->inc_time >= INCLEASE_TIME && get_dischargeCtrlData()->step == 1){
						digitalClan(&get_dischargeCtrlData()->inc_time);
						get_spdDischargeData()->discharge_power += get_spdDischargeData()->inc_power;	//2kw/100ms 增加
					}
					else{
						digitalIncreasing(&get_dischargeCtrlData()->inc_time);	//时间更新
					}
				}
				
				//打开延时放电
				if(get_spdDischargeData()->delay_sw)
					digitalIncreasing(&get_spdDischargeData()->delay_time);	//延时时间
				else
					digitalClan(&get_spdDischargeData()->delay_time);
				
			}
			/*********************************************************************************/
			
//			//线控下的延迟放电
//			if(get_controlData()->line_control){
//				if(get_controlData()->line_suspend_delay_sw)
//					digitalIncreasing(&get_controlData()->line_suspend_time);	//延时时间
//				else
//					digitalClan(&get_controlData()->line_suspend_time);
//			}
			
		}
		TIM_ClearITPendingBit(TIM5,TIM_IT_Update);//清除中断
	}
	
}

void TIM6_IRQHandler(void){
	
	if(TIM_GetITStatus(TIM6,TIM_IT_Update) != RESET){
		if((adc1_settlement >= SAMP_FULL_COUNT) && (!adc1_settlement_sw)){   //50ms结算
			digitalHi(&adc1_settlement_sw);  //允许ADC结算
		}else if((adc1_settlement < SAMP_FULL_COUNT) && (!adc1_settlement_sw)){
			//得到单次ADC数据  5ms一次
			getadc1_average();
			adc_full_TBI_DC3V3[adc1_settlement] = adc_TBI_DC3V3;
			adc_full_TBV_DC3V3[adc1_settlement] = adc_TBV_DC3V3;
			adc_full_POV3V3[adc1_settlement] = adc_POV3V3;
			adc_full_HI_DC3V3[adc1_settlement] = adc_HI_DC3V3;
			adc_full_LPV3V3[adc1_settlement] = adc_LPV3V3;
			adc_full_TEMP[adc1_settlement] = adc_TEMP;
			digitalIncreasing(&adc1_settlement);
		}
		TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
	}
	
}

void TIM7_IRQHandler(void){
	
	if(TIM_GetITStatus(TIM7,TIM_IT_Update) != RESET){
		if((adc3_settlement >= SAMP_FULL_COUNT) && (!adc3_settlement_sw)){   //50ms结算
			digitalHi(&adc3_settlement_sw);  //允许ADC结算
		}else if((adc3_settlement < SAMP_FULL_COUNT) && (!adc3_settlement_sw)){
			//得到单次ADC数据  5ms一次
			getadc3_average();
			adc_full_IDC_ADC[adc3_settlement] = adc_IDC_ADC;
			adc_full_VDC_ADC[adc3_settlement] = adc_VDC_ADC;
			adc_full_HV_DC3V3[adc3_settlement] = adc_HV_DC3V3;
			adc_full_LSPI3V3[adc3_settlement] = adc_LSPI3V3;
			adc_full_LSPV3V3[adc3_settlement] = adc_LSPV3V3;
			adc_full_LPIMA3V3[adc3_settlement] = adc_LPIMA3V3;
			digitalIncreasing(&adc3_settlement);
		}
		TIM_ClearITPendingBit(TIM7,TIM_IT_Update);
	}
	
}

#if !USE_TIM_ETR1 
//输入捕获中断
void TIM8_CC_IRQHandler(void){
	
	if(TIM_GetITStatus(TIM8,TIM_IT_CC1) != RESET){
		if(F_COUN_CALC.pulse_get){
			F_COUN_CALC.get_cnt2 = TIM8->CCR1; //得到第二个上升沿的CNT值
			//计算CNT差值
			F_COUN_CALC.difference_cnt = ICAP2_TIMER_PEROID * (F_COUN_CALC.pulse_get - 1) + F_COUN_CALC.get_cnt2 - F_COUN_CALC.get_cnt1;
			F_COUN_CALC.freq = ((float)(ICAP2_FREQ)) / ((float)(F_COUN_CALC.difference_cnt)); //计算频率
			F_COUN_CALC_average[F_COUN_CALC.average_time] = F_COUN_CALC.freq;
			digitalClan(&F_COUN_CALC.pulse_get); //清空倍数
			digitalIncreasing(&F_COUN_CALC.average_time);
			TIM8->CCR1 = 0;
			if(F_COUN_CALC.average_time >= ICAP2_AVERAGE_TIME){
				F_COUN_CALC.freq_avg = middleAverageFilter(F_COUN_CALC_average,ICAP2_AVERAGE_TIME);  //滤波处理
				digitalClan(&F_COUN_CALC.average_time);
				digitalClan(&F_COUN_CALC.freq);
			}
		}
		else{
			TIM8->CCR1 = 0;
			F_COUN_CALC.get_cnt1 = TIM8->CCR1; //得到第一个上升沿的CNT值
			digitalHi(&F_COUN_CALC.pulse_get);
		}
		TIM_ClearITPendingBit(TIM8,TIM_IT_CC1);//清除中断
	}
	
}

void TIM8_UP_IRQHandler(void){
	
	if(TIM_GetITStatus(TIM8,TIM_IT_Update) != RESET){
		//如果在采集到脉冲时溢出，做溢出处理
		if(F_COUN_CALC.pulse_get) digitalIncreasing(&F_COUN_CALC.pulse_get);
		TIM_ClearITPendingBit(TIM8,TIM_IT_Update);//清除中断
	}
}
#endif

