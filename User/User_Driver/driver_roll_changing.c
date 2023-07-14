#include "driver.h"


void driver_roll_changing(void){
	//换卷信号外部中断初始化
	BSP_GPIO_EXTI_Init(HJSH_PORT,EXTI_Trigger_Rising,ROLL_CHANGING_PREEMPTIONPRIORITY,ROLL_CHANGING_SUBPRIORITY);
}









