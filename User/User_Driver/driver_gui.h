#ifndef _DRIVER_GUI_H
#define _DRIVER_GUI_H

#include "bsp.h"

#define GUI_USART USART2
#define GUI_USART_BAUD 115200
#define GUI_USART_PREEMPTIONPRIORITY 4
#define GUI_USART_SUBPRIORITY 0


#define GUI_USART_TX BSP_GPIOA2  //sv15
#define GUI_USART_RX BSP_GPIOA3	 //sv16


#define GUI_USART_TEST_LEN 5

//发送帧头/帧尾
#define GUI_HEAD 0xAD
#define GUI_ADDR 0xB5
#define GUI_TAIL 0x0D
#define GUI_LAST 0xAA

//接收帧头/帧尾
#define REV_HEAD 0xC5
#define REV_ADDR 0x64
#define REV_TAIL 0xA9
#define REV_LAST 0xD4


void driver_GUI_USART_Configuration(USART_TypeDef *USARTx,BSP_GPIOSource_TypeDef *TX_Port,BSP_GPIOSource_TypeDef *RX_Port,uint32_t baudrate,uint8_t PreemptionPriority,uint8_t SubPriority);
void gui_send_data(USART_TypeDef *USARTx,uint8_t *send_data,uint8_t len);
void gui_receive_data(USART_TypeDef *USARTx,uint8_t *receive_data);

extern uint8_t send_gui[GUI_USART_TEST_LEN];
extern uint8_t rev_gui[GUI_USART_TEST_LEN];



#endif
