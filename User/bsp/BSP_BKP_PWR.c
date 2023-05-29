#include "app.h"


//��ʼ��BKP��PWRʱ��
void bsp_BKP_PWRInit(void){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��   
	PWR_BackupAccessCmd(ENABLE);	//ʹ�ܺ󱸼Ĵ�������
}

//ʹ��PVD��ѹ���
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


//�������ж�
uint32_t bkp_hot_activite(void){
	uint32_t error = 0;
	//��ȡ�󱸼Ĵ����������1��˵�����������������0��������
	
	if(BKP_ReadBackupRegister(ACTIVITE_BKPREG)){
		//����״̬Ϊ������״̬,��ȡ�������Ĺ���״̬,���������ֳ�
		set_controlState((controlState_e)BKP_ReadBackupRegister(CONTROL_STATE_BKPREG));
		get_controlData()->control_step = (uint8_t)BKP_ReadBackupRegister(CONTROL_STEP_BKPREG);
		//��������ȡ������ǰ�Ŀ��Ʋ���
		
		error = IN_HOT_ACTIVE;
	}
	return error;
}
























