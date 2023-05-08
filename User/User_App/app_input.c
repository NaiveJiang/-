#include "app.h"

#define INPUT_TASK_PERIOD	5

appStruct_t appInput;
//测试用变量
float test_adc1[6];
float test_adc3[6];
float set_vdc;
float set_speed;
uint8_t flash_sw;

void app_inputUpdata(void){
	driverKeyNowStateUpdate();
	getadc1_average(test_adc1);
	getadc3_average(test_adc3);
	appInput.loops += INPUT_TASK_PERIOD;
}
uint32_t fp;
void app_inputTask(void *Parameters){
	TickType_t xLastWakeTime = xTaskGetTickCount();
	
	while(1){
		vTaskDelayUntil(&xLastWakeTime, INPUT_TASK_PERIOD);
		app_inputUpdata();
		dac_ch1_voltageOut(set_vdc);
		dac_ch2_voltageOut(set_speed);
		can_Send(USE_CANx,&can_id20A);
//		rs485_send_data(RS_485,sendata,RS485_TEST_LEN);
		if(flash_sw){                  //flash测试
			app_FlashWriteUdata();
			flash_sw = 0;
		}
		if(!(fp % 50))
			gui_send_data(GUI_USART,send_gui,GUI_USART_TEST_LEN);   //串口2发送
		IWDG_Feed(); //喂狗
		fp++;
	}
}

void app_inputTaskInit(void){
	xTaskCreate(app_inputTask,"INPUT",INPUT_STACK_SIZE,NULL,INPUT_PRIORITY,&appInput.xHandleTask);
}


