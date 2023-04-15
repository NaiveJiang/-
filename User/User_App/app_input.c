#include "app.h"

#define INPUT_TASK_PERIOD	5

appStruct_t appInput;
float test_adc1[6];
float test_adc3[6];
void app_inputUpdata()
{
//	driverPhotoelectricSwitchNowStateUpdate();
	driverKeyNowStateUpdate();
	getadc1_average(test_adc1);
	getadc3_average(test_adc3);
	appInput.loops += INPUT_TASK_PERIOD;
}

void app_inputTask(void *Parameters)
{
	TickType_t xLastWakeTime = xTaskGetTickCount();
	
	while(1){
		vTaskDelayUntil(&xLastWakeTime, INPUT_TASK_PERIOD);
		app_inputUpdata();
		IWDG_Feed(); //Î¹¹·
	}
}

void app_inputTaskInit(void)
{
	xTaskCreate(app_inputTask,"INPUT",INPUT_STACK_SIZE,NULL,INPUT_PRIORITY,&appInput.xHandleTask);
}


