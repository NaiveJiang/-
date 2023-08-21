#include "app.h"



//FLASH写入
void app_FlashWriteUdata(void)
{
	uint8_t i = 0;
	for(i = 0;i < NAME_OF_LIST;i++){
		bsp_WriteCpuFlash(FLASH_SAVE_START_ADDR + (i*2),(uint8_t *)&parameter[i], 2);
	}
}

//FLASH读取
void app_FlashReadUdata(void)
{
	uint8_t i = 0;
	for(i = 1;i < NAME_OF_LIST;i++){
		bsp_ReadCpuFlash(FLASH_SAVE_START_ADDR + (i*2),(uint8_t *)&parameter[i], 2);
	}
}

//flash数据保存更新
void app_flash_dataUpdate(void){
	parameter[POWER] = (uint32_t)(get_controlData()->rated_power * 100); 			//额定功率
	parameter[MAX_POW] = (uint32_t)(get_spdDischargeData()->max_pow * 100);			//线速最大功率
	parameter[LOW_POWER] = (uint32_t)(get_dischargeCtrlData()->low_power * 100);	//最低功率
	parameter[MAX_SPEED] = (uint32_t)(get_spdDischargeData()->max_spd * 100);		//最大速度
	parameter[ROLLER_WIDTH] = (uint32_t)(get_spdDischargeData()->roller_width * 100);	//放电宽度
	//滚筒参数已在串口更新
	parameter[SPD_UP] = (uint32_t)(get_controlData()->set_speed_up * 100);			//最低达速
	parameter[SET_SPD_DELAY] = get_spdDischargeData()->set_delay_time;
	parameter[SET_LINE_DELAY] = get_spdDischargeData()->set_line_delay_time;
	parameter[SET_PULSE_DELAY] = get_pulseDischargeData()->set_delay_time;
	parameter[SET_DELAY_LENGTH1] = (uint32_t)(get_rcCtrlData()->set_delay_length1 * 100);
	parameter[SET_DELAY_LENGTH2] = (uint32_t)(get_rcCtrlData()->set_delay_length2 * 100);
	parameter[SET_STATE] = (uint32_t)get_powSetData()->set_state;						//工作状态
	
	parameter[POWER_DENSITY] = (uint32_t)get_dischargeCtrlData()->power_density; 		//功率密度 kw/m2
	parameter[MANUAL_POWER] = (uint32_t)(get_controlData()->manual_power * 100);		//手动功率
	
	parameter[SET_REMAIN] = get_spdDischargeData()->set_remain_time;
	
	parameter[C_SEC] = get_controlData()->rtc_sec;
	parameter[C_HOR] = get_controlData()->rtc_hour;
	parameter[C_DAY1] = get_controlData()->rtc_day1;
	parameter[C_DAY2] = get_controlData()->rtc_day2;
	parameter[C_DAY3] = get_controlData()->rtc_day3;
	
	parameter[PWD1] = get_controlData()->password[0];
	parameter[PWD1] = get_controlData()->password[1];
	parameter[PWD1] = get_controlData()->password[2];
	
	parameter[V_PWD] = get_controlData()->verify_password;
}

