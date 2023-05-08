#include "BSP_USART.h"
#include "BSP_DMA_DEFINE.h"

uint8_t Array_USART1_RX[Length_USART1_RX_Buff];
uint8_t Array_USART2_RX[Length_USART2_RX_Buff];
uint8_t Array_USART3_RX[Length_USART3_RX_Buff];
uint8_t Array_UART4_RX [Length_UART4_RX_Buff];
uint8_t Array_UART5_RX [Length_UART5_RX_Buff];


uint8_t Array_USART1_TX[Length_USART1_TX_Buff];
uint8_t Array_USART2_TX[Length_USART2_TX_Buff];
uint8_t Array_USART3_TX[Length_USART3_TX_Buff];
uint8_t Array_UART4_TX [Length_UART4_TX_Buff];
uint8_t Array_UART5_TX [Length_UART5_TX_Buff];


/*
***************************************************
函数名：BSP_USART_RCC_Init
功能：配置USART外设时钟
入口参数：	USARTx：USART号
返回值：无
应用范围：内部调用
备注：
***************************************************
*/
void BSP_USART_RCC_Init(USART_TypeDef* USARTx){
	if(USARTx == USART1)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	else if(USARTx == USART2)
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	else if(USARTx == USART3)
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	else if(USARTx == UART4)
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
	else if(USARTx == UART5)
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
}

/*
***************************************************
函数名：GPIO_TO_NVIC_IRQChannel
功能：输出NVIC中断通道
入口参数：	GPIO：引脚号
返回值：NVIC_IRQChannel
应用范围：内部调用
备注：
***************************************************
*/
uint8_t	USART_TO_NVIC_IRQChannel(USART_TypeDef* USARTx){
	uint8_t NVIC_IRQChannel;
	
	if(USARTx == USART1)
		NVIC_IRQChannel = USART1_IRQn;
	else if(USARTx == USART2)
		NVIC_IRQChannel = USART2_IRQn;
	else if(USARTx == USART3)
		NVIC_IRQChannel = USART3_IRQn;
	else if(USARTx == UART4)
		NVIC_IRQChannel = UART4_IRQn;
	else if(USARTx == UART5)
		NVIC_IRQChannel = UART5_IRQn;
	return NVIC_IRQChannel;	
}

/*
***************************************************
函数名：USART_TO_ArrayRX
功能：输出串口接收数组地址
入口参数：	GPIO：引脚号
返回值：ArrayRX
应用范围：内部调用
备注：
***************************************************
*/
uint32_t USART_TO_ArrayRX(USART_TypeDef* USARTx){
	uint32_t ArrayRX;
	
	if(USARTx == USART1)
		ArrayRX = (uint32_t)&Array_USART1_RX;
	else if(USARTx == USART2)
		ArrayRX = (uint32_t)&Array_USART2_RX;
	else if(USARTx == USART3)
		ArrayRX = (uint32_t)&Array_USART3_RX;
	else if(USARTx == UART4)
		ArrayRX = (uint32_t)&Array_UART4_RX;
	else if(USARTx == UART5)
		ArrayRX = (uint32_t)&Array_UART5_RX;
	
	return ArrayRX;	
}

/*
***************************************************
函数名：USART_TO_LengthArrayRX
功能：输出串口接收数组长度
入口参数：	GPIO：引脚号
返回值：ArrayRX
应用范围：内部调用
备注：
***************************************************
*/
uint32_t	USART_TO_LengthArrayRX(USART_TypeDef* USARTx){
	uint32_t Length;
	
	if(USARTx == USART1)
		Length = Length_USART1_RX_Buff;
	else if(USARTx == USART2)
		Length = Length_USART2_RX_Buff;
	else if(USARTx == USART3)
		Length = Length_USART3_RX_Buff;
	else if(USARTx == UART4)
		Length = Length_UART4_RX_Buff;
	else if(USARTx == UART5)
		Length = Length_UART5_RX_Buff;

	return Length;	
}

/*
***************************************************
函数名：USART_TO_ArrayTX
功能：输出串口发送数组地址
入口参数：	GPIO：引脚号
返回值：ArrayRX
应用范围：内部调用
备注：
***************************************************
*/
uint32_t USART_TO_ArrayTX(USART_TypeDef* USARTx){
	uint32_t ArrayTX;
	
	if(USARTx == USART1)
		ArrayTX = (uint32_t)&Array_USART1_TX;
	else if(USARTx == USART2)
		ArrayTX = (uint32_t)&Array_USART2_TX;
	else if(USARTx == USART3)
		ArrayTX = (uint32_t)&Array_USART3_TX;
	else if(USARTx == UART4)
		ArrayTX = (uint32_t)&Array_UART4_TX;
	else if(USARTx == UART5)
		ArrayTX = (uint32_t)&Array_UART5_TX;
	
	return ArrayTX;	
}

/*
***************************************************
函数名：USART_TO_LengthArrayTX
功能：输出串口接收数组长度
入口参数：	GPIO：引脚号
返回值：ArrayTX
应用范围：内部调用
备注：
***************************************************
*/
uint32_t USART_TO_LengthArrayTX(USART_TypeDef* USARTx){
	uint32_t Length;
	
	if(USARTx == USART1)
		Length = Length_USART1_TX_Buff;
	else if(USARTx == USART2)
		Length = Length_USART2_TX_Buff;
	else if(USARTx == USART3)
		Length = Length_USART3_TX_Buff;
	else if(USARTx == UART4)
		Length = Length_UART4_TX_Buff;
	else if(USARTx == UART5)
		Length = Length_UART5_TX_Buff;
	
	return Length;	
}

BSP_USART_Typedef* USART_TO_BSP_DMA(USART_TypeDef* USARTx, uint16_t USART_DMAReq){
	BSP_USART_Typedef *USART_DMA_Pack;
	if(USART_DMAReq == USART_DMAReq_Tx){
		if(USARTx == USART1)
			USART_DMA_Pack = &BSP_DMA_USART1_TX;
		else if(USARTx == USART2)
			USART_DMA_Pack = &BSP_DMA_USART2_TX;
		else if(USARTx == USART3)
			USART_DMA_Pack = &BSP_DMA_USART3_TX;
		else if(USARTx == UART4)
			USART_DMA_Pack = &BSP_DMA_UART4_TX;
		else if(USARTx == UART5)
			USART_DMA_Pack = &BSP_DMA_UART5_TX;
	}
	else if(USART_DMAReq == USART_DMAReq_Rx){
		if(USARTx == USART1)
			USART_DMA_Pack = &BSP_DMA_USART1_RX;
		else if(USARTx == USART2)
			USART_DMA_Pack = &BSP_DMA_USART2_RX;
		else if(USARTx == USART3)
			USART_DMA_Pack = &BSP_DMA_USART3_RX;
		else if(USARTx == UART4)
			USART_DMA_Pack = &BSP_DMA_UART4_RX;
		else if(USARTx == UART5)
			USART_DMA_Pack = &BSP_DMA_UART5_RX;
	}
	return USART_DMA_Pack;
}

/*
***************************************************
函数名：BSP_USART_Init
功能：USART初始化
入口参数：	BSP_USARTx：串口号
					PreemptionPriority：抢占优先级 0xff时失能串口中断
					SubPriority：子优先级 0xff时失能串口中断
返回值：无
应用范围：外部调用
备注：不用串口中断时请  PreemptionPriority=0xff，SubPriority=0xff
***************************************************
*/
void BSP_USART_Init(USART_TypeDef *USARTx,BSP_GPIOSource_TypeDef *USART_TX,BSP_GPIOSource_TypeDef *USART_RX,uint32_t baudrate,uint8_t PreemptionPriority, uint8_t SubPriority){
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_InitTypeDef USART_InitStruct;
	
	/*************初始化USARTx时钟***************/
	BSP_USART_RCC_Init(USARTx);
	
	/*************初始化USARTx引脚***************/
	if(USART_TX != NULL)
		BSP_GPIO_Init(USART_TX, GPIO_Mode_AF_PP);	//初始化USART_TX引脚为复用推挽
	if(USART_RX != NULL)
		BSP_GPIO_Init(USART_RX, GPIO_Mode_IN_FLOATING);	//初始化USART_RX引脚为浮空输入
	
	/*************USARTx初始化设置***************/
	USART_InitStruct.USART_BaudRate = baudrate;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USARTx, &USART_InitStruct); //初始化串口x
	
	/*************USARTx NVIC配置***************/
	if( (PreemptionPriority != 0xff) && (SubPriority != 0xff) ){
		NVIC_InitStructure.NVIC_IRQChannel = USART_TO_NVIC_IRQChannel(USARTx);
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=PreemptionPriority ;//抢占优先级
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;		//子优先级
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
		NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
		if(USARTx != UART5)
			USART_ITConfig(USARTx, USART_IT_IDLE, ENABLE);//开启串口空闲中断
		else
			USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
	}
	
	if(USARTx != UART5){ 		//UART5没有DMA
		if(USART_TX != NULL)
			USART_DMACmd(USARTx,USART_DMAReq_Tx,ENABLE);	//开启串口DMA发送
		if(USART_RX != NULL)
			USART_DMACmd(USARTx,USART_DMAReq_Rx,ENABLE);	//开启串口DMA接收
	}
	
	USART_Cmd(USARTx, ENABLE);                    	//使能串口
}

//串口DMA配置
void BSP_USART_DMA_Init(BSP_USART_Typedef *USARTx_DMA){
	DMA_InitTypeDef DMA_InitStructure;
	if(USARTx_DMA->dma == __DMA1)
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //DMA1 时钟使能
	else
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE); //DMA1 时钟使能
	
	DMA_DeInit(USARTx_DMA->DMA_channel);
    
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USARTx_DMA->USARTx->DR; //DMA外设地址
	if(USARTx_DMA->mode == TX_MODE){ //发送还是接收
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
		DMA_InitStructure.DMA_MemoryBaseAddr = USART_TO_ArrayTX(USARTx_DMA->USARTx);
		DMA_InitStructure.DMA_BufferSize = USART_TO_LengthArrayTX(USARTx_DMA->USARTx);
	}
	else{
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
		DMA_InitStructure.DMA_MemoryBaseAddr = USART_TO_ArrayRX(USARTx_DMA->USARTx);
//		DMA_InitStructure.DMA_BufferSize = sizeof(USART_TO_ArrayRX(USARTx_DMA->USARTx)) / sizeof(uint8_t);
		DMA_InitStructure.DMA_BufferSize = USART_TO_LengthArrayRX(USARTx_DMA->USARTx);
	}
    
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; //DMA_Mode_Normal
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
    DMA_Init(USARTx_DMA->DMA_channel,&DMA_InitStructure);
	
	DMA_Cmd(USARTx_DMA->DMA_channel,ENABLE);
	
}

//DMA串口发送
void BSP_USART_SendData_DMA(BSP_USART_Typedef *BSP_USARTx,uint8_t *data,uint16_t len){
	while(DMA_GetCurrDataCounter(BSP_USARTx->DMA_channel));
	DMA_Cmd(BSP_USARTx->DMA_channel, DISABLE);
	DMA_ClearFlag((BSP_USARTx->DMATC_FLAG) | (BSP_USARTx->DMAHT_FLAG));
	BSP_USARTx->DMA_channel->CMAR = (uint32_t)data;
	BSP_USARTx->DMA_channel->CNDTR = len;
	DMA_Cmd(BSP_USARTx->DMA_channel, ENABLE);
}

//串口发送
void BSP_USART_SendData_(BSP_USART_Typedef *BSP_USARTx,uint8_t *data,uint16_t len){
	uint8_t i;
	for(i=0;i<len;i++){
		BSP_USARTx->USARTx->DR = data[i];
		while((BSP_USARTx->USARTx->SR & USART_FLAG_TC)==0);	//等待发送结束
	}
}







