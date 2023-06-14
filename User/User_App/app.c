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
		
		parameter[CONTROL_MODE] = DEAFULT_CONTROL_MODE;
		
		parameter[POWER_MODE] = DEAFULT_POWER_MODE;
		
		parameter[SPD_SIGNAL] = DEAFULT_SPD_SIGNAL;
		
		parameter[POWER_DENSITY] = DEAFULT_POWER_DENSITY;
		
		parameter[SPD_MAX] = DEAFULT_SPD_MAX;
		
		parameter[SPD_MIN] = DEAFULT_SPD_MIN;
		
		parameter[MANUAL_POWER] = DEAFULT_MANUAL_POWER;
		
		parameter[SPD_UP] = DEAFULT_SPD_UP;
		
		parameter[ROLLER_DIAMETER] = DEAFULT_ROLLER_DIAMETER;
		
		parameter[ROLLER_WIDTH] = DEAFULT_ROLLER_WIDTH;
		
		parameter[ROLLER_PULSE] = DEAFULT_ROLLER_PULSE;
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
	
	get_controlData()->control_mode = parameter[CONTROL_MODE];
	get_controlData()->power_mode = parameter[POWER_MODE];
	get_controlData()->speed_signal = parameter[SPD_SIGNAL];
	
	//��ȡ���Ʋ���
	get_controlData()->power_density = (uint16_t)parameter[POWER_DENSITY];
	get_controlData()->speed_max = (float)parameter[SPD_MAX] * 0.01f;
	get_controlData()->speed_min = (float)parameter[SPD_MIN] * 0.01f;
	get_controlData()->manual_power = (float)parameter[MANUAL_POWER] * 0.01f;
	get_controlData()->set_speed_up = (float)parameter[SPD_UP] * 0.01f;
	
	//�õ���Ͳ�ܳ�
	get_dischargeCtrlData()->spdCtrl->roller_pulse_length = (float)parameter[ROLLER_DIAMETER] * 0.01f * PI / (float)parameter[ROLLER_PULSE];
	//�õ���Ͳ���
	get_dischargeCtrlData()->spdCtrl->roller_width = (float)parameter[ROLLER_WIDTH];
}



