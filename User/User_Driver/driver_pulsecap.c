#include "driver.h"

//脉冲捕获保存
pulse_captureStruct BSPSI_CALC;
pulse_captureStruct LSPSI_CALC;
pulse_captureStruct F_COUN_CALC;

#if !USE_TIM_ETR1 
__IO float F_COUN_CALC_average[ICAP2_AVERAGE_TIME];
#endif

//输入捕获配置
BSP_TIM_ICAP_TypeDef BSPSI_ICAP = {		//PB6-------T4CH1
	BSPSI,
	TIM_ICPolarity_Falling,
};

BSP_TIM_ICAP_TypeDef LSPSI_ICAP = {		//PB7-------T4CH2
	LSPSI,
	TIM_ICPolarity_Falling,
};

BSP_TIM_ICAP_TypeDef F_COUN_ICAP = {	//PC6-------T3CH1
	F_COUN,
	TIM_ICPolarity_Falling,
};


void drive_pulse_capture_Init(void){
#if USE_TEST_ICAP
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_Pin_TO_PinAFConfig(TIM_TO_GPIO_PinRemap(ICAP1_TIMER));
#endif
	//TIM ICAP1初始化
	BSP_TIM_ICAP_Init(ICAP1_TIMER,ICAP1_TIMER_PEROID,ICAP1_TIMER_PRESCALER,&BSPSI_ICAP,&LSPSI_ICAP,NULL,NULL,ICAP1_PREEMPTIONPRIORITY,ICAP1_SUBPRIORITY);

#if !USE_TIM_ETR1
	//TIM ICAP2初始化
	BSP_TIM_ICAP_Init(ICAP2_TIMER,ICAP2_TIMER_PEROID,ICAP2_TIMER_PRESCALER,&F_COUN_ICAP,NULL,NULL,NULL,ICAP2_PREEMPTIONPRIORITY,ICAP2_SUBPRIORITY);
#else
	//初始化外部计数时钟
	
	
#endif
}


