#include "app.h"

uint32_t res_loop = 0;
//串口2中断服务函数
void USART2_IRQHandler(void){
	uint16_t len = 0;
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET){
		len = USART2->SR;
		len = USART2->DR;
		
		DMA_Cmd(DMA1_Channel6,DISABLE);
		len = Length_USART2_RX_Buff - DMA_GetCurrDataCounter(DMA1_Channel6);//缓存中的字节数
		DMA_ClearFlag(DMA1_FLAG_TC6 | DMA1_FLAG_HT6);
		DMA1_Channel6->CNDTR = Length_USART2_RX_Buff;
		DMA_Cmd(DMA1_Channel6, ENABLE);
		if(len){
			gui_receive_data(USART2,rev_gui);
			res_loop++;
		}
	}
}



void USART3_IRQHandler(void){
	uint16_t len = 0;
#if RS485_IT_TEST
	//中断发送测试
	if(USART_GetITStatus(USART3,USART_IT_TC) != RESET){
		USART_ClearITPendingBit(USART3, USART_IT_TC);
		DMA_ClearFlag(DMA1_FLAG_TC2 | DMA1_FLAG_HT2);
		RS485_EN_CTRL = RECEIVE_ENABLE; //切换为485接收模式
	}
#endif
	
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET){
		len = USART3->SR;
		len = USART3->DR;
		
		DMA_Cmd(DMA1_Channel2,DISABLE);
		len = Length_USART3_RX_Buff - DMA_GetCurrDataCounter(DMA1_Channel2);//缓存中的字节数
		DMA_ClearFlag(DMA1_FLAG_TC2 | DMA1_FLAG_HT2);
		DMA1_Channel5->CNDTR = Length_USART3_RX_Buff;
		DMA_Cmd(DMA1_Channel2, ENABLE);
		if(len){
			rs485_receive_data(USART3,resdata);
			res_loop++;
		}
	}
		
}



