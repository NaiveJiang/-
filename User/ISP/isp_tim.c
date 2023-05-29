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
			//д�뱸�����ݵ�BKP
			BKP_WriteBackupRegister(ACTIVITE_BKPREG,(uint16_t)(get_supervisiorData()->machine_active));
			BKP_WriteBackupRegister(CONTROL_STATE_BKPREG,get_controlState());
			BKP_WriteBackupRegister(CONTROL_STEP_BKPREG,(uint16_t)(get_controlData()->control_step));
			
			digitalClan(&get_supervisiorData()->write_bkp_time);
		}else{
			digitalIncreasing(&get_supervisiorData()->write_bkp_time);
		}
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//����ж�
}
void TIM4_IRQHandler(void){
	//CNT����ж�
	if(TIM_GetITStatus(TIM4,TIM_IT_Update) != RESET){
		//����ڲɼ�������ʱ��������������
		if(BSPSI_CALC.pulse_get) digitalIncreasing(&BSPSI_CALC.pulse_get);
		if(LSPSI_CALC.pulse_get) digitalIncreasing(&LSPSI_CALC.pulse_get);
	}
	//BSPSI�����ٶ�����
	if(TIM_GetITStatus(TIM4,TIM_IT_CC1) != RESET){
		if(BSPSI_CALC.pulse_get){
			BSPSI_CALC.get_cnt2 = TIM4->CCR1; //�õ��ڶ��������ص�CNTֵ
			//����CNT��ֵ
			BSPSI_CALC.difference_cnt = ICAP1_TIMER_PEROID * (BSPSI_CALC.pulse_get - 1) + BSPSI_CALC.get_cnt2 - BSPSI_CALC.get_cnt1;
			BSPSI_CALC.freq = ((float)(ICAP1_FREQ)) / ((float)(BSPSI_CALC.difference_cnt)); //����Ƶ��
			digitalClan(&BSPSI_CALC.pulse_get); //��ձ���
			TIM4->CCR1 = 0;
		}
		else{
			TIM4->CCR1 = 0;
			BSPSI_CALC.get_cnt1 = TIM4->CCR1; //�õ���һ�������ص�CNTֵ
			digitalHi(&BSPSI_CALC.pulse_get);
		}
	}
	//LSPSI�ⲿ�������ٶ�����
	if(TIM_GetITStatus(TIM4,TIM_IT_CC2) != RESET){
		if(LSPSI_CALC.pulse_get){
			LSPSI_CALC.get_cnt2 = TIM4->CCR2; //�õ��ڶ��������ص�CNTֵ
			//����CNT��ֵ
			LSPSI_CALC.difference_cnt = ICAP1_TIMER_PEROID * (LSPSI_CALC.pulse_get - 1) + LSPSI_CALC.get_cnt2 - LSPSI_CALC.get_cnt1;
			LSPSI_CALC.freq = ICAP1_FREQ / LSPSI_CALC.difference_cnt; //����Ƶ��
			digitalClan(&LSPSI_CALC.pulse_get); //��ձ���
			TIM4->CCR2 = 0;
		}
		else{
			TIM4->CCR2 = 0;
			LSPSI_CALC.get_cnt1 = TIM4->CCR2; //�õ���һ�������ص�CNTֵ
			digitalHi(&LSPSI_CALC.pulse_get);
		}
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2);//����ж�
}

void TIM6_IRQHandler(void){
	if(TIM_GetITStatus(TIM6,TIM_IT_Update) != RESET){
		if((adc1_settlement >= SAMP_FULL_COUNT) && (!adc1_settlement_sw)){   //50ms����
			digitalHi(&adc1_settlement_sw);  //����ADC����
		}else if((adc1_settlement < SAMP_FULL_COUNT) && (!adc1_settlement_sw)){
			//�õ�����ADC����  5msһ��
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
		if((adc3_settlement >= SAMP_FULL_COUNT) && (!adc3_settlement_sw)){   //50ms����
			digitalHi(&adc3_settlement_sw);  //����ADC����
		}else if((adc3_settlement < SAMP_FULL_COUNT) && (!adc3_settlement_sw)){
			//�õ�����ADC����  5msһ��
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
//���벶���ж�
void TIM8_CC_IRQHandler(void){
	if(TIM_GetITStatus(TIM8,TIM_IT_CC1) != RESET){
		if(F_COUN_CALC.pulse_get){
			F_COUN_CALC.get_cnt2 = TIM8->CCR1; //�õ��ڶ��������ص�CNTֵ
			//����CNT��ֵ
			F_COUN_CALC.difference_cnt = ICAP2_TIMER_PEROID * (F_COUN_CALC.pulse_get - 1) + F_COUN_CALC.get_cnt2 - F_COUN_CALC.get_cnt1;
			F_COUN_CALC.freq = ((float)(ICAP2_FREQ)) / ((float)(F_COUN_CALC.difference_cnt)); //����Ƶ��
			F_COUN_CALC_average[F_COUN_CALC.average_time] = F_COUN_CALC.freq;
			digitalClan(&F_COUN_CALC.pulse_get); //��ձ���
			digitalIncreasing(&F_COUN_CALC.average_time);
			TIM8->CCR1 = 0;
			if(F_COUN_CALC.average_time >= ICAP2_AVERAGE_TIME){
				F_COUN_CALC.freq_avg = middleAverageFilter(F_COUN_CALC_average,ICAP2_AVERAGE_TIME);  //�˲�����
				digitalClan(&F_COUN_CALC.average_time);
				digitalClan(&F_COUN_CALC.freq);
			}
		}
		else{
			TIM8->CCR1 = 0;
			F_COUN_CALC.get_cnt1 = TIM8->CCR1; //�õ���һ�������ص�CNTֵ
			digitalHi(&F_COUN_CALC.pulse_get);
		}
	}
	TIM_ClearITPendingBit(TIM8,TIM_IT_CC1);//����ж�
}

void TIM8_UP_IRQHandler(void){
	if(TIM_GetITStatus(TIM8,TIM_IT_Update) != RESET){
		//����ڲɼ�������ʱ��������������
		if(F_COUN_CALC.pulse_get) digitalIncreasing(&F_COUN_CALC.pulse_get);
	}
	TIM_ClearITPendingBit(TIM8,TIM_IT_Update);//����ж�
}
#endif

