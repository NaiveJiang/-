#ifndef __BSP_RTC_H
#define __BSP_RTC_H
#include "stm32f10x.h"

//Mini STM32������
//RTCʵʱʱ�� ��������
//����ԭ��@ALIENTEK
//2010/6/6

#define RTC_PREEMPTIONPRIORITY 3
#define RTC_SUBPRIORITY 0


//ʱ��ṹ��
typedef struct {
	__IO uint8_t hour;
	__IO uint8_t min;
	__IO uint8_t sec;			
	//������������
	__IO uint16_t w_year;
	__IO uint8_t  w_month;
	__IO uint8_t  w_date;
	__IO uint8_t  week;		 
}_calendar_obj;					 
extern _calendar_obj calendar;	//�����ṹ��

extern uint8_t const mon_table[12];	//�·��������ݱ�
void Disp_Time(uint8_t x,uint8_t y,uint8_t size);//���ƶ�λ�ÿ�ʼ��ʾʱ��
void Disp_Week(uint8_t x,uint8_t y,uint8_t size,uint8_t lang);//��ָ��λ����ʾ����
uint8_t bsp_RTC_Init(void);        //��ʼ��RTC,����0,ʧ��;1,�ɹ�;
uint8_t Is_Leap_Year(uint16_t year);//ƽ��,�����ж�
uint8_t RTC_Get(void);         //����ʱ��   
uint8_t RTC_Get_Week(uint16_t year,uint8_t month,uint8_t day);
uint8_t RTC_Set(uint16_t syear,uint8_t smon,uint8_t sday,uint8_t hour,uint8_t min,uint8_t sec);//����ʱ��			 
uint8_t ALR_Set(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec);
#endif
