#include "app.h"

formatTrans16Struct_t speedData;
formatTrans16Struct_t powData;
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
	
	array[index_ptr++] = (uint8_t)get_maindata()->power;
	array[index_ptr++] = get_decimal_num(get_maindata()->power);
	
	speedData.u16_temp = (uint16_t)get_maindata()->speed;
	for(uint8_t i = 0; i < 2; i++)
		array[index_ptr++] = speedData.u8_temp[i];
	
	array[index_ptr++] = get_maindata()->speed_up;
	
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
	
	//首先解析页面号
	switch(receive_data[3]){
		//Main
		case 1:{
			//得到Main页面数据
			get_maindata()->main_rev_data = receive_data[4];
		}break;
		//ControlSet
		case 2:{
			//得到ControlSet页面数据
			get_ctrlSetdata()->ctrlSet_rev_data = receive_data[4];
		}break;
		//PowerSet
		case 3:{
			//得到PowerSet页面数据
			uint8_t index_ptr = 4;
			for(uint8_t i = 0; i < 2; i++)
				powData.u8_temp[i] = receive_data[index_ptr++];
			get_powSetData()->power_density = powData.u16_temp;						//功率密度
			for(uint8_t i = 0; i < 2; i++)
				powData.u8_temp[i] = receive_data[index_ptr++];
			get_powSetData()->spd_max = (float)powData.u16_temp * 0.01f;			//速比模式最大
			for(uint8_t i = 0; i < 2; i++)
				powData.u8_temp[i] = receive_data[index_ptr++];
			get_powSetData()->spd_min = (float)powData.u16_temp * 0.01f;			//速比模式最小
			for(uint8_t i = 0; i < 2; i++)
				powData.u8_temp[i] = receive_data[index_ptr++];
			get_powSetData()->manual_power = (float)powData.u16_temp * 0.01f;		//手动模式
		}break;
		//dry
		case 4:{
			//得到湿启动页面的数据
			get_drydata()->dry_rev_data = receive_data[4];
		}break;
		default: break;
	}
}











