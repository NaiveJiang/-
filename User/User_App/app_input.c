#include "app.h"

appStruct_t appInput;

appControlStruct_t controlData;
app_pulseControlStruct_t djCtrlData;
controlState_e controlState;
controlState_e last_controlState;
void set_controlState(controlState_e setState,uint8_t step){
	controlState = setState;
	get_controlData()->control_step = step;
}
controlState_e get_controlState(void){
	return controlState;
}
controlState_e get_last_controlState(void){
	return last_controlState;
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
void app_dryMode(void);
void app_stopMode(void);
void app_inputUpdata(void){
	driverKeyNowStateUpdate();
//	dac_ch1_voltageOut(set_vdc);
//	dac_ch2_voltageOut(set_speed);
	appInput.loops += INPUT_TASK_PERIOD;
}

//void app_djControl(void){
//	get_djCtrlData()->mode = get_djCtrlData()->sw;	//��ȡ�缫����״̬
//	if(get_djCtrlData()->mode != get_djCtrlData()->last_mode){
//		if(get_djCtrlData()->mode) pulse_outputHigh(&DJOPEN,100);
//		else pulse_outputHigh(&DJCLOSE,100);
//	}
//	get_djCtrlData()->last_mode = get_djCtrlData()->mode;	//״̬����
//}

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
			
			case __DRY:{
				app_dryMode();
			}break;
			
			case __ROLL_CHANGING:{
				app_roll_changing();
			}break;
			
			case __STOP:{
				app_stopMode();
			}break;
		}
		
//		//�ŵ�ܿ���
//		app_djControl();
		
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
			//�ȴ����·���������������/ʪ����(ʪ����Ϊ����ҳ��) �����߿��źŽ���
			if(get_mainData(get_maindata()->main_rev_data,CORONA) || get_mainData(get_maindata()->main_rev_data,FAN) || get_controlData()->dry_mode || get_controlData()->line_control)
				set_controlState(__STANDBY,99);
		}break;
		case 99:{
			set_controlState(__FAN_ON,0);		//����������ģʽ
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
			//����رշ��,���ص�0״̬
			if(!get_mainData(get_maindata()->main_rev_data,FAN) && !get_controlData()->line_control){
				digitalLo(&get_controlData()->wait_sw);
				FYKL = 1; //���θ�ѹ����,�������ز�����
				pulse_outputHigh(&STOPCJ3,100);		//�رշ��
				pulse_outputHigh(&STOP_P,100);	//�ر�ֱ�����ʵ�Դ
				set_controlState(__STANDBY,0);	//�ص�״̬0����
			}
			//��ѹ�������ν���󣬲�����ѹ��û���޸�ѹ�������������������״̬
			if(!FYKL && !(get_controlData()->error_sta & NEGATIVE_PRESSURE)){
				digitalIncreasing(&get_controlData()->control_step);
			}
		}break;
		case 4:{		
			//�������ʱ�Ĵ���״̬(״̬1)
			//�ٶ��źŸ��£����ڸ�״̬���Ըı�(����)
			
			//����رշ��,���ص�0״̬
			if(!get_mainData(get_maindata()->main_rev_data,FAN) && !get_controlData()->line_control){
				FYKL = 1; //���θ�ѹ����,�������ز�����
				pulse_outputHigh(&STOPCJ3,100);		//�رշ��
				pulse_outputHigh(&STOP_P,100);	//�ر�ֱ�����ʵ�Դ
				set_controlState(__STANDBY,0);	//�ص�״̬0����
			}
			//����Ƿ����ȱ��/��ѹ/�ⲿ����������ͣ�������ڲ�������������
			if(!(get_controlData()->error_sta & QSALARM_ERROR) && !(get_controlData()->error_sta & DCVCHK_ERROR)){
				//��������˵�������������ʪ��������CJ3ȷ���Ѿ��պϣ���������
				if((get_mainData(get_maindata()->main_rev_data,CORONA) || get_controlData()->dry_mode || get_controlData()->line_control) && CJ3OK){	
					digitalHi(&get_supervisiorData()->start_delay_sw);	//������ʱ�ȴ�VDC�ȶ�
					digitalClan(&get_supervisiorData()->start_delay); //������¼���
					set_controlState(__FAN_ON,99);
				}
			}
		}break;
		case 99:{
			if(get_controlData()->dry_mode)
				set_controlState(__DRY,0);		//����ʪ����ģʽ
			else
				set_controlState(__CORONA,0);	//�������ģʽ
		}break;
		
	}
}

void app_coronaMode(void){
	switch(get_controlData()->control_step){
		case 0:{	//׼���׶�
			//�������ʵ�Դ
			STOPCS = 0;		//�ſ�STOPCS
			pulse_outputHigh(&STARTS,100);
			if(adc_filter_VDC_ADC >= 420){	//�ȴ�VDC��⵽��420V,�̵���CJ2�պ�
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
				if(adc_filter_VDC_ADC >= LOW_VDC) //�ȴ�VDC��⵽��470V���ܷŵ�
					digitalIncreasing(&get_controlData()->control_step);
			}
			else{	//50ms��δ�ܱպ�CJ2������ͣ������
				set_controlState(__CORONA,99);
			}
		}break;
		case 3:{
			//ʹ��3875���
//			vTaskDelay(100);
			//DAC����Ϊ0
			dac_ch1_voltageOut(0.0f);
			dac_ch2_voltageOut(0.0f);
			digitalIncreasing(&get_controlData()->control_step);
		}break;
		case 4:{
			//���ʷŵ�ģʽ
			app_discharge();
			//���߿�ģʽ���������ͣ�������߹رյ��Σ�����ͣ��,����ֹͣ�����ص�����״̬(״̬0)���رյ��λ�ص��������״̬��״̬1��
			if((get_mainData(get_maindata()->main_rev_data,STOP_CORONA) || !get_mainData(get_maindata()->main_rev_data,CORONA)) && !get_controlData()->line_control){
				set_controlState(__CORONA,99);
			}
		}break;
		case 5:{	//��������ͣ״̬
			//�ȴ�����������
			if(!get_controlData()->line_suspend && get_controlData()->line_control){
				//���طŵ�״̬
				set_controlState(__CORONA,4);
			}
			
			if(get_mainData(get_maindata()->main_rev_data,STOP_CORONA)){
				set_controlState(__CORONA,99);
			}
		}break;
		case 99:{
			//��λһЩ״̬����
			digitalLo(&get_controlData()->line_control);
			digitalLo(&get_controlData()->line_suspend);
			//����ͣ��״̬
			set_controlState(__STOP,0);
		}break;
	}
}

void app_dryMode(void){
	switch(get_controlData()->control_step){
		case 0:{	//׼���׶�
			//�����߿��ź�
			LKEN = 0;
			//�������ʵ�Դ
			STOPCS = 0;		//�ſ�STOPCS
			pulse_outputHigh(&STARTS,100);
			if(adc_filter_VDC_ADC >= 420){	//�ȴ�VDC��⵽��420V,�̵���CJ2�պ�
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
				if(adc_filter_VDC_ADC >= LOW_VDC) //�ȴ�VDC��⵽��470V���ܷŵ�
					digitalIncreasing(&get_controlData()->control_step);
			}
			else{	//50ms��δ�ܱպ�CJ2������ͣ������
				set_controlState(__DRY,0);
			}
		}break;
		case 3:{
			//DAC����Ϊ0
			dac_ch1_voltageOut(0.0f);
			dac_ch2_voltageOut(0.0f);
			STANDBY = 0;	//����3875���
			digitalIncreasing(&get_controlData()->control_step);
		}break;
		case 4:{
			if(!get_controlData()->dry_mode){	//����ر���ʪ����
				digitalIncreasing(&get_controlData()->control_step);
			}
			else{
				//ʪ�������
				app_dryCtrl();
			}
		}break;
		case 5:{
			if(get_dischargeCtrlData()->current_power <= 10.0f){
				digitalIncreasing(&get_controlData()->control_step);
			}
			else{
				get_dryCtrlData()->dry_power *= 0.5;
				dac_ch1_voltageOut(get_dryCtrlData()->dry_power);
				dac_ch2_voltageOut(get_dryCtrlData()->dry_power);
				vTaskDelay(100);
			}
		}break;
		case 6:{
			digitalClan(&get_dryCtrlData()->dry_power);
			//���DAC����
			dac_ch1_voltageOut(0.0f);
			dac_ch2_voltageOut(0.0f);
			//��ֹ3875�������
			STANDBY = 1;
			//�رչ��ʵ�Դ
			pulse_outputHigh(&STOP_P,100);
			//�ָ��߿��ź�
			LKEN = 1;
			
			set_controlState(__DRY,99);
		}break;
		case 99:{
			set_controlState(__FAN_ON,4);
		}break;
	}
}



void app_stopMode(void){
	switch(get_controlData()->control_step){
		case 0:{
			//����ͣ�����������ʹӲ��ͣ��
			if(get_supervisiorData()->stop_alarm) ALARM = 1;
			//����ͣ���Զ��������״̬
			get_maindata()->main_rev_data &= ~CORONA;
			if(get_dischargeCtrlData()->current_power <= 10.0f){
				digitalIncreasing(&get_controlData()->control_step);
			}
			else{
				if(get_dischargeCtrlData()->mode){
					//����10kw��ÿ��0.1s����50%
					get_pulseDischargeData()->discharge_power *= 0.5f;
					dac_ch1_voltageOut(get_pulseDischargeData()->discharge_power);
					dac_ch2_voltageOut(get_pulseDischargeData()->discharge_power);
				}
				else{
					//����10kw��ÿ��0.1s����50%
					get_spdDischargeData()->discharge_power *= 0.5f;
					dac_ch1_voltageOut(get_spdDischargeData()->discharge_power);
					dac_ch2_voltageOut(get_spdDischargeData()->discharge_power);
				}
				vTaskDelay(100);	//��ʱ0.1s
			}
		}break;
		case 1:{
			//ֹͣ�ŵ�
			digitalClan(&get_pulseDischargeData()->discharge_power);
			digitalClan(&get_spdDischargeData()->discharge_power);
			digitalClan(&get_dischargeCtrlData()->step);
			dac_ch1_voltageOut(0.0f);
			dac_ch2_voltageOut(0.0f);
			
			//�ر�3875�������
			STANDBY = 1;
			
			//�ر�ֱ�����ʵ�Դ
			pulse_outputHigh(&STOP_P,100);
			
			
			digitalIncreasing(&get_controlData()->control_step);
		}break;
		case 2:{
			vTaskDelay(1000);  //��ʱ1s���CJ1/CJ2�Ƿ�Ͽ�
			//���CJ1/CJ2
			if(!CJ12OK){	//CJ1/CJ2����
				if(get_supervisiorData()->stop_alarm){	//��������ͣ�����󱨾�
					digitalIncreasing(&get_controlData()->control_step);	//�������ͣ��״̬
				}
				else{
					//���û������
					if(!get_controlData()->lock && !get_controlData()->lock_tamp)
						set_controlState(__STOP,99);	//�˳�ͣ��ģʽ
					else
						set_controlState(__STOP,4);//��������״̬
				}
			}
			else{ //CJ1/CJ2�޷��Ͽ�
				if(PUPOK){
					ALARM = 1;
					digitalHi(&get_supervisiorData()->stop_alarm);	//����ͣ������
					digitalIncreasing(&get_controlData()->control_step);	//�������ͣ��״̬
				}
			}
		}break;
		case 3:{	//�ȴ����ϸ�λ
			//����յ����ϸ�λ����(����ȷ��)�����䣩
			STOP_P = 1; 
			if(get_controlData()->error_reset){
				digitalLo(&get_controlData()->error_reset);
				//ʹ�ܹ��ϸ�λ
				pulse_outputHigh(&RESET_SYS,10);
				//Ӳ��ͣ�����
				ALARM = 0;
				STOP_P = 0;
				//���ͣ������
				digitalLo(&get_supervisiorData()->stop_alarm);
				//�����𱨾�
				get_controlData()->error_sta &= ~HIGH_VOLTAGE_SPARK_ERROR;	//��մ�𱨾�
				get_controlData()->error_sta &= ~VDC_LOW_ERROR;	//������������
				get_controlData()->error_sta &= ~VDC_LOW_WARN;	//������������
				
				if(!get_controlData()->lock && !get_controlData()->lock_tamp)	//û������
					set_controlState(__STOP,99);
				else
					digitalIncreasing(&get_controlData()->control_step); //����״̬
			}
		}break;
		case 4:{	//����״̬
			if(!get_controlData()->lock && !get_controlData()->lock_tamp){	//����
				//�������ͣ�����ϣ���Ҫ���ع��ϸ�λ
				if(get_supervisiorData()->stop_alarm)
					set_controlState(__STOP,3);
				else
					set_controlState(__STOP,99);	//�������
			}
		}break;
		case 99:{
			//����CJ3�ص����״̬
			if(CJ3OK)	//����������У��ص�״̬1
				set_controlState(__FAN_ON,4);
			else		//���ͣ������״̬0
				set_controlState(__STANDBY,0);
		}break;
	}
}



