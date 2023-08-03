#include "app.h"



//FLASHд��
void app_FlashWriteUdata(void)
{
	uint8_t i = 0;
	for(i = 0;i < NAME_OF_LIST;i++){
		bsp_WriteCpuFlash(FLASH_SAVE_START_ADDR + (i*2),(uint8_t *)&parameter[i], 2);
	}
}

//FLASH��ȡ
void app_FlashReadUdata(void)
{
	uint8_t i = 0;
	for(i = 1;i < NAME_OF_LIST;i++){
		bsp_ReadCpuFlash(FLASH_SAVE_START_ADDR + (i*2),(uint8_t *)&parameter[i], 2);
	}
}

//flash���ݱ������
void app_flash_dataUpdate(void){
	parameter[POWER] = (uint32_t)(get_controlData()->rated_power * 100); 			//�����
	parameter[MAX_POW] = (uint32_t)(get_spdDischargeData()->max_pow * 100);			//���������
	parameter[LOW_POWER] = (uint32_t)(get_dischargeCtrlData()->low_power * 100);	//��͹���
	parameter[MAX_SPEED] = (uint32_t)(get_spdDischargeData()->max_spd * 100);		//����ٶ�
	parameter[ROLLER_WIDTH] = (uint32_t)(get_spdDischargeData()->roller_width * 100);	//�ŵ���
	//��Ͳ�������ڴ��ڸ���
	parameter[SPD_UP] = (uint32_t)(get_controlData()->set_speed_up * 100);			//��ʹ���
	parameter[SET_SPD_DELAY] = get_spdDischargeData()->set_delay_time;
	parameter[SET_LINE_DELAY] = get_spdDischargeData()->set_line_delay_time;
	parameter[SET_PULSE_DELAY] = get_pulseDischargeData()->set_delay_time;
	parameter[SET_DELAY_LENGTH1] = (uint32_t)(get_rcCtrlData()->set_delay_length1 * 100);
	parameter[SET_DELAY_LENGTH2] = (uint32_t)(get_rcCtrlData()->set_delay_length2 * 100);
	parameter[SET_STATE] = (uint32_t)get_powSetData()->set_state;						//����״̬
	
	parameter[POWER_DENSITY] = (uint32_t)get_dischargeCtrlData()->power_density; 		//�����ܶ� kw/m2
	parameter[MANUAL_POWER] = (uint32_t)(get_controlData()->manual_power * 100);	//�ֶ�����
	
	parameter[SET_REMAIN] = get_spdDischargeData()->set_remain_time;
}

