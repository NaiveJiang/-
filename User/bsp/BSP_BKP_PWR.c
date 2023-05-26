#include "app.h"


//初始化BKP和PWR时钟
void bsp_BKP_PWRInit(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟   
	PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问
}

//使能PVD低压检测
void bsp_PVDInit(void){
	
	NVIC_InitTypeDef NVIC_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	
	PWR_PVDLevelConfig(PWR_PVDLevel_2V9);
	PWR_PVDCmd(ENABLE);
	
	NVIC_InitStruct.NVIC_IRQChannel = PVD_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = PVD_PREEMPTIONPRIORITY;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = PVD_SUBPRIORITY;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	EXTI_ClearITPendingBit(EXTI_Line16);
	EXTI_InitStruct.EXTI_Line = EXTI_Line16;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
}


//热启动判断
uint32_t bkp_hot_activite(void){
	uint32_t error = 0;
	//读取后备寄存器，如果是1则说明是热启动，如果是0则是冷起动
	
	if(BKP_ReadBackupRegister(ACTIVITE_BKPREG)){
		//启动状态为热启动状态,读取热启动的工作状态,方便重入现场
		set_controlState((controlState_e)BKP_ReadBackupRegister(CONTROL_STATE_BKPREG));
		//热启动获取热启动前的控制参数
		
		error = IN_HOT_ACTIVE;
	}
	return error;
}
























