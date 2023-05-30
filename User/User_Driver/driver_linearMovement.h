#ifndef __DRIVER_LINEAR_MOVEMENT_H
#define __DRIVER_LINEAR_MOVEMENT_H

#include "bsp.h"
#include "util.h"

#define SAMP_COUNT	20		/* ������������ʾ200ms�ڵĲ���������ƽ��ֵ */
#define SENSOR_LENGTH 500
#define SENSOR_SCALE 1.0f/4096*SENSOR_LENGTH
#define SAMP_MAX_ADCVAL 3722.727272727272f		//��ƫ3V��ӦADCֵ

//������ʱ�� 5ms
#define SAMP_TIMER1 TIM6
#define SAMP_TIMER2 TIM7

#define SAMP_TIMER_PEROID 3600-1
#define SAMP_TIMER_PRESCALER 100-1
#define SAMP_TIMER_PREEMPTIONPRIORITY 3
#define SAMP_TIMER_SUBPRIORITY 0
#define SAMP_FULL_COUNT 10 //�����ܴ���

//PC��ADC
#define HV_DC3V3 BSP_GPIOC0     //��ѹ�ŵ��ѹ
#define LSPI3V3 BSP_GPIOC1		//�����߿���������ٶȵ�������
#define LSPV3V3 BSP_GPIOC2		//�����߿���������ٶȵ�ѹ����
#define LPIMA3V3 BSP_GPIOC3		//�����߿���������ʵ�������
#define LPV3V3 BSP_GPIOC4		//�����߿���������ʵ�ѹ����
#define TEMP BSP_GPIOC5			//ɢ�����¶ȼ������

//PA��ADC
#define IDC_ADC BSP_GPIOA0      //IDC�������
#define VDC_ADC BSP_GPIOA1      //VDC��ѹ���
#define TBI_DC3V3 BSP_GPIOA6   //��ѹ��ԭ�ߵ���
#define TBV_DC3V3 BSP_GPIOA7   //��ѹ��ԭ�ߵ�ѹ

//PB��ADC
#define POV3V3 BSP_GPIOB0 //�������AD3.3V���
#define HI_DC3V3 BSP_GPIOB1 //��ѹ�ŵ�������



#define ADC_1             0
#define ADC_2             1
#define ADC_3             2
#define ADC_4             3

#define SPEED_1             0
#define SPEED_2             1
#define SPEED_3             2
#define SPEED_4             3


typedef struct adcFilterData
{
    float   gx1;
    float   gx2;
    float   gx3;
    float   previousInput;
    float   previousOutput;
}adcFilterData_t;

extern __IO uint16_t adc1_raw_value[10][6]; //�ɼ���ADCֵ
extern __IO uint16_t adc3_raw_value[10][6]; //�ɼ���ADCֵ
extern __IO float average_dis[4];  //����ƽ��ֵ

/*******************ADC1*******************/
extern uint8_t adc1_settlement_sw;
extern __IO uint8_t adc1_settlement;

extern __IO float adc_TBI_DC3V3;//��ѹ��ԭ�ߵ���
extern __IO float adc_TBV_DC3V3;//��ѹ��ԭ�ߵ�ѹ
extern __IO float adc_POV3V3;//�������AD3.3V
extern __IO float adc_HI_DC3V3;//��ѹ�ŵ����
extern __IO float adc_LPV3V3;//�����߿���������ʵ�ѹ����
extern __IO float adc_TEMP;//ɢ�����¶ȼ������

extern __IO float adc_full_TBI_DC3V3[SAMP_FULL_COUNT];
extern __IO float adc_full_TBV_DC3V3[SAMP_FULL_COUNT];
extern __IO float adc_full_POV3V3[SAMP_FULL_COUNT];
extern __IO float adc_full_HI_DC3V3[SAMP_FULL_COUNT];
extern __IO float adc_full_LPV3V3[SAMP_FULL_COUNT];
extern __IO float adc_full_TEMP[SAMP_FULL_COUNT];

extern __IO float adc_filter_TBI_DC3V3;
extern __IO float adc_filter_TBV_DC3V3;
extern __IO float adc_filter_POV3V3;
extern __IO float adc_filter_HI_DC3V3;
extern __IO float adc_filter_LPV3V3;
extern __IO float adc_filter_TEMP;
/*******************ADC3*******************/
extern uint8_t adc3_settlement_sw;
extern __IO uint8_t adc3_settlement;

extern __IO float adc_IDC_ADC;//��ѹ��ԭ�ߵ���
extern __IO float adc_VDC_ADC;//��ѹ��ԭ�ߵ�ѹ
extern __IO float adc_HV_DC3V3;//�������AD3.3V
extern __IO float adc_LSPI3V3;//��ѹ�ŵ����
extern __IO float adc_LSPV3V3;//�����߿���������ʵ�ѹ����
extern __IO float adc_LPIMA3V3;//ɢ�����¶ȼ������

extern __IO float adc_full_IDC_ADC[SAMP_FULL_COUNT];
extern __IO float adc_full_VDC_ADC[SAMP_FULL_COUNT];
extern __IO float adc_full_HV_DC3V3[SAMP_FULL_COUNT];
extern __IO float adc_full_LSPI3V3[SAMP_FULL_COUNT];
extern __IO float adc_full_LSPV3V3[SAMP_FULL_COUNT];
extern __IO float adc_full_LPIMA3V3[SAMP_FULL_COUNT];

extern __IO float adc_filter_IDC_ADC;
extern __IO float adc_filter_VDC_ADC;
extern __IO float adc_filter_HV_DC3V3;
extern __IO float adc_filter_LSPI3V3;
extern __IO float adc_filter_LSPV3V3;
extern __IO float adc_filter_LPIMA3V3;

void drive_ADC1_Configuration(void);
void drive_ADC3_Configuration(void);
void AdcPro(void);
uint16_t GetADC(void);
void getadc1_average(void);
void getadc3_average(void);
void adcLowPassFilter(formatTrans32Struct_t *distence, formatTrans32Struct_t *speed);
void adcLowPassFilterInit(void);
float adcFilter(float input, struct adcFilterData *filterParameters);
void drive_ADC_sampling_TIM_init(TIM_TypeDef* SAMP_TIMx);


#endif

