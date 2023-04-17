#include "BSP_CAN.h"

//CAN底层配置
void BSP_CAN_Mode_Init(CAN_TypeDef *BSP_CANx,uint8_t tsjw,uint8_t tbs1,uint8_t tbs2,uint16_t brp,uint8_t PreemptionPriority,uint8_t SubPriority){
	CAN_InitTypeDef		CAN_InitStructure;
	NVIC_InitTypeDef  	NVIC_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStruct;
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
	
	/******CAN单元配置******/
	
	//波特率相关配置，CAN波特率 = fAPB1/((tbs1+tbs2+1)*brp),APB1时钟最大频率36M,CAN2.0最大波特率1M
	CAN_InitStructure.CAN_BS1 = tbs1;		//相位缓冲段1
	CAN_InitStructure.CAN_BS2 = tbs2;		//相位缓冲段2
	CAN_InitStructure.CAN_SJW = tsjw;		//再同步补偿宽度,用于波特率精度调节,越大容错越高,即精度降低
	CAN_InitStructure.CAN_Prescaler = brp;	//波特率分频器
	
	CAN_InitStructure.CAN_TTCM = DISABLE;	//非时间触发通信模式 
	CAN_InitStructure.CAN_ABOM = ENABLE;    //软件自动离线管理,可自动恢复总线离线状态
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal; //标准模式为工作模式，其余模式都为测试用模式
	
	CAN_Init(BSP_CANx,&CAN_InitStructure);
	
	//CAN过滤器配置
	CAN_FilterInitStruct.CAN_FilterNumber = 0;
	CAN_FilterInitStruct.CAN_FilterMode = CAN_FilterMode_IdMask;  //掩码模式
	CAN_FilterInitStruct.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStruct.CAN_FilterIdHigh = 0x0000;  //过滤器32位ID标识,全0即表示所有ID都可通过
	CAN_FilterInitStruct.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStruct.CAN_FilterMaskIdHigh = 0x0000; //过滤器32位掩码，全0表示不需掩码对应位
	CAN_FilterInitStruct.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStruct.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0; //过滤器关联FIFO0
	CAN_FilterInitStruct.CAN_FilterActivation = ENABLE; //启动过滤器
	
	CAN_FilterInit(&CAN_FilterInitStruct);
	
	CAN_ITConfig(BSP_CANx,CAN_IT_FMP0,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PreemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//CAN数据发送
uint8_t BSP_CAN_Send_Msg(CAN_TypeDef *BSP_CANx,CanTxMsg *TxMessage){
	uint8_t mbox;
	uint16_t i = 0;
	mbox = CAN_Transmit(BSP_CANx,TxMessage);
	
	while(CAN_TransmitStatus(BSP_CANx, mbox)==CAN_TxStatus_Failed){
		i++;
		if(i >= 0xFFF) return 1;
	}
	return 0;
}
















