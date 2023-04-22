#ifndef _DRIVER_485_H
#define _DRIVER_485_H

#include "bsp.h"

#define RS_485 USART3
#define RS_485_BAUD 9600
#define RS_485_PREEMPTIONPRIORITY 3
#define RS_485_SUBPRIORITY 0


#define RS_485_TX BSP_GPIOB10
#define RS_485_RX BSP_GPIOB11
#define RS_485_EN BSP_GPIOD15  //RS485Ê¹ÄÜ

#define RS485_EN_CTRL PDout(15)

#define RS485_TEST_LEN 4
#define RS485_IT_TEST 0

enum{
	RECEIVE_ENABLE = 0,
	SEND_ENABLE,
};


void driver_485_Configuration(USART_TypeDef *USARTx,BSP_GPIOSource_TypeDef *TX_Port,BSP_GPIOSource_TypeDef *RX_Port,BSP_GPIOSource_TypeDef *EN_Port,uint32_t baudrate,uint8_t PreemptionPriority,uint8_t SubPriority);
void rs485_send_data(USART_TypeDef *USARTx,uint8_t *send_data,uint8_t len);
void rs485_receive_data(USART_TypeDef *USARTx,uint8_t *receive_data);

extern uint8_t sendata[RS485_TEST_LEN];
extern uint8_t resdata[RS485_TEST_LEN];






#endif
