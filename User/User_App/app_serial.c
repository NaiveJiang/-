#include "app.h"


appStruct_t appSerial;

void app_serialUpdata(void){
	gui_send_data(GUI_USART);
}

uint32_t fp;
void app_serialTask(void *Parameters){
	TickType_t xLastWakeTime = xTaskGetTickCount();
	while(1){
		vTaskDelayUntil(&xLastWakeTime,SERIAL_TASK_PEROID);
		//串口发送
		if(!(fp % 50))
			app_serialUpdata();
		fp++;
	}
}

//创建串口通信任务
void app_serialTaskInit(void){
	xTaskCreate(app_serialTask,"SERIAL",SERIAL_STACK_SIZE,NULL,SERIAL_PRIORITY,&appSerial.xHandleTask);
}



