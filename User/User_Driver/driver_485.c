#include "driver.h"

uint8_t sendata[RS485_TEST_LEN];
uint8_t resdata[RS485_TEST_LEN];


void driver_485_Configuration(USART_TypeDef *USARTx,BSP_GPIOSource_TypeDef *TX_Port,BSP_GPIOSource_TypeDef *RX_Port,BSP_GPIOSource_TypeDef *EN_Port,uint32_t baudrate,uint8_t PreemptionPriority,uint8_t SubPriority){
	
	//485ʹ�ܽ�
	BSP_GPIO_Init(EN_Port,GPIO_Mode_Out_PP);
	
	//485��������
	BSP_USART_Init(USARTx,TX_Port,RX_Port,baudrate,PreemptionPriority,SubPriority);
#if RS485_IT_TEST	
	//485DMA���������������룬��Ҫ���÷����жϽ��д���
	USART_ClearFlag(USARTx, USART_FLAG_TC); //���������ɱ�־	
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);	//�ȴ�����֡������ɺ������㷢����ɱ�־
	USART_ClearFlag(USARTx, USART_FLAG_TC);	//���������ɱ�־
	
	USART_ITConfig(USARTx, USART_IT_TC, ENABLE);				//ʹ�ܴ�������ж�
#endif

	
	BSP_USART_DMA_Init(USART_TO_BSP_DMA(USARTx,USART_DMAReq_Tx));	//���Ͷ�DMA����
	BSP_USART_DMA_Init(USART_TO_BSP_DMA(USARTx,USART_DMAReq_Rx));	//���ն�DMA����
	
	RS485_EN_CTRL = SEND_ENABLE;  //Ĭ���ȷ�ģʽ
	
}


//����
void rs485_send_data(USART_TypeDef *USARTx,uint8_t *send_data,uint8_t len){
	uint8_t *array = (uint8_t*)USART_TO_ArrayTX(USARTx);
	array[0] = 0x21;
	array[1] = 0x33;
	array[2] = len + 4;
	array[3] = 0x11;
	
	for(uint8_t i = 0; i < len; i++){
		array[4+i] = send_data[i];
	}
	
	RS485_EN_CTRL = SEND_ENABLE;
	BSP_USART_SendData_DMA(USART_TO_BSP_DMA(USARTx,USART_DMAReq_Tx),array,array[2]);
	
#if !RS485_IT_TEST
	while((USARTx->SR & USART_FLAG_TC) == 0); //485DMA���������������룬��Ҫ�ȴ��������
	USART_ClearFlag(USARTx,USART_FLAG_TC);
	RS485_EN_CTRL = RECEIVE_ENABLE;
#endif
	
	
}

void rs485_receive_data(USART_TypeDef *USARTx,uint8_t *receive_data){
	uint8_t *array = (uint8_t*)USART_TO_ArrayRX(USARTx);
	if((array[0] == 0x21) && (array[1] == 0x33) && (array[3] == 0x11)){
		for(uint8_t i = 0; i < RS485_TEST_LEN; i++){
			receive_data[i] = array[4+i];
		}
	}
}








