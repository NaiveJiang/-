#ifndef __DRIVER_PULSECAP_H
#define __DRIVER_PULSECAP_H

#include "bsp.h"

//ICAP
#define USE_TEST_ICAP 1
#define USE_TIM_ETR1 0
#define ICAP1_TIMER TIM4 //ʹ��TIM4��Ϊ���벶��,100kHz����,���������0xFFFF

#define ICAP1_TIMER_PEROID 0xFFFF
#define ICAP1_TIMER_PRESCALER 720-1
#define ICAP1_PREEMPTIONPRIORITY 6
#define ICAP1_SUBPRIORITY 0
#define ICAP1_FREQ (configCPU_CLOCK_HZ/(ICAP1_TIMER_PRESCALER+1))  //CNT����Ƶ��

#if USE_TEST_ICAP
#define BSPSI BSP_GPIOD12     
#define LSPSI BSP_GPIOD13 	
#else
#define BSPSI BSP_GPIOB6 //T4CH1
#define LSPSI BSP_GPIOB7 //T4CH2	
#endif

#if USE_TIM_ETR1  //ʹ���ⲿ����ģʽ1
#define COUNT_TIM TIM8	//��ʱ��8���ڼ���
#define COUNT_TIM_PEROID 0xFFFF	//������65535
#define COUNT_TIM_PRESCALER 0x00 //�ⲿʱ�Ӳ���Ҫ��Ƶ

#define TIME_TIM TIM3	//��ʱ��3���ڼ�ʱ,��ʱ1ms,��1kHz
#define TIME_TIM_PEROID 10-1	
#define TIME_TIM_PRESCALER 7200-1
#define TIME_TIM_PREEMPTIONPRIORITY 6
#define TIME_TIM_SUBPRIORITY 0


#else	//ʹ�����벶��ʽ
#define ICAP2_TIMER TIM8	//ʹ��TIM8��Ϊ���벶��,1.5MHz����,���������0xFFFF
#define ICAP2_TIMER_PEROID 0xFFFF
#define ICAP2_TIMER_PRESCALER 48-1
#define ICAP2_PREEMPTIONPRIORITY 5
#define ICAP2_SUBPRIORITY 0
#define ICAP2_FREQ (configCPU_CLOCK_HZ/(ICAP2_TIMER_PRESCALER+1))  //CNT����Ƶ��
#define ICAP2_AVERAGE_TIME 10		//�˲�
#endif

#define F_COUN BSP_GPIOC6	//TIM8CH1 TIM_TS_TI1FP1/TIM_TS_TI1F_ED


typedef struct{
	uint32_t pulse_get;
	float freq;
	float freq_avg;
	uint16_t get_cnt1;	//��һ�β���������
	uint16_t get_cnt2;	//�ڶ��β���������
	uint32_t difference_cnt;	//CNT��ֵ
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
