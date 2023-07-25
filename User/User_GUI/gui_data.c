#include "app.h"


formatTrans16Struct_t powData;
//发送用联合体
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
//接收用联合体
formatTrans16Struct_t powerRev;
formatTrans16Struct_t mulRev;
formatTrans16Struct_t spdRev;
formatTrans16Struct_t denRev;
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
	
	switch(get_controlData()->page_num){
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
		default:break;
	}
	
	
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
					get_dischargeCtrlData()->manual_power = ((float)powerRev.u16_temp) * 0.01f;
				}break;
				case POWER_DENSITY_MODE:{
					get_dischargeCtrlData()->power_density = ((float)powerRev.u16_temp) * 0.01f;
				}break;
				case SPEED_MODE:{
					get_spdDischargeData()->spd_max_pow = ((float)powerRev.u16_temp) * 0.01f;
				}break;
			}
			//当前控制模式
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
			get_dischargeCtrlData()->power_density = ((float)denRev.u16_temp) * 0.01f;
			get_spdDischargeData()->spd_max_pow = ((float)spdRev.u16_temp) * 0.01f;
			//flash标志
			get_supervisiorData()->flash_sw = receive_data[index_ptr++];
		}break;
		//dry
		case 3:{
			//得到湿启动页面的数据
			get_controlData()->dry_mode = receive_data[index_ptr++];
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
			
		}break;
		//PulseTrigger
		case 7:{
			//得到脉冲触发页面数据
			get_controlData()->use_pulse_corona = receive_data[index_ptr++];  //脉冲触发
			get_supervisiorData()->flash_sw = receive_data[index_ptr++];
		}break;
		default: break;
	}
}











