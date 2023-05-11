#ifndef __APP_ADC_H
#define __APP_ADC_H

#define ADC_TASK_PEROID 2

typedef enum{
	ADC_TASK_PRIORITY = 5,
	ADC_TASK_STACK_SIZE = 256,
}appAdc_e;

void app_adcTaskInit(void);

#endif

