#include "driver.h"


void driver_roll_changing(void){
	//�����ź��ⲿ�жϳ�ʼ��
	BSP_GPIO_EXTI_Init(HJSH_PORT,EXTI_Trigger_Rising,ROLL_CHANGING_PREEMPTIONPRIORITY,ROLL_CHANGING_SUBPRIORITY);
}









