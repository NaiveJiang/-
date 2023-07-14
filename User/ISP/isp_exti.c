#include "app.h"

void EXTI4_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line4) != RESET){
		//产生较大打火
		get_dryCtrlData()->dry_power *= 0.5f; //降低输出功率到当前功率的50%
		digitalHi(&get_dryCtrlData()->spark_wait);
		digitalClan(&get_dryCtrlData()->dry_time);	//重新计时
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
}

//外部中断10~15中断服务函数
void EXTI15_10_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line11) != RESET){
		//如果是湿启动状态不响应换卷信号
		if(get_controlState() != __DRY){	//不为湿启动状态
			HJSHDL = 1;	//输出换卷延时
			if(!get_rcCtrlData()->state){
				digitalHi(&get_rcCtrlData()->state);
				//记录上一次的状态
				get_controlData()->last_control_step = get_controlData()->control_step;
				last_controlState = get_controlState();
				set_controlState(__ROLL_CHANGING,get_controlData()->control_step);	//进入换卷状态
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line11);
	}
}











