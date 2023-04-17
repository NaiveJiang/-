#include "app.h"

#define INPUT_TASK_PERIOD	5

appStruct_t appInput;
//≤‚ ‘”√±‰¡ø
float test_adc1[6];
float test_adc3[6];
float set_vdc;
float set_speed;

void app_inputUpdata(void){
	driverKeyNowStateUpdate();
	getadc1_average(test_adc1);
	getadc3_average(test_adc3);
	appInput.loops += INPUT_TASK_PERIOD;
}

void app_inputTask(void *Parameters){
	TickType_t xLastWakeTime = xTaskGetTickCount();
	
	while(1){
		vTaskDelayUntil(&xLastWakeTime, INPUT_TASK_PERIOD);
		app_inputUpdata();
		dac_ch1_voltageOut(set_vdc);
		dac_ch2_voltageOut(set_speed);
		can_Send(USE_CANx,&can_id20A);
		
		IWDG_Feed(); //Œππ∑
	}
}

void app_inputTaskInit(void){
	xTaskCreate(app_inputTask,"INPUT",INPUT_STACK_SIZE,NULL,INPUT_PRIORITY,&appInput.xHandleTask);
}


