#ifndef __DRIVER_LINEAR_MOVEMENT_H
#define __DRIVER_LINEAR_MOVEMENT_H

#include "bsp.h"
#include "util.h"

#define SAMP_COUNT	20		/* 样本个数，表示200ms内的采样数据求平均值 */
#define SENSOR_LENGTH 500
#define SENSOR_SCALE 1.0f/4096*SENSOR_LENGTH

//PC组ADC
#define HV_DC3V3 BSP_GPIOC0     //高压放电电压
#define LSPI3V3 BSP_GPIOC1		//生产线控输出功率速度电流给定
#define LSPV3V3 BSP_GPIOC2		//生产线控输出功率速度电压给定
#define LPIMA3V3 BSP_GPIOC3		//生产线控制输出功率电流给定
#define LPV3V3 BSP_GPIOC4		//生产线控制输出功率电压给定
#define TEMP BSP_GPIOC5			//散热器温度检测输入

//PA组ADC
#define IDC_ADC BSP_GPIOA0      //IDC电流检测
#define VDC_ADC BSP_GPIOA1      //VDC电压检测
#define TBI_DC3V3 BSP_GPIOA6   //变压器原边电流
#define TBV_DC3V3 BSP_GPIOA7   //变压器原边电压

//PB组ADC
#define POV3V3 BSP_GPIOB0 //输出功率AD3.3V检测
#define HI_DC3V3 BSP_GPIOB1 //高压放电电流检测



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

extern __IO uint16_t adc1_raw_value[10][6]; //采集的ADC值
extern __IO uint16_t adc3_raw_value[10][6]; //采集的ADC值
extern __IO float average_dis[4];  //距离平均值
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

