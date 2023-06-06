#include "app.h"

appStruct_t appInput;
//�����ñ���
float test_adc1[6];
float test_adc3[6];
float set_vdc;
float set_speed;
uint8_t flash_sw;
appControlStruct_t controlData;
app_pulseControlStruct_t djCtrlData;
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
app_pulseControlStruct_t *get_djCtrlData(void){
	return &djCtrlData;
}

void app_standby(void);
void app_fanMode(void);
void app_coronaMode(void);

void app_inputUpdata(void){
	driverKeyNowStateUpdate();
	dac_ch1_voltageOut(set_vdc);
	dac_ch2_voltageOut(set_speed);
	appInput.loops += INPUT_TASK_PERIOD;
}

void app_djControl(void){
	get_djCtrlData()->mode = get_djCtrlData()->sw;	//��ȡ�缫����״̬
	if(get_djCtrlData()->mode != get_djCtrlData()->last_mode){
		if(get_djCtrlData()->mode) pulse_outputHigh(&DJOPEN,100);
		else pulse_outputHigh(&DJCLOSE,100);
	}
	get_djCtrlData()->last_mode = get_djCtrlData()->mode;	//״̬����
}

void app_inputTask(void *Parameters){
	TickType_t xLastWakeTime = xTaskGetTickCount();
	
	while(1){
		vTaskDelayUntil(&xLastWakeTime, INPUT_TASK_PERIOD);
		//����״̬
		switch(get_controlState()){
			case __STANDBY:{		//����0״̬
				app_standby();
			}break;
			
			case __FAN_ON:{			//�������1״̬
				app_fanMode();
			}break;
			
			case __CORONA:{
				app_coronaMode();
			}break;
			
			case __STOP:{
				
			}break;
		}
		
		//�ŵ�ܿ���
		app_djControl();
		
		app_inputUpdata();
//		can_Send(USE_CANx,&can_id20A);
//		rs485_send_data(RS_485,sendata,RS485_TEST_LEN);
	}
}

void app_inputTaskInit(void){
	xTaskCreate(app_inputTask,"INPUT",INPUT_STACK_SIZE,NULL,INPUT_PRIORITY,&appInput.xHandleTask);
}

//����0״̬
void app_standby(void){
	switch(get_controlData()->control_step){
		case 0:{
			//����ȱ��͵�ѹ�����������,VDC������ⲿ��ػ�,�������ڼ�ػ�
			if(!(get_controlData()->error_sta & QSALARM_ERROR) && !(get_controlData()->error_sta & DCVCHK_ERROR))
				digitalIncreasing(&get_controlData()->control_step);
		}break;
		case 1:{
			//�ȴ����·���������������/ʪ����(ʪ����Ϊ����ҳ��)
			if(get_mainData(get_maindata()->main_rev_data,CORONA) || get_mainData(get_maindata()->main_rev_data,DRY) || get_mainData(get_maindata()->main_rev_data,FAN))
				get_controlData()->control_step = 99;
		}break;
		case 99:{
			set_controlState(__FAN_ON);		//����������ģʽ
			digitalClan(&get_controlData()->control_step);
		}break;
	}
}


void app_fanMode(void){
	switch(get_controlData()->control_step){
		case 0:{
			pulse_outputHigh(&O2CJ3,100);		//�������
			digitalHi(&get_controlData()->wait_sw);
			digitalIncreasing(&get_controlData()->control_step);
		}break;
		case 1:{
			if(get_controlData()->wait_time > 20){	//�ȴ�0.2s�������Ƿ�����
				digitalLo(&get_controlData()->wait_sw);
				digitalIncreasing(&get_controlData()->control_step);
			}
		}break;
		case 2:{
			error_set(&CJ3OK,CJ3_ERROR,0);		//��CJ3δ�պϣ��������
			//CJ3OK=1 �������
			if(CJ3OK){
				digitalHi(&get_controlData()->wait_sw);
				digitalIncreasing(&get_controlData()->control_step);
			}
		}break;
		case 3:{
			if(get_controlData()->wait_time > 500){	//�ȴ�5s������ѹ
				digitalLo(&get_controlData()->wait_sw);
				PBLE3 = 0; 	//���������������
				FYKL = 0;	//�����ѹ��������
			}
			//��ѹ�������ν���󣬲�����ѹ��û���޸�ѹ�������������������״̬
			if(!FYKL && !(get_controlData()->error_sta & NEGATIVE_PRESSURE))
				digitalIncreasing(&get_controlData()->control_step);
		}break;
		case 4:{		//�������ʱ�Ĵ���״̬(״̬1)
			//����رշ��,���ص�0״̬
			if(!get_mainData(get_maindata()->main_rev_data,FAN)){
				pulse_outputHigh(&STOPCJ3,100);		//�رշ��
				FYKL = 1; //���θ�ѹ����,�������ز�����
				pulse_outputHigh(&STOP_P,100);	//�ر�ֱ�����ʵ�Դ
				set_controlState(__STANDBY);	//�ص�״̬0����
				digitalClan(&get_controlData()->control_step);
			}
			//����Ƿ����ȱ��/��ѹ/�ⲿ����������ͣ�������ڲ�������������
			if(!(get_controlData()->error_sta & QSALARM_ERROR) && !(get_controlData()->error_sta & DCVCHK_ERROR) && !(adc_filter_VDC_ADC < LOW_VDC)){
				//��������˵�������������ʪ��������CJ3ȷ���Ѿ��պϣ���������
				if((get_mainData(get_maindata()->main_rev_data,CORONA) || get_mainData(get_maindata()->main_rev_data,DRY)) && CJ3OK)	
					get_controlData()->control_step = 99;
			}
		}break;
		case 99:{
			set_controlState(__CORONA);	//�������ģʽ
			digitalClan(&get_controlData()->control_step);
		}break;
			
	}
}

void app_coronaMode(void){
	switch(get_controlData()->control_step){
		case 0:{	//׼���׶�
			if(adc_filter_VDC_ADC >= LOW_VDC){	//�ȴ�VDC��⵽��470V,�̵����պ�
				//�պϼ̵���CJ2
				pulse_outputHigh(&UPEDCJ2,50);
				digitalHi(&get_controlData()->wait_sw);
				digitalIncreasing(&get_controlData()->control_step);
			}
		}break;
		case 1:{
			//�ȴ�50ms��������ڼ�CJ2�޷��պϣ������ͣ������
			if(get_controlData()->wait_time >= 5){
				digitalLo(&get_controlData()->wait_sw);
				digitalIncreasing(&get_controlData()->control_step);
			}
		}break;
		case 2:{
			error_set(&CJ12OK,CJ12_ERROR,0);	//��CJ2�޷��պϣ������ͣ������	
			if(CJ12OK){		//�̵����պ�
				digitalIncreasing(&get_controlData()->control_step);
			}
			else{	//50ms��δ�ܱպ�CJ2������ͣ������
				get_controlData()->control_step = 99;
			}
		}break;
		case 3:{
			//�������ʵ�Դ
			STOPCS = 0;		//�ſ�STOPCS
			pulse_outputHigh(&STARTS,100);
			//ʹ��3875���
			
//			vTaskDelay(100);
			STANDBY = 0;	//����3875���
			//DAC����Ϊ0
			dac_ch1_voltageOut(0.0f);
			dac_ch2_voltageOut(0.0f);
			digitalIncreasing(&get_controlData()->control_step);
		}break;
		case 4:{	
			if(get_mainData(get_maindata()->main_rev_data,DRY)){	//���ʹ��ʪ����
				digitalHi(&get_controlData()->dry_mode);
				//�����߿��ź�
				LKEN = 0;
				//ʪ�����ŵ�
				app_dryMode();
			}
			//���ʪ����ʱ����ͣ��������Ҫͣ����������������״̬(״̬1)
			if(!get_mainData(get_maindata()->main_rev_data,DRY) && get_controlData()->dry_mode){
				digitalLo(&get_controlData()->dry_mode);
				//3875��ֹ���
				STANDBY = 1;
				//�رչ��ʵ�Դ
				pulse_outputHigh(&STOP_P,100);
				//���DAC����
				digitalClan(&get_dryData()->dry_power);
				dac_ch1_voltageOut(0.0f);
				dac_ch2_voltageOut(0.0f);
				//��ת״̬
				set_controlState(__FAN_ON);
				get_controlData()->control_step = 4;
			}
//			else{
//				//�ָ��߿��ź�
//				LKEN = 1;
//				digitalIncreasing(&get_controlData()->control_step);
//			}
		}break;
		case 5:{
			//���ʷŵ�ģʽ
			
		}break;
		case 99:{
			
		}break;
	}
}



//��������״̬
void app_corona_state(void){
	switch(get_controlData()->control_step){
		case 0:{
			//ֻ����Щ�������ˣ����ܽ���̵����պ�
			if(!(get_controlData()->error_sta & 0xFFFFFFFE))
				digitalIncreasing(&get_controlData()->control_step);
		}break;
		case 1:{
			//�ȴ�VDC��⵽��80%,�̵����պ�
			if(adc_filter_VDC_ADC >= LOW_VDC){
				//�պϼ̵���CJ2
				pulse_outputHigh(&UPEDCJ2,50);
				digitalIncreasing(&get_controlData()->control_step);
			}
		}break;
		case 2:{
			//�ȴ�50ms��������ڼ�CJ2�޷��պϣ������ͣ������
			digitalHi(&get_controlData()->wait_sw);
			if(get_controlData()->wait_time >= 5){
				digitalLo(&get_controlData()->wait_sw);
				error_set(&CJ12OK,CJ12_ERROR,0);	//��CJ2�޷��պϣ������ͣ������	
				if(CJ12OK){		//�̵����պ�
					digitalIncreasing(&get_controlData()->control_step);
				}
				else{	//50ms��δ�ܱպ�CJ2������ͣ������
					get_controlData()->control_step = 99;
				}
			}
		}break;
		case 3:{
			//ʹ��3875���
			STOPCS = 0;		//�ſ�STOPCS
			vTaskDelay(100);
			STANDBY = 0;	//����3875���
			//DAC����Ϊ0
			dac_ch1_voltageOut(0.0f);
			dac_ch2_voltageOut(0.0f);
			digitalIncreasing(&get_controlData()->control_step);
		}break;
		case 4:{
			//������  ���
//			if(get_controlData()->speed_up){
//				
//			}
//			else{	//û���پ�ͣ��
//				
//			}
//			//���տ���ģʽ�ŵ�
			//�����С����δ����ǰ������С���ʷŵ磬���ٺ�Ͱ���ģʽѡ���ʷŵ�
//			if(get_controlData()->corona_in_low_sw){
//				//δ���٣���С���ʷŵ�
//				if(!get_controlData()->speed_up){
//					//�ŵ�
//				}
//				else{
//					
//				}
//			}
//			else{
//				//���ŵ�
//				
//			}
			//����رյ���
			if(!get_mainData(get_maindata()->main_rev_data,CORONA)){
				get_controlData()->control_step = 99;
			}
		}break;
		case 99:{
//			set_controlState(CORONA_STOP);		//����ֹͣ�ŵ�״̬
			digitalClan(&get_controlData()->control_step);
		}break;
	}
}

