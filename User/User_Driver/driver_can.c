#include "driver_can.h"

can_sendStruct_t can_id20A;
can_sendStruct_t can_id209;

can_receiveStruct_t can_id20A_res;
can_receiveStruct_t can_id209_res;

void driver_CAN_Configuration(uint8_t PreemptionPriority,uint8_t SubPriority){
	//PB8,PB9,PD0,PD1需要重映射,PA11和PA12不用
	//使用重映射打开复用时钟AFIO
#if !USE_HDX_LPSSI_CAN
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);
#endif	
	
	//GPIO初始化
	BSP_GPIO_Init(CAN_TX,GPIO_Mode_AF_PP);
	BSP_GPIO_Init(CAN_RX,GPIO_Mode_IPU);
	
	//PB8,PB9对应CAN1重映射1,PD0,PD1对应CAN1重映射2
#if USE_TEST_BOARD_CANB89		
	GPIO_PinRemapConfig(GPIO_Remap1_CAN1,ENABLE);
#elif USE_TEST_BOARD_CAND01
	GPIO_PinRemapConfig(GPIO_Remap2_CAN1,ENABLE);
#endif
	
	//CAN初始化
	BSP_CAN_Mode_Init(USE_CANx,CAN_SJWTQ,CAN_TBS1,CAN_TBS2,CAN_BRP,PreemptionPriority,SubPriority);
}

//CAN发送
void can_Send(CAN_TypeDef *CANx,can_sendStruct_t* sendata){
	uint8_t mbox;
	uint16_t i=0;
	CanTxMsg txMessage;
	txMessage.StdId = sendata->id;
	txMessage.IDE = CAN_Id_Standard;
	txMessage.RTR = CAN_RTR_DATA;
	txMessage.DLC = 0x08;
	
	txMessage.Data[0] = (uint8_t)(sendata->data1>>8);
	txMessage.Data[1] = (uint8_t) sendata->data1;
	txMessage.Data[2] = (uint8_t)(sendata->data2>>8);
	txMessage.Data[3] = (uint8_t) sendata->data2;
	txMessage.Data[4] = (uint8_t)(sendata->data3>>8);
	txMessage.Data[5] = (uint8_t) sendata->data3;
	txMessage.Data[6] = (uint8_t)(sendata->data4>>8);
	txMessage.Data[7] = (uint8_t) sendata->data4;
	
	mbox= CAN_Transmit(CANx, &txMessage);   
	
	//等待发送结束
	while(CAN_TransmitStatus(CANx, mbox)==CAN_TxStatus_Failed){
		i++;	
		if(i>=0xFFF)
		break;
	}
}

void can_receive_msg(CanRxMsg *can_rx_msg,can_receiveStruct_t* receivedata){		   		   
    receivedata->data1 = ((uint16_t)can_rx_msg->Data[0] << 8) | can_rx_msg->Data[1];
	receivedata->data2 = ((uint16_t)can_rx_msg->Data[2] << 8) | can_rx_msg->Data[3];
	receivedata->data3 = ((uint16_t)can_rx_msg->Data[4] << 8) | can_rx_msg->Data[5];
	receivedata->data4 = ((uint16_t)can_rx_msg->Data[6] << 8) | can_rx_msg->Data[7];	
}

void can_data_pre(can_sendStruct_t* can_senData,uint32_t ID,uint16_t data1,uint16_t data2,uint16_t data3,uint16_t data4){
	can_senData->id = ID;
	can_senData->data1 = data1;
	can_senData->data2 = data2;
	can_senData->data3 = data3;
	can_senData->data4 = data4;
}




