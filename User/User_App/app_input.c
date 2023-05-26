#include "app.h"

#define INPUT_TASK_PERIOD	5

appStruct_t appInput;
//�����ñ���
float test_adc1[6];
float test_adc3[6];
float set_vdc;
float set_speed;
uint8_t flash_sw;
appControlStruct_t controlData;
controlState_e controlState;
void set_controlState(controlState_e setState){
	controlState = setState;
}
controlState_e get_controlState(void){
	return controlState;
}
appControlStruct_t *get_controlData(void){
	return &controlData;
}

void app_coronaWait(void);

void app_inputUpdata(void){
	driverKeyNowStateUpdate();
	dac_ch1_voltageOut(set_vdc);
	dac_ch2_voltageOut(set_speed);
	appInput.loops += INPUT_TASK_PERIOD;
}
void app_inputTask(void *Parameters){
	TickType_t xLastWakeTime = xTaskGetTickCount();
	
	while(1){
		vTaskDelayUntil(&xLastWakeTime, INPUT_TASK_PERIOD);
		//����״̬
		switch(get_controlState()){
			case CORONA_WAIT:{
				app_coronaWait();
			}break;
			case CORONA_STATE:{
				
			}break;
			case CORONA_STOP:{
				
			}break;
			
		}
		app_inputUpdata();
//		can_Send(USE_CANx,&can_id20A);
//		rs485_send_data(RS_485,sendata,RS485_TEST_LEN);
		if(flash_sw){                  //flash����
			app_FlashWriteUdata();
			flash_sw = 0;
		}
		
		IWDG_Feed(); //ι��
	}
}

void app_inputTaskInit(void){
	xTaskCreate(app_inputTask,"INPUT",INPUT_STACK_SIZE,NULL,INPUT_PRIORITY,&appInput.xHandleTask);
}

void app_coronaWait(void){
	switch(get_controlData()->control_step){
		case 0:{
			//ȷ���Ƿ������λ����Ƿ������
			if(get_mainData(get_maindata()->main_rev_data,CORONA) || get_mainData(get_maindata()->main_rev_data,FAN))
				digitalIncreasing(&get_controlData()->control_step);
		}break;
		case 1:{
			//�򿪷��
			pulse_output(&O2CJ3,100);
			digitalHi(&get_controlData()->wait_sw);
			//�ȴ�5s��رո�ѹ����
			if(get_controlData()->wait_time >= 500){
				digitalLo(&get_controlData()->wait_sw);
				//�رո�ѹ����
				FYKL = 0;
				digitalIncreasing(&get_controlData()->control_step);
			}
		}break;
		case 2:{
			//�ȴ��Ƿ��е��ο��ؿ���
			if(get_mainData(get_maindata()->main_rev_data,CORONA))
				get_controlData()->control_step = 99;
		}break;
		case 99:{
			//�������
			set_controlState(CORONA_STATE);
			digitalClan(&get_controlData()->control_step);
		}break;
	}
}

