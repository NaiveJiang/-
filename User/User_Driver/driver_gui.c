#include "driver.h"
#include "app.h"
uint8_t send_gui[GUI_USART_TEST_LEN];
uint8_t rev_gui[GUI_USART_TEST_LEN];

void driver_GUI_USART_Configuration(USART_TypeDef *USARTx,BSP_GPIOSource_TypeDef *TX_Port,BSP_GPIOSource_TypeDef *RX_Port,uint32_t baudrate,uint8_t PreemptionPriority,uint8_t SubPriority){
	
	BSP_USART_Init(USARTx,TX_Port,RX_Port,baudrate,PreemptionPriority,SubPriority);

	BSP_USART_DMA_Init(USART_TO_BSP_DMA(USARTx,USART_DMAReq_Tx));	//∑¢ÀÕ∂ÀDMA≈‰÷√
	BSP_USART_DMA_Init(USART_TO_BSP_DMA(USARTx,USART_DMAReq_Rx));	//Ω” ’∂ÀDMA≈‰÷√
	
}




