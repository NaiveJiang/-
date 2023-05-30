#ifndef _DRIVER_CAN_H
#define _DRIVER_CAN_H

#include "bsp.h"

#define CAN_ID1 0x20A
#define CAN_ID2 0x209
#define CAN_ID3 0x205
#define CAN_ID4 0x206


#define USE_CANx CAN1
#define CAN_PREEMPTIONPRIORITY 2
#define CAN_SUBPRIORITY 0

//������ = 36M/((1+9+8)*2) = 1M
#define CAN_TBS1 CAN_BS1_9tq
#define CAN_TBS2 CAN_BS2_8tq
#define CAN_BRP 2
#define CAN_SJWTQ CAN_SJW_1tq //��󾫶�

//������ֻ������1��Ϊ1
#define USE_TEST_BOARD_CANB89	0	//ʹ�ò��԰�PB8/PB9��ΪCAN�ӿ�
#define USE_TEST_BOARD_CAND01	0	//ʹ�ò��԰�PD0/PD1��ΪCAN�ӿ�
#define USE_HDX_LPSSI_CAN		1	//ʹ��HDX_LPSSI���̰�PA11/PA12��ΪCAN�ӿ�

#if USE_TEST_BOARD_CANB89		//ʹ�ò��԰�PB8/PB9
#define CAN_TX BSP_GPIOB9   
#define CAN_RX BSP_GPIOB8

#elif USE_TEST_BOARD_CAND01		//ʹ�ò��԰�PD0/PD1
#define CAN_TX BSP_GPIOD1   
#define CAN_RX BSP_GPIOD0 

#elif USE_HDX_LPSSI_CAN			//ʹ��HDX_LPSSI���̰�PA11/PA12
#define CAN_TX BSP_GPIOA12   	
#define CAN_RX BSP_GPIOA11   

#endif

//CAN���ͽṹ��
typedef struct{
	uint32_t id;
	volatile uint16_t data1;
	volatile uint16_t data2;
	volatile uint16_t data3;
	volatile uint16_t data4;
}can_sendStruct_t;

//CAN���սṹ��
typedef struct{
	volatile uint16_t data1;
	volatile uint16_t data2;
	volatile uint16_t data3;
	volatile uint16_t data4;
}can_receiveStruct_t;



void can_Send(CAN_TypeDef *CANx,can_sendStruct_t* sendata);
void can_receive_msg(CanRxMsg *can_rx_msg,can_receiveStruct_t* receivedata);
void driver_CAN_Configuration(uint8_t PreemptionPriority,uint8_t SubPriority);
void can_data_pre(can_sendStruct_t* can_senData,uint32_t ID,uint16_t data1,uint16_t data2,uint16_t data3,uint16_t data4);

extern can_sendStruct_t can_id20A;
extern can_sendStruct_t can_id209;

extern can_receiveStruct_t can_id20A_res;
extern can_receiveStruct_t can_id209_res;

#endif
