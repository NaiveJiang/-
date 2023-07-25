#include "app.h"

roll_changingStruct_t rc_ctrl;
roll_changingStruct_t *get_rcCtrlData(void){
	return &rc_ctrl;
}


void app_roll_changing(void){
	uint8_t le_error;
	switch(get_rcCtrlData()->step){
		case 0:{ //�����ӳٷŵ�
			digitalHi(&get_rcCtrlData()->delay_sw);	//�����ӳٴ���
			digitalIncreasing(&get_rcCtrlData()->step);
		}break;
		case 1:{
			//���㵱ǰ��λ�� m
			get_rcCtrlData()->length += get_rcCtrlData()->speed * 0.01666f * 0.01f;
			if(get_rcCtrlData()->length >= get_rcCtrlData()->set_delay_length1){
				digitalLo(&get_rcCtrlData()->delay_sw);
				digitalClan(&get_rcCtrlData()->length);
				digitalIncreasing(&get_rcCtrlData()->step);
			}
			
			if(get_rcCtrlData()->delay_sw){
				//�����һ��״̬Ϊ����״̬
				if(get_last_controlState() == __CORONA){
					//�ӳٷŵ�
					if(get_dischargeCtrlData()->mode){
						pulse_dischargeUpdate();
					}
					else{
						spd_discharge_delay();
					}
				}
			}
		}break;
		case 2:{
			//�����ǰ�������С��10kw������ֱ�ӹرյ���
			if(get_dischargeCtrlData()->current_power <= 10.0f){
				digitalIncreasing(&get_rcCtrlData()->step);
			}
			else{	
				if(get_dischargeCtrlData()->mode){	//���崥��״̬
					//����10kw��ÿ��0.1s����50%
					get_pulseDischargeData()->discharge_power *= 0.5f;
					dac_ch1_voltageOut(get_pulseDischargeData()->discharge_power);
					dac_ch2_voltageOut(get_pulseDischargeData()->discharge_power);
				}
				else{	//����״̬
					//����10kw��ÿ��0.1s����50%
					get_spdDischargeData()->discharge_power *= 0.5f;
					dac_ch1_voltageOut(get_spdDischargeData()->discharge_power);
					dac_ch2_voltageOut(get_spdDischargeData()->discharge_power);
				}
				vTaskDelay(100);	//��ʱ0.1s
			}
		}break;
		case 3:{
			//ֹͣdac���
			digitalClan(&get_spdDischargeData()->discharge_power);
			digitalClan(&get_pulseDischargeData()->discharge_power);
			dac_ch1_voltageOut(0.0f);
			dac_ch2_voltageOut(0.0f);
			
			//�ر�3875�������
			STANDBY = 1;
			PBLE3 = 1;	//����LE3���ڼ��缫�պ�
			
			//�򿪵缫
			DJCLOSE = 1;
			digitalHi(&get_rcCtrlData()->delay_sw);	//�����ӳٴ���
			
			digitalIncreasing(&get_rcCtrlData()->step);
		}break;
		case 4:{
			//���㵱ǰ��λ�� m
			get_rcCtrlData()->length += get_rcCtrlData()->speed * 0.01666f * 0.01f;
			//��ʱ�ȴ�
			if(get_rcCtrlData()->length >= get_rcCtrlData()->set_delay_length2){
				DJCLOSE = 0; //�պϵ缫
				digitalLo(&get_rcCtrlData()->delay_sw);
				digitalClan(&get_rcCtrlData()->length);
			}
			if(!get_rcCtrlData()->delay_sw){
				//�ȴ��缫�պ�
				//���LE�����жϵ缫��λ
				le_error = (uint8_t)LE0 | (uint8_t)(LE1 << 1) | (uint8_t)(LE2 << 2);
			
				if(le_error != 2){	//���������2����缫��λ
					get_rcCtrlData()->step = 99;
				}
			}
		}break;
		case 99:{
			HJSHDL = 0;	//������ʱ�ر�
			digitalLo(&get_rcCtrlData()->state);	//���㻻���־
			digitalClan(&get_rcCtrlData()->step);	
			set_controlState(get_last_controlState(),get_controlData()->last_control_step);	//�ص�ԭ����״̬
		}break;
	}
}







