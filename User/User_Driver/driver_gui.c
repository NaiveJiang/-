#include "driver.h"
#include "app.h"
uint8_t send_gui[GUI_USART_TEST_LEN];
uint8_t rev_gui[GUI_USART_TEST_LEN];

void driver_GUI_USART_Configuration(USART_TypeDef *USARTx,BSP_GPIOSource_TypeDef *TX_Port,BSP_GPIOSource_TypeDef *RX_Port,uint32_t baudrate,uint8_t PreemptionPriority,uint8_t SubPriority){
	
	BSP_USART_Init(USARTx,TX_Port,RX_Port,baudrate,PreemptionPriority,SubPriority);

	BSP_USART_DMA_Init(USART_TO_BSP_DMA(USARTx,USART_DMAReq_Tx));	//发送端DMA配置
	BSP_USART_DMA_Init(USART_TO_BSP_DMA(USARTx,USART_DMAReq_Rx));	//接收端DMA配置
	
}

//测试
void gui_send_data(USART_TypeDef *USARTx,uint8_t *send_data,uint8_t len){
	uint8_t *array = (uint8_t*)USART_TO_ArrayTX(USARTx);
	uint8_t index_ptr = 0;
	array[index_ptr++] = GUI_HEAD;  //帧头
	array[index_ptr++] = GUI_ADDR;	
	array[index_ptr++] = 0;   		//帧长
	
	for(uint8_t i = 0; i < len; i++){
		array[index_ptr++] = send_data[i];
	}
	
	array[index_ptr++] = GUI_TAIL;	//帧尾
	array[index_ptr++] = GUI_LAST;
	
	array[2] = index_ptr + 2;  //计算帧长
	
	//MODBUS-CRC16校验
	Append__MODBUS_CRC16_Checksum(array,array[2]);
	
	BSP_USART_SendData_DMA(USART_TO_BSP_DMA(USARTx,USART_DMAReq_Tx),array,array[2]);
}

void gui_receive_data(USART_TypeDef *USARTx,uint8_t *receive_data){
	uint8_t *array = (uint8_t*)USART_TO_ArrayRX(USARTx);
	if((array[0] == REV_HEAD) && (array[1] == REV_ADDR)){ //校验帧头
		if((array[array[2]-4] == REV_TAIL) && (array[array[2]-3] == REV_LAST)){ //校验帧尾
			if(!Verify_MODBUS_CRC16_Checksum(array,array[2])) return;  //crc16校验不通过
			else{
				for(uint8_t i = 0; i < GUI_USART_TEST_LEN; i++){
					receive_data[i] = array[3+i];
				}
			}
		}	
	}
}




