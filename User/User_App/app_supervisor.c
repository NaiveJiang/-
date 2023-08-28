#include "app.h"


appStruct_t appSupervisior;
appSupStruct_t supervisiorData;

appSupStruct_t *get_supervisiorData(void){
	return &supervisiorData;
}


uint8_t rst_error = 0;
uint8_t read_cj3 = 0;
//��������
void app_sup_LE(void){
	uint8_t le_error;
	if(!LE3) get_controlData()->error_sta &= 0xFFFFFF87;	//LE3Ϊ1ʱ��Ч
	else if(!(get_controlData()->error_sta & EMERGENCY_STOP)){ //���û�м�ͣ����
		le_error = (uint8_t)LE0 | (uint8_t)(LE1 << 1) | (uint8_t)(LE2 << 2);
		get_controlData()->error_sta &= 0xFFFFFF87;
		//���θ�ѹ���غ������޸�ѹ���
		if(FYKL && !le_error) get_controlData()->error_sta |= (le_error) << 3;
		else get_controlData()->error_sta |= (le_error + 1) << 3;
	}
	else get_controlData()->error_sta &= 0xFFFFFF87;
}

void remain_day(void){
	if(get_controlData()->rtc_day1){	//����1δ��
		if(get_controlData()->rtc_day1 <= 15 && get_controlData()->rtc_day1 > 5){  //��ʣ��15��
			get_controlData()->remain_day = 1;
		}
		else if(get_controlData()->rtc_day1 <= 5){	//��ʣ��5��
			get_controlData()->remain_day = 2;
		}
		else{	//����1����15��
			digitalClan(&get_controlData()->remain_day); 
		}
	}
	else{	//����1�ѵ�
		if(get_controlData()->rtc_day2){ //����2δ��
			if(get_controlData()->rtc_day2 <= 15 && get_controlData()->rtc_day2 > 5){  //��ʣ��15��
				get_controlData()->remain_day = 4;
			}
			else if(get_controlData()->rtc_day1 <= 5){	//��ʣ��5��
				get_controlData()->remain_day = 5;
			}
			else{ //����2����15�죬����1����ʱΪ3������1����ʱΪ0
				if(!get_controlData()->lock) //�������1�Ѿ�����
					digitalClan(&get_controlData()->remain_day); 
			}
		}
		else{	//����2�ѵ�
			if(get_controlData()->rtc_day3){ //����3δ��
				if(get_controlData()->rtc_day1 <= 15 && get_controlData()->rtc_day1 > 5){  //��ʣ��15��
					get_controlData()->remain_day = 7;
				}
				else if(get_controlData()->rtc_day1 <= 5){	//��ʣ��5��
					get_controlData()->remain_day = 8;
				}
				else{	//����3����15�죬����2����Ϊ6������2����Ϊ0
					if(!get_controlData()->lock) //�������2�Ѿ�����
						digitalClan(&get_controlData()->remain_day);
				}
			}
			else{	//����3�ѵ�
				if(get_controlData()->lock)  //����״̬
					get_controlData()->remain_day = 9;
				else
					digitalClan(&get_controlData()->remain_day);
			}
		}
	}
}

void lock_to_unlock(void){
	if(!get_controlData()->rtc_day1){	//�������1�ѵ�
		if(get_controlData()->rtc_day2){	//����2δ��
			//�����һ������������ȷ����
			if(get_controlData()->verify_password == get_controlData()->password[0]){
				digitalLo(&get_controlData()->lock);	//��������
				digitalHi(&get_controlData()->pwd_ok);
			}
			else{
				digitalHi(&get_controlData()->lock); //��������������
				digitalLo(&get_controlData()->pwd_ok);
				//�������2С��15����С��15��ķ�������
				if(get_controlData()->rtc_day2 > 15)
					get_controlData()->remain_day = 3; //����1����
			}
		}
		else{	//����2�ѵ�
			if(get_controlData()->rtc_day3){	//����3δ��
				//����ڶ�������������ȷ����
				if(get_controlData()->verify_password == get_controlData()->password[1]){
					digitalLo(&get_controlData()->lock);	//��������
					digitalHi(&get_controlData()->pwd_ok);
				}
				else{
					digitalHi(&get_controlData()->lock); //��������������
					digitalLo(&get_controlData()->pwd_ok);
					//�������3С��15����С��15��ķ�������
					if(get_controlData()->rtc_day3 > 15)
						get_controlData()->remain_day = 6;	//����2����
				}
			}
			else{	//����3�ѵ�
				//�������������������ȷ����
				if(get_controlData()->verify_password == get_controlData()->password[2]){
					digitalLo(&get_controlData()->lock);	//��������
					digitalHi(&get_controlData()->pwd_ok);
				}
				else{
					digitalHi(&get_controlData()->lock); //��������������
					digitalLo(&get_controlData()->pwd_ok);
					get_controlData()->remain_day = 9;	//����3����
				}
			}
		}
	}
}

//״̬�����
void app_supervisiorTask(void *Parameters){
	TickType_t xLastWakeTime = xTaskGetTickCount();
	while(1){
		vTaskDelayUntil(&xLastWakeTime, SUPER_TASK_PERIOD);
		
		
		//��PBLE3ʱ��⻥������״̬
		if(!PBLE3) app_sup_LE();							//��������
		error_set(&QSALARM,QSALARM_ERROR,1);				//ȱ����
		error_set(&DCVCHK,DCVCHK_ERROR,1);				//��ѹ��Դ���
		error_set(&JTJC,EMERGENCY_STOP,1);				//��ͣ����
		error_set(&IN_ALARM,SYSTEM_ERROR,1);				//ϵͳ����
		error_set(&PVDD,POWER_DOWN_ERROR,1);				//��ѹ��Դ���
//		error_set(&DHAL,HIGH_VOLTAGE_SPARK_ERROR,1);		//��ѹ��𱨾�		
		error_set(&HIAL,HIGH_VOLTAGE_OVERCURRENT,1);		//��ѹ�ŵ����
		error_set(&IGBTBAL,IGBTB_OVERCURRENT,1);			//IGBTB����
		error_set(&IGBTAAL,IGBTA_OVERCURRENT,1);			//IGBTA����
		error_set(&TIAL,PRIMARY_OVERCURRENT,1);			//ԭ�ߵ�������
		
//		if(get_rcCtrlData()->state){	//���������ź�
//			if(!get_rcCtrlData()->sw){
//				digitalHi(&get_rcCtrlData()->sw);
//				//��¼��һ�ε�״̬
//				get_controlData()->last_control_step = get_controlData()->control_step;
//				last_controlState = get_controlState(); 
//			}
//			set_controlState(__ROLL_CHANGING,get_controlData()->control_step);
//		}
		
		
		//�ٶ��źŸ���
		get_spdDischargeData()->speed_signal = get_setStateData(get_powSetData()->set_state,SPEEDSIGNAL);
		
		
		//�����ź�
		LKEN = get_setStateData(get_powSetData()->set_state,CONTROLMODE);
		
//		//ֻ���ڵ���״̬VDC�ļ����������
		if(get_controlState() == __CORONA){
			//��������ʱ����Ҫ�ȴ�һ��ʱ��ʹVDC�ȶ�
			if(get_supervisiorData()->start_delay >= START_DELAY_TIME){
				if(adc_filter_VDC_ADC < LOW_VDC)	//VDC����450
					get_controlData()->error_sta |= VDC_LOW_ERROR;	//�ⲿ����������;���
				else
					get_controlData()->error_sta &= ~VDC_LOW_ERROR;	//������������
				
				if(adc_filter_VDC_ADC < WARN_VDC)	//VDC����470
					get_controlData()->error_sta |= VDC_LOW_WARN;	//�ⲿ��������Ԥ��
				else
					get_controlData()->error_sta &= ~VDC_LOW_WARN;	//��������Ԥ��
			}
		}
		
		
		//���ټ��(����ֶ����٣����δ��ټ��)
		if(!get_controlData()->manual_mode){
			if(get_spdDischargeData()->local_speed < get_controlData()->set_speed_up){	//���ڵ�����С�趨�ٶȼ�����
				digitalLo(&get_controlData()->speed_up);
				SPDUP = 0;
			}
			else{
				digitalHi(&get_controlData()->speed_up);
				SPDUP = 1;
				//���ٺ��л�Ϊ����״̬
				digitalLo(&get_dischargeCtrlData()->mode);
//				if(get_dischargeCtrlData()->mode){	//���������ŵ�״̬
//					//����ֱ�ӽ������ٷŵ�
//					digitalLo(&get_dischargeCtrlData()->mode);
//					digitalClan(&get_dischargeCtrlData()->step);
//				}
			}
		}
		else{
			digitalHi(&get_controlData()->speed_up);
			SPDUP = 1;
		}
		
		//��ѹ���
		if(!FYKL){ //���θ�ѹ����ʱ�򲻼��
			if((get_controlData()->error_sta & 0x78) == NEGATIVE_PRESSURE){
				//�޸�ѹ
				digitalLo(&get_controlData()->fy_ok);
			}
			else{
				//�и�ѹ
				digitalHi(&get_controlData()->fy_ok);
			}
		}
		else{
			//�޸�ѹ
			digitalLo(&get_controlData()->fy_ok);
		}
		
		//ʹ�����崥��
		if(get_controlData()->use_pulse_corona){	
			if(!get_controlData()->speed_up){
				//δ���ٴ������崥��ģʽ
				digitalHi(&get_dischargeCtrlData()->mode);
				//����������־λ��ֹ�л�����״̬���
				digitalHi(&get_spdDischargeData()->remain_local_sw);
				digitalHi(&get_spdDischargeData()->remain_external_sw);
			}
		}
		
		//�߿�״̬
		if(LKEN) {	//�߿��ź���Ч
			if(LRUN){
				digitalHi(&get_controlData()->line_control);	//����������
				digitalLo(&get_controlData()->line_suspend);
			}
			else{
				if(get_controlData()->line_control)
					digitalHi(&get_controlData()->line_suspend);	//��������ͣ
			}
		}
		else{	//�߿��ź���Ч
			digitalLo(&get_controlData()->line_control);
			digitalLo(&get_controlData()->line_suspend);
		}
		
		//�ֶ�����תΪ��ѹ
		get_dischargeCtrlData()->manual_power = get_controlData()->manual_power * SAMP_MAX_VOLTAGE / get_controlData()->rated_power;
		
		//�ɼ���ѹadcתΪ������� kw
		get_dischargeCtrlData()->current_power = adc_filter_POV3V3 * get_controlData()->rated_power / MAX_POWER_ADC;
		
		//�����ٶ�ģʽѡ���ȡ��ǰ���ٶ�	��������/�ⲿ����/�����ߵ�ѹ/�����ߵ���
		get_spdDischargeData()->speed = get_speed(get_spdDischargeData()->speed_signal);
		
		//����������ͱ���
		if(get_dischargeCtrlData()->current_power < get_supervisiorData()->pwr_lo_warn){
			PLBJ = 1;
		}
		//����������߱���
		else if(get_dischargeCtrlData()->current_power > get_supervisiorData()->pwr_hi_warn){
			PHBJ = 1;
		}
		else{
			PLBJ = 0;
			PHBJ = 0;
		}
		
		//������ʳ�������ʵ�1%��˵�����ڷŵ�,�����ŵ�̵���
		if(get_dischargeCtrlData()->current_power >= get_controlData()->rated_power * 0.01f){
			FD = 1;
		}
		else{
			FD = 0;
		}
		
//		//����״̬�£�����������ʵ���������������4kw��˵��������ܿأ�һ��ʱ���ͣ��������
//		if((fabs(get_spdDischargeData()->discharge_power - get_dischargeCtrlData()->current_power) > 4.0f) && !get_dischargeCtrlData()->mode){
//			
//		}
//		
//		//����״̬�£�����������ʵ���������������4kw��˵��������ܿأ�һ��ʱ���ͣ��������
//		if((fabs(get_pulseDischargeData()->discharge_power - get_dischargeCtrlData()->current_power) > 4.0f) && get_dischargeCtrlData()->mode){
//			
//		}
		
		/************************************************************************************/
		//����յ���𱨾�����Ҫ���д��λ
		if((get_controlData()->error_sta & HIGH_VOLTAGE_SPARK_ERROR) && (get_supervisiorData()->spark_last_count != get_supervisiorData()->spark_count)){
			pulse_outputLow(&RESET_DH,50);	//���λ
			digitalLo(&get_dryCtrlData()->spark_wait);
		}
		//��ֹ�ظ�����
		get_supervisiorData()->spark_last_count = get_supervisiorData()->spark_count;
		
		/************************************************************************************/
		//����ͣ��(һЩ������ʹ�û���ͣ��)
		//����1sϵͳ�ȶ����ڼ��ͣ������
		if(get_controlData()->start_init){
			//��������ͣ��
			if(!PBLE3){		//���������������
				//�����������رض�ͣ��
				if(get_controlData()->error_sta & 0x78){
					digitalHi(&get_supervisiorData()->stop_alarm);	//ͣ������
					if(get_controlState() != __STOP)
						set_controlState(__STOP,0);	//����ͣ��
				}
			}
			//��������ͣ��
			if(get_controlData()->error_sta & 0x7F386){
				digitalHi(&get_supervisiorData()->stop_alarm);	//ͣ������
				if(get_controlState() != __STOP)
					set_controlState(__STOP,0);	//����ͣ��
			}
		}
		/************************************************************************************/
		//��ȡ�¶�
		get_controlData()->temp = get_ntc_temp(ntc_adc,(uint16_t)adc_filter_TEMP,BASE_TEMP) * 0.1f;		
		
		//������������Ϊ����3�����룬ֱ�ӳ��׽���
		if(get_controlData()->verify_password != get_controlData()->password[2]){
			//��������
			lock_to_unlock();
		
			//ʣ��ʱ�䴦��
			remain_day();
		}
		else{
			digitalLo(&get_controlData()->lock); //����
			digitalHi(&get_controlData()->pwd_ok);
			digitalClan(&get_controlData()->remain_day); 
		}
		
		
		if(get_controlData()->lock || get_controlData()->lock_tamp){ //������
			//����Ѿ�����ͣ��״̬��������ִ��
			//�����ʱ���ڷŵ�״̬������Ҫ����ͣ�����ܽ��������������޷�ֹͣ�ŵ�
			if(get_controlState() != __CORONA && get_controlState() != __STOP) //�����ǰ���Ƿŵ�״̬��ͣ��״̬
				set_controlState(__STOP,0);	//ǿ��ͣ������������״̬
			
		}
		
		/***************************test******************************/
		if(rst_error){	//���ϸ�λ
			pulse_outputHigh(&RESET_SYS,10);
			digitalLo(&rst_error);
		}
		read_cj3 = CJ3OK;
		/*************************************************************/
		
		//д��flash
		if(get_supervisiorData()->flash_sw){
			app_flash_dataUpdate();
			app_FlashWriteUdata();
			digitalLo(&get_supervisiorData()->flash_sw);
		}
		
		IWDG_Feed(); //ι��
	}
}	


void app_supervisiorTaskInit(void){
	xTaskCreate(app_supervisiorTask,"SUPERVISIOR",SUPER_STACK_SIZE,NULL,SUPER_PRIORITY,&appSupervisior.xHandleTask);
}



