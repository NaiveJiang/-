#include "app.h"

appStruct_t appAdc;

void app_adc1Updata(void){
	//计算采样数据
	adc_filter_TBI_DC3V3 = middleAverageFilter(adc_full_TBI_DC3V3,SAMP_FULL_COUNT);
	adc_filter_TBV_DC3V3 = middleAverageFilter(adc_full_TBV_DC3V3,SAMP_FULL_COUNT);
	adc_filter_POV3V3 = middleAverageFilter(adc_full_POV3V3,SAMP_FULL_COUNT);
	adc_filter_HI_DC3V3 = middleAverageFilter(adc_full_HI_DC3V3,SAMP_FULL_COUNT);
	adc_filter_LPV3V3 = middleAverageFilter(adc_full_LPV3V3,SAMP_FULL_COUNT);
	adc_filter_TEMP = middleAverageFilter(adc_full_TEMP,SAMP_FULL_COUNT);
	digitalLo(&adc1_settlement_sw);  //完成采样计算
	digitalClan(&adc1_settlement);
	
}

void app_adc3Updata(void){
	//计算采样数据
	adc_filter_IDC_ADC = middleAverageFilter(adc_full_IDC_ADC,SAMP_FULL_COUNT);
	adc_filter_VDC_ADC = middleAverageFilter(adc_full_VDC_ADC,SAMP_FULL_COUNT);
	adc_filter_HV_DC3V3 = middleAverageFilter(adc_full_HV_DC3V3,SAMP_FULL_COUNT);
	adc_filter_LSPI3V3 = middleAverageFilter(adc_full_LSPI3V3,SAMP_FULL_COUNT);
	adc_filter_LSPV3V3 = middleAverageFilter(adc_full_LSPV3V3,SAMP_FULL_COUNT);
	adc_filter_LPIMA3V3 = middleAverageFilter(adc_full_LPIMA3V3,SAMP_FULL_COUNT);
	digitalLo(&adc3_settlement_sw);  //完成采样计算
	digitalClan(&adc3_settlement);
	
}

void app_adcTask(void *Parameters){
	TickType_t xLastWakeTime = xTaskGetTickCount();
	
	while(1){
		vTaskDelayUntil(&xLastWakeTime,ADC_TASK_PEROID);
		if(adc1_settlement_sw){
			app_adc1Updata();
			taskYIELD(); //切换任务
		}
		if(adc3_settlement_sw){
			app_adc3Updata();
			taskYIELD(); 
		}
		if(!adc1_settlement_sw && !adc3_settlement_sw)
			taskYIELD();
	}
}

//创建ADC数据转换任务
void app_adcTaskInit(void){
	xTaskCreate(app_adcTask,"ADC",ADC_TASK_STACK_SIZE,NULL,ADC_TASK_PRIORITY,&appAdc.xHandleTask);
}



