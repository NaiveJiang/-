#include "HJ_USART.h"

uint8_t initFlg = 1;
uint8_t command = 0;
uint8_t fuze = 0;

void USART1_IRQHandler(void){
	if(USART_GetITStatus(WIFI_USART,USART_IT_RXNE)!=RESET){
		command = USART_ReceiveData(WIFI_USART);
//		if(initFlg == 0){
//			if(command == 1) fuze++;
//			else fuze = 0;
//			if(fuze >= 20){
//				initFlg = 1;
//				fuze = 0;
//			}
//		}
		while(USART_GetFlagStatus(WIFI_USART,USART_FLAG_RXNE));
	}
}
