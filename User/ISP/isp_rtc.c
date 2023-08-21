#include "app.h"

//RTC时钟中断
//每秒触发一次  
//extern uint16_t tcnt; 
volatile int loops_rtc = 0;
void RTC_IRQHandler(void){		 
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET){//秒钟中断			
//		RTC_Get();//更新时间
		digitalIncreasing(&get_controlData()->rtc_sec);	//秒
		if(get_controlData()->rtc_sec >= 3600){
			digitalIncreasing(&get_controlData()->rtc_hour); //时
			digitalClan(&get_controlData()->rtc_sec); //清除秒
			
		}
		
		if((!(get_controlData()->rtc_hour % 5)) && get_controlData()->rtc_hour){	//每隔5h写入一次flash
			if(get_controlData()->last_hour != get_controlData()->rtc_hour) //防止重复触发
				digitalHi(&get_supervisiorData()->flash_sw);
		}
		
		if((!(get_controlData()->rtc_hour % 24)) && get_controlData()->rtc_hour){	//减去天数
			if(get_controlData()->last_hour != get_controlData()->rtc_hour){ //防止重复触发
				if(get_controlData()->rtc_day1)
					digitalDecline(&get_controlData()->rtc_day1);	//天数减一
				if(get_controlData()->rtc_day2)
					digitalDecline(&get_controlData()->rtc_day2);
				if(get_controlData()->rtc_day3)
					digitalDecline(&get_controlData()->rtc_day3);
			}
		}
		get_controlData()->last_hour = get_controlData()->rtc_hour;
		
		RTC_ClearITPendingBit(RTC_IT_SEC | RTC_IT_OW);		//清闹钟中断
 	}
	
	if(RTC_GetITStatus(RTC_IT_ALR) != RESET){//闹钟中断
		loops_rtc++;
		RTC_ClearITPendingBit(RTC_IT_ALR);		//清闹钟中断	
  	}
	RTC_WaitForLastTask();	  	    						 	   	 
}

//低压检测中断
void PVD_IRQHandler(void){
	EXTI_ClearITPendingBit(EXTI_Line16);
	//启动标志复位
	digitalLo(&get_supervisiorData()->machine_active);
	BKP_WriteBackupRegister(ACTIVITE_BKPREG,(uint16_t)(get_supervisiorData()->machine_active));
}

//入侵检测中断
void TAMPER_IRQHandler(void){
	if(BKP_GetITStatus()!=RESET){ 
		//将机器上锁，机器进入停机状态
		digitalHi(&get_controlData()->lock_tamp);
		BKP_ClearITPendingBit();//清除侵入检测中断
		BKP_ClearFlag();//清除侵入检测事件
	}
}

