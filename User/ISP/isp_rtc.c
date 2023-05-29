#include "app.h"

//RTC时钟中断
//每秒触发一次  
//extern uint16_t tcnt; 
volatile int loops_rtc = 0;
void RTC_IRQHandler(void){		 
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET){//秒钟中断			
		RTC_Get();//更新时间   
//		printf("THE ERROR \r\n");
//		printf("RTC GetCounter = %d \r\n",RTC_GetCounter());//输出此时的秒数
 	}
	if(RTC_GetITStatus(RTC_IT_ALR) != RESET){//闹钟中断
//		printf("THE ALARM  READY =%d \r\n",RTC_GetCounter());//输出此时的秒数
		loops_rtc++;
//		PDout(7) = 1;
		RTC_ClearITPendingBit(RTC_IT_ALR);		//清闹钟中断	

//		PWR_BackupAccessCmd(ENABLE);
//		RTC_EnterConfigMode();	
//	    RTC_WaitForLastTask();
//		RTC_SetAlarm(40+RTC_GetCounter());	  //配置下次闹钟为40s后
//	    ALR_Set(2023, 4, 18, 22, 55, 30);
//		RTC_WaitForLastTask();
//		RTC_ExitConfigMode();   
//		PWR_BackupAccessCmd(DISABLE);
  	}
	
	RTC_ClearITPendingBit(RTC_IT_SEC | RTC_IT_OW);		//清闹钟中断
	RTC_WaitForLastTask();	  	    						 	   	 
}

//低压检测中断
void PVD_IRQHandler(void){
	EXTI_ClearITPendingBit(EXTI_Line16);
	//启动标志复位
	digitalLo(&get_supervisiorData()->machine_active);
	BKP_WriteBackupRegister(ACTIVITE_BKPREG,(uint16_t)(get_supervisiorData()->machine_active));
}

