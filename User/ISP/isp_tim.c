#include "app.h"

void TIM3_IRQHandler(void){
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET){
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}

#if !USE_TEST_ICAP
//���벶���ж�
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
			BSPSI_CALC.get_cnt2 = TIM4->CNT; //�õ��ڶ��������ص�CNTֵ
			//����CNT��ֵ
			BSPSI_CALC.difference_cnt = ICAP_TIMER_PEROID * (BSPSI_CALC.pulse_get - 1) + BSPSI_CALC.get_cnt2 - BSPSI_CALC.get_cnt1;
			BSPSI_CALC.freq = ICAP_FREQ / BSPSI_CALC.difference_cnt; //����Ƶ��
			digitalClan(&BSPSI_CALC.pulse_get); //��ձ���
		}
		else{
			BSPSI_CALC.get_cnt1 = TIM4->CNT; //�õ���һ�������ص�CNTֵ
			digitalHi(&BSPSI_CALC.pulse_get);
		}
	}
	//LSPSI�ⲿ�������ٶ�����
	if(TIM_GetITStatus(TIM4,TIM_IT_CC2) != RESET){
		if(LSPSI_CALC.pulse_get){
			LSPSI_CALC.get_cnt2 = TIM4->CNT; //�õ��ڶ��������ص�CNTֵ
			//����CNT��ֵ
			LSPSI_CALC.difference_cnt = ICAP_TIMER_PEROID * (LSPSI_CALC.pulse_get - 1) + LSPSI_CALC.get_cnt2 - LSPSI_CALC.get_cnt1;
			LSPSI_CALC.freq = ICAP_FREQ / LSPSI_CALC.difference_cnt; //����Ƶ��
			digitalClan(&LSPSI_CALC.pulse_get); //��ձ���
		}
		else{
			LSPSI_CALC.get_cnt1 = TIM4->CNT; //�õ���һ�������ص�CNTֵ
			digitalHi(&LSPSI_CALC.pulse_get);
		}
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2);//����ж�
}
#else

//���벶���ж�
void TIM2_IRQHandler(void){
	//CNT����ж�
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET){
		//����ڲɼ�������ʱ��������������
		if(BSPSI_CALC.pulse_get) digitalIncreasing(&BSPSI_CALC.pulse_get);
		if(LSPSI_CALC.pulse_get) digitalIncreasing(&LSPSI_CALC.pulse_get);
	}
	//BSPSI�����ٶ�����
	if(TIM_GetITStatus(TIM2,TIM_IT_CC1) != RESET){
		if(BSPSI_CALC.pulse_get){
			BSPSI_CALC.get_cnt2 = TIM2->CNT; //�õ��ڶ��������ص�CNTֵ
			//����CNT��ֵ
			BSPSI_CALC.difference_cnt = ICAP_TIMER_PEROID * (BSPSI_CALC.pulse_get - 1) + BSPSI_CALC.get_cnt2 - BSPSI_CALC.get_cnt1;
			BSPSI_CALC.freq = ICAP_FREQ / BSPSI_CALC.difference_cnt; //����Ƶ��
			digitalClan(&BSPSI_CALC.pulse_get); //��ձ���
		}
		else{
			BSPSI_CALC.get_cnt1 = TIM2->CNT; //�õ���һ�������ص�CNTֵ
			digitalHi(&BSPSI_CALC.pulse_get);
		}
	}
	//LSPSI�ⲿ�������ٶ�����
	if(TIM_GetITStatus(TIM2,TIM_IT_CC2) != RESET){
		if(LSPSI_CALC.pulse_get){
			LSPSI_CALC.get_cnt2 = TIM2->CNT; //�õ��ڶ��������ص�CNTֵ
			//����CNT��ֵ
			LSPSI_CALC.difference_cnt = ICAP_TIMER_PEROID * (LSPSI_CALC.pulse_get - 1) + LSPSI_CALC.get_cnt2 - LSPSI_CALC.get_cnt1;
			LSPSI_CALC.freq = ICAP_FREQ / LSPSI_CALC.difference_cnt; //����Ƶ��
			digitalClan(&LSPSI_CALC.pulse_get); //��ձ���
		}
		else{
			LSPSI_CALC.get_cnt1 = TIM2->CNT; //�õ���һ�������ص�CNTֵ
			digitalHi(&LSPSI_CALC.pulse_get);
		}
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2);//����ж�
}
#endif



