#include "app.h"
/*********************************************************
			灭火顺序 8->7->5->4->6
**********************************************************/
void app_outfireTwoWorkReady(void)
{
	/*待补充*/ //用按键控制小车启动//
	//finishFlag = SET_ZERO;
	if(outfireRobotState.workMode == OUT_FIRE){
			app_judgeFunc();//判断返回节点	
		if(outfireRobotState.returnFlag[RETURN] == 0){
			outfireRobotState.robotTaskstep = EIGHTH_FIRE;//先从8号点开始判断
			outfireRobotState.step = DOING;
			outfireRobotState.workStep = NULL;
			outfireRobotState.moveWays = STOP;
			outfireRobotState.beginFlag = FREE;
		}else if(outfireRobotState.returnFlag[RETURN] == 1){
			outfireRobotState.robotTaskstep = RETURN;
			outfireRobotState.step = DOING;
			outfireRobotState.workStep = NULL;
			outfireRobotState.moveWays = STOP;
			outfireRobotState.beginFlag = FREE;
		}
	}
}

void app_findEighthFire(void){   //先进入8号火点的函数
	if(outfireRobotState.fireArray[EIGHTH_FIRE] == 1){//存在8号火点的情况
		switch(outfireRobotState.workStep){
			case 0:{
				calibrationFinish = app_calibration(LEFT);		//初始校准,只有车姿态校准为与路线方向平行后才会进入下一步前进,LEFT是以左边墙壁为基准校准的
				if(calibrationFinish == CALIBRATION_FINISHED){  //校准完成
					outfireRobotState.workStep++; //进入下一步动作
				}break;
			}
			case 1:{
				calibrationGostraight(LEFT);		//靠左沿墙前进,以左墙超声波为基准进行边校准边前进的前进方式
				if(SRF_04_Data1.getDistance < 60 && SRF_04_Data2.getDistance < 60){//如果前方检测到距墙小于60的超声波值,则停止,进入下一步操作
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
				}
			}
			case 2:{
				outfireRobotState.moveWays = GO_STRAIGHT;//普通前进方式,由于沿墙函数存在局限性,需要使用该方式让车保持直线行驶
				if(SRF_04_Data1.getDistance <= 13 && SRF_04_Data2.getDistance <= 13){ //超声波值为小于等于13,则停止,进入下一步动作
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
				}break;
			}
			case 3:{
				outfireRobotState.moveWays = REVERSE_LEFT_TURN_90;  //原地左转90°
				outfireRobotState.workStep++;
				break;
			}
			case 4:{
				outfireRobotState.moveWays = STOP;//记住,转向动作一定要记得停止,否则很可能会转过头或者一直在旋转,所以在原地左转90°的下一步就是STOP
				vTaskDelay(200);  //该处延时200ms意义不大,可考虑删除
				outfireRobotState.workStep++;
				break;
			}
			case 5:{
				calibrationFinish = app_calibration(RIGHT);  //以右边墙壁校准,防止接下来的转向动作出现转向角度不够准确的情况
				if(calibrationFinish == CALIBRATION_FINISHED){
					outfireRobotState.moveWays = OTHER;//由于lastmoveWays和moveWays不能同时都为STOP,所以特地增加一个OTHER标志,该标志没有任何动作,是闲置标志位
					outfireRobotState.workStep++;
				}break;
			}
			case 6:{
				calibrationGostraight(RIGHT);//沿右墙前进
				if(SRF_04_Data1.getDistance <=53 && SRF_04_Data2.getDistance <=53){	//前进到超声波数值小于等于53时进入下一步动作
					outfireRobotState.workStep++;
				}break;
			}
			case 7:{
				outfireRobotState.moveWays = STOP;//停止
				outfireRobotState.workStep++;
				break;
			}
			case 8:{
				outfireRobotState.moveWays = REVERSE_LEFT_TURN_90;//原地左转90°
				outfireRobotState.workStep++;
				break;
			}
			case 9:{
				outfireRobotState.moveWays = STOP;//停止,作用case4有解释
				outfireRobotState.workStep++;
				break;
			}
			case 10:{
				calibrationGostraight(LEFT);//沿左墙校准前进,但这里使用GO_STRAIGHT应该会更好,沿墙函数里是有存在一定超声波范围才会起作用的限制,但是只能在超声波数据都是稳定的情况下,函数里的限制对于第一个转角来说是不起作用的
				if(SRF_04_Data4.getDistance <50){ //如果左边最后一个超声波数值小于50,代表车体已经完全进入房间4,进行下一步
					outfireRobotState.workStep++;
				}break;
			}
			case 11:{//case11是一个特殊处理动作,目的是防止车离左墙过近导致车转向时会撞墙
				app_LeftWheel(FRONT,parameter[NAME_GO_STRAIGHT__LEFT_RATE]);	
				app_RightWheel(FRONT,parameter[NAME_GO_STRAIGHT__RIGHT_RATE]);//先让车前进1.2s(确保车完全处于4号放间内)
				vTaskDelay(1200);
				app_LeftWheel(FRONT,parameter[NAME_SET_ZERO__LEFT_RATE]);//让车停止
				app_RightWheel(FRONT,parameter[NAME_SET_ZERO__RIGHT_RATE]);
				outfireRobotState.moveWays = FRONT_TURN_RIGHT_90;//向前右转90°(差速转向)
				outfireRobotState.workStep++;
				break;
			}
			case 12:{
				outfireRobotState.moveWays = FRONT_TURN_LEFT_90;//向前左转90°,使方向向前方
				outfireRobotState.workStep++;
				break;
			}
			case 13:{
				outfireRobotState.moveWays = STOP;//停止,使用原地校准前一定要停止
				outfireRobotState.workStep++;
				break;
			}
			case 14:{
				outfireRobotState.moveWays = OTHER;  //闲置标志
				calibrationFinish = app_calibration(LEFT);//原地校准,为了使方向正确对着前方
				if(calibrationFinish == CALIBRATION_FINISHED){
					outfireRobotState.workStep++;
				}break;
			}
			case 15:{
				outfireRobotState.moveWays = OTHER;
				calibrationGostraight(RIGHT);//靠右墙前进
				if(SRF_04_Data1.getDistance <43 && SRF_04_Data2.getDistance <43){//超声波数值小于43就停止
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
				}break;
			}
			case 16:{
				outfireRobotState.moveWays = REVERSE_LEFT_TURN_45;//原地左转45°,为了之后的AD寻火能够使车能够直接前进寻火,准确定位车的方向,否则车的方向很难确定
				outfireRobotState.workStep++;
				break;
			}
			case 17:{//该地方没有调用STOP很可能转过,如果不是因为AD寻火函数,车可能会跑偏
				calibrationFinish = app_findFire();//AD寻火函数,使车能够正对着火焰方向,这样灭火是最准确的
				outfireRobotState.moveWays = OTHER;
				if(calibrationFinish == CALIBRATION_FINISHED && SRF_04_Data1.getDistance <25 && SRF_04_Data2.getDistance <25){//由于AD寻火函数里头自带了一个前进动作,所以两个超声波距离墙体小于25车就会停止
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
				}break;
			}
			case 18:{
				app_fan();//打开风扇
				calibrationFinish = checkFire();//检查火焰是否灭掉
				if(calibrationFinish == CALIBRATION_FINISHED){//确认火灭掉了
					outfireRobotState.workStep++;
				}break;
			}
			case 19:{
				app_goBack(600,NO_TURN);//后退600ms,防止车距墙太近撞墙
				outfireRobotState.workStep++;
				break;
			}
			case 20:{
				outfireRobotState.moveWays = REVERSE_RIGHT_TURN_135;//原地右转135°,目的是使车正对着下方墙壁,方便进入下一个对7火点的判断
				outfireRobotState.workStep++;
				break;
			}
			case 21:{
				outfireRobotState.moveWays = STOP;//停止
				vTaskDelay(200);
				outfireRobotState.workStep++;
				break;
			}
			case 22:{
				outfireRobotState.moveWays = OTHER;
				calibrationFinish = app_calibration(LEFT);//原地校准
				if(calibrationFinish == CALIBRATION_FINISHED){
					outfireRobotState.workStep++;
				}break;
			}
			case 23:{//case23到case25其实可以合并
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep++;
				break;
			}
			case 24:{
				outfireRobotState.moveWays = OTHER;
				outfireRobotState.workStep++;
				break;
			}
			case 25:{
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep = 99;
				break;
			}
			case 99:{//进入下一个函数的准备阶段
				if(outfireRobotState.returnFlag[EIGHTH_FIRE] == 1){//如果8是返回节点,意思是8是最后一个火点
					outfireRobotState.robotTaskstep = RETURN;//进入返回标志
				}else{
					outfireRobotState.robotTaskstep = SENVENTH_FIRE;//否则进入7火点的判断
				}
				outfireRobotState.moveWays = OTHER;//闲置标志,便于下一阶段的操作
				outfireRobotState.workStep = 0;
				break;
			}
		}
	}
	else{//如果是8号火点不存在的情况
		if(outfireRobotState.fireArray[SENVENTH_FIRE] == 1){//如果存在7火点,仍然会进入房间4
			switch(outfireRobotState.workStep){  //case0~case9与存在8火点的步骤都是一样的,都是进入房间
				case 0:{
					calibrationFinish = app_calibration(LEFT);
					if(calibrationFinish == CALIBRATION_FINISHED){
						outfireRobotState.workStep++;
					}break;
				}
				case 1:{
					calibrationGostraight(LEFT);
					if(SRF_04_Data1.getDistance < 60 && SRF_04_Data2.getDistance < 60){
						outfireRobotState.moveWays = STOP;
						outfireRobotState.workStep++;
					}
				}
				case 2:{
					outfireRobotState.moveWays = GO_STRAIGHT;
					if(SRF_04_Data1.getDistance <= 13 && SRF_04_Data2.getDistance <= 13){
						outfireRobotState.moveWays = STOP;
						outfireRobotState.workStep++;
					}break;
				}
				case 3:{
					outfireRobotState.moveWays = REVERSE_LEFT_TURN_90;
					outfireRobotState.workStep++;
					break;
				}
				case 4:{
					outfireRobotState.moveWays = STOP;
					vTaskDelay(200);
					outfireRobotState.workStep++;
					break;
				}
				case 5:{
					calibrationFinish = app_calibration(RIGHT);
					if(calibrationFinish == CALIBRATION_FINISHED){
						outfireRobotState.moveWays = OTHER;
						outfireRobotState.workStep++;
					}break;
				}
				case 6:{
					calibrationGostraight(RIGHT);
					if(SRF_04_Data1.getDistance <=53 && SRF_04_Data2.getDistance <=53){
						outfireRobotState.workStep++;
					}break;
				}
				case 7:{
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
					break;
				}
				case 8:{
					outfireRobotState.moveWays = REVERSE_LEFT_TURN_90;
					outfireRobotState.workStep++;
					break;
				}
				case 9:{
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
					break;
				}
				case 10:{
					outfireRobotState.moveWays = GO_STRAIGHT;//前进600ms,这里就没有调用沿左墙校准的函数,延时600ms是让车体完全进入房间4
					vTaskDelay(600);
					outfireRobotState.workStep++;
					break;
				}
				case 11:{
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
					break;
				}
				case 12:{
					outfireRobotState.moveWays = OTHER;
					calibrationGostraight(LEFT);//沿左墙前进
					if(SRF_04_Data1.getDistance <35 && SRF_04_Data2.getDistance <35){//超声波数值小于35时停止
						outfireRobotState.moveWays = STOP;
						outfireRobotState.workStep++;
					}break;
				}
				case 13:{
					outfireRobotState.moveWays = REVERSE_RIGHT_TURN_90;//原地右转90度正对7火点方向,与7火点函数的初始状态对接
					outfireRobotState.workStep++;
					break;
				}
				case 14:{
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep = 99;
					break;
				}
				case 99:{
					outfireRobotState.moveWays = OTHER;
					outfireRobotState.robotTaskstep = SENVENTH_FIRE;//进入7火点函数
					outfireRobotState.workStep = 0;
					break;
				}
			}
		}
		else{//房间4的火点都不存在,不会进入到房间4,会进入4火点函数度对接,即最后停在分岔口处
			//case0~case7都相同
			switch(outfireRobotState.workStep){
				case 0:{
					calibrationFinish = app_calibration(LEFT);
					if(calibrationFinish == CALIBRATION_FINISHED){
						outfireRobotState.workStep++;
					}break;
				}
				case 1:{
					calibrationGostraight(LEFT);
					if(SRF_04_Data1.getDistance < 60 && SRF_04_Data2.getDistance < 60){
						outfireRobotState.moveWays = STOP;
						outfireRobotState.workStep++;
					}
				}
				case 2:{
					outfireRobotState.moveWays = GO_STRAIGHT;
					if(SRF_04_Data1.getDistance <= 13 && SRF_04_Data2.getDistance <= 13){
						outfireRobotState.moveWays = STOP;
						outfireRobotState.workStep++;
					}break;
				}
				case 3:{
					outfireRobotState.moveWays = REVERSE_LEFT_TURN_90;
					outfireRobotState.workStep++;
					break;
				}
				case 4:{
					outfireRobotState.moveWays = STOP;
					vTaskDelay(200);
					outfireRobotState.workStep++;
					break;
				}
				case 5:{
					calibrationFinish = app_calibration(RIGHT);
					if(calibrationFinish == CALIBRATION_FINISHED){
						outfireRobotState.moveWays = OTHER;
						outfireRobotState.workStep++;
					}break;
				}
				case 6:{
					calibrationGostraight(RIGHT);
					if(SRF_04_Data1.getDistance <=53 && SRF_04_Data2.getDistance <=53){
						outfireRobotState.workStep++;
					}break;
				}
				case 7:{
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
					break;
				}
				case 8:{
					outfireRobotState.moveWays = REVERSE_RIGHT_TURN_90;//原地右转90°,为了进入4号火点的函数对接
					outfireRobotState.workStep++;
					break;
				}
				case 9:{
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep = 99;
					break;
				}
				case 99:{
					outfireRobotState.moveWays = OTHER;
					outfireRobotState.robotTaskstep = FOURTH_FIRE;//进入4号火点函数
					outfireRobotState.workStep = 0;
					break;
				}
			}
		}
	}
}


void app_findSixFire(void){//6号火点函数,但是接口是灭完了房间3的火点后的状态
	if(outfireRobotState.fireArray[SIXTH_FIRE] == 1){
		switch(outfireRobotState.workStep){
			case 0:{
				outfireRobotState.moveWays = OTHER;
				calibrationGostraight(RIGHT);//沿右墙前进
				if(SRF_04_Data1.getDistance <25 && SRF_04_Data2.getDistance <25){//前方墙壁距离小于25
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
				}break;
			}
			case 1:{
				calibrationFinish = app_findFire();//由于是正对房间3的出口,所以直接使用AD寻火函数,如果调用原地转向45°可能会撞墙
				outfireRobotState.moveWays = OTHER;
				if(calibrationFinish == CALIBRATION_FINISHED && SRF_04_Data1.getDistance <28 && SRF_04_Data2.getDistance <28 ){//离前方墙壁28时停止
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
				}break;
			}
			case 2:{
				outfireRobotState.moveWays = OTHER;
				app_fan();//灭火函数
				calibrationFinish = checkFire();
				if(calibrationFinish == CALIBRATION_FINISHED){
					outfireRobotState.workStep++;
				}break;
			}
			case 3:{
				app_goBack(100,NO_TURN);//后退100ms,太长可能撞墙
				outfireRobotState.workStep++;
				break;
			}
			case 4:{
				outfireRobotState.moveWays = REVERSE_LEFT_TURN_135;//原子左转135°,使车头大概朝着路口方向,之后再通过原地校准使其精确即可
				outfireRobotState.workStep++;
				break;
			}
			case 5:{
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep++;
				break;
			}
			case 6:{
				calibrationFinish = app_calibration(RIGHT);
				if(calibrationFinish == CALIBRATION_FINISHED){
					outfireRobotState.workStep = 99;
				}break;
			}
			case 99:{
				if(outfireRobotState.returnFlag[SIXTH_FIRE] == 1){//因为6号火点一定是最后的火点,所以必定返回
					outfireRobotState.robotTaskstep = RETURN;
				}
				outfireRobotState.moveWays = OTHER;
				outfireRobotState.workStep = 0;
				break;
			}
		}
	}	
}

void app_findFourthFire(void){//4号火点判断和5号火点判断结合在一起,所以4和5的返回节点都是一样的
	if(outfireRobotState.fireArray[FOURTH_FIRE] == 1 && outfireRobotState.fireArray[FIFTH_FIRE] == 1 ){	//4号火点和5号火点都存在的情况
		switch(outfireRobotState.workStep){//对接7号火点函数的车的结束状态进行下一步操作
			case 0:{//这一步是特殊处理动作,防止距离左墙过近导致的情况,如果能靠右墙近一些,这样对于之后的6号火点就不会出现碰撞
				if(SRF_04_Data1.getDistance >16 && SRF_04_Data2.getDistance >16){  //如果前方超声波的距离大于16则前进
					outfireRobotState.moveWays = GO_STRAIGHT;
				}else if(SRF_04_Data1.getDistance <16&& SRF_04_Data2.getDistance <16){//小于16则停止
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
				}break;
			}	
			case 1:{
				outfireRobotState.moveWays = REVERSE_LEFT_TURN_90;//原地左转90°使车头方向与道路方向一致,因为7号火点函数结束时车头的状态是正对前方墙壁的
				outfireRobotState.workStep++;
				break;
			}
			case 2:{
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep++;
				break;
			}
			case 3:{
				outfireRobotState.moveWays = OTHER;
				calibrationFinish = app_calibration(RIGHT);//原地校准
				if(calibrationFinish == CALIBRATION_FINISHED){
					outfireRobotState.workStep++;
				}break;
			}
			case 4:{
				outfireRobotState.moveWays = GO_STRAIGHT;//普通前进,这里不使用沿墙函数也是为了避免沿墙函数的弊端
				if(SRF_04_Data1.getDistance <18 && SRF_04_Data2.getDistance <18){//距离前方墙壁小于18就停止
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
				}
				break;
			}
			case 5:{
				outfireRobotState.moveWays = REVERSE_RIGHT_TURN_90;//原地右转90°
				outfireRobotState.workStep++;
				break;
			}
			case 6:{
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep++;
				break;
			}
			case 7:{
				outfireRobotState.moveWays = OTHER;
				calibrationFinish = app_calibration(LEFT);//原地校准
				if(calibrationFinish == CALIBRATION_FINISHED){
					outfireRobotState.workStep++;
				}break;
			}
			case 8:{
				outfireRobotState.moveWays = OTHER;
				calibrationGostraight(LEFT);//沿左墙前进
				if(SRF_04_Data1.getDistance <45 && SRF_04_Data2.getDistance <45){//距离前方墙壁45°停止,目的是为了之后对位4号火点的方向更接近45°
					outfireRobotState.workStep++;
				}
				break;
			}
			case 9:{
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep++;
				break;
			}
			case 10:{
				outfireRobotState.moveWays = REVERSE_RIGHT_TURN_90;//原地右转90°
				outfireRobotState.workStep++;
				break;
			}
			case 11:{
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep++;
				break;
			}
			case 12:{
				outfireRobotState.moveWays = OTHER;
				calibrationFinish = app_calibration(LEFT);//原地校准
				if(calibrationFinish == CALIBRATION_FINISHED){
					outfireRobotState.workStep++;
				}break;
			}
			case 13:{
				calibrationGostraight(LEFT);//沿左墙前进
				if(SRF_04_Data1.getDistance <30 && SRF_04_Data2.getDistance <30){//检测到前方距离小于30停止
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
				}break;
			}
			case 14:{
				outfireRobotState.moveWays = REVERSE_RIGHT_TURN_45;//原地右转45°,目的同处理其他火点同理
				outfireRobotState.workStep++;
				break;
			}
			case 15:{
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep++;
				break;
			}
			case 16:{
				calibrationFinish = app_findFire();//AD寻火
				outfireRobotState.moveWays = OTHER;
				if(calibrationFinish == CALIBRATION_FINISHED && SRF_04_Data1.getDistance <26&& SRF_04_Data2.getDistance <26){//超声波距离小于26停止
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
				}break;
			}
			case 17:{
				outfireRobotState.moveWays = OTHER;
				app_fan();
				calibrationFinish = checkFire();
				if(calibrationFinish == CALIBRATION_FINISHED){
					outfireRobotState.workStep++;
				}break;
			}
			case 18:{
				app_goBack(600,NO_TURN);
				outfireRobotState.workStep++;
				break;
			}
			case 19:{
				outfireRobotState.moveWays = REVERSE_RIGHT_TURN_135;//原地右转135度,使车方向正对5火点所在的墙壁方向
				outfireRobotState.workStep++;
				break;
			}
			case 20:{
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep++;
				break;
			}
			case 21:{
				outfireRobotState.moveWays = OTHER;
				calibrationFinish = app_calibration(LEFT);//原地校准
				if(calibrationFinish == CALIBRATION_FINISHED){
					outfireRobotState.workStep++;
				}break;
			}
			case 22:{
				calibrationGostraight(RIGHT);//沿右墙前进,这里不沿着左墙同样是为了避免沿墙函数的弊端
				if(SRF_04_Data1.getDistance <35 && SRF_04_Data2.getDistance <35){
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
				}
				break;
			}
			case 23:{
				outfireRobotState.moveWays = REVERSE_RIGHT_TURN_45;//原地右转45°,目的同处理其他火点相同
				outfireRobotState.workStep++;
				break;
			}
			case 24:{
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep++;
				break;
			}
			case 25:{
				calibrationFinish = app_findFire();//寻火函数
				outfireRobotState.moveWays = OTHER;
				if(calibrationFinish == CALIBRATION_FINISHED && SRF_04_Data1.getDistance <24 && SRF_04_Data2.getDistance <24){//距离前方墙壁小于24停止
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
				}break;
			}
			case 26:{
				outfireRobotState.moveWays = OTHER;
				app_fan();//灭火函数,灭5号点
				calibrationFinish = checkFire();
				if(calibrationFinish == CALIBRATION_FINISHED){
					outfireRobotState.workStep++;
				}break;
			}
			case 27:{
				app_goBack(200,NO_TURN);//后退200ms
				outfireRobotState.workStep++;
				break;
			}
			case 28:{
				outfireRobotState.moveWays = REVERSE_LEFT_TURN_135;//原地左转135°,使车头对着房间3的门口,对接6号火点的初始状态
				outfireRobotState.workStep++;
				break;
			}
			case 29:{
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep++;
				break;
			}
			case 30:{
				outfireRobotState.moveWays = OTHER;
				calibrationFinish = app_calibration(RIGHT);
				if(calibrationFinish == CALIBRATION_FINISHED){
					outfireRobotState.workStep = 99;
				}break;
			}
			case 99:{
				if(outfireRobotState.returnFlag[FOURTH_FIRE] == 1 || outfireRobotState.returnFlag[FIFTH_FIRE] == 1){
					outfireRobotState.robotTaskstep = RETURN;
				}
				else{
					outfireRobotState.robotTaskstep = SIXTH_FIRE;
					
				}
				outfireRobotState.moveWays = OTHER;
				outfireRobotState.workStep = 0;
				break;
			}
		}
	}
	else if(outfireRobotState.fireArray[FOURTH_FIRE] == 1 && outfireRobotState.fireArray[FIFTH_FIRE] == 0){//有4号点但没有5号点
		switch(outfireRobotState.workStep){//case0~case21都是同有4、5点时相同的
			case 0:{
				if(SRF_04_Data1.getDistance >16&& SRF_04_Data2.getDistance >16){
					outfireRobotState.moveWays = GO_STRAIGHT;
				}else if(SRF_04_Data1.getDistance <16&& SRF_04_Data2.getDistance <16){
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
				}break;
			}	
			case 1:{
				outfireRobotState.moveWays = REVERSE_LEFT_TURN_90;
				outfireRobotState.workStep++;
				break;
			}
			case 2:{
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep++;
				break;
			}
			case 3:{
				outfireRobotState.moveWays = OTHER;
				calibrationFinish = app_calibration(RIGHT);
				if(calibrationFinish == CALIBRATION_FINISHED){
					outfireRobotState.workStep++;
				}break;
			}
			case 4:{
				outfireRobotState.moveWays = GO_STRAIGHT;
				if(SRF_04_Data1.getDistance <18 && SRF_04_Data2.getDistance <18){
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
				}
				break;
			}
			case 5:{
				outfireRobotState.moveWays = REVERSE_RIGHT_TURN_90;
				outfireRobotState.workStep++;
				break;
			}
			case 6:{
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep++;
				break;
			}
			case 7:{
				outfireRobotState.moveWays = OTHER;
				calibrationFinish = app_calibration(LEFT);
				if(calibrationFinish == CALIBRATION_FINISHED){
					outfireRobotState.workStep++;
				}break;
			}
			case 8:{
				calibrationGostraight(LEFT);
				if(SRF_04_Data1.getDistance <45 && SRF_04_Data2.getDistance <45){
					outfireRobotState.workStep++;
				}
				break;
			}
			case 9:{
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep++;
				break;
			}
			case 10:{
				outfireRobotState.moveWays = REVERSE_RIGHT_TURN_90;
				outfireRobotState.workStep++;
				break;
			}
			case 11:{
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep++;
				break;
			}
			case 12:{
				outfireRobotState.moveWays = OTHER;
				calibrationFinish = app_calibration(LEFT);
				if(calibrationFinish == CALIBRATION_FINISHED){
					outfireRobotState.workStep++;
				}break;
			}
			case 13:{
				calibrationGostraight(LEFT);
				if(SRF_04_Data1.getDistance <30 && SRF_04_Data2.getDistance <30){
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
				}break;
			}
			case 14:{
				outfireRobotState.moveWays = REVERSE_RIGHT_TURN_45;
				outfireRobotState.workStep++;
				break;
			}
			case 15:{
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep++;
				break;
			}
			case 16:{
				calibrationFinish = app_findFire();
				outfireRobotState.moveWays = OTHER;
				if(calibrationFinish == CALIBRATION_FINISHED && SRF_04_Data1.getDistance <26&& SRF_04_Data2.getDistance <26){
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
				}break;
			}
			case 17:{
				outfireRobotState.moveWays = OTHER;
				app_fan();
				calibrationFinish = checkFire();
				if(calibrationFinish == CALIBRATION_FINISHED){
					outfireRobotState.workStep++;
				}break;
			}
			case 18:{
				app_goBack(600,NO_TURN);
				outfireRobotState.workStep++;
				break;
			}
			case 19:{
				outfireRobotState.moveWays = REVERSE_RIGHT_TURN_135;
				outfireRobotState.workStep++;
				break;
			}
			case 20:{
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep++;
				break;
			}
			case 21:{
				outfireRobotState.moveWays = OTHER;
				calibrationFinish = app_calibration(LEFT);
				if(calibrationFinish == CALIBRATION_FINISHED){
					outfireRobotState.workStep++;
				}break;
			}
			case 22:{
				calibrationGostraight(RIGHT);//沿右墙前进,为了避免沿墙函数的弊端
				if(SRF_04_Data1.getDistance <20 && SRF_04_Data2.getDistance <20){//如果小于20则停止
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
				}
				break;
			}
			case 23:{
				outfireRobotState.moveWays = REVERSE_LEFT_TURN_90;//原地左转90°,车头方向沿房间3出口,对接6号火点函数
				outfireRobotState.workStep++;
				break;
			}
			case 24:{
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep++;
				break;
			}
			case 25:{
				outfireRobotState.moveWays = OTHER;
				calibrationFinish = app_calibration(RIGHT);
				if(calibrationFinish == CALIBRATION_FINISHED){
					outfireRobotState.workStep = 99;
				}break;
			}
			case 99:{
				if(outfireRobotState.returnFlag[FOURTH_FIRE] == 1){
					outfireRobotState.robotTaskstep = RETURN;
				}
				else{
					outfireRobotState.robotTaskstep = SIXTH_FIRE;
					
				}
				outfireRobotState.moveWays = OTHER;
				outfireRobotState.workStep = 0;
				break;
			}
		}
	}		
	else if(outfireRobotState.fireArray[FOURTH_FIRE] == 0){//如果没有4号火点
		if(outfireRobotState.fireArray[FIFTH_FIRE] == 1){//存在5号火点
			switch(outfireRobotState.workStep){//case0~case7与存在4火点的步骤相同,都是进入房间3的函数
				case 0:{
					if(SRF_04_Data1.getDistance >16&& SRF_04_Data2.getDistance >16){
						outfireRobotState.moveWays = GO_STRAIGHT;
					}else if(SRF_04_Data1.getDistance <16&& SRF_04_Data2.getDistance <16){
						outfireRobotState.moveWays = STOP;
						outfireRobotState.workStep++;
					}break;
				}	
				case 1:{
					outfireRobotState.moveWays = REVERSE_LEFT_TURN_90;
					outfireRobotState.workStep++;
					break;
				}
				case 2:{
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
					break;
				}
				case 3:{
					outfireRobotState.moveWays = OTHER;
					calibrationFinish = app_calibration(RIGHT);
					if(calibrationFinish == CALIBRATION_FINISHED){
						outfireRobotState.workStep++;
					}break;
				}
				case 4:{
					outfireRobotState.moveWays = GO_STRAIGHT;
					if(SRF_04_Data1.getDistance <18 && SRF_04_Data2.getDistance <18){
						outfireRobotState.moveWays = STOP;
						outfireRobotState.workStep++;
					}
					break;
				}
				case 5:{
					outfireRobotState.moveWays = REVERSE_RIGHT_TURN_90;
					outfireRobotState.workStep++;
					break;
				}
				case 6:{
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
					break;
				}
				case 7:{
					outfireRobotState.moveWays = OTHER;
					calibrationFinish = app_calibration(LEFT);
					if(calibrationFinish == CALIBRATION_FINISHED){
						outfireRobotState.workStep++;
					}break;
				}
				case 8:{
					calibrationGostraight(LEFT);//沿左墙前进
					if(SRF_04_Data1.getDistance <35 && SRF_04_Data2.getDistance <35){//小于35停止
						outfireRobotState.workStep++;
					}
					break;
				}
				case 9:{
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
					break;
				}
				case 10:{
					outfireRobotState.moveWays = REVERSE_LEFT_TURN_45;//原地左转45°
					outfireRobotState.workStep++;
					break;
				}
				case 11:{//该处没有调用STOP,可能存在转过头的情况
					calibrationFinish = app_findFire();//AD寻火函数
					outfireRobotState.moveWays = OTHER;
					if(calibrationFinish == CALIBRATION_FINISHED && SRF_04_Data1.getDistance <27&& SRF_04_Data2.getDistance <27){
						outfireRobotState.moveWays = STOP;
						outfireRobotState.workStep++;
					}break;
				}
				case 12:{
					outfireRobotState.moveWays = OTHER;
					app_fan();//灭火函数
					calibrationFinish = checkFire();
					if(calibrationFinish == CALIBRATION_FINISHED){
						outfireRobotState.workStep++;
					}break;
				}
				case 13:{
					app_goBack(150,NO_TURN);//后退150ms,如果后退时间太长可能左转135°后会撞到出口的墙壁
					outfireRobotState.workStep++;
					break;
				}
				case 14:{
					outfireRobotState.moveWays = REVERSE_LEFT_TURN_135;//原地左转135°,对接6号火点函数的初始状态
					outfireRobotState.workStep++;
					break;
				}
				case 15:{
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
					break;
				}
				case 16:{
					outfireRobotState.moveWays = OTHER;
					calibrationFinish = app_calibration(RIGHT);//原地校准
					if(calibrationFinish == CALIBRATION_FINISHED){
						outfireRobotState.workStep = 99;
					}break;
				}
				case 99:{
					if(outfireRobotState.returnFlag[FIFTH_FIRE] == 1){//如果5是返回节点的话
						outfireRobotState.robotTaskstep = RETURN;
					}
					else{
						outfireRobotState.robotTaskstep = SIXTH_FIRE;//进入6号火点的函数
					}
					outfireRobotState.moveWays = OTHER;
					outfireRobotState.workStep = 0;
					break;
				}
			}
		}
		else{//没有进入房间3的火点,不会进入房间3,即仅剩下一个6号火点,此处是从分岔口去6号火点的路线,而6号火点函数是对接着房间3火点函数结束,所以稍微有些不同
			switch(outfireRobotState.workStep){//case0~case3都与去房间3的的步骤相同
				case 0:{
					if(SRF_04_Data1.getDistance >16&& SRF_04_Data2.getDistance >16){
						outfireRobotState.moveWays = GO_STRAIGHT;
					}else if(SRF_04_Data1.getDistance <16&& SRF_04_Data2.getDistance <16){
						outfireRobotState.moveWays = STOP;
						outfireRobotState.workStep++;
					}break;
				}	
				case 1:{
					outfireRobotState.moveWays = REVERSE_LEFT_TURN_90;//
					outfireRobotState.workStep++;
					break;
				}
				case 2:{
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
					break;
				}
				case 3:{
					outfireRobotState.moveWays = OTHER;
					calibrationFinish = app_calibration(RIGHT);
					if(calibrationFinish == CALIBRATION_FINISHED){
						outfireRobotState.workStep++;
					}break;
				}
				case 4:{
					calibrationGostraight(LEFT);//沿左墙前进,但这里应该调用GO_STRAIGHT会更好
					if(SRF_04_Data1.getDistance <20 && SRF_04_Data2.getDistance <20){//前方距墙小于20停止
						outfireRobotState.moveWays = STOP;
						outfireRobotState.workStep++;
					}
					break;
				}
				case 5:{
					calibrationFinish = app_findFire();//AD寻火
					outfireRobotState.moveWays = OTHER;
					if(calibrationFinish == CALIBRATION_FINISHED && SRF_04_Data1.getDistance <28 && SRF_04_Data2.getDistance <28){//距墙距离小于28停止
						outfireRobotState.moveWays = STOP;
						outfireRobotState.workStep++;
					}break;
				}
				case 6:{
					app_fan();//灭火函数
					outfireRobotState.moveWays = OTHER;
					calibrationFinish = checkFire();//检测是否灭火完成
					if(calibrationFinish == CALIBRATION_FINISHED){
						outfireRobotState.workStep++;
					}break;
				}
				case 7:{
					app_goBack(180,NO_TURN);//后退180ms,太长可能会撞到墙壁
					outfireRobotState.workStep++;
					break;
				}
				case 8:{
					outfireRobotState.moveWays = REVERSE_LEFT_TURN_135;//原地左转135°
					outfireRobotState.workStep++;
					break;
				}
				case 9:{
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
					break;
				}
				case 10:{
					outfireRobotState.moveWays = OTHER;
					calibrationFinish = app_calibration(RIGHT);
					if(calibrationFinish == CALIBRATION_FINISHED){
						outfireRobotState.workStep = 99;
					}break;
				}
				case 99:{
					if(outfireRobotState.returnFlag[SIXTH_FIRE] == 1){//因为6号火点一定是最后的火点,所以必定返回
						outfireRobotState.robotTaskstep = RETURN;
					}
					outfireRobotState.moveWays = OTHER;
					outfireRobotState.workStep = 0;
					break;
				}
			}
		}
	}				
}
		
				
void app_findSeventhFire(void){
	if(outfireRobotState.fireArray[SENVENTH_FIRE] == 1){ //如果7号火点存在
		switch(outfireRobotState.workStep){//从8火点函数结束后进行的对接,所以要从8火点函数的车最后的结束状态来进行下一步操作
			case 0:{
				calibrationGostraight(LEFT);//沿左墙前进
				if(SRF_04_Data1.getDistance <26 && SRF_04_Data2.getDistance <26){//超声波数值小于26停止
					outfireRobotState.workStep++;
				}
				break;
			}
			case 1:{
				outfireRobotState.moveWays = REVERSE_LEFT_TURN_45;//原地左转45°,大概对着7火点的方向,目的同8号火点的处理方式相同
				outfireRobotState.workStep++;
				break;
			}
			case 2:{
				calibrationFinish = app_findFire();//AD寻火,目的是为了准确对向7火点方向
				outfireRobotState.moveWays = OTHER;
				if(calibrationFinish == CALIBRATION_FINISHED && SRF_04_Data1.getDistance <22 && SRF_04_Data2.getDistance <22){//距离7火点超声波数值小于22的位置停止
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
				}break;
			}
			case 3:{
				app_fan();//灭火
				outfireRobotState.moveWays = OTHER;
				calibrationFinish = checkFire();//检查火焰是否存在
				if(calibrationFinish == CALIBRATION_FINISHED){
					outfireRobotState.workStep++;
				}break;
			}
			case 4:{
				app_goBack(800,NO_TURN);//后退800ms
				outfireRobotState.workStep++;
				break;
			}
			case 5:{
				outfireRobotState.moveWays = REVERSE_LEFT_TURN_135;//原地左转135°,目的是正对上方墙壁,便于返回
				outfireRobotState.workStep++;
				break;
			}
			case 6:{
				outfireRobotState.moveWays = STOP;
				vTaskDelay(200);
				outfireRobotState.workStep++;
				break;
			}
			case 7:{
				calibrationGostraight(RIGHT);//沿右墙前进
				if(SRF_04_Data1.getDistance <14 && SRF_04_Data2.getDistance <14){//如果前方检测小于14停止
					outfireRobotState.workStep++;
				}
				break;
			}
			case 8:{
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep++;
				break;
			}
			case 9:{
				outfireRobotState.moveWays = REVERSE_LEFT_TURN_90;//原地左转90°
				outfireRobotState.workStep++;
				break;
			}
			case 10:{
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep++;
				break;
			}
			case 11:{
				outfireRobotState.moveWays = OTHER;
				calibrationFinish = app_calibration(RIGHT);//原地校准
				if(calibrationFinish == CALIBRATION_FINISHED){
					outfireRobotState.workStep++;
				}break;
			}
			case 12:{
				calibrationGostraight(RIGHT);//沿右墙前进
				if(SRF_04_Data1.getDistance <= 65 && SRF_04_Data2.getDistance <= 65){//这个地方同出门第一个拐角作用相同,都是避免了沿墙前进函数的弊端
					outfireRobotState.workStep++;
				}break;
			}
			case 13:{
					outfireRobotState.moveWays = GO_STRAIGHT;//切换为普通前进
				 if(SRF_04_Data1.getDistance < 16 && SRF_04_Data2.getDistance < 16){//检测到前方墙体距离为16停止,对接4号火点函数
					outfireRobotState.workStep++;
				}break;
			}
			case 14:{
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep = 99;
				break;
			}
			case 99:{
				if(outfireRobotState.returnFlag[SENVENTH_FIRE] == 1){//如果7号火点是最后一个火点了
					outfireRobotState.robotTaskstep = RETURN;//进入返回标志
				}else{
					outfireRobotState.robotTaskstep = FOURTH_FIRE;//否则进入4号火点函数对接
				}
				outfireRobotState.moveWays = OTHER;
				outfireRobotState.workStep = 0;
				vTaskDelay(200);
				break;
			}
		}
	}else{//如果没有7火点
		switch(outfireRobotState.workStep){
			case 0:{
				outfireRobotState.moveWays = REVERSE_RIGHT_TURN_180;//原地向后转180°
				outfireRobotState.workStep++;
				break;
			}
			case 1:{
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep++;
				break;
			}
			case 2:{
				calibrationGostraight(RIGHT);//沿右墙前进
				if(SRF_04_Data1.getDistance <12 && SRF_04_Data2.getDistance <12){//距离前方墙壁距离16时停止
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
				}break;
			}
			case 3:{
				outfireRobotState.moveWays = REVERSE_LEFT_TURN_90;//原地左转90°
				outfireRobotState.workStep++;
				break;
			}
			case 4:{
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep++;
				break;
			}
			case 5:{
				outfireRobotState.moveWays = OTHER;
				calibrationFinish = app_calibration(RIGHT);//原地校准
				if(calibrationFinish == CALIBRATION_FINISHED){
					outfireRobotState.workStep++;
				}break;
			}
			case 6:{
				calibrationGostraight(RIGHT);//沿右墙前进
				if(SRF_04_Data1.getDistance < 60 && SRF_04_Data2.getDistance < 60){//作用同出门第一个拐角相同,为了避免沿墙前进函数的弊端
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
				}break;
			}
			case 7:{
				outfireRobotState.moveWays = GO_STRAIGHT;//切换为普通前进函数
				if(SRF_04_Data1.getDistance <= 16 && SRF_04_Data2.getDistance <= 16){//距离前方墙壁的距离小于等于16时停止,对接4号火点函数接口
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep = 99;
				}break;
			}
			case 99:{
				outfireRobotState.robotTaskstep = FOURTH_FIRE;//进入4号火点函数
				outfireRobotState.moveWays = OTHER;
				outfireRobotState.workStep = 0;
				break;
			}
		}
	}
}
		
				
void app_returnFromEighthFire(void){//8火点返回函数
	switch(outfireRobotState.workStep){//对接8火点结束的状态
		case 0:{
			outfireRobotState.moveWays = REVERSE_RIGHT_TURN_180;//原地右转180°
			outfireRobotState.workStep++;
			break;
		}
		case 1:{
			outfireRobotState.moveWays = STOP;
			outfireRobotState.workStep++;
			break;
		}
		case 2:{
			calibrationGostraight(RIGHT);//沿着右墙前进
			if(SRF_04_Data1.getDistance <10 && SRF_04_Data2.getDistance <10){//如果前方检测距离为10则停止
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep++;
			}break;
		}
		case 3:{
			outfireRobotState.moveWays = REVERSE_LEFT_TURN_90;//原地左转90°,车头方向对着房间4的出口
			outfireRobotState.workStep++;
			break;
		}
		case 4:{
			outfireRobotState.moveWays = STOP;
			outfireRobotState.workStep++;
			break;
		}
		case 5:{
			outfireRobotState.moveWays = OTHER;
			calibrationFinish = app_calibration(RIGHT);//原地校准
			if(calibrationFinish == CALIBRATION_FINISHED){
				outfireRobotState.workStep++;
			}break;
		}
		case 6:{
			calibrationGostraight(RIGHT);//沿右墙前进
			if(SRF_04_Data5.getDistance > 50){//如果右边第一个超声波检测大于50,则车体前半部分已经离开了房间4,此时转向时机很合适
				outfireRobotState.workStep++;
			}break;
		}
		case 7:{
			outfireRobotState.moveWays = STOP;
			outfireRobotState.workStep++;
			break;
		}
		case 8:{
			outfireRobotState.moveWays = FRONT_TURN_RIGHT_180;//向前转向180°
			outfireRobotState.workStep++;
			break;
		}
		case 9:{
			outfireRobotState.moveWays = STOP;
			outfireRobotState.workStep++;
			break;
		}
		case 10:{
			outfireRobotState.moveWays = GO_STRAIGHT;//前进800ms,目的是使车进入过道
			vTaskDelay(800);
			outfireRobotState.workStep++;
			break;
		}
		case 11:{
			outfireRobotState.moveWays = STOP;
			outfireRobotState.workStep++;
			break;
		}
		case 12:{
			outfireRobotState.moveWays = OTHER;
			calibrationFinish = app_calibration(RIGHT);//校准,防止车头是歪着的
			if(calibrationFinish == CALIBRATION_FINISHED){
				outfireRobotState.workStep++;
			}break;
		}
		case 13:{
			calibrationGostraight(RIGHT);//沿右墙前进
			if(SRF_04_Data1.getDistance <32 && SRF_04_Data2.getDistance <32){//前方距离小于45时停止
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep = 99;
			}break;
		}
		case 99:{
			outfireRobotState.moveWays = OTHER;
			outfireRobotState.step = FINISH;//完成标志位
			outfireRobotState.workStep = 0;
			break;
		}
	}		
}

void app_returnFromSixthFire(void){//6号火点返回函数
	switch(outfireRobotState.workStep){//对接着6号火点灭火结束后的状态
		case 0:{
			calibrationGostraight(LEFT);//沿左墙前进,这里调用GO_STRAIGHT可能更好
			if(SRF_04_Data6.getDistance> 40){//右边第二个超声波大于40表示车体已经到达了分岔口
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep++;
			}break;
		}
		case 1:{
			outfireRobotState.moveWays = OTHER;
			calibrationFinish = app_calibration(LEFT);//原地校准,防止车体歪斜
			if(calibrationFinish == CALIBRATION_FINISHED){
				outfireRobotState.workStep++;
			}break;
		}
		case 2:{
			outfireRobotState.moveWays = REVERSE_LEFT_TURN_90;//原地左转90°
			outfireRobotState.workStep++;
			break;
		}
		case 3:{
			outfireRobotState.moveWays = STOP;
			outfireRobotState.workStep++;
			break;
		}
		case 4:{
			if(SRF_04_Data1.getDistance >13&& SRF_04_Data2.getDistance >13){//目的是为了防止离左墙太远,可能之后会撞墙
					outfireRobotState.moveWays = GO_STRAIGHT;
				}else if(SRF_04_Data1.getDistance <13&& SRF_04_Data2.getDistance <13){
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
				}break;
		}	
		case 5:{
			outfireRobotState.moveWays = REVERSE_RIGHT_TURN_90;
			outfireRobotState.workStep++;
			break;
		}
		case 6:{
			outfireRobotState.moveWays = STOP;
			outfireRobotState.workStep++;
			break;
		}
		case 7:{
			outfireRobotState.moveWays = OTHER;
			calibrationFinish = app_calibration(LEFT);
			if(calibrationFinish == CALIBRATION_FINISHED){
				outfireRobotState.workStep++;
			}break;
		}
		case 8:{
			outfireRobotState.moveWays = GO_STRAIGHT;//前进1800ms,目的是使车到达路口处
			vTaskDelay(1800);
			outfireRobotState.workStep++;
			break;
		}
		case 9:{
			outfireRobotState.moveWays = STOP;
			outfireRobotState.workStep++;
			break;
		}
		case 10:{
			outfireRobotState.moveWays = FRONT_TURN_RIGHT_90;//向前右转90°
			outfireRobotState.workStep++;
			break;
		}
		case 11:{
			outfireRobotState.moveWays = STOP;
			outfireRobotState.workStep++;
			break;
		}
		case 12:{
			outfireRobotState.moveWays = GO_STRAIGHT;//前进1000ms使车完全进入过道
			vTaskDelay(1000);
			outfireRobotState.workStep++;
			break;
		}
		case 13:{
			outfireRobotState.moveWays = STOP;
			outfireRobotState.workStep++;
			break;
		}
		case 14:{
			calibrationFinish = app_calibration(LEFT);//原地校准,防止车体歪斜
			if(calibrationFinish == CALIBRATION_FINISHED){
				outfireRobotState.workStep++;
			}break;
		}
		case 15:{
			outfireRobotState.moveWays = OTHER;
			calibrationGostraight(RIGHT);//沿右墙前进
			if(SRF_04_Data1.getDistance <45 && SRF_04_Data2.getDistance <45){//距离前方墙面小于45停止
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep = 99;
			}
			break;
		}
		case 99:{
			outfireRobotState.moveWays = OTHER;
			outfireRobotState.step = FINISH;//完成标志位
			outfireRobotState.workStep = 0;
			break;
		}
	}
}

void app_returnFromFourthFire(void){//4号火点和5号火点的返回函数
	switch(outfireRobotState.workStep){
		case 0:{
			calibrationGostraight(RIGHT);//沿右墙前进
			if(SRF_04_Data1.getDistance <= 12 && SRF_04_Data2.getDistance <= 12){//检测前方墙壁小于等于12时停止
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep++;
			}break;
		}
		case 1:{
			outfireRobotState.moveWays = REVERSE_LEFT_TURN_90;//原地左转90°
			outfireRobotState.workStep++;
			break;
		}
		case 2:{
			outfireRobotState.moveWays = STOP;
			outfireRobotState.workStep++;
			break;
		}
		case 3:{
			outfireRobotState.moveWays = OTHER;
			calibrationFinish = app_calibration(RIGHT);//校准防止车体歪斜,因为分岔口这个过道很复杂,所以必须要保持车体直行
			if(calibrationFinish == CALIBRATION_FINISHED){
				outfireRobotState.workStep++;
			}break;
		}
		case 4:{
			calibrationGostraight(RIGHT);//沿右墙前进,这里调用GO_STRAIGHT会更好
			if(SRF_04_Data6.getDistance> 40){//右边最后一个超声波大于40,表示车体已经到达分岔口出
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep++;
			}break;
		}
		case 5:{
			outfireRobotState.moveWays = OTHER;
			calibrationFinish = app_calibration(LEFT);//原地校准防止车体歪斜
			if(calibrationFinish == CALIBRATION_FINISHED){
				outfireRobotState.workStep++;
			}break;
		}
		case 6:{
			outfireRobotState.moveWays = REVERSE_LEFT_TURN_90;//原地左转90°
			outfireRobotState.workStep++;
			break;
		}
		case 7:{
			outfireRobotState.moveWays = STOP;
			outfireRobotState.workStep++;
			break;
		}
		case 8:{
			if(SRF_04_Data1.getDistance >11&& SRF_04_Data2.getDistance >11){//为了防止车体过于远离左墙而导致之后的动作会发生撞墙
					outfireRobotState.moveWays = GO_STRAIGHT;
				}else if(SRF_04_Data1.getDistance <11&& SRF_04_Data2.getDistance <11){
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
				}break;
		}	
		case 9:{
			outfireRobotState.moveWays = REVERSE_RIGHT_TURN_90;
			outfireRobotState.workStep++;
			break;
		}
		case 10:{
			outfireRobotState.moveWays = STOP;
			outfireRobotState.workStep++;
			break;
		}
		case 11:{
			outfireRobotState.moveWays = OTHER;
			calibrationFinish = app_calibration(LEFT);
			if(calibrationFinish == CALIBRATION_FINISHED){
				outfireRobotState.workStep++;
			}break;
		}
		case 12:{
			calibrationGostraight(LEFT);//前进1800ms使车到达回家过道的路口处
			vTaskDelay(1800);
			outfireRobotState.workStep++;
			break;
		}
		case 13:{
			outfireRobotState.moveWays = STOP;
			outfireRobotState.workStep++;
			break;
		}
		case 14:{
			outfireRobotState.moveWays = FRONT_TURN_RIGHT_90;//向前左转90°
			outfireRobotState.workStep++;
			break;
		}
		case 15:{
			outfireRobotState.moveWays = STOP;
			outfireRobotState.workStep++;
			break;
		}
		case 16:{
			outfireRobotState.moveWays = GO_STRAIGHT;//前进1000ms使车完全进入过道
			vTaskDelay(1000);
			outfireRobotState.workStep++;
			break;
		}
		case 17:{
			outfireRobotState.moveWays = STOP;
			outfireRobotState.workStep++;
			break;
		}
		case 18:{
			calibrationFinish = app_calibration(LEFT);//原地校准
			if(calibrationFinish == CALIBRATION_FINISHED){
				outfireRobotState.workStep++;
			}break;
		}
		case 19:{
			outfireRobotState.moveWays = OTHER;
			calibrationGostraight(RIGHT);//沿右墙前进
			if(SRF_04_Data1.getDistance <45 && SRF_04_Data2.getDistance <45){//超声波小于45停止
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep = 99;
			}
			break;
		}
		case 99:{
			outfireRobotState.moveWays = OTHER;
			outfireRobotState.step = FINISH;//完成标志位
			outfireRobotState.workStep = 0;
			break;
		}
	}
			
}

void app_returnFromSenventhFire(void){//7号火点返回函数,对接7号火点函数结束时的状态
	switch(outfireRobotState.workStep){
		case 0:{
			outfireRobotState.moveWays = REVERSE_RIGHT_TURN_90;//原地右转90°
			outfireRobotState.workStep++;
			break;
		}
		case 1:{
			outfireRobotState.moveWays = STOP;
			vTaskDelay(200);//该延时可有可无
			outfireRobotState.workStep++;
			break;
		}
		case 2:{
			calibrationFinish = app_calibration(LEFT);//原地校准,保证车体不是歪斜的
			if(calibrationFinish == CALIBRATION_FINISHED){
				outfireRobotState.workStep++;
			}break;
		}
		case 3:{
			outfireRobotState.moveWays = GO_STRAIGHT;//前进1500ms使车到达回家过道的路口处
			vTaskDelay(1500);
			outfireRobotState.workStep++;
			break;
		}
		case 4:{
			outfireRobotState.moveWays = FRONT_TURN_RIGHT_90;//向前转向90°
			outfireRobotState.workStep++;
			break;
		}
		case 5:{
			outfireRobotState.moveWays = STOP;
			outfireRobotState.workStep++;
			break;
		}
		case 6:{
			outfireRobotState.moveWays = GO_STRAIGHT;//前进600ms使车完全进入过道
			vTaskDelay(600);
			outfireRobotState.workStep++;
			break;
		}
		case 7:{
			outfireRobotState.moveWays = STOP;
			outfireRobotState.workStep++;
			break;
		}
		case 8:{
			outfireRobotState.moveWays = OTHER;
			calibrationGostraight(RIGHT);//沿右墙前进
			if(SRF_04_Data1.getDistance <45 && SRF_04_Data2.getDistance <45){//前方超声波距墙小于45停车
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep = 99;
			}break;
		}
		case 99:{
			outfireRobotState.moveWays = OTHER;
			outfireRobotState.step = FINISH;//完成标志位
			outfireRobotState.workStep = 0;
			break;
		}
	}		
}


void returnHome(outfireRobotState_t *lastfire){//返程函数
	if(lastfire->returnFlag[SIXTH_FIRE] == 1){//6号火点为返回节点
		app_returnFromSixthFire();//6号火点返程函数
	}else if(lastfire->returnFlag[FOURTH_FIRE] == 1){//4号火点为返回节点
		app_returnFromFourthFire();//4、5号火点返程函数
	}else if(lastfire->returnFlag[EIGHTH_FIRE] == 1){//8火点为返回节点
		app_returnFromEighthFire();//8火点返程函数
	}else if(lastfire->returnFlag[FIFTH_FIRE] == 1){//5火点为返回节点
		app_returnFromFourthFire();//4、5号火点返程函数
	}else if(lastfire->returnFlag[SENVENTH_FIRE] == 1){//7火点为返回节点
		app_returnFromSenventhFire();//7火点返程函数
	}
}

/***寻火任务**/
void app_outfireTwoWorkDoing(void){
	if(outfireRobotState.beginFlag == FREE && outfireRobotState.workMode != TEST){
		switch(outfireRobotState.robotTaskstep){
			case SIXTH_FIRE:app_findSixFire(); break;//6号火点函数(从房间3开始)
			case FOURTH_FIRE:app_findFourthFire(); break;//4、5、6火点函数(由分岔口去6火点)
			case EIGHTH_FIRE:app_findEighthFire(); break;//8号火点函数
			case SENVENTH_FIRE:app_findSeventhFire();break;//7号火点函数
			case RETURN:returnHome(&outfireRobotState);break;//返程函数
		}
	}

}
