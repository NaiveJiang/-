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
		
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//清除中断
}

void TIM4_IRQHandler(void){
	
	//CNT溢出中断
	if(TIM_GetITStatus(TIM4,TIM_IT_Update) != RESET){
		//如果在采集到脉冲时溢出，做溢出处理
		if(BSPSI_CALC.pulse_get) digitalIncreasing(&BSPSI_CALC.pulse_get);
		if(LSPSI_CALC.pulse_get) digitalIncreasing(&LSPSI_CALC.pulse_get);
	}
	//BSPSI本地速度脉冲，低电平触发，用于达速检测和速比模式速度
	if(TIM_GetITStatus(TIM4,TIM_IT_CC1) != RESET){
		if(BSPSI_CALC.pulse_get){
			BSPSI_CALC.get_cnt2 = TIM4->CCR1; //得到第二个上升沿的CNT值
			//计算CNT差值
			BSPSI_CALC.difference_cnt = ICAP1_TIMER_PEROID * (BSPSI_CALC.pulse_get - 1) + BSPSI_CALC.get_cnt2 - BSPSI_CALC.get_cnt1;
			BSPSI_CALC.freq = ((float)(ICAP1_FREQ)) / ((float)(BSPSI_CALC.difference_cnt)); //计算频率
			get_dischargeCtrlData()->spdCtrl->local_speed = BSPSI_CALC.freq * get_dischargeCtrlData()->spdCtrl->roller_pulse_length * 60.0f;	//计算得到本地滚筒速度m/min
			digitalClan(&BSPSI_CALC.pulse_get); //清空倍数
			TIM4->CCR1 = 0;
		}
		else{
			TIM4->CCR1 = 0;
			BSPSI_CALC.get_cnt1 = TIM4->CCR1; //得到第一个上升沿的CNT值
			digitalHi(&BSPSI_CALC.pulse_get);
		}
		/********************************************************************************************************************************/
		//脉冲触发模式
		if(get_dischargeCtrlData()->mode){
			//置位脉冲触发标志
			digitalHi(&get_dischargeCtrlData()->pulseCtrl->discharge_sw);
			get_dischargeCtrlData()->pulseCtrl->discharge_time = get_dischargeCtrlData()->pulseCtrl->set_delay_time;	//重新装填时间
		}
		/********************************************************************************************************************************/
	}
	//LSPSI外部生产线速度脉冲,用于速比模式的速度
	if(TIM_GetITStatus(TIM4,TIM_IT_CC2) != RESET){
		if(LSPSI_CALC.pulse_get){
			LSPSI_CALC.get_cnt2 = TIM4->CCR2; //得到第二个上升沿的CNT值
			//计算CNT差值
			LSPSI_CALC.difference_cnt = ICAP1_TIMER_PEROID * (LSPSI_CALC.pulse_get - 1) + LSPSI_CALC.get_cnt2 - LSPSI_CALC.get_cnt1;
			LSPSI_CALC.freq = ((float)(ICAP1_FREQ)) / ((float)(LSPSI_CALC.difference_cnt)); //计算频率
			get_dischargeCtrlData()->spdCtrl->external_speed = LSPSI_CALC.freq * get_dischargeCtrlData()->spdCtrl->roller_pulse_length * 60.0f;	//计算得到滚筒速度
			digitalClan(&LSPSI_CALC.pulse_get); //清空倍数
			TIM4->CCR2 = 0;
		}
		else{
			TIM4->CCR2 = 0;
			LSPSI_CALC.get_cnt1 = TIM4->CCR2; //得到第一个上升沿的CNT值
			digitalHi(&LSPSI_CALC.pulse_get);
		}
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2);//清除中断
}

void TIM5_IRQHandler(void){
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);//清除中断
	//CNT溢出中断
	if(TIM_GetITStatus(TIM5,TIM_IT_Update) != RESET){
		if(get_controlState() == __CORONA){
			/*********************************************************************************/
			if(get_controlData()->dry_mode){	//如果为湿启动
				if(!get_dryCtrlData()->spark_wait){	//没有打火
					if(get_dryCtrlData()->dry_time >= DRY_INCREASE_TIME){	//1s
						digitalClan(&get_dryCtrlData()->dry_time);
						get_dryCtrlData()->dry_power += get_dryCtrlData()->inc_power;
						
					}
					else{
						digitalIncreasing(&get_dryCtrlData()->dry_time);
					}
					
					//功率限幅
					if(get_dryCtrlData()->dry_power > get_dischargeCtrlData()->manual_power){
						get_dryCtrlData()->dry_power = get_dischargeCtrlData()->manual_power;
					}
					
					if(get_dryCtrlData()->spark_wait) digitalIncreasing(&get_dryCtrlData()->spark_wait_time);
					if(get_dryCtrlData()->spark_wait_time >= SPARK_WAIT_TIME){	//等待0.2s
						digitalLo(&get_dryCtrlData()->spark_wait);
						digitalClan(&get_dryCtrlData()->spark_wait_time);
					}
				}
			}
			/*********************************************************************************/
			//脉冲触发方式
			if(get_dischargeCtrlData()->mode){
				
				//打开延时放电
				if(get_dischargeCtrlData()->spdCtrl->delay_sw)
					digitalIncreasing(&get_dischargeCtrlData()->spdCtrl->delay_time);	//延时时间
				else
					digitalClan(&get_dischargeCtrlData()->spdCtrl->delay_time);
				
				if(get_dischargeCtrlData()->pulseCtrl->discharge_sw){	//有脉冲触发才有自减，防止重复进入
					
					if(!get_dischargeCtrlData()->pulseCtrl->discharge_time){		//超过设定时间没有触发脉冲进入
						digitalLo(&get_dischargeCtrlData()->pulseCtrl->discharge_sw);	//复位脉冲触发标志,停止放电
					}
					else{
						digitalDecline(&get_dischargeCtrlData()->pulseCtrl->discharge_time);	//自减
					}
					
					//脉冲触发情况下,每隔100ms 2kw增加输出功率
					if(get_dischargeCtrlData()->inc_time >= INCLEASE_TIME){
						digitalClan(&get_dischargeCtrlData()->inc_time);
						get_dischargeCtrlData()->pulseCtrl->discharge_power += get_dischargeCtrlData()->pulseCtrl->inc_power;
						
						//如果处于手动模式，按照100ms 2kw的增速达到最大手动设置功率
						if(get_ctrlSetData(get_ctrlSetdata()->ctrlSet_rev_data,POWERMODE) == MANUAL_MODE){
							if(!get_controlData()->line_control){
								if(get_dischargeCtrlData()->pulseCtrl->discharge_power > get_dischargeCtrlData()->manual_power)
									get_dischargeCtrlData()->pulseCtrl->discharge_power = get_dischargeCtrlData()->manual_power;
							}
							else{
								get_dischargeCtrlData()->line_set_power = get_line_set_power(get_dischargeCtrlData()->line_power_mode);
								if(get_dischargeCtrlData()->pulseCtrl->discharge_power >= get_dischargeCtrlData()->line_set_power)
									get_dischargeCtrlData()->pulseCtrl->discharge_power = get_dischargeCtrlData()->line_set_power;
							}
						}
						//其他模式按照100ms 2kw的增速达到最小设定功率
						else{
							if(get_dischargeCtrlData()->pulseCtrl->discharge_power > get_dischargeCtrlData()->low_power)
								get_dischargeCtrlData()->pulseCtrl->discharge_power = get_dischargeCtrlData()->low_power;
						}
						
					}
					else{
						digitalIncreasing(&get_dischargeCtrlData()->inc_time);	//时间更新
					}
				}
			}
			else{
				//手动模式下
				if(get_ctrlSetData(get_ctrlSetdata()->ctrlSet_rev_data,POWERMODE) == MANUAL_MODE){
					if(get_dischargeCtrlData()->inc_time >= INCLEASE_TIME){
						digitalClan(&get_dischargeCtrlData()->inc_time);
						get_dischargeCtrlData()->spdCtrl->discharge_power += get_dischargeCtrlData()->spdCtrl->inc_power;	//2kw/100ms 增加
					}
					else{
						digitalIncreasing(&get_dischargeCtrlData()->inc_time);	//时间更新
					}
				}
				
			}
			/*********************************************************************************/
			//线控下的延迟放电
			if(get_controlData()->line_control){
				if(get_controlData()->line_suspend_delay_sw)
					digitalIncreasing(&get_controlData()->line_suspend_time);	//延时时间
				else
					digitalClan(&get_controlData()->line_suspend_time);
			}
			
		}
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
	}
	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
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
	}
	TIM_ClearITPendingBit(TIM7,TIM_IT_Update);
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
	}
	TIM_ClearITPendingBit(TIM8,TIM_IT_CC1);//清除中断
}

void TIM8_UP_IRQHandler(void){
	
	if(TIM_GetITStatus(TIM8,TIM_IT_Update) != RESET){
		//如果在采集到脉冲时溢出，做溢出处理
		if(F_COUN_CALC.pulse_get) digitalIncreasing(&F_COUN_CALC.pulse_get);
	}
	TIM_ClearITPendingBit(TIM8,TIM_IT_Update);//清除中断
}
#endif

