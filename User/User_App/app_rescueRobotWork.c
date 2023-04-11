#include "app.h"

LimiFlag_t LimiFlag;
void roomChoose(rescueRobotState_t *rescueRobotState);
void app_rescueWorkReady(void)
{
	/*待补充*/ //用按键控制小车启动//
	if(rescueRobotState.workMode == RESCUEING){
		rescueRobotState.step = DOING;
		rescueRobotState.workStep = NULL;
		rescueRobotState.rescueTaskstep = WALKING_MODE;
		command = '0';
	}
}
/***寻火任务**/
void app_rescueWorkDoing(void){
	app_UCLIMI_FORNT(SRF_04_Data1.getDistance,SRF_04_Data2.getDistance); //前限位
	app_UCLIMI_LEFT (SRF_04_Data3.getDistance,SRF_04_Data4.getDistance);  //左限位
	app_UCLIMI_RIGHT(SRF_04_Data5.getDistance,SRF_04_Data6.getDistance); //右限位
	app_UCLIMI_SPEED(SRF_04_Data1.getDistance,SRF_04_Data2.getDistance);  //速度限制
	if(rescueRobotState.beginFlag == FREE && rescueRobotState.workMode != TEST){
		switch(rescueRobotState.rescueTaskstep){
			case WALKING_MODE:roomChoose(&rescueRobotState);break;
			case DRIVING_MODE:wifiControl(command);break;
			case DRIVER_INDEEP:break;
		}
	
	}
}
 
///////////救援串口控制限位////////////////
void app_UCLIMI_SPEED(float distance1,float distance2){
	if((distance1<25.0)&&(distance2<25.0)){
	  speed_cut = go;
	}
	else{
		speed_cut = stop;
	}
}

void app_UCLIMI_FORNT(float distance1,float distance2){//前限位
 if((distance1<15.0)&&(distance2<15.0))  LimiFlag.forward = stop;
   else  LimiFlag.forward = go;
} 
void app_UCLIMI_LEFT(float distance1,float distance2){ //左限位
 if((distance1<13.0)&&(distance2<13.0)) LimiFlag.left = stop;
   else  LimiFlag.left = go;
}  
void app_UCLIMI_RIGHT(float distance1,float distance2){//右限位
 if((distance1<13.0)&&(distance2<1.0))  LimiFlag.right = stop;
   else LimiFlag.right = go;
}
///////////rescue_AUTOTASK/////////////////
void rescue_AUTOTASK_PUTCLAW(void){
		app_CLAW_HOLDorUNON(claw_UNON,250);
		vTaskDelay(20);		
		app_CLAW_UPorDOWN(claw_DOWN,240);
		app_CAMERA_UPorDOWN(camera_DOWN,220);	
}
void rescue_AUTOTASK_HOLDCLAW(void){
		app_CLAW_HOLDorUNON(claw_HOLD,350);
		vTaskDelay(500);	
		app_CLAW_UPorDOWN(claw_UP,200);
		app_CAMERA_UPorDOWN(camera_UP,200);
}
/***********rescue_AutoBackTask****/
/***********doing know*************/
void app_back_One(void)
{
	
}

void app_back_Two(void)
{
	
}
void app_back_Three(void)
{
	
}
void app_back_Four(void)
{
	
}
void app_RoomOne(void){
	switch(rescueRobotState.workStep){
		case 0:{
			calibrationFinish = app_calibration(RIGHT);
			if(calibrationFinish == CALIBRATION_FINISHED){
				rescueRobotState.workStep++;
			}break;
		}
		case 1:{
			rescueRobotState.moveWays = GO_STRAIGHT;
			calibrationGostraight(RIGHT);
			if(SRF_04_Data1.getDistance <24 && SRF_04_Data2.getDistance <24){
				rescueRobotState.workStep++;
			}break;
		}
		case 2:{
			rescueRobotState.moveWays = STOP;
			rescueRobotState.workStep++;
			break;
		}
		case 3:{
			rescueRobotState.moveWays = REVERSE_LEFT_TURN_90;
			rescueRobotState.workStep++;
			break;
		}
		case 4:{
			rescueRobotState.moveWays = STOP;
			rescueRobotState.workStep++;
			break;
		}
		case 5:{
			calibrationFinish = app_calibration(RIGHT);
			if(calibrationFinish == CALIBRATION_FINISHED){
				rescueRobotState.workStep++;
			}break;
		}
		case 6:{
			calibrationGostraight(RIGHT);
			if(SRF_04_Data1.getDistance <= 76&& SRF_04_Data2.getDistance <= 76){
				rescueRobotState.moveWays = STOP;
				rescueRobotState.workStep++;
			}break;
		}
		case 7:{
			rescueRobotState.moveWays = REVERSE_LEFT_TURN_90;
			rescueRobotState.workStep++;
			break;
		}
		case 8:{
			rescueRobotState.moveWays = STOP;
			rescueRobotState.workStep = 99;
			break;
		}
		case 99:{
			rescueRobotState.moveWays = OTHER;
			rescueRobotState.workStep = 99;
			rescueRobotState.rescueTaskstep = DRIVING_MODE;
			break;
		}
	}
}
void app_RoomTwo(void){
	switch(rescueRobotState.workStep){
  case 0:{
			calibrationFinish = app_calibration(LEFT);
			if(calibrationFinish == CALIBRATION_FINISHED){
				rescueRobotState.moveWays = OTHER;
				rescueRobotState.workStep++;
			}break;
		}
			case 1:{
				calibrationGostraight(LEFT);
				if(calibrationFinish == CALIBRATION_FINISHED){
				rescueRobotState.workStep++;
				}break;		
				}				
			case 2:{
				if(SRF_04_Data1.getDistance <=29 && SRF_04_Data2.getDistance <=29){
			 rescueRobotState.moveWays =REVERSE_RIGHT_TURN_90;
				rescueRobotState.workStep ++;
				}
				break;
			}
			case 3:{
				rescueRobotState.moveWays = STOP;
				rescueRobotState.workStep++;
				break;
				}
			case 4:{
			calibrationFinish = app_calibration(LEFT);
				if(calibrationFinish == CALIBRATION_FINISHED){			
					if(SRF_04_Data5.getDistance >25&&SRF_04_Data6.getDistance >25){	
						rescueRobotState.workStep++;
				}
			}break;
		}
			case 5:{
				rescueRobotState.moveWays = GO_STRAIGHT;
				if(SRF_04_Data5.getDistance <25&&SRF_04_Data5.getDistance <25 && SRF_04_Data3.getDistance >40&&SRF_04_Data4.getDistance >40 )
				rescueRobotState.workStep ++;
				break;			  
			}
			case 6:{
			if(SRF_04_Data1.getDistance <=105 && SRF_04_Data2.getDistance <=105){				
				rescueRobotState.moveWays =REVERSE_LEFT_TURN_90;
				rescueRobotState.workStep++;
				}					
				break;
			}
			case 7:{
			 rescueRobotState.moveWays = STOP;
				rescueRobotState.workStep++;
				break;
				
			}
			case 8:{
				calibrationGostraight(LEFT);
				if(SRF_04_Data1.getDistance <=71 && SRF_04_Data2.getDistance <=71){
				rescueRobotState.workStep ++;
				}
				break;
			}
			case 9:{
				if(SRF_04_Data1.getDistance <=33 && SRF_04_Data2.getDistance <=33){
				rescueRobotState.moveWays =REVERSE_RIGHT_TURN_90;
				rescueRobotState.workStep++;
				}
				break;
			}
			case 10:{
			rescueRobotState.moveWays = STOP;
			rescueRobotState.workStep = 99;
			break;
		}
		case 99:{
			rescueRobotState.moveWays = OTHER;
			rescueRobotState.workStep = 99;
			rescueRobotState.rescueTaskstep = DRIVING_MODE;
			break;
		}
	}
}
void app_RoomThree(void){
	switch(rescueRobotState.workStep){
	case 0:{
			calibrationGostraight(LEFT);
			rescueRobotState.workStep++;
			break;
		}
			case 1:{
				if(SRF_04_Data1.getDistance <=29 && SRF_04_Data2.getDistance <=29){
					rescueRobotState.moveWays = REVERSE_LEFT_TURN_90;
				 if(calibrationFinish == CALIBRATION_FINISHED){
				rescueRobotState.workStep++;
				}
				}
				break;
			}
			case 2:{			 
			 rescueRobotState.moveWays = STOP;
				rescueRobotState.workStep++;
				break;
			}
			case 3:{
       calibrationGostraight(RIGHT);
			if(SRF_04_Data1.getDistance <=28 && SRF_04_Data2.getDistance <=28){
				rescueRobotState.workStep++;
				}
			break;
			}
			case 4:{
				rescueRobotState.moveWays = REVERSE_RIGHT_TURN_90;
				rescueRobotState.workStep++;
				break;			
			}
			case 5:{
				rescueRobotState.moveWays = STOP;
				rescueRobotState.workStep = 99;
				break;
			}
			case 99:{
				rescueRobotState.moveWays = OTHER;
				rescueRobotState.workStep = 99;
				rescueRobotState.rescueTaskstep = DRIVING_MODE;
				break;
		}
	}
	
}
void app_RoomFour(void){
	switch(rescueRobotState.workStep){
		case 0:{
			calibrationFinish = app_calibration(RIGHT);
			if(calibrationFinish == CALIBRATION_FINISHED){
				rescueRobotState.workStep++;
			}break;
		}
		case 1:{
			calibrationGostraight(RIGHT);
			if(SRF_04_Data1.getDistance <24 && SRF_04_Data2.getDistance <24){
				rescueRobotState.workStep++;
			}break;
		}
		case 2:{
			rescueRobotState.moveWays = STOP;
			rescueRobotState.workStep++;
			break;
		}
		case 3:{
			rescueRobotState.moveWays = REVERSE_LEFT_TURN_90;
			rescueRobotState.workStep++;
			break;
		}
		case 4:{
			rescueRobotState.moveWays = STOP;
			rescueRobotState.workStep++;
			break;
		}
		case 5:{
			calibrationFinish = app_calibration(RIGHT);
			if(calibrationFinish == CALIBRATION_FINISHED){
				rescueRobotState.workStep++;
			}break;
		}
		case 6:{
			rescueRobotState.moveWays = GO_STRAIGHT;
			calibrationGostraight(RIGHT);
			if(SRF_04_Data1.getDistance <= 75&& SRF_04_Data2.getDistance <= 75){
				rescueRobotState.moveWays = STOP;
				rescueRobotState.workStep++;
			}break;
		}
		case 7:{
			rescueRobotState.moveWays = REVERSE_LEFT_TURN_90;
			rescueRobotState.workStep++;
			break;
		}
		case 8:{
			rescueRobotState.moveWays = STOP;
			rescueRobotState.workStep = 99;
			break;
		}
		case 99:{
			rescueRobotState.moveWays = OTHER;
			rescueRobotState.workStep = 99;
			rescueRobotState.rescueTaskstep = DRIVING_MODE;
			break;
		}
	}
}	

/*
void app_RoomTwo(void){
	switch(rescueRobotState.workStep){
		case 0:{
			calibrationFinish = app_calibration(RIGHT);
			if(calibrationFinish == CALIBRATION_FINISHED){
				rescueRobotState.moveWays = OTHER;
				rescueRobotState.workStep++;
			}break;
		}
		case 1:{
			rescueRobotState.moveWays = GO_STRAIGHT;
			calibrationGostraight(RIGHT);
			if(SRF_04_Data1.getDistance <=20 && SRF_04_Data2.getDistance <=20){
				rescueRobotState.workStep++;
			}break;
		}
		case 2:{
			rescueRobotState.moveWays = REVERSE_LEFT_TURN_90;
			rescueRobotState.workStep++;
			break;
		}
		case 3:{
			rescueRobotState.moveWays = STOP;
			rescueRobotState.workStep++;
			break;
		}
		case 4:{
			calibrationFinish = app_calibration(RIGHT);
			if(calibrationFinish == CALIBRATION_FINISHED){
				rescueRobotState.moveWays = OTHER;
				rescueRobotState.workStep++;
			}break;
		}
		case 5:{
			rescueRobotState.moveWays = GO_STRAIGHT;
			calibrationGostraight(RIGHT);
			if(SRF_04_Data1.getDistance<25 && SRF_04_Data2.getDistance<25){
				rescueRobotState.moveWays = STOP;
				rescueRobotState.workStep++;
			}break;
		}
		case 6:{
			rescueRobotState.moveWays = REVERSE_LEFT_TURN_90;
			rescueRobotState.workStep++;
			break;
		}
		case 7:{
			rescueRobotState.moveWays = STOP;
			rescueRobotState.workStep++;
			break;
		}
		case 8:{
			calibrationFinish = app_calibration(RIGHT);
			if(calibrationFinish == CALIBRATION_FINISHED){
				rescueRobotState.workStep++;
			}break;
		}
		case 9:{
			rescueRobotState.moveWays = GO_STRAIGHT;
			calibrationGostraight(RIGHT);
			if(SRF_04_Data5.getDistance > 88){
				rescueRobotState.moveWays = STOP;
				rescueRobotState.workStep++;
			}break;
		}
		case 10:{
			rescueRobotState.moveWays = FRONT_TURN_RIGHT_90;
			rescueRobotState.workStep++;
			break;
		}
		case 11:{
			rescueRobotState.moveWays = STOP;
			rescueRobotState.workStep++;
			break;
		}
		case 12:{
			rescueRobotState.moveWays = GO_STRAIGHT;
			calibrationGostraight(RIGHT);
			if(SRF_04_Data1.getDistance < 25 && SRF_04_Data2.getDistance < 25){
				rescueRobotState.moveWays = STOP;
				rescueRobotState.workStep++;
			}break;
		}
		case 13:{
			rescueRobotState.moveWays = REVERSE_RIGHT_TURN_90;
			rescueRobotState.workStep++;
			break;
		}
		case 14:{
			rescueRobotState.moveWays = STOP;
			rescueRobotState.workStep++;
			break;
		}
		case 15:{
			calibrationFinish = app_calibration(LEFT);
			if(calibrationFinish == CALIBRATION_FINISHED){
				rescueRobotState.workStep = 99;
			}break;
		}
		case 99:{
			rescueRobotState.moveWays = OTHER;
			rescueRobotState.workStep = 0;
			rescueRobotState.rescueTaskstep = DRIVING_MODE;
			break;
		}
	}
}
void app_RoomThree(void){
	switch(rescueRobotState.workStep){
		case 0:{
			calibrationFinish = app_calibration(LEFT);
			if(calibrationFinish == CALIBRATION_FINISHED){
				rescueRobotState.moveWays = OTHER;
				rescueRobotState.workStep++;
			}break;
		}
		case 1:{
			rescueRobotState.moveWays = GO_STRAIGHT;
			calibrationGostraight(LEFT);
			if(SRF_04_Data1.getDistance <=25 && SRF_04_Data2.getDistance <=25){
				rescueRobotState.moveWays = STOP;
				rescueRobotState.workStep++;
			}break;
		}
		case 2:{
			rescueRobotState.moveWays = REVERSE_LEFT_TURN_90;
			rescueRobotState.workStep++;
			break;
		}
		case 3:{
			rescueRobotState.moveWays = STOP;
			rescueRobotState.workStep++;
			break;
		}
		case 4:{
			calibrationFinish = app_calibration(RIGHT);
			if(calibrationFinish == CALIBRATION_FINISHED){
				rescueRobotState.workStep++;
			}break;
		}
		case 5:{
			rescueRobotState.moveWays = GO_STRAIGHT;
			calibrationGostraight(RIGHT);
			if(SRF_04_Data1.getDistance <=53 && SRF_04_Data2.getDistance <=53){
				rescueRobotState.moveWays = STOP;
				rescueRobotState.workStep++;
			}break;
		}
		case 6:{
			rescueRobotState.moveWays = REVERSE_LEFT_TURN_90;
			rescueRobotState.workStep++;
			break;
		}
		case 7:{
			rescueRobotState.moveWays = STOP;
			rescueRobotState.workStep++;
			break;
		}
		case 8:{
			rescueRobotState.moveWays = GO_STRAIGHT;
			calibrationGostraight(RIGHT);
			vTaskDelay(200);
			rescueRobotState.workStep++;
			break;
		}
		case 9:{
			rescueRobotState.moveWays = STOP;
			rescueRobotState.workStep = 99;
			break;
		}
		case 99:{
			rescueRobotState.moveWays = OTHER;
			rescueRobotState.workStep = 0;
			rescueRobotState.rescueTaskstep = DRIVING_MODE;
			break;
		}
	}
}
void app_RoomFour(void){
	switch(rescueRobotState.workStep){
		case 0:{
			calibrationFinish = app_calibration(LEFT);
			if(calibrationFinish == CALIBRATION_FINISHED){
				rescueRobotState.moveWays = OTHER;
				rescueRobotState.workStep++;
			}break;
		}
		case 1:{
			rescueRobotState.moveWays = GO_STRAIGHT;
			calibrationGostraight(LEFT);
			if(SRF_04_Data1.getDistance <=25 && SRF_04_Data2.getDistance <=25){
				rescueRobotState.moveWays = STOP;
				rescueRobotState.workStep++;
			}break;
		}
		case 2:{
			rescueRobotState.moveWays = REVERSE_LEFT_TURN_90;
			rescueRobotState.workStep++;
			break;
		}
		case 3:{
			rescueRobotState.moveWays = STOP;
			rescueRobotState.workStep++;
			break;
		}
		case 4:{
			calibrationFinish = app_calibration(RIGHT);
			if(calibrationFinish == CALIBRATION_FINISHED){
				rescueRobotState.workStep++;
			}break;
		}
		case 5:{
			rescueRobotState.moveWays = GO_STRAIGHT;
			calibrationGostraight(RIGHT);
			if(SRF_04_Data1.getDistance <=25 && SRF_04_Data2.getDistance <=25){
				rescueRobotState.moveWays = STOP;
				rescueRobotState.workStep++;
			}break;
		}
		case 6:{
			rescueRobotState.moveWays = REVERSE_RIGHT_TURN_90;
			rescueRobotState.workStep++;
			break;
		}
		case 7:{
			rescueRobotState.moveWays = STOP;
			rescueRobotState.workStep++;
			break;
		}
		case 8:{
			calibrationFinish = app_calibration(LEFT);
			if(calibrationFinish == CALIBRATION_FINISHED){
				rescueRobotState.moveWays = OTHER;
				rescueRobotState.workStep = 99;
			}break;
		}
		case 99:{
			rescueRobotState.moveWays = OTHER;
			rescueRobotState.workStep = 0;
			rescueRobotState.rescueTaskstep = DRIVING_MODE;
			break;
		}
	}
}
*/
void roomChoose(rescueRobotState_t *rescueRobotState){
	switch(rescueRobotState->rescuePosition){
		case ROOM_ONE:app_RoomOne();break;
		case ROOM_TWO:app_RoomTwo();break;
		case ROOM_THREE:app_RoomThree();break;
		case ROOM_FOUR:app_RoomFour();break;
	}
}

