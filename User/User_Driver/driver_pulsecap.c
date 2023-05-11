#include "driver.h"

//脉冲捕获保存
pulse_captureStruct BSPSI_CALC;
pulse_captureStruct LSPSI_CALC;

//输入捕获配置
BSP_TIM_ICAP_TypeDef BSPSI_ICAP = {		//PB6-------CH1
	BSPSI,
	TIM_ICPolarity_Falling,
};

BSP_TIM_ICAP_TypeDef LSPSI_ICAP = {		//PB7-------CH2
	LSPSI,
	TIM_ICPolarity_Falling,
};


void drive_pulse_capture_Init(TIM_TypeDef* ICAP_TIMx){
	//TIM ICAP初始化
	BSP_TIM_ICAP_Init(ICAP_TIMx,ICAP_TIMER_PEROID,ICAP_TIMER_PRESCALER,&BSPSI_ICAP,&LSPSI_ICAP,NULL,NULL,ICAP_PREEMPTIONPRIORITY,ICAP_SUBPRIORITY);
}


