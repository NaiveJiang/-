#include "app.h"

dischargeStruct_t dischargeCtrl;
dischargeStruct_t* get_dischargeCtrlData(void){
	return &dischargeCtrl;
}	
void pulse_dischargeUpdate(void);
void spd_dischargeUpdate(void);
float get_speed(uint8_t speedMode);
float get_line_set_power(uint8_t line_power_mode);
void spd_discharge_delay(void);

void app_discharge(void){
	switch(get_dischargeCtrlData()->mode){
		//����ģʽ
		case 0:{
			switch(get_dischargeCtrlData()->step){
				case 0:{		//״̬4�����ٱȽϴ���
					//�����٣�δ���ٲ��ŵ�
					if(!(get_controlData()->error_sta & SPEED_UP_ERROR)){
						//ʹ��3875�������
						STANDBY = 0;
						digitalIncreasing(&get_dischargeCtrlData()->step);
					}
				}break;
				case 1:{
					//����ٶȽ��͵��������£��ӳٷŵ�(22״̬),���ߴ����߿���ͣ(�߿�״̬�޹ش������ֱ�ӿ�ʼ��ʱ)
					if(get_controlData()->error_sta & SPEED_UP_ERROR || get_controlData()->line_suspend){
						if(!get_controlData()->use_pulse_corona){	//��������ŵ�ģʽ
							digitalHi(&get_dischargeCtrlData()->spdCtrl->delay_sw);	//������ʱ����
							digitalIncreasing(&get_dischargeCtrlData()->step);
						}
						else{
							//�ص����崥��ģʽ
							digitalHi(&get_dischargeCtrlData()->mode);
							digitalClan(&get_dischargeCtrlData()->step);
						}
					}
					//�������ٷŵ�
					else{
						spd_dischargeUpdate();
					}
				}break;
				case 2:{	//״̬22
					//�����ʱ�ִ���,�����߿��ź�LRUN��������,����ת��step1
					if(!(get_controlData()->error_sta & SPEED_UP_ERROR) || (!get_controlData()->line_suspend && get_controlData()->line_control)){
						digitalLo(&get_dischargeCtrlData()->spdCtrl->delay_sw);	//�ر���ʱ����
						digitalClan(&get_dischargeCtrlData()->spdCtrl->delay_time);	//�����ʱʱ��
						get_dischargeCtrlData()->step = 1;	//״̬��ת
					}
					else{
						//������߿�״̬�£������߿���ͣʱ��
						if(get_controlData()->line_control){
							if(get_dischargeCtrlData()->spdCtrl->delay_time >= get_dischargeCtrlData()->spdCtrl->set_line_delay_time){
								digitalIncreasing(&get_dischargeCtrlData()->step);
								digitalLo(&get_dischargeCtrlData()->spdCtrl->delay_sw);	//�ر���ʱ����
							}
						}
						else{
							if(get_dischargeCtrlData()->spdCtrl->delay_time >= get_dischargeCtrlData()->spdCtrl->set_delay_time){
								digitalIncreasing(&get_dischargeCtrlData()->step);
								digitalLo(&get_dischargeCtrlData()->spdCtrl->delay_sw);	//�ر���ʱ����
							}
						}
						
						if(get_dischargeCtrlData()->spdCtrl->delay_sw)
							spd_discharge_delay();	//�ӳٷŵ����
					}
				}break;
				case 3:{
					//�����ǰ�������С��10kw������ֱ�ӹرյ���
					if(get_dischargeCtrlData()->current_power <= 10.0f){
						digitalIncreasing(&get_dischargeCtrlData()->step);
					}
					else{	//����10kw��ÿ��0.1s����50%
						get_dischargeCtrlData()->spdCtrl->discharge_power *= 0.5f;
						dac_ch1_voltageOut(get_dischargeCtrlData()->spdCtrl->discharge_power);
						dac_ch2_voltageOut(get_dischargeCtrlData()->spdCtrl->discharge_power);
						vTaskDelay(100);	//��ʱ0.1s
					}
				}break;
				case 4:{
					//ֹͣdac���
					digitalClan(&get_dischargeCtrlData()->spdCtrl->discharge_power);
					dac_ch1_voltageOut(get_dischargeCtrlData()->spdCtrl->discharge_power);
					dac_ch2_voltageOut(get_dischargeCtrlData()->spdCtrl->discharge_power);
					
					//�ر�3875�������
					STANDBY = 1;
					
					digitalClan(&get_dischargeCtrlData()->step); //�ص����ٱȽϴ���
					//���Ϊ�߿�״̬����Ҫ�ȴ�����������
					//������ͣ״̬
					if(get_controlData()->line_suspend){
						set_controlState(__CORONA);
						get_controlData()->control_step = 6;
					}
				}break;
			}
			
		}break;
		//���崥��ģʽ
		case 1:{
			//δ���ٰ������崥��
			if(get_controlData()->error_sta & SPEED_UP_ERROR){
				pulse_dischargeUpdate();	//����ŵ�
			}
			else{	//���ٽ������ٿ���
				digitalLo(&get_dischargeCtrlData()->mode);
				digitalClan(&get_dischargeCtrlData()->step);
			}
		}break;
		default:break;
	}
}


//���崥���ŵ�
void pulse_dischargeUpdate(void){
	//���㹦�������ĵ�ѹ
	get_dischargeCtrlData()->pulseCtrl->inc_power = INCREASE_DISCHARGE_MAX100MS * SAMP_MAX_VOLTAGE / get_controlData()->rated_power;
	//���崥����־�������ŵ�
	if(!get_dischargeCtrlData()->pulseCtrl->discharge_sw){
		//�������ʱ��
		digitalClan(&get_dischargeCtrlData()->inc_time);
		//�����ǰ�������С��10kw������ֱ�ӹرյ���
		if(get_dischargeCtrlData()->current_power <= 10.0f){
			
			//ֹͣ�ŵ�
			digitalClan(&get_dischargeCtrlData()->pulseCtrl->discharge_power);
			dac_ch1_voltageOut(get_dischargeCtrlData()->pulseCtrl->discharge_power);
			dac_ch2_voltageOut(get_dischargeCtrlData()->pulseCtrl->discharge_power);
			
			//�ر�3875�������
			STANDBY = 1;
			
			//���Ϊ�߿�״̬����Ҫ�ȴ�����������
			//������ͣ״̬
			if(get_controlData()->line_suspend){
				set_controlState(__CORONA);
				get_controlData()->control_step = 6;
			}
		}
		else{	//����10kw��ÿ��0.1s����50%
			get_dischargeCtrlData()->pulseCtrl->discharge_power *= 0.5f;
			dac_ch1_voltageOut(get_dischargeCtrlData()->pulseCtrl->discharge_power);
			dac_ch2_voltageOut(get_dischargeCtrlData()->pulseCtrl->discharge_power);
			vTaskDelay(100);	//��ʱ0.1s
		}
	}
	else{
		//ʹ��3875�������
		STANDBY = 0;
		//���
		dac_ch1_voltageOut(get_dischargeCtrlData()->pulseCtrl->discharge_power);
		dac_ch2_voltageOut(get_dischargeCtrlData()->pulseCtrl->discharge_power);
	}
}


void spd_dischargeUpdate(void){
	//���ʿ���ģʽ����
	get_dischargeCtrlData()->power_ctrlState = get_ctrlSetData(get_ctrlSetdata()->ctrlSet_rev_data,POWERMODE);
	//���ݹ��ʿ���ģʽ����
	switch(get_dischargeCtrlData()->power_ctrlState){
		case MANUAL_MODE:{	//�ֶ�ģʽ����100ms 2kw�������ӵ������
			//���㹦�������ĵ�ѹ
			get_dischargeCtrlData()->spdCtrl->inc_power = INCREASE_DISCHARGE_MAX100MS * SAMP_MAX_VOLTAGE / get_controlData()->rated_power;
			
			//�����޷�
			if(!get_controlData()->line_control){		//���߿�ģʽ��ʹ����Ļ���õ��ֶ�����dac�޷�
				if(get_dischargeCtrlData()->spdCtrl->discharge_power >= get_dischargeCtrlData()->manual_power){
					get_dischargeCtrlData()->spdCtrl->discharge_power = get_dischargeCtrlData()->manual_power;
				}
			}
			else{	//�߿�ģʽ��ʹ�������߸�������dac����޷� = �����߹��ʸ���adc����(�ٷֱ�) * �����(��ƫ��ѹ)
				get_dischargeCtrlData()->line_set_power = get_line_set_power(get_dischargeCtrlData()->line_power_mode);
				if(get_dischargeCtrlData()->spdCtrl->discharge_power >= get_dischargeCtrlData()->line_set_power){
					get_dischargeCtrlData()->spdCtrl->discharge_power = get_dischargeCtrlData()->line_set_power;
				}
			}
			
		}break;
		
		case POWER_DENSITY_MODE:{	//�����ܶ�ģʽ
			//�����ٶ�ģʽѡ���ȡ��ǰ���ٶ�	��������/�ⲿ����/�����ߵ�ѹ/�����ߵ���
			get_dischargeCtrlData()->spdCtrl->speed = get_speed(get_dischargeCtrlData()->spdCtrl->speed_signal);
			//�����������=���������ܶ�*��ǰ���ٶ�*������
			get_dischargeCtrlData()->spdCtrl->power_density = (float)get_powSetData()->power_density;	//��ȡ���������ܶ�
			get_dischargeCtrlData()->spdCtrl->discharge_power = get_dischargeCtrlData()->spdCtrl->power_density * get_dischargeCtrlData()->spdCtrl->speed * get_dischargeCtrlData()->spdCtrl->roller_width;
			//�������תΪdac���
			get_dischargeCtrlData()->spdCtrl->discharge_power *= SAMP_MAX_VOLTAGE / get_controlData()->rated_power;
		}break;
		
		case SPEED_MODE:{	//�ٱ�ģʽ
			//�����ٶ�ģʽѡ���ȡ��ǰ���ٶ�	��������/�ⲿ����/�����ߵ�ѹ/�����ߵ���
			get_dischargeCtrlData()->spdCtrl->speed = get_speed(get_dischargeCtrlData()->spdCtrl->speed_signal);
			//�������ϵ��
			get_dischargeCtrlData()->spdCtrl->scale = (get_dischargeCtrlData()->spdCtrl->power_scale[1] - get_dischargeCtrlData()->spdCtrl->power_scale[0]) / (get_dischargeCtrlData()->spdCtrl->speed_scale[1] - get_dischargeCtrlData()->spdCtrl->speed_scale[0]);
			//���ݱ���ϵ���������ǰ���������
			get_dischargeCtrlData()->spdCtrl->discharge_power = get_dischargeCtrlData()->spdCtrl->scale * get_dischargeCtrlData()->spdCtrl->speed;
			//�����ǰ�ٶ�������Ĺ���С����С������ʣ�������С����������
			if(get_dischargeCtrlData()->spdCtrl->discharge_power < get_dischargeCtrlData()->low_power)
				get_dischargeCtrlData()->spdCtrl->discharge_power = get_dischargeCtrlData()->low_power;
			//�������תΪdac���
			get_dischargeCtrlData()->spdCtrl->discharge_power *= SAMP_MAX_VOLTAGE / get_controlData()->rated_power;
		}break;
	}
	
	//���
	dac_ch1_voltageOut(get_dischargeCtrlData()->spdCtrl->discharge_power);
	dac_ch2_voltageOut(get_dischargeCtrlData()->spdCtrl->discharge_power);
}

//�����ӳٷŵ����
void spd_discharge_delay(void){
	//���ʿ���ģʽ����
	get_dischargeCtrlData()->power_ctrlState = get_ctrlSetData(get_ctrlSetdata()->ctrlSet_rev_data,POWERMODE);
	//���ݹ��ʿ���ģʽ����
	switch(get_dischargeCtrlData()->power_ctrlState){
		case POWER_DENSITY_MODE:{	//�����ܶ�ģʽ��������С����С���ʰ�����С���ʷŵ�
			//�����ٶ�ģʽѡ���ȡ��ǰ���ٶ�	��������/�ⲿ����/�����ߵ�ѹ/�����ߵ���
			get_dischargeCtrlData()->spdCtrl->speed = get_speed(get_dischargeCtrlData()->spdCtrl->speed_signal);
			//�����������=���������ܶ�*��ǰ���ٶ�*������
			get_dischargeCtrlData()->spdCtrl->power_density = (float)get_powSetData()->power_density;	//��ȡ���������ܶ�
			get_dischargeCtrlData()->spdCtrl->discharge_power = get_dischargeCtrlData()->spdCtrl->power_density * get_dischargeCtrlData()->spdCtrl->speed * get_dischargeCtrlData()->spdCtrl->roller_width;
			if(get_dischargeCtrlData()->spdCtrl->discharge_power < get_dischargeCtrlData()->low_power)
				get_dischargeCtrlData()->spdCtrl->discharge_power = get_dischargeCtrlData()->low_power;
			//�������תΪdac���
			get_dischargeCtrlData()->spdCtrl->discharge_power *= SAMP_MAX_VOLTAGE / get_controlData()->rated_power;
		}break;
		case SPEED_MODE:{
			//�����ٶ�ģʽѡ���ȡ��ǰ���ٶ�	��������/�ⲿ����/�����ߵ�ѹ/�����ߵ���
			get_dischargeCtrlData()->spdCtrl->speed = get_speed(get_dischargeCtrlData()->spdCtrl->speed_signal);
			//�������ϵ��
			get_dischargeCtrlData()->spdCtrl->scale = (get_dischargeCtrlData()->spdCtrl->power_scale[1] - get_dischargeCtrlData()->spdCtrl->power_scale[0]) / (get_dischargeCtrlData()->spdCtrl->speed_scale[1] - get_dischargeCtrlData()->spdCtrl->speed_scale[0]);
			//���ݱ���ϵ���������ǰ���������
			get_dischargeCtrlData()->spdCtrl->discharge_power = get_dischargeCtrlData()->spdCtrl->scale * get_dischargeCtrlData()->spdCtrl->speed;
			//�����ǰ�ٶ�������Ĺ���С����С������ʣ�������С����������
			if(get_dischargeCtrlData()->spdCtrl->discharge_power < get_dischargeCtrlData()->low_power)
				get_dischargeCtrlData()->spdCtrl->discharge_power = get_dischargeCtrlData()->low_power;
			//�������תΪdac���
			get_dischargeCtrlData()->spdCtrl->discharge_power *= SAMP_MAX_VOLTAGE / get_controlData()->rated_power;
		}break;
		default: break; //�ֶ�ģʽ����ԭ����
	}
	//���
	dac_ch1_voltageOut(get_dischargeCtrlData()->spdCtrl->discharge_power);
	dac_ch2_voltageOut(get_dischargeCtrlData()->spdCtrl->discharge_power);
}



//�ٶ��ź�ѡ��
float get_speed(uint8_t speedMode){
	float speed;
	switch(speedMode){
		case 0:{		//��������
			speed = get_dischargeCtrlData()->spdCtrl->local_speed;
		}break;
		case 1:{		//�ⲿ����
			speed = get_dischargeCtrlData()->spdCtrl->external_speed;
		}break;
		case 2:{		//�����ߵ�ѹ
			//���㵱ǰ�ٶ�=�����ߵ�ѹ�ٷֱ� * ����趨����
			get_dischargeCtrlData()->spdCtrl->line_voltage_speed = adc_filter_LSPV3V3 * get_controlData()->set_spd_max;
			speed = get_dischargeCtrlData()->spdCtrl->line_voltage_speed;
		}break;
		case 3:{		//�����ߵ���
			//���㵱ǰ�ٶ�=�����ߵ����ٷֱ� * ����趨����
			get_dischargeCtrlData()->spdCtrl->line_current_speed = adc_filter_LSPI3V3 * get_controlData()->set_spd_max;
			speed = get_dischargeCtrlData()->spdCtrl->line_current_speed;
		}break;
		default:break;
	}
	return speed;
}

float get_line_set_power(uint8_t line_power_mode){
	float power;
	switch(line_power_mode){
		case 0:{
			power = adc_filter_LPV3V3 * SAMP_MAX_VOLTAGE;
		}break;
		case 1:{
			power = adc_LPIMA3V3 * SAMP_MAX_VOLTAGE;
		}break;
	}
	return power;
}

