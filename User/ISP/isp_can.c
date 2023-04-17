#include "app.h"

uint32_t can_loops;
//CAN1Ω” ’÷–∂œ
void USB_LP_CAN1_RX0_IRQHandler(void){
  	CanRxMsg RxMessage;
	if(CAN_GetITStatus(CAN1,CAN_IT_FMP0)!=RESET){
		CAN_ClearITPendingBit(CAN1, CAN_IT_FF0);
		CAN_ClearFlag(CAN1, CAN_FLAG_FF0);
		CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
		switch(RxMessage.StdId){ 
				case CAN_ID1 : 	can_receive_msg(&RxMessage,&can_id20A_res); break;
				case CAN_ID2 :	can_receive_msg(&RxMessage,&can_id209_res); break;	   		 	
			}
		can_loops++;
	}
}














