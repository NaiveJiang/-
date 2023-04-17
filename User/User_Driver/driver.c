#include "driver.h"

void driver_Init(void){
	drive_ADC1_Configuration();   //ADC���ó�ʼ��
	drive_ADC3_Configuration();
	
	driver_DAC1_Configuration();  //DAC���ó�ʼ��
	driver_DAC2_Configuration();
	
	driver_CAN_Configuration(CAN_PREEMPTIONPRIORITY,CAN_SUBPRIORITY);  //CAN���ó�ʼ��
	can_data_pre(&can_id20A,CAN_ID1,1919,810,2077,1405);
	can_data_pre(&can_id209,CAN_ID2,810,1405,1919,2077);
	
	driver_IWDG_Init(4,625);
	
	driver_InitKeyHard();
	BSP_GPIO_Init(BSP_GPIOD7,GPIO_Mode_Out_PP);
	
}
