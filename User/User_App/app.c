#include "app.h"
#include "GUI.h"

uint32_t parameter[NAME_OF_LIST];
appStruct_t app;
uint8_t version;
//默认参数初始化
void app_ParameterInit(void){
	
	{
		parameter[NAME_VERSION] = VERSION;
		
		parameter[POWER] = DEAFULT_POWER;
		
		parameter[SET_STATE] = DEAFULT_SET_STATE;
		
		parameter[POWER_DENSITY] = DEAFULT_POWER_DENSITY;
		
		parameter[MAX_POW] = DEAFULT_MAX_POW;
		
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
		
		parameter[C_SEC] = DEAFULT_SEC;
		
		parameter[C_HOR] = DEAFULT_HOR;
		
		parameter[C_DAY1] = DEAFULT_DAY1;
		
		parameter[C_DAY2] = DEAFULT_DAY2;
		
		parameter[C_DAY3] = DEAFULT_DAY3;
		
		parameter[PWD1] = DEAFULT_PASSWORD1;
		
		parameter[PWD2] = DEAFULT_PASSWORD2;
		
		parameter[PWD3] = DEAFULT_PASSWORD3;
		
		parameter[V_PWD] = DEAFULT_VERIFY_PASSWORD;
		
	}
	
	//从flash中读取版本号
	bsp_ReadCpuFlash(FLASH_SAVE_START_ADDR + NAME_VERSION,(uint8_t *)&version, 2);
	
	//如果版本号为0xFF，即flash无内容
	if(version == 0xFF){
		app_FlashWriteUdata();
	}
	//flash中存在内容，则版本号不为0xFF，读取flash中的内容
	else{
		app_FlashReadUdata();
	}
}

void app_ControlParameterLoad(void){
	//读取flash的控制信息
	get_controlData()->rated_power = (float)parameter[POWER] * 1e-2f;
	
	get_powSetData()->set_state = parameter[SET_STATE];
	
	//获取控制参数
	get_dischargeCtrlData()->power_density = (float)parameter[POWER_DENSITY];
	get_spdDischargeData()->max_spd = (float)parameter[MAX_SPEED] * 1e-2f;
	
	get_spdDischargeData()->max_pow = (float)parameter[MAX_POW] * 1e-2f;
	
	get_controlData()->manual_power = (float)parameter[MANUAL_POWER] * 1e-2f;
	get_controlData()->set_speed_up = (float)parameter[SPD_UP] * 1e-2f;
	get_dischargeCtrlData()->low_power = (float)parameter[LOW_POWER] * 1e-2f;
	
	get_spdDischargeData()->set_delay_time = parameter[SET_SPD_DELAY];
	get_pulseDischargeData()->set_delay_time = parameter[SET_PULSE_DELAY];
	
	get_spdDischargeData()->set_remain_time = parameter[SET_REMAIN];
	get_spdDischargeData()->set_line_delay_time = parameter[SET_LINE_DELAY];
	
	get_rcCtrlData()->set_delay_length1 = (float)parameter[SET_DELAY_LENGTH1] * 1e-2f;
	get_rcCtrlData()->set_delay_length2 = (float)parameter[SET_DELAY_LENGTH2] * 1e-2f;
	
	//获取机器运行时间
	get_controlData()->rtc_sec = parameter[C_SEC];
	get_controlData()->rtc_hour = parameter[C_HOR];
	get_controlData()->rtc_day1 = parameter[C_DAY1];
	get_controlData()->rtc_day2 = parameter[C_DAY2];
	get_controlData()->rtc_day3 = parameter[C_DAY3];
	
	//机器密码
	get_controlData()->password[0] = parameter[PWD1];
	get_controlData()->password[1] = parameter[PWD2];
	get_controlData()->password[2] = parameter[PWD3];
	
	get_controlData()->verify_password = parameter[V_PWD];
	
	//得到滚筒单个脉冲所占线长 
	get_spdDischargeData()->roller_pulse_length = (float)parameter[ROLLER_DIAMETER_LOCAL] * (1e-5f) * PI / (float)parameter[ROLLER_PULSE_LOCAL]; //本地滚轴
	get_spdDischargeData()->external_pulase_length = (float)parameter[ROLLER_DIAMETER_EXTERNAL] * (1e-5f) * PI / (float)parameter[ROLLER_PULSE_EXTERNAL]; //外地滚轴
	get_dryCtrlData()->max_power = get_controlData()->rated_power * 0.5f;	//湿启动功率为额定功率的50%
	
	//得到滚筒宽度
	get_spdDischargeData()->roller_width = (float)parameter[ROLLER_WIDTH] * 1e-2f;
}



