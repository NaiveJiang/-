#ifndef __BSP_USART_H
#define __BSP_USART_H

#include "stm32f10x.h"
#include "stdlib.h"
#include "stdio.h"
#include "BSP_GPIO.h"

//宏定义串口接收数组长度
#define	Length_USART1_RX_Buff 256
#define	Length_USART2_RX_Buff 64
#define	Length_USART3_RX_Buff 512
#define	Length_UART4_RX_Buff 	512
#define	Length_UART5_RX_Buff 	512

//宏定义串口发送数组长度
#define	Length_USART1_TX_Buff 256
#define	Length_USART2_TX_Buff 64
#define	Length_USART3_TX_Buff 512
#define	Length_UART4_TX_Buff 	512
#define	Length_UART5_TX_Buff 	512


extern uint8_t Array_USART1_RX[Length_USART1_RX_Buff];
extern uint8_t Array_USART2_RX[Length_USART2_RX_Buff];
extern uint8_t Array_USART3_RX[Length_USART3_RX_Buff];
extern uint8_t Array_UART4_RX [Length_UART4_RX_Buff];
extern uint8_t Array_UART5_RX [Length_UART5_RX_Buff];


extern uint8_t Array_USART1_TX[Length_USART1_TX_Buff];
extern uint8_t Array_USART2_TX[Length_USART2_TX_Buff];
extern uint8_t Array_USART3_TX[Length_USART3_TX_Buff];
extern uint8_t Array_UART4_TX [Length_UART4_TX_Buff];
extern uint8_t Array_UART5_TX [Length_UART5_TX_Buff];

typedef enum{
	TX_MODE = 0,
	RX_MODE,
}USART_MODE_e;

typedef enum{
	__DMA1 = 0,
	__DMA2,
	__NO_DMA,
}DMA_e;

typedef struct{
	DMA_e dma;
	USART_MODE_e mode;
	DMA_Channel_TypeDef* DMA_channel;
	USART_TypeDef* USARTx;
	uint32_t DMATC_FLAG;
	uint32_t DMAHT_FLAG;
}BSP_USART_Typedef;

typedef struct {
	uint16_t Length;
	uint8_t *Array_Recv;
}BSP_USART_Recv_TypeDef;

void BSP_USART_Init(USART_TypeDef *USARTx,BSP_GPIOSource_TypeDef *USART_TX,BSP_GPIOSource_TypeDef *USART_RX,uint32_t baudrate,uint8_t PreemptionPriority, uint8_t SubPriority);
void BSP_USART_DMA_Init(BSP_USART_Typedef *USARTx_DMA);
BSP_USART_Typedef* USART_TO_BSP_DMA(USART_TypeDef* USARTx, uint16_t USART_DMAReq);
void BSP_USART_SendData_DMA(BSP_USART_Typedef *BSP_USARTx,uint8_t *data,uint16_t len);
void BSP_USART_SendData_(BSP_USART_Typedef *BSP_USARTx,uint8_t *data,uint16_t len);
uint32_t USART_TO_ArrayRX(USART_TypeDef* USARTx);
uint32_t USART_TO_ArrayTX(USART_TypeDef* USARTx);
#endif

