#include "driver.h"
#include "app.h"
uint8_t send_gui[GUI_USART_TEST_LEN];
uint8_t rev_gui[GUI_USART_TEST_LEN];

void driver_GUI_USART_Configuration(USART_TypeDef *USARTx,BSP_GPIOSource_TypeDef *TX_Port,BSP_GPIOSource_TypeDef *RX_Port,uint32_t baudrate,uint8_t PreemptionPriority,uint8_t SubPriority){
	
	BSP_USART_Init(USARTx,TX_Port,RX_Port,baudrate,PreemptionPriority,SubPriority);

	BSP_USART_DMA_Init(USART_TO_BSP_DMA(USARTx,USART_DMAReq_Tx));	//���Ͷ�DMA����
	BSP_USART_DMA_Init(USART_TO_BSP_DMA(USARTx,USART_DMAReq_Rx));	//���ն�DMA����
	
}

//����
void gui_send_data(USART_TypeDef *USARTx,uint8_t *send_data,uint8_t len){
	uint8_t *array = (uint8_t*)USART_TO_ArrayTX(USARTx);
	uint8_t index_ptr = 0;
	array[index_ptr++] = GUI_HEAD;  //֡ͷ
	array[index_ptr++] = GUI_ADDR;	
	array[index_ptr++] = 0;   		//֡��
	
	for(uint8_t i = 0; i < len; i++){
		array[index_ptr++] = send_data[i];
	}
	
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
				for(uint8_t i = 0; i < GUI_USART_TEST_LEN; i++){
					receive_data[i] = array[3+i];
				}
			}
		}	
	}
}




