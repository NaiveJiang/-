#include "app.h"

//RTCʱ���ж�
//ÿ�봥��һ��  
//extern uint16_t tcnt; 
volatile int loops_rtc = 0;
void RTC_IRQHandler(void){		 
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET){//�����ж�			
//		RTC_Get();//����ʱ��
		digitalIncreasing(&get_controlData()->rtc_sec);	//��
		if(get_controlData()->rtc_sec >= 3600){
			digitalIncreasing(&get_controlData()->rtc_hour); //ʱ
			digitalClan(&get_controlData()->rtc_sec); //�����
			
		}
		
		if((!(get_controlData()->rtc_hour % 5)) && get_controlData()->rtc_hour){	//ÿ��5hд��һ��flash
			if(get_controlData()->last_hour != get_controlData()->rtc_hour) //��ֹ�ظ�����
				digitalHi(&get_supervisiorData()->flash_sw);
		}
		
		if((!(get_controlData()->rtc_hour % 24)) && get_controlData()->rtc_hour){	//��ȥ����
			if(get_controlData()->last_hour != get_controlData()->rtc_hour){ //��ֹ�ظ�����
				if(get_controlData()->rtc_day1)
					digitalDecline(&get_controlData()->rtc_day1);	//������һ
				if(get_controlData()->rtc_day2)
					digitalDecline(&get_controlData()->rtc_day2);
				if(get_controlData()->rtc_day3)
					digitalDecline(&get_controlData()->rtc_day3);
			}
		}
		get_controlData()->last_hour = get_controlData()->rtc_hour;
		
		RTC_ClearITPendingBit(RTC_IT_SEC | RTC_IT_OW);		//�������ж�
 	}
	
	if(RTC_GetITStatus(RTC_IT_ALR) != RESET){//�����ж�
		loops_rtc++;
		RTC_ClearITPendingBit(RTC_IT_ALR);		//�������ж�	
  	}
	RTC_WaitForLastTask();	  	    						 	   	 
}

//��ѹ����ж�
void PVD_IRQHandler(void){
	EXTI_ClearITPendingBit(EXTI_Line16);
	//������־��λ
	digitalLo(&get_supervisiorData()->machine_active);
	BKP_WriteBackupRegister(ACTIVITE_BKPREG,(uint16_t)(get_supervisiorData()->machine_active));
}

//���ּ���ж�
void TAMPER_IRQHandler(void){
	if(BKP_GetITStatus()!=RESET){ 
		//��������������������ͣ��״̬
		digitalHi(&get_controlData()->lock_tamp);
		BKP_ClearITPendingBit();//����������ж�
		BKP_ClearFlag();//����������¼�
	}
}

