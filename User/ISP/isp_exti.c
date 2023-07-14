#include "app.h"

void EXTI4_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line4) != RESET){
		//�����ϴ���
		get_dryCtrlData()->dry_power *= 0.5f; //����������ʵ���ǰ���ʵ�50%
		digitalHi(&get_dryCtrlData()->spark_wait);
		digitalClan(&get_dryCtrlData()->dry_time);	//���¼�ʱ
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
}

//�ⲿ�ж�10~15�жϷ�����
void EXTI15_10_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line11) != RESET){
		//�����ʪ����״̬����Ӧ�����ź�
		if(get_controlState() != __DRY){	//��Ϊʪ����״̬
			HJSHDL = 1;	//���������ʱ
			if(!get_rcCtrlData()->state){
				digitalHi(&get_rcCtrlData()->state);
				//��¼��һ�ε�״̬
				get_controlData()->last_control_step = get_controlData()->control_step;
				last_controlState = get_controlState();
				set_controlState(__ROLL_CHANGING,get_controlData()->control_step);	//���뻻��״̬
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line11);
	}
}











