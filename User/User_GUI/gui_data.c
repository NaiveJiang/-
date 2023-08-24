#include "app.h"



//发送用联合体
formatTrans32Struct_t errorSend;
formatTrans32Struct_t totaltimeSend;
formatTrans16Struct_t powerSend;
formatTrans16Struct_t speedSend;
formatTrans16Struct_t VDC_S;
formatTrans16Struct_t IDC_S;
formatTrans16Struct_t TBV_S;
formatTrans16Struct_t TBI_S;
formatTrans16Struct_t HV_S;
formatTrans16Struct_t HI_S;
formatTrans16Struct_t LSPV_S;
formatTrans16Struct_t LSPI_S;
formatTrans16Struct_t LPV_S;
formatTrans16Struct_t LPMA_S;
formatTrans16Struct_t PGD;
formatTrans16Struct_t POUT_S;

formatTrans16Struct_t ratepowerSend;
formatTrans16Struct_t maxpowerSend;
formatTrans16Struct_t minpowerSend;
formatTrans16Struct_t maxspeedSend;
formatTrans16Struct_t coronawidthSend;
formatTrans16Struct_t local_rollerdiameterSend;
formatTrans16Struct_t local_rollerpulseSend;
formatTrans16Struct_t external_rollerdiameterSend;
formatTrans16Struct_t external_rollerpulseSend;
formatTrans16Struct_t speedupSend;
formatTrans16Struct_t speeddelaySend;
formatTrans16Struct_t linedelaySend;
formatTrans16Struct_t pulsedelaySend;
formatTrans16Struct_t electrode_open_lengthSend;
formatTrans16Struct_t electrode_close_lengthSend;

//接收用联合体
formatTrans32Struct_t dayRev[3];
formatTrans32Struct_t passwordRev[3];
formatTrans32Struct_t verify_passwordRev;
formatTrans16Struct_t powerRev;
formatTrans16Struct_t mulRev;
formatTrans16Struct_t spdRev;
formatTrans16Struct_t denRev;

formatTrans16Struct_t ratepowerRev;
formatTrans16Struct_t maxpowerRev;
formatTrans16Struct_t minpowerRev;
formatTrans16Struct_t maxspeedRev;
formatTrans16Struct_t local_rollerdiameterRev;
formatTrans16Struct_t coronawidthRev;
formatTrans16Struct_t local_rollerpulseRev;
formatTrans16Struct_t speedupRev;
formatTrans16Struct_t external_rollerdiameterRev;
formatTrans16Struct_t external_rollerpulseRev;
formatTrans16Struct_t speeddelayRev;
formatTrans16Struct_t linedelayRev;
formatTrans16Struct_t pulsedelayRev;
formatTrans16Struct_t electrode_open_lengthRev;
formatTrans16Struct_t electrode_close_lengthRev;

uint8_t get_decimal_num(float num){
	uint32_t int_num;
	float decimal_num;
	int_num = (uint32_t)num;
	decimal_num = (num - int_num) * 100;
	int_num = (uint32_t)decimal_num;
	return int_num;
}

void gui_send_data(USART_TypeDef *USARTx){
	uint8_t *array = (uint8_t*)USART_TO_ArrayTX(USARTx);
	uint8_t index_ptr = 0;
	array[index_ptr++] = GUI_HEAD;  //帧头
	array[index_ptr++] = GUI_ADDR;	
	array[index_ptr++] = 0;   		//帧长
	
	array[index_ptr++] = get_controlData()->page_num; //发送页面号，防止接收错误页面信息乱码
	errorSend.u32_temp = get_controlData()->error_sta;
	//发送内容
	switch(get_controlData()->page_num){
		case 0:{ //初始化会发送机器参数进行覆盖
			ratepowerSend.u16_temp = parameter[POWER];
			maxpowerSend.u16_temp = parameter[MAX_POW];
			minpowerSend.u16_temp = parameter[LOW_POWER];
			maxspeedSend.u16_temp = parameter[MAX_SPEED];
			coronawidthSend.u16_temp = parameter[ROLLER_WIDTH];
			local_rollerdiameterSend.u16_temp = parameter[ROLLER_DIAMETER_LOCAL];
			local_rollerpulseSend.u16_temp = parameter[ROLLER_PULSE_LOCAL];
			external_rollerdiameterSend.u16_temp = parameter[ROLLER_DIAMETER_EXTERNAL];
			external_rollerpulseSend.u16_temp = parameter[ROLLER_PULSE_EXTERNAL];
			speedupSend.u16_temp = parameter[SPD_UP];
			speeddelaySend.u16_temp = parameter[SET_SPD_DELAY];
			linedelaySend.u16_temp = parameter[SET_LINE_DELAY];
			pulsedelaySend.u16_temp = parameter[SET_PULSE_DELAY];
			electrode_open_lengthSend.u16_temp = parameter[SET_DELAY_LENGTH1];
			electrode_close_lengthSend.u16_temp = parameter[SET_DELAY_LENGTH2];
			for(uint8_t i = 0; i < 2; i++){
				array[index_ptr++] = ratepowerSend.u8_temp[i];
			}
			for(uint8_t i = 0; i < 2; i++){
				array[index_ptr++] = maxpowerSend.u8_temp[i];
			}
			for(uint8_t i = 0; i < 2; i++){
				array[index_ptr++] = minpowerSend.u8_temp[i];
			}
			for(uint8_t i = 0; i < 2; i++){
				array[index_ptr++] = maxspeedSend.u8_temp[i];
			}
			for(uint8_t i = 0; i < 2; i++){
				array[index_ptr++] = coronawidthSend.u8_temp[i];
			}
			for(uint8_t i = 0; i < 2; i++){
				array[index_ptr++] = local_rollerdiameterSend.u8_temp[i];
			}
			for(uint8_t i = 0; i < 2; i++){
				array[index_ptr++] = local_rollerpulseSend.u8_temp[i];
			}
			for(uint8_t i = 0; i < 2; i++){
				array[index_ptr++] = external_rollerdiameterSend.u8_temp[i];
			}
			for(uint8_t i = 0; i < 2; i++){
				array[index_ptr++] = external_rollerpulseSend.u8_temp[i];
			}
			for(uint8_t i = 0; i < 2; i++){
				array[index_ptr++] = speedupSend.u8_temp[i];
			}
			for(uint8_t i = 0; i < 2; i++){
				array[index_ptr++] = speeddelaySend.u8_temp[i];
			}
			for(uint8_t i = 0; i < 2; i++){
				array[index_ptr++] = linedelaySend.u8_temp[i];
			}
			for(uint8_t i = 0; i < 2; i++){
				array[index_ptr++] = pulsedelaySend.u8_temp[i];
			}
			for(uint8_t i = 0; i < 2; i++){
				array[index_ptr++] = electrode_open_lengthSend.u8_temp[i];
			}
			for(uint8_t i = 0; i < 2; i++){
				array[index_ptr++] = electrode_close_lengthSend.u8_temp[i];
			}
			array[index_ptr++] = get_controlData()->use_pulse_corona;
		}break;
		//Main
		case 1:{
			//当前功率 小数点保留后两位
			powerSend.u16_temp = (uint16_t)(get_dischargeCtrlData()->current_power * 100);
			for(uint8_t i = 0; i < 2; i++){
				array[index_ptr++] = powerSend.u8_temp[i];
			}
			//当前速度 小数点保留后两位
			speedSend.u16_temp = (uint16_t)(get_spdDischargeData()->speed * 100);
			for(uint8_t i = 0; i < 2; i++)
				array[index_ptr++] = speedSend.u8_temp[i];
			//达速标志
			array[index_ptr++] = get_controlData()->speed_up;
			array[index_ptr++] = get_controlData()->fy_ok;
		}break;
		//PowerSet
		case 2:{
			//当前功率 小数点保留后两位
			powerSend.u16_temp = (uint16_t)(get_dischargeCtrlData()->current_power * 100);
			for(uint8_t i = 0; i < 2; i++){
				array[index_ptr++] = powerSend.u8_temp[i];
			}
			//当前速度 小数点保留后两位
			speedSend.u16_temp = (uint16_t)(get_spdDischargeData()->speed * 100);
			for(uint8_t i = 0; i < 2; i++)
				array[index_ptr++] = speedSend.u8_temp[i];
		}break;
		//dry
		case 3:{
			//当前功率 小数点保留后两位
			powerSend.u16_temp = (uint16_t)(get_dischargeCtrlData()->current_power * 100);
			for(uint8_t i = 0; i < 2; i++){
				array[index_ptr++] = powerSend.u8_temp[i];
			}
			array[index_ptr++] = get_controlData()->fy_ok;
		}break;
		//Test
		case 6:{
			VDC_S.u16_temp = (uint16_t)(adc_filter_VDC_ADC * 5.500f);
			IDC_S.u16_temp = (uint16_t)(adc_filter_IDC_ADC * 18.333f);
			TBV_S.u16_temp = (uint16_t)(adc_filter_TBV_DC3V3 * 4.125f);
			TBI_S.u16_temp = (uint16_t)(adc_filter_TBI_DC3V3 * 16.500f);
			HV_S.u16_temp = (uint16_t)(adc_filter_HV_DC3V3 * 0.264f);
			HI_S.u16_temp = (uint16_t)(adc_filter_HI_DC3V3 * 330);
			LSPV_S.u16_temp = (uint16_t)(adc_filter_LSPV3V3 * 3300);
			LSPI_S.u16_temp = (uint16_t)(adc_filter_LSPI3V3 * 2640 + 660);
			LPV_S.u16_temp = (uint16_t)(adc_filter_LPV3V3 * 3300);
			LPMA_S.u16_temp = (uint16_t)(adc_filter_LPIMA3V3 * 2640 + 660);
			PGD.u16_temp = (uint16_t)(adc_filter_TEMP * 0.806f);
			POUT_S.u16_temp = (uint16_t)(adc_filter_POV3V3 * 0.806f);
			for(uint8_t i = 0; i < 2; i++){
				array[index_ptr++] = VDC_S.u8_temp[i];
			}
			for(uint8_t i = 0; i < 2; i++){
				array[index_ptr++] = IDC_S.u8_temp[i];
			}
			for(uint8_t i = 0; i < 2; i++){
				array[index_ptr++] = TBV_S.u8_temp[i];
			}
			for(uint8_t i = 0; i < 2; i++){
				array[index_ptr++] = TBI_S.u8_temp[i];
			}
			for(uint8_t i = 0; i < 2; i++){
				array[index_ptr++] = HV_S.u8_temp[i];
			}
			for(uint8_t i = 0; i < 2; i++){
				array[index_ptr++] = HI_S.u8_temp[i];
			}
			for(uint8_t i = 0; i < 2; i++){
				array[index_ptr++] = LSPV_S.u8_temp[i];
			}
			for(uint8_t i = 0; i < 2; i++){
				array[index_ptr++] = LSPI_S.u8_temp[i];
			}
			for(uint8_t i = 0; i < 2; i++){
				array[index_ptr++] = LPV_S.u8_temp[i];
			}
			for(uint8_t i = 0; i < 2; i++){
				array[index_ptr++] = LPMA_S.u8_temp[i];
			}
			for(uint8_t i = 0; i < 2; i++){
				array[index_ptr++] = PGD.u8_temp[i];
			}
			for(uint8_t i = 0; i < 2; i++){
				array[index_ptr++] = POUT_S.u8_temp[i];
			}
		}break;
		//TotalTime
		case 9:{
			totaltimeSend.u32_temp = get_controlData()->rtc_hour;
			//使用时长
			for(uint8_t i = 0; i < 4; i++){
				array[index_ptr++] = totaltimeSend.u8_temp[i];
			}
		}break;
		//LockNote
		case 10:{
			//距离锁机结束剩余时间
			array[index_ptr++] = get_controlData()->remain_day;
		}break;
		//UnLock
		case 12:{
			//密码是否正确标志
			array[index_ptr++] = get_controlData()->pwd_ok;
		}break;
		default:break;
	}
	//换卷信号
	array[index_ptr++] = get_rcCtrlData()->state;
	
	//错误报警
	for(uint8_t i = 0; i < 4; i++){
		array[index_ptr++] = errorSend.u8_temp[i];
	}
	
	//停机报警
	array[index_ptr++] = get_supervisiorData()->stop_alarm;
	
	array[index_ptr++] = GUI_TAIL;	//帧尾
	array[index_ptr++] = GUI_LAST;
	
	array[2] = index_ptr + 2;  //计算帧长
	
	//MODBUS-CRC16校验
	Append__MODBUS_CRC16_Checksum(array,array[2]);
	
	BSP_USART_SendData_DMA(USART_TO_BSP_DMA(USARTx,USART_DMAReq_Tx),array,array[2]);
}

//接收中断的函数
void gui_receive_data(USART_TypeDef *USARTx,uint8_t *receive_data){
	uint8_t *array = (uint8_t*)USART_TO_ArrayRX(USARTx);
	if((array[0] == REV_HEAD) && (array[1] == REV_ADDR)){ //校验帧头
		if((array[array[2]-4] == REV_TAIL) && (array[array[2]-3] == REV_LAST)){ //校验帧尾
			if(!Verify_MODBUS_CRC16_Checksum(array,array[2])) return;  //crc16校验不通过
			else{
				//执行数据解析
				gui_data_unPackge(array);
			}
		}	
	}
}

void gui_data_unPackge(uint8_t *receive_data){
	
	get_controlData()->page_num = receive_data[3];
	uint8_t index_ptr = 4;
	//首先解析页面号
	switch(get_controlData()->page_num){
		//Main
		case 1:{
			//得到Main页面数据
			get_maindata()->main_rev_data = receive_data[index_ptr++];
			//获取控制参数
			for(uint8_t i = 0; i < 2; i++)
				powerRev.u8_temp[i] = receive_data[index_ptr++];
			switch(get_dischargeCtrlData()->power_ctrlState){
				case MANUAL_MODE:{
					get_controlData()->manual_power = ((float)powerRev.u16_temp) * 0.01f;
				}break;
				case POWER_DENSITY_MODE:{
					get_dischargeCtrlData()->power_density = ((float)powerRev.u16_temp);
				}break;
				case SPEED_MODE:{
					get_spdDischargeData()->max_pow = ((float)powerRev.u16_temp) * 0.01f;
				}break;
			}
			//当前功率模式
			get_dischargeCtrlData()->power_ctrlState = get_setStateData(receive_data[index_ptr++],POWERMODE);
			//flash标志
			get_supervisiorData()->flash_sw = receive_data[index_ptr++];
		}break;
		//PowerSet
		case 2:{
			//得到PowerSet页面数据
			get_powSetData()->set_state = receive_data[index_ptr++];	//工作状态
			for(uint8_t i = 0; i < 2; i++)	//手动功率
				mulRev.u8_temp[i] = receive_data[index_ptr++];
			for(uint8_t i = 0; i < 2; i++)	//速比
				spdRev.u8_temp[i] = receive_data[index_ptr++];
			for(uint8_t i = 0; i < 2; i++)	//功率密度
				denRev.u8_temp[i] = receive_data[index_ptr++];
			//控制数据更新
			get_controlData()->manual_power = ((float)mulRev.u16_temp) * 0.01f;
			get_dischargeCtrlData()->power_density = (float)denRev.u16_temp;
			get_spdDischargeData()->max_pow = ((float)spdRev.u16_temp) * 0.01f;
			//flash标志
			get_supervisiorData()->flash_sw = receive_data[index_ptr++];
		}break;
		//dry
		case 3:{
			//得到湿启动页面的数据
			get_controlData()->dry_mode = receive_data[index_ptr++];
			get_maindata()->main_rev_data = receive_data[index_ptr++];
		}break;
		//M_UTD
		case 4:{
			//得到手动达速页面数据
			get_controlData()->manual_mode = receive_data[index_ptr++];
			//flash标志
			get_supervisiorData()->flash_sw = receive_data[index_ptr++];
		}break;
		//ParameterSet
		case 5:{
			//得到参数设置页面的数据
			for(uint8_t i = 0; i < 2; i++){
				ratepowerRev.u8_temp[i] = receive_data[index_ptr++];
			}
			for(uint8_t i = 0; i < 2; i++){
				maxpowerRev.u8_temp[i] = receive_data[index_ptr++];
			}
			for(uint8_t i = 0; i < 2; i++){
				minpowerRev.u8_temp[i] = receive_data[index_ptr++];
			}
			for(uint8_t i = 0; i < 2; i++){
				maxspeedRev.u8_temp[i] = receive_data[index_ptr++];
			}
			for(uint8_t i = 0; i < 2; i++){
				coronawidthRev.u8_temp[i] = receive_data[index_ptr++];
			}
			for(uint8_t i = 0; i < 2; i++){
				local_rollerdiameterRev.u8_temp[i] = receive_data[index_ptr++];
			}
			for(uint8_t i = 0; i < 2; i++){
				local_rollerpulseRev.u8_temp[i] = receive_data[index_ptr++];
			}
			for(uint8_t i = 0; i < 2; i++){
				external_rollerdiameterRev.u8_temp[i] = receive_data[index_ptr++];
			}
			for(uint8_t i = 0; i < 2; i++){
				external_rollerpulseRev.u8_temp[i] = receive_data[index_ptr++];
			}
			for(uint8_t i = 0; i < 2; i++){
				speedupRev.u8_temp[i] = receive_data[index_ptr++];
			}
			for(uint8_t i = 0; i < 2; i++){
				speeddelayRev.u8_temp[i] = receive_data[index_ptr++];
			}
			for(uint8_t i = 0; i < 2; i++){
				linedelayRev.u8_temp[i] = receive_data[index_ptr++];
			}
			for(uint8_t i = 0; i < 2; i++){
				pulsedelayRev.u8_temp[i] = receive_data[index_ptr++];
			}
			for(uint8_t i = 0; i < 2; i++){
				electrode_open_lengthRev.u8_temp[i] = receive_data[index_ptr++];
			}
			for(uint8_t i = 0; i < 2; i++){
				electrode_close_lengthRev.u8_temp[i] = receive_data[index_ptr++];
			}
			get_controlData()->rated_power = (float)ratepowerRev.u16_temp * 1e-2f;					//额定功率
			get_spdDischargeData()->max_pow = (float)maxpowerRev.u16_temp * 1e-2f;					//线速下的最大输出功率
			get_dischargeCtrlData()->low_power = (float)minpowerRev.u16_temp * 1e-2f;  				//最低输出功率
			get_spdDischargeData()->max_spd = (float)maxspeedRev.u16_temp * 1e-2f;					//最大线速
			get_spdDischargeData()->roller_width = (float)coronawidthRev.u16_temp * 1e-2f;			//放电宽度
			parameter[ROLLER_DIAMETER_LOCAL] = (uint32_t)local_rollerdiameterRev.u16_temp;  		//本地滚轴直径mm
			parameter[ROLLER_PULSE_LOCAL] = (uint32_t)local_rollerpulseRev.u16_temp;			//本地码盘齿数
			parameter[ROLLER_DIAMETER_EXTERNAL] = (uint32_t)external_rollerdiameterRev.u16_temp;	//外部滚轴直径mm
			parameter[ROLLER_PULSE_EXTERNAL] = (uint32_t)external_rollerpulseRev.u16_temp;	//外部脉冲齿数
			get_controlData()->set_speed_up = (float)speedupRev.u16_temp * 1e-2f;					//设定达速
			get_spdDischargeData()->set_delay_time = (uint32_t)speeddelayRev.u16_temp;				//线速延时放电时间
			get_spdDischargeData()->set_line_delay_time = (uint32_t)linedelayRev.u16_temp;			//线控延时放电时间
			get_pulseDischargeData()->set_delay_time = (uint32_t)pulsedelayRev.u16_temp;			//脉冲延时放电时间
			get_rcCtrlData()->set_delay_length1 = (float)electrode_open_lengthRev.u16_temp * 1e-2f;	//抬电极距离m
			get_rcCtrlData()->set_delay_length2 = (float)electrode_close_lengthRev.u16_temp * 1e-2f;//合电极距离m
			//计算新的滚轴线脉冲
			get_spdDischargeData()->roller_pulse_length = (float)parameter[ROLLER_DIAMETER_LOCAL] * (1e-5f) * PI / (float)parameter[ROLLER_PULSE_LOCAL]; //本地滚轴
			get_spdDischargeData()->external_pulase_length = (float)parameter[ROLLER_DIAMETER_EXTERNAL] * (1e-5f) * PI / (float)parameter[ROLLER_PULSE_EXTERNAL]; //外地滚轴
			//计算新的湿启动最大功率
			get_dryCtrlData()->max_power = get_controlData()->rated_power * 0.5f;
			//当前控制模式
			get_dischargeCtrlData()->power_ctrlState = get_setStateData(receive_data[index_ptr++],POWERMODE);
			//flash更新
			get_supervisiorData()->flash_sw = receive_data[index_ptr++];
		}break;
		//PulseTrigger
		case 7:{
			//得到脉冲触发页面数据
			get_controlData()->use_pulse_corona = receive_data[index_ptr++];  //脉冲触发
			get_supervisiorData()->flash_sw = receive_data[index_ptr++];
		}break;
		//ErrorCode
		case 8:{
			//故障复位信号
			get_controlData()->error_reset = receive_data[index_ptr++];
		}break;
		//MachineLock
		case 11:{
			//设定的三个日期以及对应的密码
			for(uint8_t i = 0; i < 4; i++){
				dayRev[0].u8_temp[i] = receive_data[index_ptr++];
			}
			for(uint8_t i = 0; i < 4; i++){
				dayRev[1].u8_temp[i] = receive_data[index_ptr++];
			}
			for(uint8_t i = 0; i < 4; i++){
				dayRev[2].u8_temp[i] = receive_data[index_ptr++];
			}

			for(uint8_t i = 0; i < 4; i++){
				passwordRev[0].u8_temp[i] = receive_data[index_ptr++];
			}			
			for(uint8_t i = 0; i < 4; i++){
				passwordRev[1].u8_temp[i] = receive_data[index_ptr++];
			}
			for(uint8_t i = 0; i < 4; i++){
				passwordRev[2].u8_temp[i] = receive_data[index_ptr++];
			}
			
			get_controlData()->rtc_day1 = dayRev[0].u32_temp;
			get_controlData()->rtc_day2 = dayRev[1].u32_temp;
			get_controlData()->rtc_day3 = dayRev[2].u32_temp;
			get_controlData()->password[0] = passwordRev[0].u32_temp;
			get_controlData()->password[1] = passwordRev[1].u32_temp;
			get_controlData()->password[2] = passwordRev[2].u32_temp;
			
			//flash更新
			get_supervisiorData()->flash_sw = receive_data[index_ptr++];
		}break;
		//UnLock
		case 12:{
			//获得发送的密码
			for(uint8_t i = 0; i < 4; i++){
				verify_passwordRev.u8_temp[i] = receive_data[index_ptr++];
			}
			get_controlData()->verify_password = verify_passwordRev.u32_temp;
			//flash更新
			get_supervisiorData()->flash_sw = receive_data[index_ptr++];
		}break;
		default: break;
	}
}











