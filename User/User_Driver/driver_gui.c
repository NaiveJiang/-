#include "driver.h"

uint8_t send_gui[GUI_USART_TEST_LEN];
uint8_t rev_gui[GUI_USART_TEST_LEN];

void driver_GUI_USART_Configuration(USART_TypeDef *USARTx,BSP_GPIOSource_TypeDef *TX_Port,BSP_GPIOSource_TypeDef *RX_Port,uint32_t baudrate,uint8_t PreemptionPriority,uint8_t SubPriority){
	
	BSP_USART_Init(USARTx,TX_Port,RX_Port,baudrate,PreemptionPriority,SubPriority);

	BSP_USART_DMA_Init(USART_TO_BSP_DMA(USARTx,USART_DMAReq_Tx));	//∑¢ÀÕ∂ÀDMA≈‰÷√
	BSP_USART_DMA_Init(USART_TO_BSP_DMA(USARTx,USART_DMAReq_Rx));	//Ω” ’∂ÀDMA≈‰÷√
	
}

//≤‚ ‘
void gui_send_data(USART_TypeDef *USARTx,uint8_t *send_data,uint8_t len){
	uint8_t *array = (uint8_t*)USART_TO_ArrayTX(USARTx);
	array[0] = 0xA0;
	array[1] = 0xB5;
	array[2] = len + 4;
	array[3] = 0x11;
	
	for(uint8_t i = 0; i < len; i++){
		array[4+i] = send_data[i];
	}
	
	BSP_USART_SendData_DMA(USART_TO_BSP_DMA(USARTx,USART_DMAReq_Tx),array,array[2]);
}

void gui_receive_data(USART_TypeDef *USARTx,uint8_t *receive_data){
	uint8_t *array = (uint8_t*)USART_TO_ArrayRX(USARTx);
	if((array[0] == 0xA0) && (array[1] == 0xB5) && (array[3] == 0x11)){
		for(uint8_t i = 0; i < GUI_USART_TEST_LEN; i++){
			receive_data[i] = array[4+i];
		}
	}
}




