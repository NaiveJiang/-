#include "driver.h"

void driver_Init(void){
#if !USE_TEST_ICAP  //�������벶��ӿں�ADC��ͻ
	drive_ADC1_Configuration();   //ADC���ó�ʼ��
	drive_ADC3_Configuration();
#endif	
	driver_DAC1_Configuration();  //DAC���ó�ʼ��
	driver_DAC2_Configuration();
	
	drive_pulse_capture_Init(ICAP_TIMER);
	driver_CAN_Configuration(CAN_PREEMPTIONPRIORITY,CAN_SUBPRIORITY);  //CAN���ó�ʼ��
	can_data_pre(&can_id20A,CAN_ID1,1919,810,2077,1405);
	can_data_pre(&can_id209,CAN_ID2,810,1405,1919,2077);
	
	driver_GUI_USART_Configuration(GUI_USART,GUI_USART_TX,GUI_USART_RX,GUI_USART_BAUD,GUI_USART_PREEMPTIONPRIORITY,GUI_USART_SUBPRIORITY);  //GUI���ó�ʼ��
//	driver_485_Configuration(RS_485,RS_485_TX,RS_485_RX,RS_485_EN,RS_485_BAUD,RS_485_PREEMPTIONPRIORITY,RS_485_SUBPRIORITY);  //RS485���ó�ʼ��
	
	
	driver_IWDG_Init(4,625);
	
//	driver_InitKeyHard();
//	BSP_GPIO_Init(BSP_GPIOD7,GPIO_Mode_Out_PP);
	
}
