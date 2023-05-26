#include "bsp.h"
#include "util.h"

//Mini STM32������
//RTCʵʱʱ�� ��������
//����ԭ��@ALIENTEK
//2010/6/6
_calendar_obj calendar;//ʱ�ӽṹ�� 
 
static void RTC_NVIC_Config(uint8_t PreemptionPriority,uint8_t SubPriority){	
    NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;		//RTCȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PreemptionPriority;	//��ռ���ȼ�1λ,�����ȼ�3λ
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;	//��ռ���ȼ�0λ,�����ȼ�4λ
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//ʹ�ܸ�ͨ���ж�
	NVIC_Init(&NVIC_InitStructure);		//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}

//ʵʱʱ������
//��ʼ��RTCʱ��,ͬʱ���ʱ���Ƿ�������
//BKP->DR1���ڱ����Ƿ��һ�����õ�����
//����0:����
//����:�������
uint8_t bsp_RTC_Init(void){
	//����ǲ��ǵ�һ������ʱ��
	uint8_t temp=0;  
	if (BKP_ReadBackupRegister(BKP_DR1) != RTC_BACKUP_DATA){		//��ָ���ĺ󱸼Ĵ���DR1�ж�������:��������д���ָ�����ݲ����
		BKP_DeInit();				//��λ�������� 
		
		RCC_LSEConfig(RCC_LSE_ON);	//�����ⲿ���پ���(LSE),ʹ��������پ���
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET){	//���ָ����RCC��־λ�������,�ȴ����پ������
			temp++;
			delay_ms(10);
		}
		
		if(temp>=250) return 1;//��ʼ��ʱ��ʧ��,����������	    
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		//����RTCʱ��(RTCCLK),ѡ��LSE��ΪRTCʱ��    
		RCC_RTCCLKCmd(ENABLE);	//ʹ��RTCʱ��  
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_WaitForSynchro();		//�ȴ�RTC�Ĵ���ͬ��  
//		RTC_ITConfig(RTC_IT_SEC|RTC_IT_ALR, ENABLE);		//ʹ��RTC���ж�
		RTC_ITConfig(RTC_IT_ALR, ENABLE);		//ʹ��RTC�����ж�
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_EnterConfigMode();/// ��������	
		
		RTC_SetCounter(0);    //���ü�����ʼֵ 
		RTC_WaitForLastTask();
		
		RTC_SetPrescaler(32767); //����RTCԤ��Ƶ��ֵ
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
				
		RTC_Set(2023,5,9,11,02,0);  //����ʱ��	
		
//		RTC_SetAlarm(40+RTC_GetCounter());      //���Ӷ�ʱ40S
		ALR_Set(2023, 5, 9, 11, 02, 20);
		RTC_WaitForLastTask();
		
		RTC_ExitConfigMode(); //�˳�����ģʽ  
		BKP_WriteBackupRegister(BKP_DR1, RTC_BACKUP_DATA);	//��ָ���ĺ󱸼Ĵ���DR1��д���û���������
	}
	else{	//ϵͳ������ʱ
		RTC_WaitForSynchro();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
//		RTC_ITConfig(RTC_IT_SEC|RTC_IT_ALR, ENABLE);	//ʹ��RTC���ж�
		RTC_ITConfig(RTC_IT_ALR, ENABLE);		//ʹ��RTC�����ж�
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		
//		RTC_EnterConfigMode();/// ��������			
//		RTC_SetAlarm(40+RTC_GetCounter());      //���Ӷ�ʱ40S
//		RTC_WaitForLastTask();
//		
//		RTC_ExitConfigMode(); //�˳�����ģʽ 
	}
	RTC_NVIC_Config(RTC_PREEMPTIONPRIORITY,RTC_SUBPRIORITY);//RCT�жϷ�������		    				     
	RTC_Get();//����ʱ��	
	return 0; //ok
}

//�ж��Ƿ������꺯��
//�·�   1  2  3  4  5  6  7  8  9  10 11 12
//����   31 29 31 30 31 30 31 31 30 31 30 31
//������ 31 28 31 30 31 30 31 31 30 31 30 31
//����:���
//���:������ǲ�������.1,��.0,����
uint8_t Is_Leap_Year(uint16_t year){			  
	if(year%4==0){ //�����ܱ�4����
		if(year%100==0){
			if(year%400==0) return 1;//�����00��β,��Ҫ�ܱ�400���� 	   
			else return 0;   
		}else return 1;   
	}else return 0;	
}

//����ʱ��
//�������ʱ��ת��Ϊ����
//��1970��1��1��Ϊ��׼
//1970~2099��Ϊ�Ϸ����
//����ֵ:0,�ɹ�;����:�������.
//�·����ݱ�											 
uint8_t const table_week[12] = {0,3,3,6,1,4,6,2,5,0,3,5}; //���������ݱ�	  
//ƽ����·����ڱ�
const uint8_t mon_table[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
uint8_t RTC_Set(uint16_t syear,uint8_t smon,uint8_t sday,uint8_t hour,uint8_t min,uint8_t sec){
	uint16_t t;
	uint32_t seccount = 0;
	if(syear < 1970 || syear > 2099) return 1;	   
	for(t = 1970; t < syear; t++){	//��������ݵ��������
		if(Is_Leap_Year(t))seccount += 31622400;//�����������
		else seccount += 31536000;			  //ƽ���������
	}
	smon -= 1;
	for(t = 0; t < smon; t++){	   //��ǰ���·ݵ����������
		seccount += (uint32_t)mon_table[t] * 86400;//�·����������
		if(Is_Leap_Year(syear) && t == 1)seccount += 86400;//����2�·�����һ���������	   
	}
	seccount += (uint32_t)(sday - 1) * 86400;//��ǰ�����ڵ���������� 
	seccount += (uint32_t)hour * 3600;//Сʱ������
    seccount += (uint32_t)min * 60;	 //����������
	seccount += sec;//�������Ӽ���ȥ

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��  
	PWR_BackupAccessCmd(ENABLE);	//ʹ��RTC�ͺ󱸼Ĵ������� 
	RTC_SetCounter(seccount);	//����RTC��������ֵ

	RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������  	
	RTC_Get();
	return 0;	    
}


//�õ���ǰ��ʱ��
//����ֵ:0,�ɹ�;����:�������.
uint8_t RTC_Get(void){
	static uint16_t daycnt = 0;
	uint32_t timecount = 0; 
	uint32_t temp = 0;
	uint16_t temp1 = 0;	  
    timecount = RTC_GetCounter();	 
 	temp = timecount / 86400;   //�õ�����(��������Ӧ��)
	if(daycnt != temp){//����һ����
		daycnt = temp;
		temp1 = 1970;	//��1970�꿪ʼ
		while(temp >= 365){	 
			if(Is_Leap_Year(temp1)){//������
				if(temp >= 366)temp -= 366;//�����������
				else {
					temp1++;
					break;
				}  
			}
			else temp -= 365;	  //ƽ�� 
			temp1++;  
		}   
		calendar.w_year = temp1;//�õ����
		temp1 = 0;
		while(temp >= 28){//������һ����
			if(Is_Leap_Year(calendar.w_year) && temp1 == 1){//�����ǲ�������/2�·�
				if(temp >= 29) temp -= 29;//�����������
				else break; 
			}
			else {
				if(temp >= mon_table[temp1]) temp -= mon_table[temp1];//ƽ��
				else break;
			}
			temp1++;  
		}
		calendar.w_month = temp1 + 1;	//�õ��·�
		calendar.w_date = temp + 1;  	//�õ����� 
	}
	temp = timecount % 86400;     		//�õ�������   	   
	calendar.hour = temp / 3600;     	//Сʱ
	calendar.min = (temp % 3600) / 60; 	//����	
	calendar.sec = (temp % 3600) % 60; 	//����
	calendar.week = RTC_Get_Week(calendar.w_year,calendar.w_month,calendar.w_date);//��ȡ����   
	return 0;
}	 
//������������ڼ�
//��������:���빫�����ڵõ�����(ֻ����1901-2099��)
//������������������� 
//����ֵ�����ں�																						 
uint8_t RTC_Get_Week(uint16_t year,uint8_t month,uint8_t day){	
	uint16_t temp2;
	uint8_t yearH,yearL;
	
	yearH = year / 100;
	yearL = year % 100; 
	// ���Ϊ21����,�������100  
	if(yearH > 19) yearL += 100;
	// ����������ֻ��1900��֮���  
	temp2 = yearL + yearL / 4;
	temp2 = temp2 % 7; 
	temp2 = temp2 + day + table_week[month - 1];
	if((yearL % 4 == 0) && (month < 3)) temp2--;
	return (temp2 % 7);
}			  

uint8_t ALR_Set(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t min, uint8_t sec){
    uint16_t t;
    uint32_t seccounter = 0;

    if(year < 1970 || year > 2099) return 1;

    for(t = 1970; t < year; t++){
        if(Is_Leap_Year(t)) seccounter += 31622400;
        else seccounter += 31536000;
    }

    month -= 1;

    for(t = 0; t < month; t++){
        seccounter += (uint32_t)mon_table[t] * 86400;

        if(Is_Leap_Year(year) && t == 1) seccounter += 86400;
    }

    seccounter += (uint32_t)(day - 1) * 86400;
    seccounter += (uint32_t)hour * 3600;
    seccounter += (uint32_t)min * 60;
    seccounter += sec;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP | RCC_APB1Periph_PWR, ENABLE);
    PWR_BackupAccessCmd(ENABLE);
    RTC_EnterConfigMode();
    RTC_SetAlarm(seccounter);
    RTC_WaitForLastTask();
    return 0;
}


