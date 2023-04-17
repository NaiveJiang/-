#ifndef __BSP_CAN_H
#define __BSP_CAN_H

#include "stm32f10x.h"
#include "stdlib.h"
#include "stdio.h"
#include "BSP_GPIO.h"

typedef struct{
	CAN_TypeDef* CANx;			//CANͨ��
	BSP_GPIOSource_TypeDef *CANx_RX;	//CANx_RX ����
	BSP_GPIOSource_TypeDef *CANx_TX;	//CANx_TX ����
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;	//CAN����������
}BSP_CAN_TypeDef;

typedef struct{
	CAN_TypeDef* 	CANx;
	CanTxMsg* 		TxMessage;
}BSP_CANSend_TypeDef;

void BSP_CAN_Mode_Init(CAN_TypeDef *BSP_CANx,uint8_t tsjw,uint8_t tbs1,uint8_t tbs2,uint16_t brp,uint8_t PreemptionPriority,uint8_t SubPriority);
uint8_t BSP_CAN_Send_Msg(CAN_TypeDef *BSP_CANx,CanTxMsg *TxMessage);





#endif

