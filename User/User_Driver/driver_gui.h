#ifndef _DRIVER_GUI_H
#define _DRIVER_GUI_H

#include "bsp.h"

#define GUI_USART USART2
#define GUI_USART_BAUD 115200
#define GUI_USART_PREEMPTIONPRIORITY 4
#define GUI_USART_SUBPRIORITY 0


#define GUI_USART_TX BSP_GPIOA2
#define GUI_USART_RX BSP_GPIOA3


#define GUI_USART_TEST_LEN 10

void driver_GUI_USART_Configuration(USART_TypeDef *USARTx,BSP_GPIOSource_TypeDef *TX_Port,BSP_GPIOSource_TypeDef *RX_Port,uint32_t baudrate,uint8_t PreemptionPriority,uint8_t SubPriority);
void gui_send_data(USART_TypeDef *USARTx,uint8_t *send_data,uint8_t len);
void gui_receive_data(USART_TypeDef *USARTx,uint8_t *receive_data);

extern uint8_t send_gui[GUI_USART_TEST_LEN];
extern uint8_t rev_gui[GUI_USART_TEST_LEN];



#endif
