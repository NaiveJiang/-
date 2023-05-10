#include "app.h"

void TIM3_IRQHandler(void){
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET){
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}

#if !USE_TEST_ICAP
//输入捕获中断
void TIM4_IRQHandler(void){
	//CNT溢出中断
	if(TIM_GetITStatus(TIM4,TIM_IT_Update) != RESET){
		//如果在采集到脉冲时溢出，做溢出处理
		if(BSPSI_CALC.pulse_get) digitalIncreasing(&BSPSI_CALC.pulse_get);
		if(LSPSI_CALC.pulse_get) digitalIncreasing(&LSPSI_CALC.pulse_get);
	}
	//BSPSI本地速度脉冲
	if(TIM_GetITStatus(TIM4,TIM_IT_CC1) != RESET){
		if(BSPSI_CALC.pulse_get){
			BSPSI_CALC.get_cnt2 = TIM4->CNT; //得到第二个上升沿的CNT值
			//计算CNT差值
			BSPSI_CALC.difference_cnt = ICAP_TIMER_PEROID * (BSPSI_CALC.pulse_get - 1) + BSPSI_CALC.get_cnt2 - BSPSI_CALC.get_cnt1;
			BSPSI_CALC.freq = ICAP_FREQ / BSPSI_CALC.difference_cnt; //计算频率
			digitalClan(&BSPSI_CALC.pulse_get); //清空倍数
		}
		else{
			BSPSI_CALC.get_cnt1 = TIM4->CNT; //得到第一个上升沿的CNT值
			digitalHi(&BSPSI_CALC.pulse_get);
		}
	}
	//LSPSI外部生产线速度脉冲
	if(TIM_GetITStatus(TIM4,TIM_IT_CC2) != RESET){
		if(LSPSI_CALC.pulse_get){
			LSPSI_CALC.get_cnt2 = TIM4->CNT; //得到第二个上升沿的CNT值
			//计算CNT差值
			LSPSI_CALC.difference_cnt = ICAP_TIMER_PEROID * (LSPSI_CALC.pulse_get - 1) + LSPSI_CALC.get_cnt2 - LSPSI_CALC.get_cnt1;
			LSPSI_CALC.freq = ICAP_FREQ / LSPSI_CALC.difference_cnt; //计算频率
			digitalClan(&LSPSI_CALC.pulse_get); //清空倍数
		}
		else{
			LSPSI_CALC.get_cnt1 = TIM4->CNT; //得到第一个上升沿的CNT值
			digitalHi(&LSPSI_CALC.pulse_get);
		}
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2);//清除中断
}
#else

//输入捕获中断
void TIM2_IRQHandler(void){
	//CNT溢出中断
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET){
		//如果在采集到脉冲时溢出，做溢出处理
		if(BSPSI_CALC.pulse_get) digitalIncreasing(&BSPSI_CALC.pulse_get);
		if(LSPSI_CALC.pulse_get) digitalIncreasing(&LSPSI_CALC.pulse_get);
	}
	//BSPSI本地速度脉冲
	if(TIM_GetITStatus(TIM2,TIM_IT_CC1) != RESET){
		if(BSPSI_CALC.pulse_get){
			BSPSI_CALC.get_cnt2 = TIM2->CNT; //得到第二个上升沿的CNT值
			//计算CNT差值
			BSPSI_CALC.difference_cnt = ICAP_TIMER_PEROID * (BSPSI_CALC.pulse_get - 1) + BSPSI_CALC.get_cnt2 - BSPSI_CALC.get_cnt1;
			BSPSI_CALC.freq = ICAP_FREQ / BSPSI_CALC.difference_cnt; //计算频率
			digitalClan(&BSPSI_CALC.pulse_get); //清空倍数
		}
		else{
			BSPSI_CALC.get_cnt1 = TIM2->CNT; //得到第一个上升沿的CNT值
			digitalHi(&BSPSI_CALC.pulse_get);
		}
	}
	//LSPSI外部生产线速度脉冲
	if(TIM_GetITStatus(TIM2,TIM_IT_CC2) != RESET){
		if(LSPSI_CALC.pulse_get){
			LSPSI_CALC.get_cnt2 = TIM2->CNT; //得到第二个上升沿的CNT值
			//计算CNT差值
			LSPSI_CALC.difference_cnt = ICAP_TIMER_PEROID * (LSPSI_CALC.pulse_get - 1) + LSPSI_CALC.get_cnt2 - LSPSI_CALC.get_cnt1;
			LSPSI_CALC.freq = ICAP_FREQ / LSPSI_CALC.difference_cnt; //计算频率
			digitalClan(&LSPSI_CALC.pulse_get); //清空倍数
		}
		else{
			LSPSI_CALC.get_cnt1 = TIM2->CNT; //得到第一个上升沿的CNT值
			digitalHi(&LSPSI_CALC.pulse_get);
		}
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2);//清除中断
}
#endif



