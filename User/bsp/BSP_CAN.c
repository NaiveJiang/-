#include "BSP_CAN.h"

//CAN�ײ�����
void BSP_CAN_Mode_Init(CAN_TypeDef *BSP_CANx,uint8_t tsjw,uint8_t tbs1,uint8_t tbs2,uint16_t brp,uint8_t PreemptionPriority,uint8_t SubPriority){
	CAN_InitTypeDef		CAN_InitStructure;
	NVIC_InitTypeDef  	NVIC_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStruct;
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
	
	/******CAN��Ԫ����******/
	
	//������������ã�CAN������ = fAPB1/((tbs1+tbs2+1)*brp),APB1ʱ�����Ƶ��36M,CAN2.0�������1M
	CAN_InitStructure.CAN_BS1 = tbs1;		//��λ�����1
	CAN_InitStructure.CAN_BS2 = tbs2;		//��λ�����2
	CAN_InitStructure.CAN_SJW = tsjw;		//��ͬ���������,���ڲ����ʾ��ȵ���,Խ���ݴ�Խ��,�����Ƚ���
	CAN_InitStructure.CAN_Prescaler = brp;	//�����ʷ�Ƶ��
	
	CAN_InitStructure.CAN_TTCM = DISABLE;	//��ʱ�䴥��ͨ��ģʽ 
	CAN_InitStructure.CAN_ABOM = ENABLE;    //����Զ����߹���,���Զ��ָ���������״̬
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal; //��׼ģʽΪ����ģʽ������ģʽ��Ϊ������ģʽ
	
	CAN_Init(BSP_CANx,&CAN_InitStructure);
	
	//CAN����������
	CAN_FilterInitStruct.CAN_FilterNumber = 0;
	CAN_FilterInitStruct.CAN_FilterMode = CAN_FilterMode_IdMask;  //����ģʽ
	CAN_FilterInitStruct.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStruct.CAN_FilterIdHigh = 0x0000;  //������32λID��ʶ,ȫ0����ʾ����ID����ͨ��
	CAN_FilterInitStruct.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStruct.CAN_FilterMaskIdHigh = 0x0000; //������32λ���룬ȫ0��ʾ���������Ӧλ
	CAN_FilterInitStruct.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStruct.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0; //����������FIFO0
	CAN_FilterInitStruct.CAN_FilterActivation = ENABLE; //����������
	
	CAN_FilterInit(&CAN_FilterInitStruct);
	
	CAN_ITConfig(BSP_CANx,CAN_IT_FMP0,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PreemptionPriority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//CAN���ݷ���
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
















