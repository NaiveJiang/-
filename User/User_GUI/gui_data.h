#ifndef __GUI_DATA_H
#define __GUI_DATA_H

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

typedef struct{
	float power;
	float speed;
	uint8_t speed_up;
}main_pageStruct;

extern __IO main_pageStruct mainData;



void gui_send_data(USART_TypeDef *USARTx);
void gui_receive_data(USART_TypeDef *USARTx,uint8_t *receive_data);

#endif
