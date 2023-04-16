#ifndef __DRIVER_LINEAR_MOVEMENT_H
#define __DRIVER_LINEAR_MOVEMENT_H

#include "bsp.h"
#include "util.h"

#define SAMP_COUNT	20		/* ������������ʾ200ms�ڵĲ���������ƽ��ֵ */
#define SENSOR_LENGTH 500
#define SENSOR_SCALE 1.0f/4096*SENSOR_LENGTH

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
void drive_ADC1_Configuration(void);
void drive_ADC3_Configuration(void);
void AdcPro(void);
uint16_t GetADC(void);
void getadc1_average(__IO float *real);
void getadc3_average(__IO float *real);
void adcLowPassFilter(formatTrans32Struct_t *distence, formatTrans32Struct_t *speed);
void adcLowPassFilterInit(void);
float adcFilter(float input, struct adcFilterData *filterParameters);



#endif

