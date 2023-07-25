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
	parameter[POWER] = (uint32_t)get_controlData()->rated_power; 		//额定功率
	parameter[SET_STATE] = (uint32_t)get_powSetData()->set_state;		//工作状态
	parameter[POWER_DENSITY] = (uint32_t)get_dischargeCtrlData()->power_density; //功率密度 kw/m2
	parameter[MAX_SPEED] = (uint32_t)get_spdDischargeData()->max_spd;	//最大速度
	parameter[SPD_MAX_POW] = (uint32_t)(get_spdDischargeData()->spd_max_pow * 100);	//速比最大功率
	parameter[SPD_MIN_POW] = (uint32_t)(get_spdDischargeData()->spd_min_pow * 100);	//速比最小功率
	parameter[MANUAL_POWER] = (uint32_t)(get_controlData()->manual_power * 100);	//手动功率
	parameter[SPD_UP] = (uint32_t)(get_controlData()->set_speed_up * 100);			//最低达速
	parameter[LOW_POWER] = (uint32_t)(get_dischargeCtrlData()->low_power * 100);	//最低功率
	parameter[SET_SPD_DELAY] = get_spdDischargeData()->set_delay_time;
	parameter[SET_PULSE_DELAY] = get_pulseDischargeData()->set_delay_time;
	parameter[SET_REMAIN] = get_spdDischargeData()->set_remain_time;
	get_spdDischargeData()->set_line_delay_time = parameter[SET_LINE_DELAY];
	parameter[SET_DELAY_LENGTH1] = (uint32_t)(get_rcCtrlData()->set_delay_length1 * 100);
	parameter[SET_DELAY_LENGTH2] = (uint32_t)(get_rcCtrlData()->set_delay_length2 * 100);
	parameter[ROLLER_WIDTH] = (uint32_t)(get_spdDischargeData()->roller_width * 100);
}

