#ifndef __DRIVER_PULSECAP_H
#define __DRIVER_PULSECAP_H

#include "bsp.h"

//ICAP
#define USE_TEST_ICAP 0
#define USE_TIM_ETR1 0
#define ICAP1_TIMER TIM4 //使用TIM4作为输入捕获,100kHz计数,计数到最大0xFFFF

#define ICAP1_TIMER_PEROID 0xFFFF
#define ICAP1_TIMER_PRESCALER 720-1
#define ICAP1_PREEMPTIONPRIORITY 5
#define ICAP1_SUBPRIORITY 0
#define ICAP1_FREQ (configCPU_CLOCK_HZ/(ICAP1_TIMER_PRESCALER+1))  //CNT计数频率

#if USE_TEST_ICAP
#define BSPSI BSP_GPIOD12     
#define LSPSI BSP_GPIOD13 	
#else
#define BSPSI BSP_GPIOB6 //T4CH1
#define LSPSI BSP_GPIOB7 //T4CH2	
#endif

#if USE_TIM_ETR1  //使用外部计数模式1
#define COUNT_TIM TIM8	//定时器8用于计数
#define COUNT_TIM_PEROID 0xFFFF	//最大计数65535
#define COUNT_TIM_PRESCALER 0x00 //外部时钟不需要分频

#define TIME_TIM TIM3	//定时器3用于计时,定时1ms,即1kHz
#define TIME_TIM_PEROID 10-1	
#define TIME_TIM_PRESCALER 7200-1
#define TIME_TIM_PREEMPTIONPRIORITY 6
#define TIME_TIM_SUBPRIORITY 0


#else	//使用输入捕获方式
#define ICAP2_TIMER TIM8	//使用TIM8作为输入捕获,1.5MHz计数,计数到最大0xFFFF
#define ICAP2_TIMER_PEROID 0xFFFF
#define ICAP2_TIMER_PRESCALER 48-1
#define ICAP2_PREEMPTIONPRIORITY 5
#define ICAP2_SUBPRIORITY 0
#define ICAP2_FREQ (configCPU_CLOCK_HZ/(ICAP2_TIMER_PRESCALER+1))  //CNT计数频率
#define ICAP2_AVERAGE_TIME 10		//滤波
#endif

#define F_COUN BSP_GPIOC6	//TIM8CH1 TIM_TS_TI1FP1/TIM_TS_TI1F_ED


typedef struct{
	uint32_t pulse_get;
	float freq;
	float freq_avg;
	uint16_t get_cnt1;	//第一次采样上升沿
	uint16_t get_cnt2;	//第二次采样上升沿
	uint32_t difference_cnt;	//CNT差值
	uint8_t average_time;
}pulse_captureStruct;

void drive_pulse_capture_Init(void);
extern pulse_captureStruct BSPSI_CALC;
extern pulse_captureStruct LSPSI_CALC;
extern pulse_captureStruct F_COUN_CALC;

#if !USE_TIM_ETR1 
extern __IO float F_COUN_CALC_average[ICAP2_AVERAGE_TIME];
#endif

#endif
