#include "driver.h"

void driver_Init(void){
	drive_ADC1_Configuration();   //ADC≈‰÷√≥ı ºªØ
	drive_ADC3_Configuration();
	driver_IWDG_Init(4,625);
	
	driver_InitKeyHard();
	BSP_GPIO_Init(BSP_GPIOD7,GPIO_Mode_Out_PP);
	
}
