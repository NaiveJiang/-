#ifndef __DRIVER_PULSECAP_H
#define __DRIVER_PULSECAP_H

#include "bsp.h"

//ICAP
#define USE_TEST_ICAP 1
#if USE_TEST_ICAP
#define ICAP_TIMER TIM2 //使用TIM4作为输入捕获,100kHz计数,计数到最大0xFFFF
#else
#define ICAP_TIMER TIM2
#endif

#define ICAP_TIMER_PEROID 0xFFFF
#define ICAP_TIMER_PRESCALER 720-1
#define ICAP_PREEMPTIONPRIORITY 6
#define ICAP_SUBPRIORITY 0
#define ICAP_FREQ (configCPU_CLOCK_HZ/(ICAP_TIMER_PRESCALER+1))  //CNT计数频率

#if USE_TEST_ICAP
#define BSPSI BSP_GPIOA0 //T2CH1    
#define LSPSI BSP_GPIOA1 //T2CH2	
#else
#define BSPSI BSP_GPIOB6 //T4CH1    T2CH1
#define LSPSI BSP_GPIOB7 //T4CH2	T2CH2
#endif

typedef struct{
	uint32_t pulse_get;
	float freq;
	uint16_t get_cnt1;	//第一次采样上升沿
	uint16_t get_cnt2;	//第二次采样上升沿
	uint32_t difference_cnt;	//CNT差值
}pulse_captureStruct;

void drive_pulse_capture_Init(TIM_TypeDef* ICAP_TIMx);
extern pulse_captureStruct BSPSI_CALC;
extern pulse_captureStruct LSPSI_CALC;


#endif
