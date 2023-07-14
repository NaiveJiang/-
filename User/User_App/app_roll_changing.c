#include "app.h"

roll_changingStruct_t rc_ctrl;
roll_changingStruct_t *get_rcCtrlData(void){
	return &rc_ctrl;
}


void app_roll_changing(void){
	uint8_t le_error;
	switch(get_rcCtrlData()->step){
		case 0:{ //触发延迟放电
			digitalHi(&get_rcCtrlData()->delay_sw);	//开启延迟触发
			digitalIncreasing(&get_rcCtrlData()->step);
		}break;
		case 1:{
			//计算当前的位移 m
			get_rcCtrlData()->length += (uint16_t)(get_rcCtrlData()->speed * 0.01666f * 0.01f);
			if(get_rcCtrlData()->length >= get_rcCtrlData()->set_delay_length1){
				digitalLo(&get_rcCtrlData()->delay_sw);
				digitalClan(&get_rcCtrlData()->length);
				digitalIncreasing(&get_rcCtrlData()->step);
			}
			
			if(get_rcCtrlData()->delay_sw){
				//如果上一次状态为电晕状态
				if(get_last_controlState() == __CORONA){
					//延迟放电
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
			//如果当前输出功率小于10kw，可以直接关闭电晕
			if(get_dischargeCtrlData()->current_power <= 10.0f){
				digitalIncreasing(&get_rcCtrlData()->step);
			}
			else{	
				if(get_dischargeCtrlData()->mode){	//脉冲触发状态
					//超出10kw则每隔0.1s降低50%
					get_pulseDischargeData()->discharge_power *= 0.5f;
					dac_ch1_voltageOut(get_pulseDischargeData()->discharge_power);
					dac_ch2_voltageOut(get_pulseDischargeData()->discharge_power);
				}
				else{	//线速状态
					//超出10kw则每隔0.1s降低50%
					get_spdDischargeData()->discharge_power *= 0.5f;
					dac_ch1_voltageOut(get_spdDischargeData()->discharge_power);
					dac_ch2_voltageOut(get_spdDischargeData()->discharge_power);
				}
				vTaskDelay(100);	//延时0.1s
			}
		}break;
		case 3:{
			//停止dac输出
			digitalClan(&get_spdDischargeData()->discharge_power);
			digitalClan(&get_pulseDischargeData()->discharge_power);
			dac_ch1_voltageOut(0.0f);
			dac_ch2_voltageOut(0.0f);
			
			//关闭3875移相输出
			STANDBY = 1;
			PBLE3 = 1;	//屏蔽LE3用于检测电极闭合
			
			//打开电极
			DJCLOSE = 1;
			digitalHi(&get_rcCtrlData()->delay_sw);	//开启延迟触发
			
			digitalIncreasing(&get_rcCtrlData()->step);
		}break;
		case 4:{
			//计算当前的位移 m
			get_rcCtrlData()->length += (uint16_t)(get_rcCtrlData()->speed * 0.01666f * 0.01f);
			//延时等待
			if(get_rcCtrlData()->length >= get_rcCtrlData()->set_delay_length2){
				DJCLOSE = 0; //闭合电极
				digitalLo(&get_rcCtrlData()->delay_sw);
				digitalClan(&get_rcCtrlData()->length);
			}
			if(!get_rcCtrlData()->delay_sw){
				//等待电极闭合
				//检测LE编码判断电极到位
				le_error = (uint8_t)LE0 | (uint8_t)(LE1 << 1) | (uint8_t)(LE2 << 2);
			
				if(le_error != 2){	//如果不等于2，则电极到位
					get_rcCtrlData()->step = 99;
				}
			}
		}break;
		case 99:{
			HJSHDL = 0;	//换卷延时关闭
			digitalLo(&get_rcCtrlData()->state);	//清零换卷标志
			digitalClan(&get_rcCtrlData()->step);	
			set_controlState(get_last_controlState(),get_controlData()->last_control_step);	//回到原来的状态
		}break;
	}
}







