#include "app.h"
#include "GUI.h"

uint32_t parameter[NAME_OF_LIST];
appStruct_t app;
uint8_t version;
//Ĭ�ϲ�����ʼ��
void app_ParameterInit(void){
	
	{
		parameter[NAME_VERSION] = VERSION;
		
		parameter[POWER] = DEAFULT_POWER;
		
		parameter[SET_STATE] = DEAFULT_SET_STATE;
		
		parameter[POWER_DENSITY] = DEAFULT_POWER_DENSITY;
		
		parameter[SPD_MAX_POW] = DEAFULT_SPD_MAX_POW;
		
		parameter[SPD_MIN_POW] = DEAFULT_SPD_MIN_POW;
		
		parameter[MANUAL_POWER] = DEAFULT_MANUAL_POWER;
		
		parameter[MAX_SPEED] = DEAFULT_MAX_SPEED;
		
		parameter[LOW_POWER] = DEAFULT_LOW_POWER;
		
		parameter[SET_SPD_DELAY] = DEAFULT_SET_SPD_DELAY_TIME;
		
		parameter[SET_PULSE_DELAY] = DEAFULT_SET_PULSE_DELAY_TIME;
		
		parameter[SET_REMAIN] = DEAFULT_SET_REMAIN_DELAY_TIME;
		
		parameter[SET_LINE_DELAY] = DEAFULT_SET_LINE_DELAY_TIME;
		
		parameter[SET_DELAY_LENGTH1] = DEAFULT_SET_DELAY_LENGTH1;
		
		parameter[SET_DELAY_LENGTH2] = DEAFULT_SET_DELAY_LENGTH2;
		
		parameter[SPD_UP] = DEAFULT_SPD_UP;
		
		parameter[ROLLER_DIAMETER_LOCAL] = DEAFULT_ROLLER_DIAMETER_LOACAL;
		
		parameter[ROLLER_DIAMETER_EXTERNAL] = DEAFULT_ROLLER_DIAMETER_EXTERNAL;
		
		parameter[ROLLER_WIDTH] = DEAFULT_ROLLER_WIDTH;
		
		parameter[ROLLER_PULSE_LOCAL] = DEAFULT_ROLLER_PULSE_LOCAL;
		
		parameter[ROLLER_PULSE_EXTERNAL] = DEAFULT_ROLLER_PULSE_EXTERNAL;
		
	}
	
	//��flash�ж�ȡ�汾��
	bsp_ReadCpuFlash(FLASH_SAVE_START_ADDR + NAME_VERSION,(uint8_t *)&version, 2);
	
	//����汾��Ϊ0xFF����flash������
	if(version == 0xFF){
		app_FlashWriteUdata();
	}
	//flash�д������ݣ���汾�Ų�Ϊ0xFF����ȡflash�е�����
	else{
		app_FlashReadUdata();
	}
}

void app_ControlParameterLoad(void){
	//��ȡflash�Ŀ�����Ϣ
	get_controlData()->rated_power = (float)parameter[POWER] * 0.01f;
	
	get_powSetData()->set_state = parameter[SET_STATE];
	
	//��ȡ���Ʋ���
	get_dischargeCtrlData()->power_density = (float)parameter[POWER_DENSITY];
	get_spdDischargeData()->max_spd = (float)parameter[MAX_SPEED];
	
	get_spdDischargeData()->spd_max_pow = (float)parameter[SPD_MAX_POW] * 0.01f;
	get_spdDischargeData()->spd_min_pow = (float)parameter[SPD_MIN_POW] * 0.01f;
	
	get_controlData()->manual_power = (float)parameter[MANUAL_POWER] * 0.01f;
	get_controlData()->set_speed_up = (float)parameter[SPD_UP] * 0.01f;
	get_dischargeCtrlData()->low_power = (float)parameter[LOW_POWER] * 0.01f;
	
	get_spdDischargeData()->set_delay_time = parameter[SET_SPD_DELAY];
	get_pulseDischargeData()->set_delay_time = parameter[SET_PULSE_DELAY];
	get_spdDischargeData()->set_remain_time = parameter[SET_REMAIN];
	get_spdDischargeData()->set_line_delay_time = parameter[SET_LINE_DELAY];
	get_rcCtrlData()->set_delay_length1 = (float)parameter[SET_DELAY_LENGTH1] * 0.01f;
	get_rcCtrlData()->set_delay_length2 = (float)parameter[SET_DELAY_LENGTH2] * 0.01f;
	
	//�õ���Ͳ����������ռ�߳� 
	get_spdDischargeData()->roller_pulse_length = (float)parameter[ROLLER_DIAMETER_LOCAL] * 0.01f * PI / (float)parameter[ROLLER_PULSE_LOCAL]; //���ع���
	get_spdDischargeData()->external_pulase_length = (float)parameter[ROLLER_DIAMETER_EXTERNAL] * 0.01f * PI / (float)parameter[ROLLER_PULSE_EXTERNAL]; //��ع���
	get_dryCtrlData()->max_power = get_controlData()->rated_power * 0.5f;	//ʪ��������Ϊ����ʵ�50%
	
	//�õ���Ͳ���
	get_spdDischargeData()->roller_width = (float)parameter[ROLLER_WIDTH] * 0.01f;
}



