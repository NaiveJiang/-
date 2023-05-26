#include "app.h"

void driver_Init(void){
	//IO口初始化
	driver_port_Init();
	//如果为冷启动，先进行端口准备
	if(!(get_controlData()->error_sta & IN_HOT_ACTIVE)) driver_port_detection();
	//辅助定时器配置
	BSP_TIM_INT_Init(ASSIST_TIM,ASSIST_TIM_PEROID,ASSIST_TIM_PRESCALER,ASSIST_TIM_PREEMPTIONPRIORITY,ASSIST_TIM_SUBPRIORITY);
	//ADC配置初始化
	drive_ADC1_Configuration();   
	drive_ADC3_Configuration();
	drive_ADC_sampling_TIM_init(SAMP_TIMER1);
	drive_ADC_sampling_TIM_init(SAMP_TIMER2);
	//DAC配置初始化
	driver_DAC1_Configuration();  
	driver_DAC2_Configuration();
	//输入捕获配置初始化
	drive_pulse_capture_Init();
	//CAN配置初始化
	driver_CAN_Configuration(CAN_PREEMPTIONPRIORITY,CAN_SUBPRIORITY);  
	can_data_pre(&can_id20A,CAN_ID1,1919,810,2077,1405);
	can_data_pre(&can_id209,CAN_ID2,810,1405,1919,2077);
	//GUI串口配置初始化
	driver_GUI_USART_Configuration(GUI_USART,GUI_USART_TX,GUI_USART_RX,GUI_USART_BAUD,GUI_USART_PREEMPTIONPRIORITY,GUI_USART_SUBPRIORITY); 
	//RS485配置初始化	
	driver_485_Configuration(RS_485,RS_485_TX,RS_485_RX,RS_485_EN,RS_485_BAUD,RS_485_PREEMPTIONPRIORITY,RS_485_SUBPRIORITY);  
	//看门狗初始化
	driver_IWDG_Init(4,625);
	
//	driver_InitKeyHard();
//	BSP_GPIO_Init(BSP_GPIOD7,GPIO_Mode_Out_PP);
	
}
