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
	array[index_ptr++] = GUI_HEAD;  //֡ͷ
	array[index_ptr++] = GUI_ADDR;	
	array[index_ptr++] = 0;   		//֡��
	
	array[index_ptr++] = (uint8_t)mainData.power;
	array[index_ptr++] = get_decimal_num(mainData.power);
	
	speedData.u16_temp = (uint16_t)mainData.speed;
	for(uint8_t i = 0; i < 2; i++)
		array[index_ptr++] = speedData.u8_temp[i];
	
	array[index_ptr++] = mainData.speed_up;
	
	array[index_ptr++] = GUI_TAIL;	//֡β
	array[index_ptr++] = GUI_LAST;
	
	array[2] = index_ptr + 2;  //����֡��
	
	//MODBUS-CRC16У��
	Append__MODBUS_CRC16_Checksum(array,array[2]);
	
	BSP_USART_SendData_DMA(USART_TO_BSP_DMA(USARTx,USART_DMAReq_Tx),array,array[2]);
}

void gui_receive_data(USART_TypeDef *USARTx,uint8_t *receive_data){
	uint8_t *array = (uint8_t*)USART_TO_ArrayRX(USARTx);
	if((array[0] == REV_HEAD) && (array[1] == REV_ADDR)){ //У��֡ͷ
		if((array[array[2]-4] == REV_TAIL) && (array[array[2]-3] == REV_LAST)){ //У��֡β
			if(!Verify_MODBUS_CRC16_Checksum(array,array[2])) return;  //crc16У�鲻ͨ��
			else{
				//ִ�����ݽ���
				gui_data_unPackge(array);
			}
		}	
	}
}

void gui_data_unPackge(uint8_t *receive_data){
	
	//���Ƚ���ҳ���
	switch(receive_data[3]){
		//Main
		case 1:{
			//�õ�Mainҳ������
			mainData.main_rev_data = receive_data[4];
		}break;
		//ControlSet
		case 2:{
			//�õ�ControlSetҳ������
			ctrlSetData.ctrlSet_rev_data = receive_data[4];
		}break;
		//PowerSet
		case 3:{
			//�õ�PowerSetҳ������
			uint8_t index_ptr = 4;
			for(uint8_t i = 0; i < 2; i++)
				powData.u8_temp[i] = receive_data[index_ptr++];
			get_powSetData()->power_density = powData.u16_temp;						//�����ܶ�
			for(uint8_t i = 0; i < 2; i++)
				powData.u8_temp[i] = receive_data[index_ptr++];
			get_powSetData()->spd_max = (float)powData.u16_temp * 0.01f;			//�ٱ�ģʽ���
			for(uint8_t i = 0; i < 2; i++)
				powData.u8_temp[i] = receive_data[index_ptr++];
			get_powSetData()->spd_min = (float)powData.u16_temp * 0.01f;			//�ٱ�ģʽ��С
			for(uint8_t i = 0; i < 2; i++)
				powData.u8_temp[i] = receive_data[index_ptr++];
			get_powSetData()->manual_power = (float)powData.u16_temp * 0.01f;		//�ֶ�ģʽ
		}break;
		default: break;
	}
}











