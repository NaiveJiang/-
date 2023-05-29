#include "app.h"

//RTCʱ���ж�
//ÿ�봥��һ��  
//extern uint16_t tcnt; 
volatile int loops_rtc = 0;
void RTC_IRQHandler(void){		 
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET){//�����ж�			
		RTC_Get();//����ʱ��   
//		printf("THE ERROR \r\n");
//		printf("RTC GetCounter = %d \r\n",RTC_GetCounter());//�����ʱ������
 	}
	if(RTC_GetITStatus(RTC_IT_ALR) != RESET){//�����ж�
//		printf("THE ALARM  READY =%d \r\n",RTC_GetCounter());//�����ʱ������
		loops_rtc++;
//		PDout(7) = 1;
		RTC_ClearITPendingBit(RTC_IT_ALR);		//�������ж�	

//		PWR_BackupAccessCmd(ENABLE);
//		RTC_EnterConfigMode();	
//	    RTC_WaitForLastTask();
//		RTC_SetAlarm(40+RTC_GetCounter());	  //�����´�����Ϊ40s��
//	    ALR_Set(2023, 4, 18, 22, 55, 30);
//		RTC_WaitForLastTask();
//		RTC_ExitConfigMode();   
//		PWR_BackupAccessCmd(DISABLE);
  	}
	
	RTC_ClearITPendingBit(RTC_IT_SEC | RTC_IT_OW);		//�������ж�
	RTC_WaitForLastTask();	  	    						 	   	 
}

//��ѹ����ж�
void PVD_IRQHandler(void){
	EXTI_ClearITPendingBit(EXTI_Line16);
	//������־��λ
	digitalLo(&get_supervisiorData()->machine_active);
	BKP_WriteBackupRegister(ACTIVITE_BKPREG,(uint16_t)(get_supervisiorData()->machine_active));
}

