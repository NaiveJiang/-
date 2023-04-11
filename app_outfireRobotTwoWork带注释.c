#include "app.h"
/*********************************************************
			���˳�� 8->7->5->4->6
**********************************************************/
void app_outfireTwoWorkReady(void)
{
	/*������*/ //�ð�������С������//
	//finishFlag = SET_ZERO;
	if(outfireRobotState.workMode == OUT_FIRE){
			app_judgeFunc();//�жϷ��ؽڵ�	
		if(outfireRobotState.returnFlag[RETURN] == 0){
			outfireRobotState.robotTaskstep = EIGHTH_FIRE;//�ȴ�8�ŵ㿪ʼ�ж�
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

void app_findEighthFire(void){   //�Ƚ���8�Ż��ĺ���
	if(outfireRobotState.fireArray[EIGHTH_FIRE] == 1){//����8�Ż������
		switch(outfireRobotState.workStep){
			case 0:{
				calibrationFinish = app_calibration(LEFT);		//��ʼУ׼,ֻ�г���̬У׼Ϊ��·�߷���ƽ�к�Ż������һ��ǰ��,LEFT�������ǽ��Ϊ��׼У׼��
				if(calibrationFinish == CALIBRATION_FINISHED){  //У׼���
					outfireRobotState.workStep++; //������һ������
				}break;
			}
			case 1:{
				calibrationGostraight(LEFT);		//������ǽǰ��,����ǽ������Ϊ��׼���б�У׼��ǰ����ǰ����ʽ
				if(SRF_04_Data1.getDistance < 60 && SRF_04_Data2.getDistance < 60){//���ǰ����⵽��ǽС��60�ĳ�����ֵ,��ֹͣ,������һ������
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
				}
			}
			case 2:{
				outfireRobotState.moveWays = GO_STRAIGHT;//��ͨǰ����ʽ,������ǽ�������ھ�����,��Ҫʹ�ø÷�ʽ�ó�����ֱ����ʻ
				if(SRF_04_Data1.getDistance <= 13 && SRF_04_Data2.getDistance <= 13){ //������ֵΪС�ڵ���13,��ֹͣ,������һ������
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
				}break;
			}
			case 3:{
				outfireRobotState.moveWays = REVERSE_LEFT_TURN_90;  //ԭ����ת90��
				outfireRobotState.workStep++;
				break;
			}
			case 4:{
				outfireRobotState.moveWays = STOP;//��ס,ת����һ��Ҫ�ǵ�ֹͣ,����ܿ��ܻ�ת��ͷ����һֱ����ת,������ԭ����ת90�����һ������STOP
				vTaskDelay(200);  //�ô���ʱ200ms���岻��,�ɿ���ɾ��
				outfireRobotState.workStep++;
				break;
			}
			case 5:{
				calibrationFinish = app_calibration(RIGHT);  //���ұ�ǽ��У׼,��ֹ��������ת��������ת��ǶȲ���׼ȷ�����
				if(calibrationFinish == CALIBRATION_FINISHED){
					outfireRobotState.moveWays = OTHER;//����lastmoveWays��moveWays����ͬʱ��ΪSTOP,�����ص�����һ��OTHER��־,�ñ�־û���κζ���,�����ñ�־λ
					outfireRobotState.workStep++;
				}break;
			}
			case 6:{
				calibrationGostraight(RIGHT);//����ǽǰ��
				if(SRF_04_Data1.getDistance <=53 && SRF_04_Data2.getDistance <=53){	//ǰ������������ֵС�ڵ���53ʱ������һ������
					outfireRobotState.workStep++;
				}break;
			}
			case 7:{
				outfireRobotState.moveWays = STOP;//ֹͣ
				outfireRobotState.workStep++;
				break;
			}
			case 8:{
				outfireRobotState.moveWays = REVERSE_LEFT_TURN_90;//ԭ����ת90��
				outfireRobotState.workStep++;
				break;
			}
			case 9:{
				outfireRobotState.moveWays = STOP;//ֹͣ,����case4�н���
				outfireRobotState.workStep++;
				break;
			}
			case 10:{
				calibrationGostraight(LEFT);//����ǽУ׼ǰ��,������ʹ��GO_STRAIGHTӦ�û����,��ǽ���������д���һ����������Χ�Ż������õ�����,����ֻ���ڳ��������ݶ����ȶ��������,����������ƶ��ڵ�һ��ת����˵�ǲ������õ�
				if(SRF_04_Data4.getDistance <50){ //���������һ����������ֵС��50,�������Ѿ���ȫ���뷿��4,������һ��
					outfireRobotState.workStep++;
				}break;
			}
			case 11:{//case11��һ�����⴦����,Ŀ���Ƿ�ֹ������ǽ�������³�ת��ʱ��ײǽ
				app_LeftWheel(FRONT,parameter[NAME_GO_STRAIGHT__LEFT_RATE]);	
				app_RightWheel(FRONT,parameter[NAME_GO_STRAIGHT__RIGHT_RATE]);//���ó�ǰ��1.2s(ȷ������ȫ����4�ŷż���)
				vTaskDelay(1200);
				app_LeftWheel(FRONT,parameter[NAME_SET_ZERO__LEFT_RATE]);//�ó�ֹͣ
				app_RightWheel(FRONT,parameter[NAME_SET_ZERO__RIGHT_RATE]);
				outfireRobotState.moveWays = FRONT_TURN_RIGHT_90;//��ǰ��ת90��(����ת��)
				outfireRobotState.workStep++;
				break;
			}
			case 12:{
				outfireRobotState.moveWays = FRONT_TURN_LEFT_90;//��ǰ��ת90��,ʹ������ǰ��
				outfireRobotState.workStep++;
				break;
			}
			case 13:{
				outfireRobotState.moveWays = STOP;//ֹͣ,ʹ��ԭ��У׼ǰһ��Ҫֹͣ
				outfireRobotState.workStep++;
				break;
			}
			case 14:{
				outfireRobotState.moveWays = OTHER;  //���ñ�־
				calibrationFinish = app_calibration(LEFT);//ԭ��У׼,Ϊ��ʹ������ȷ����ǰ��
				if(calibrationFinish == CALIBRATION_FINISHED){
					outfireRobotState.workStep++;
				}break;
			}
			case 15:{
				outfireRobotState.moveWays = OTHER;
				calibrationGostraight(RIGHT);//����ǽǰ��
				if(SRF_04_Data1.getDistance <43 && SRF_04_Data2.getDistance <43){//��������ֵС��43��ֹͣ
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
				}break;
			}
			case 16:{
				outfireRobotState.moveWays = REVERSE_LEFT_TURN_45;//ԭ����ת45��,Ϊ��֮���ADѰ���ܹ�ʹ���ܹ�ֱ��ǰ��Ѱ��,׼ȷ��λ���ķ���,���򳵵ķ������ȷ��
				outfireRobotState.workStep++;
				break;
			}
			case 17:{//�õط�û�е���STOP�ܿ���ת��,���������ΪADѰ����,�����ܻ���ƫ
				calibrationFinish = app_findFire();//ADѰ����,ʹ���ܹ������Ż��淽��,�����������׼ȷ��
				outfireRobotState.moveWays = OTHER;
				if(calibrationFinish == CALIBRATION_FINISHED && SRF_04_Data1.getDistance <25 && SRF_04_Data2.getDistance <25){//����ADѰ������ͷ�Դ���һ��ǰ������,������������������ǽ��С��25���ͻ�ֹͣ
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
				}break;
			}
			case 18:{
				app_fan();//�򿪷���
				calibrationFinish = checkFire();//�������Ƿ����
				if(calibrationFinish == CALIBRATION_FINISHED){//ȷ�ϻ������
					outfireRobotState.workStep++;
				}break;
			}
			case 19:{
				app_goBack(600,NO_TURN);//����600ms,��ֹ����ǽ̫��ײǽ
				outfireRobotState.workStep++;
				break;
			}
			case 20:{
				outfireRobotState.moveWays = REVERSE_RIGHT_TURN_135;//ԭ����ת135��,Ŀ����ʹ���������·�ǽ��,���������һ����7�����ж�
				outfireRobotState.workStep++;
				break;
			}
			case 21:{
				outfireRobotState.moveWays = STOP;//ֹͣ
				vTaskDelay(200);
				outfireRobotState.workStep++;
				break;
			}
			case 22:{
				outfireRobotState.moveWays = OTHER;
				calibrationFinish = app_calibration(LEFT);//ԭ��У׼
				if(calibrationFinish == CALIBRATION_FINISHED){
					outfireRobotState.workStep++;
				}break;
			}
			case 23:{//case23��case25��ʵ���Ժϲ�
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
			case 99:{//������һ��������׼���׶�
				if(outfireRobotState.returnFlag[EIGHTH_FIRE] == 1){//���8�Ƿ��ؽڵ�,��˼��8�����һ�����
					outfireRobotState.robotTaskstep = RETURN;//���뷵�ر�־
				}else{
					outfireRobotState.robotTaskstep = SENVENTH_FIRE;//�������7�����ж�
				}
				outfireRobotState.moveWays = OTHER;//���ñ�־,������һ�׶εĲ���
				outfireRobotState.workStep = 0;
				break;
			}
		}
	}
	else{//�����8�Ż�㲻���ڵ����
		if(outfireRobotState.fireArray[SENVENTH_FIRE] == 1){//�������7���,��Ȼ����뷿��4
			switch(outfireRobotState.workStep){  //case0~case9�����8���Ĳ��趼��һ����,���ǽ��뷿��
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
					outfireRobotState.moveWays = GO_STRAIGHT;//ǰ��600ms,�����û�е�������ǽУ׼�ĺ���,��ʱ600ms���ó�����ȫ���뷿��4
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
					calibrationGostraight(LEFT);//����ǽǰ��
					if(SRF_04_Data1.getDistance <35 && SRF_04_Data2.getDistance <35){//��������ֵС��35ʱֹͣ
						outfireRobotState.moveWays = STOP;
						outfireRobotState.workStep++;
					}break;
				}
				case 13:{
					outfireRobotState.moveWays = REVERSE_RIGHT_TURN_90;//ԭ����ת90������7��㷽��,��7��㺯���ĳ�ʼ״̬�Խ�
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
					outfireRobotState.robotTaskstep = SENVENTH_FIRE;//����7��㺯��
					outfireRobotState.workStep = 0;
					break;
				}
			}
		}
		else{//����4�Ļ�㶼������,������뵽����4,�����4��㺯���ȶԽ�,�����ͣ�ڷֲ�ڴ�
			//case0~case7����ͬ
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
					outfireRobotState.moveWays = REVERSE_RIGHT_TURN_90;//ԭ����ת90��,Ϊ�˽���4�Ż��ĺ����Խ�
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
					outfireRobotState.robotTaskstep = FOURTH_FIRE;//����4�Ż�㺯��
					outfireRobotState.workStep = 0;
					break;
				}
			}
		}
	}
}


void app_findSixFire(void){//6�Ż�㺯��,���ǽӿ��������˷���3�Ļ����״̬
	if(outfireRobotState.fireArray[SIXTH_FIRE] == 1){
		switch(outfireRobotState.workStep){
			case 0:{
				outfireRobotState.moveWays = OTHER;
				calibrationGostraight(RIGHT);//����ǽǰ��
				if(SRF_04_Data1.getDistance <25 && SRF_04_Data2.getDistance <25){//ǰ��ǽ�ھ���С��25
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
				}break;
			}
			case 1:{
				calibrationFinish = app_findFire();//���������Է���3�ĳ���,����ֱ��ʹ��ADѰ����,�������ԭ��ת��45����ܻ�ײǽ
				outfireRobotState.moveWays = OTHER;
				if(calibrationFinish == CALIBRATION_FINISHED && SRF_04_Data1.getDistance <28 && SRF_04_Data2.getDistance <28 ){//��ǰ��ǽ��28ʱֹͣ
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
				}break;
			}
			case 2:{
				outfireRobotState.moveWays = OTHER;
				app_fan();//�����
				calibrationFinish = checkFire();
				if(calibrationFinish == CALIBRATION_FINISHED){
					outfireRobotState.workStep++;
				}break;
			}
			case 3:{
				app_goBack(100,NO_TURN);//����100ms,̫������ײǽ
				outfireRobotState.workStep++;
				break;
			}
			case 4:{
				outfireRobotState.moveWays = REVERSE_LEFT_TURN_135;//ԭ����ת135��,ʹ��ͷ��ų���·�ڷ���,֮����ͨ��ԭ��У׼ʹ�侫ȷ����
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
				if(outfireRobotState.returnFlag[SIXTH_FIRE] == 1){//��Ϊ6�Ż��һ�������Ļ��,���Աض�����
					outfireRobotState.robotTaskstep = RETURN;
				}
				outfireRobotState.moveWays = OTHER;
				outfireRobotState.workStep = 0;
				break;
			}
		}
	}	
}

void app_findFourthFire(void){//4�Ż���жϺ�5�Ż���жϽ����һ��,����4��5�ķ��ؽڵ㶼��һ����
	if(outfireRobotState.fireArray[FOURTH_FIRE] == 1 && outfireRobotState.fireArray[FIFTH_FIRE] == 1 ){	//4�Ż���5�Ż�㶼���ڵ����
		switch(outfireRobotState.workStep){//�Խ�7�Ż�㺯���ĳ��Ľ���״̬������һ������
			case 0:{//��һ�������⴦����,��ֹ������ǽ�������µ����,����ܿ���ǽ��һЩ,��������֮���6�Ż��Ͳ��������ײ
				if(SRF_04_Data1.getDistance >16 && SRF_04_Data2.getDistance >16){  //���ǰ���������ľ������16��ǰ��
					outfireRobotState.moveWays = GO_STRAIGHT;
				}else if(SRF_04_Data1.getDistance <16&& SRF_04_Data2.getDistance <16){//С��16��ֹͣ
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
				}break;
			}	
			case 1:{
				outfireRobotState.moveWays = REVERSE_LEFT_TURN_90;//ԭ����ת90��ʹ��ͷ�������·����һ��,��Ϊ7�Ż�㺯������ʱ��ͷ��״̬������ǰ��ǽ�ڵ�
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
				calibrationFinish = app_calibration(RIGHT);//ԭ��У׼
				if(calibrationFinish == CALIBRATION_FINISHED){
					outfireRobotState.workStep++;
				}break;
			}
			case 4:{
				outfireRobotState.moveWays = GO_STRAIGHT;//��ͨǰ��,���ﲻʹ����ǽ����Ҳ��Ϊ�˱�����ǽ�����ı׶�
				if(SRF_04_Data1.getDistance <18 && SRF_04_Data2.getDistance <18){//����ǰ��ǽ��С��18��ֹͣ
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
				}
				break;
			}
			case 5:{
				outfireRobotState.moveWays = REVERSE_RIGHT_TURN_90;//ԭ����ת90��
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
				calibrationFinish = app_calibration(LEFT);//ԭ��У׼
				if(calibrationFinish == CALIBRATION_FINISHED){
					outfireRobotState.workStep++;
				}break;
			}
			case 8:{
				outfireRobotState.moveWays = OTHER;
				calibrationGostraight(LEFT);//����ǽǰ��
				if(SRF_04_Data1.getDistance <45 && SRF_04_Data2.getDistance <45){//����ǰ��ǽ��45��ֹͣ,Ŀ����Ϊ��֮���λ4�Ż��ķ�����ӽ�45��
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
				outfireRobotState.moveWays = REVERSE_RIGHT_TURN_90;//ԭ����ת90��
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
				calibrationFinish = app_calibration(LEFT);//ԭ��У׼
				if(calibrationFinish == CALIBRATION_FINISHED){
					outfireRobotState.workStep++;
				}break;
			}
			case 13:{
				calibrationGostraight(LEFT);//����ǽǰ��
				if(SRF_04_Data1.getDistance <30 && SRF_04_Data2.getDistance <30){//��⵽ǰ������С��30ֹͣ
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
				}break;
			}
			case 14:{
				outfireRobotState.moveWays = REVERSE_RIGHT_TURN_45;//ԭ����ת45��,Ŀ��ͬ�����������ͬ��
				outfireRobotState.workStep++;
				break;
			}
			case 15:{
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep++;
				break;
			}
			case 16:{
				calibrationFinish = app_findFire();//ADѰ��
				outfireRobotState.moveWays = OTHER;
				if(calibrationFinish == CALIBRATION_FINISHED && SRF_04_Data1.getDistance <26&& SRF_04_Data2.getDistance <26){//����������С��26ֹͣ
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
				outfireRobotState.moveWays = REVERSE_RIGHT_TURN_135;//ԭ����ת135��,ʹ����������5������ڵ�ǽ�ڷ���
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
				calibrationFinish = app_calibration(LEFT);//ԭ��У׼
				if(calibrationFinish == CALIBRATION_FINISHED){
					outfireRobotState.workStep++;
				}break;
			}
			case 22:{
				calibrationGostraight(RIGHT);//����ǽǰ��,���ﲻ������ǽͬ����Ϊ�˱�����ǽ�����ı׶�
				if(SRF_04_Data1.getDistance <35 && SRF_04_Data2.getDistance <35){
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
				}
				break;
			}
			case 23:{
				outfireRobotState.moveWays = REVERSE_RIGHT_TURN_45;//ԭ����ת45��,Ŀ��ͬ�������������ͬ
				outfireRobotState.workStep++;
				break;
			}
			case 24:{
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep++;
				break;
			}
			case 25:{
				calibrationFinish = app_findFire();//Ѱ����
				outfireRobotState.moveWays = OTHER;
				if(calibrationFinish == CALIBRATION_FINISHED && SRF_04_Data1.getDistance <24 && SRF_04_Data2.getDistance <24){//����ǰ��ǽ��С��24ֹͣ
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
				}break;
			}
			case 26:{
				outfireRobotState.moveWays = OTHER;
				app_fan();//�����,��5�ŵ�
				calibrationFinish = checkFire();
				if(calibrationFinish == CALIBRATION_FINISHED){
					outfireRobotState.workStep++;
				}break;
			}
			case 27:{
				app_goBack(200,NO_TURN);//����200ms
				outfireRobotState.workStep++;
				break;
			}
			case 28:{
				outfireRobotState.moveWays = REVERSE_LEFT_TURN_135;//ԭ����ת135��,ʹ��ͷ���ŷ���3���ſ�,�Խ�6�Ż��ĳ�ʼ״̬
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
	else if(outfireRobotState.fireArray[FOURTH_FIRE] == 1 && outfireRobotState.fireArray[FIFTH_FIRE] == 0){//��4�ŵ㵫û��5�ŵ�
		switch(outfireRobotState.workStep){//case0~case21����ͬ��4��5��ʱ��ͬ��
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
				calibrationGostraight(RIGHT);//����ǽǰ��,Ϊ�˱�����ǽ�����ı׶�
				if(SRF_04_Data1.getDistance <20 && SRF_04_Data2.getDistance <20){//���С��20��ֹͣ
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
				}
				break;
			}
			case 23:{
				outfireRobotState.moveWays = REVERSE_LEFT_TURN_90;//ԭ����ת90��,��ͷ�����ط���3����,�Խ�6�Ż�㺯��
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
	else if(outfireRobotState.fireArray[FOURTH_FIRE] == 0){//���û��4�Ż��
		if(outfireRobotState.fireArray[FIFTH_FIRE] == 1){//����5�Ż��
			switch(outfireRobotState.workStep){//case0~case7�����4���Ĳ�����ͬ,���ǽ��뷿��3�ĺ���
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
					calibrationGostraight(LEFT);//����ǽǰ��
					if(SRF_04_Data1.getDistance <35 && SRF_04_Data2.getDistance <35){//С��35ֹͣ
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
					outfireRobotState.moveWays = REVERSE_LEFT_TURN_45;//ԭ����ת45��
					outfireRobotState.workStep++;
					break;
				}
				case 11:{//�ô�û�е���STOP,���ܴ���ת��ͷ�����
					calibrationFinish = app_findFire();//ADѰ����
					outfireRobotState.moveWays = OTHER;
					if(calibrationFinish == CALIBRATION_FINISHED && SRF_04_Data1.getDistance <27&& SRF_04_Data2.getDistance <27){
						outfireRobotState.moveWays = STOP;
						outfireRobotState.workStep++;
					}break;
				}
				case 12:{
					outfireRobotState.moveWays = OTHER;
					app_fan();//�����
					calibrationFinish = checkFire();
					if(calibrationFinish == CALIBRATION_FINISHED){
						outfireRobotState.workStep++;
					}break;
				}
				case 13:{
					app_goBack(150,NO_TURN);//����150ms,�������ʱ��̫��������ת135����ײ�����ڵ�ǽ��
					outfireRobotState.workStep++;
					break;
				}
				case 14:{
					outfireRobotState.moveWays = REVERSE_LEFT_TURN_135;//ԭ����ת135��,�Խ�6�Ż�㺯���ĳ�ʼ״̬
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
					calibrationFinish = app_calibration(RIGHT);//ԭ��У׼
					if(calibrationFinish == CALIBRATION_FINISHED){
						outfireRobotState.workStep = 99;
					}break;
				}
				case 99:{
					if(outfireRobotState.returnFlag[FIFTH_FIRE] == 1){//���5�Ƿ��ؽڵ�Ļ�
						outfireRobotState.robotTaskstep = RETURN;
					}
					else{
						outfireRobotState.robotTaskstep = SIXTH_FIRE;//����6�Ż��ĺ���
					}
					outfireRobotState.moveWays = OTHER;
					outfireRobotState.workStep = 0;
					break;
				}
			}
		}
		else{//û�н��뷿��3�Ļ��,������뷿��3,����ʣ��һ��6�Ż��,�˴��Ǵӷֲ��ȥ6�Ż���·��,��6�Ż�㺯���ǶԽ��ŷ���3��㺯������,������΢��Щ��ͬ
			switch(outfireRobotState.workStep){//case0~case3����ȥ����3�ĵĲ�����ͬ
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
					calibrationGostraight(LEFT);//����ǽǰ��,������Ӧ�õ���GO_STRAIGHT�����
					if(SRF_04_Data1.getDistance <20 && SRF_04_Data2.getDistance <20){//ǰ����ǽС��20ֹͣ
						outfireRobotState.moveWays = STOP;
						outfireRobotState.workStep++;
					}
					break;
				}
				case 5:{
					calibrationFinish = app_findFire();//ADѰ��
					outfireRobotState.moveWays = OTHER;
					if(calibrationFinish == CALIBRATION_FINISHED && SRF_04_Data1.getDistance <28 && SRF_04_Data2.getDistance <28){//��ǽ����С��28ֹͣ
						outfireRobotState.moveWays = STOP;
						outfireRobotState.workStep++;
					}break;
				}
				case 6:{
					app_fan();//�����
					outfireRobotState.moveWays = OTHER;
					calibrationFinish = checkFire();//����Ƿ�������
					if(calibrationFinish == CALIBRATION_FINISHED){
						outfireRobotState.workStep++;
					}break;
				}
				case 7:{
					app_goBack(180,NO_TURN);//����180ms,̫�����ܻ�ײ��ǽ��
					outfireRobotState.workStep++;
					break;
				}
				case 8:{
					outfireRobotState.moveWays = REVERSE_LEFT_TURN_135;//ԭ����ת135��
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
					if(outfireRobotState.returnFlag[SIXTH_FIRE] == 1){//��Ϊ6�Ż��һ�������Ļ��,���Աض�����
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
	if(outfireRobotState.fireArray[SENVENTH_FIRE] == 1){ //���7�Ż�����
		switch(outfireRobotState.workStep){//��8��㺯����������еĶԽ�,����Ҫ��8��㺯���ĳ����Ľ���״̬��������һ������
			case 0:{
				calibrationGostraight(LEFT);//����ǽǰ��
				if(SRF_04_Data1.getDistance <26 && SRF_04_Data2.getDistance <26){//��������ֵС��26ֹͣ
					outfireRobotState.workStep++;
				}
				break;
			}
			case 1:{
				outfireRobotState.moveWays = REVERSE_LEFT_TURN_45;//ԭ����ת45��,��Ŷ���7���ķ���,Ŀ��ͬ8�Ż��Ĵ���ʽ��ͬ
				outfireRobotState.workStep++;
				break;
			}
			case 2:{
				calibrationFinish = app_findFire();//ADѰ��,Ŀ����Ϊ��׼ȷ����7��㷽��
				outfireRobotState.moveWays = OTHER;
				if(calibrationFinish == CALIBRATION_FINISHED && SRF_04_Data1.getDistance <22 && SRF_04_Data2.getDistance <22){//����7��㳬������ֵС��22��λ��ֹͣ
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
				}break;
			}
			case 3:{
				app_fan();//���
				outfireRobotState.moveWays = OTHER;
				calibrationFinish = checkFire();//�������Ƿ����
				if(calibrationFinish == CALIBRATION_FINISHED){
					outfireRobotState.workStep++;
				}break;
			}
			case 4:{
				app_goBack(800,NO_TURN);//����800ms
				outfireRobotState.workStep++;
				break;
			}
			case 5:{
				outfireRobotState.moveWays = REVERSE_LEFT_TURN_135;//ԭ����ת135��,Ŀ���������Ϸ�ǽ��,���ڷ���
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
				calibrationGostraight(RIGHT);//����ǽǰ��
				if(SRF_04_Data1.getDistance <14 && SRF_04_Data2.getDistance <14){//���ǰ�����С��14ֹͣ
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
				outfireRobotState.moveWays = REVERSE_LEFT_TURN_90;//ԭ����ת90��
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
				calibrationFinish = app_calibration(RIGHT);//ԭ��У׼
				if(calibrationFinish == CALIBRATION_FINISHED){
					outfireRobotState.workStep++;
				}break;
			}
			case 12:{
				calibrationGostraight(RIGHT);//����ǽǰ��
				if(SRF_04_Data1.getDistance <= 65 && SRF_04_Data2.getDistance <= 65){//����ط�ͬ���ŵ�һ���ս�������ͬ,���Ǳ�������ǽǰ�������ı׶�
					outfireRobotState.workStep++;
				}break;
			}
			case 13:{
					outfireRobotState.moveWays = GO_STRAIGHT;//�л�Ϊ��ͨǰ��
				 if(SRF_04_Data1.getDistance < 16 && SRF_04_Data2.getDistance < 16){//��⵽ǰ��ǽ�����Ϊ16ֹͣ,�Խ�4�Ż�㺯��
					outfireRobotState.workStep++;
				}break;
			}
			case 14:{
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep = 99;
				break;
			}
			case 99:{
				if(outfireRobotState.returnFlag[SENVENTH_FIRE] == 1){//���7�Ż�������һ�������
					outfireRobotState.robotTaskstep = RETURN;//���뷵�ر�־
				}else{
					outfireRobotState.robotTaskstep = FOURTH_FIRE;//�������4�Ż�㺯���Խ�
				}
				outfireRobotState.moveWays = OTHER;
				outfireRobotState.workStep = 0;
				vTaskDelay(200);
				break;
			}
		}
	}else{//���û��7���
		switch(outfireRobotState.workStep){
			case 0:{
				outfireRobotState.moveWays = REVERSE_RIGHT_TURN_180;//ԭ�����ת180��
				outfireRobotState.workStep++;
				break;
			}
			case 1:{
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep++;
				break;
			}
			case 2:{
				calibrationGostraight(RIGHT);//����ǽǰ��
				if(SRF_04_Data1.getDistance <12 && SRF_04_Data2.getDistance <12){//����ǰ��ǽ�ھ���16ʱֹͣ
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
				}break;
			}
			case 3:{
				outfireRobotState.moveWays = REVERSE_LEFT_TURN_90;//ԭ����ת90��
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
				calibrationFinish = app_calibration(RIGHT);//ԭ��У׼
				if(calibrationFinish == CALIBRATION_FINISHED){
					outfireRobotState.workStep++;
				}break;
			}
			case 6:{
				calibrationGostraight(RIGHT);//����ǽǰ��
				if(SRF_04_Data1.getDistance < 60 && SRF_04_Data2.getDistance < 60){//����ͬ���ŵ�һ���ս���ͬ,Ϊ�˱�����ǽǰ�������ı׶�
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep++;
				}break;
			}
			case 7:{
				outfireRobotState.moveWays = GO_STRAIGHT;//�л�Ϊ��ͨǰ������
				if(SRF_04_Data1.getDistance <= 16 && SRF_04_Data2.getDistance <= 16){//����ǰ��ǽ�ڵľ���С�ڵ���16ʱֹͣ,�Խ�4�Ż�㺯���ӿ�
					outfireRobotState.moveWays = STOP;
					outfireRobotState.workStep = 99;
				}break;
			}
			case 99:{
				outfireRobotState.robotTaskstep = FOURTH_FIRE;//����4�Ż�㺯��
				outfireRobotState.moveWays = OTHER;
				outfireRobotState.workStep = 0;
				break;
			}
		}
	}
}
		
				
void app_returnFromEighthFire(void){//8��㷵�غ���
	switch(outfireRobotState.workStep){//�Խ�8��������״̬
		case 0:{
			outfireRobotState.moveWays = REVERSE_RIGHT_TURN_180;//ԭ����ת180��
			outfireRobotState.workStep++;
			break;
		}
		case 1:{
			outfireRobotState.moveWays = STOP;
			outfireRobotState.workStep++;
			break;
		}
		case 2:{
			calibrationGostraight(RIGHT);//������ǽǰ��
			if(SRF_04_Data1.getDistance <10 && SRF_04_Data2.getDistance <10){//���ǰ��������Ϊ10��ֹͣ
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep++;
			}break;
		}
		case 3:{
			outfireRobotState.moveWays = REVERSE_LEFT_TURN_90;//ԭ����ת90��,��ͷ������ŷ���4�ĳ���
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
			calibrationFinish = app_calibration(RIGHT);//ԭ��У׼
			if(calibrationFinish == CALIBRATION_FINISHED){
				outfireRobotState.workStep++;
			}break;
		}
		case 6:{
			calibrationGostraight(RIGHT);//����ǽǰ��
			if(SRF_04_Data5.getDistance > 50){//����ұߵ�һ��������������50,����ǰ�벿���Ѿ��뿪�˷���4,��ʱת��ʱ���ܺ���
				outfireRobotState.workStep++;
			}break;
		}
		case 7:{
			outfireRobotState.moveWays = STOP;
			outfireRobotState.workStep++;
			break;
		}
		case 8:{
			outfireRobotState.moveWays = FRONT_TURN_RIGHT_180;//��ǰת��180��
			outfireRobotState.workStep++;
			break;
		}
		case 9:{
			outfireRobotState.moveWays = STOP;
			outfireRobotState.workStep++;
			break;
		}
		case 10:{
			outfireRobotState.moveWays = GO_STRAIGHT;//ǰ��800ms,Ŀ����ʹ���������
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
			calibrationFinish = app_calibration(RIGHT);//У׼,��ֹ��ͷ�����ŵ�
			if(calibrationFinish == CALIBRATION_FINISHED){
				outfireRobotState.workStep++;
			}break;
		}
		case 13:{
			calibrationGostraight(RIGHT);//����ǽǰ��
			if(SRF_04_Data1.getDistance <32 && SRF_04_Data2.getDistance <32){//ǰ������С��45ʱֹͣ
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep = 99;
			}break;
		}
		case 99:{
			outfireRobotState.moveWays = OTHER;
			outfireRobotState.step = FINISH;//��ɱ�־λ
			outfireRobotState.workStep = 0;
			break;
		}
	}		
}

void app_returnFromSixthFire(void){//6�Ż�㷵�غ���
	switch(outfireRobotState.workStep){//�Խ���6�Ż�����������״̬
		case 0:{
			calibrationGostraight(LEFT);//����ǽǰ��,�������GO_STRAIGHT���ܸ���
			if(SRF_04_Data6.getDistance> 40){//�ұߵڶ�������������40��ʾ�����Ѿ������˷ֲ��
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep++;
			}break;
		}
		case 1:{
			outfireRobotState.moveWays = OTHER;
			calibrationFinish = app_calibration(LEFT);//ԭ��У׼,��ֹ������б
			if(calibrationFinish == CALIBRATION_FINISHED){
				outfireRobotState.workStep++;
			}break;
		}
		case 2:{
			outfireRobotState.moveWays = REVERSE_LEFT_TURN_90;//ԭ����ת90��
			outfireRobotState.workStep++;
			break;
		}
		case 3:{
			outfireRobotState.moveWays = STOP;
			outfireRobotState.workStep++;
			break;
		}
		case 4:{
			if(SRF_04_Data1.getDistance >13&& SRF_04_Data2.getDistance >13){//Ŀ����Ϊ�˷�ֹ����ǽ̫Զ,����֮���ײǽ
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
			outfireRobotState.moveWays = GO_STRAIGHT;//ǰ��1800ms,Ŀ����ʹ������·�ڴ�
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
			outfireRobotState.moveWays = FRONT_TURN_RIGHT_90;//��ǰ��ת90��
			outfireRobotState.workStep++;
			break;
		}
		case 11:{
			outfireRobotState.moveWays = STOP;
			outfireRobotState.workStep++;
			break;
		}
		case 12:{
			outfireRobotState.moveWays = GO_STRAIGHT;//ǰ��1000msʹ����ȫ�������
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
			calibrationFinish = app_calibration(LEFT);//ԭ��У׼,��ֹ������б
			if(calibrationFinish == CALIBRATION_FINISHED){
				outfireRobotState.workStep++;
			}break;
		}
		case 15:{
			outfireRobotState.moveWays = OTHER;
			calibrationGostraight(RIGHT);//����ǽǰ��
			if(SRF_04_Data1.getDistance <45 && SRF_04_Data2.getDistance <45){//����ǰ��ǽ��С��45ֹͣ
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep = 99;
			}
			break;
		}
		case 99:{
			outfireRobotState.moveWays = OTHER;
			outfireRobotState.step = FINISH;//��ɱ�־λ
			outfireRobotState.workStep = 0;
			break;
		}
	}
}

void app_returnFromFourthFire(void){//4�Ż���5�Ż��ķ��غ���
	switch(outfireRobotState.workStep){
		case 0:{
			calibrationGostraight(RIGHT);//����ǽǰ��
			if(SRF_04_Data1.getDistance <= 12 && SRF_04_Data2.getDistance <= 12){//���ǰ��ǽ��С�ڵ���12ʱֹͣ
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep++;
			}break;
		}
		case 1:{
			outfireRobotState.moveWays = REVERSE_LEFT_TURN_90;//ԭ����ת90��
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
			calibrationFinish = app_calibration(RIGHT);//У׼��ֹ������б,��Ϊ�ֲ����������ܸ���,���Ա���Ҫ���ֳ���ֱ��
			if(calibrationFinish == CALIBRATION_FINISHED){
				outfireRobotState.workStep++;
			}break;
		}
		case 4:{
			calibrationGostraight(RIGHT);//����ǽǰ��,�������GO_STRAIGHT�����
			if(SRF_04_Data6.getDistance> 40){//�ұ����һ������������40,��ʾ�����Ѿ�����ֲ�ڳ�
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep++;
			}break;
		}
		case 5:{
			outfireRobotState.moveWays = OTHER;
			calibrationFinish = app_calibration(LEFT);//ԭ��У׼��ֹ������б
			if(calibrationFinish == CALIBRATION_FINISHED){
				outfireRobotState.workStep++;
			}break;
		}
		case 6:{
			outfireRobotState.moveWays = REVERSE_LEFT_TURN_90;//ԭ����ת90��
			outfireRobotState.workStep++;
			break;
		}
		case 7:{
			outfireRobotState.moveWays = STOP;
			outfireRobotState.workStep++;
			break;
		}
		case 8:{
			if(SRF_04_Data1.getDistance >11&& SRF_04_Data2.getDistance >11){//Ϊ�˷�ֹ�������Զ����ǽ������֮��Ķ����ᷢ��ײǽ
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
			calibrationGostraight(LEFT);//ǰ��1800msʹ������ؼҹ�����·�ڴ�
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
			outfireRobotState.moveWays = FRONT_TURN_RIGHT_90;//��ǰ��ת90��
			outfireRobotState.workStep++;
			break;
		}
		case 15:{
			outfireRobotState.moveWays = STOP;
			outfireRobotState.workStep++;
			break;
		}
		case 16:{
			outfireRobotState.moveWays = GO_STRAIGHT;//ǰ��1000msʹ����ȫ�������
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
			calibrationFinish = app_calibration(LEFT);//ԭ��У׼
			if(calibrationFinish == CALIBRATION_FINISHED){
				outfireRobotState.workStep++;
			}break;
		}
		case 19:{
			outfireRobotState.moveWays = OTHER;
			calibrationGostraight(RIGHT);//����ǽǰ��
			if(SRF_04_Data1.getDistance <45 && SRF_04_Data2.getDistance <45){//������С��45ֹͣ
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep = 99;
			}
			break;
		}
		case 99:{
			outfireRobotState.moveWays = OTHER;
			outfireRobotState.step = FINISH;//��ɱ�־λ
			outfireRobotState.workStep = 0;
			break;
		}
	}
			
}

void app_returnFromSenventhFire(void){//7�Ż�㷵�غ���,�Խ�7�Ż�㺯������ʱ��״̬
	switch(outfireRobotState.workStep){
		case 0:{
			outfireRobotState.moveWays = REVERSE_RIGHT_TURN_90;//ԭ����ת90��
			outfireRobotState.workStep++;
			break;
		}
		case 1:{
			outfireRobotState.moveWays = STOP;
			vTaskDelay(200);//����ʱ���п���
			outfireRobotState.workStep++;
			break;
		}
		case 2:{
			calibrationFinish = app_calibration(LEFT);//ԭ��У׼,��֤���岻����б��
			if(calibrationFinish == CALIBRATION_FINISHED){
				outfireRobotState.workStep++;
			}break;
		}
		case 3:{
			outfireRobotState.moveWays = GO_STRAIGHT;//ǰ��1500msʹ������ؼҹ�����·�ڴ�
			vTaskDelay(1500);
			outfireRobotState.workStep++;
			break;
		}
		case 4:{
			outfireRobotState.moveWays = FRONT_TURN_RIGHT_90;//��ǰת��90��
			outfireRobotState.workStep++;
			break;
		}
		case 5:{
			outfireRobotState.moveWays = STOP;
			outfireRobotState.workStep++;
			break;
		}
		case 6:{
			outfireRobotState.moveWays = GO_STRAIGHT;//ǰ��600msʹ����ȫ�������
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
			calibrationGostraight(RIGHT);//����ǽǰ��
			if(SRF_04_Data1.getDistance <45 && SRF_04_Data2.getDistance <45){//ǰ����������ǽС��45ͣ��
				outfireRobotState.moveWays = STOP;
				outfireRobotState.workStep = 99;
			}break;
		}
		case 99:{
			outfireRobotState.moveWays = OTHER;
			outfireRobotState.step = FINISH;//��ɱ�־λ
			outfireRobotState.workStep = 0;
			break;
		}
	}		
}


void returnHome(outfireRobotState_t *lastfire){//���̺���
	if(lastfire->returnFlag[SIXTH_FIRE] == 1){//6�Ż��Ϊ���ؽڵ�
		app_returnFromSixthFire();//6�Ż�㷵�̺���
	}else if(lastfire->returnFlag[FOURTH_FIRE] == 1){//4�Ż��Ϊ���ؽڵ�
		app_returnFromFourthFire();//4��5�Ż�㷵�̺���
	}else if(lastfire->returnFlag[EIGHTH_FIRE] == 1){//8���Ϊ���ؽڵ�
		app_returnFromEighthFire();//8��㷵�̺���
	}else if(lastfire->returnFlag[FIFTH_FIRE] == 1){//5���Ϊ���ؽڵ�
		app_returnFromFourthFire();//4��5�Ż�㷵�̺���
	}else if(lastfire->returnFlag[SENVENTH_FIRE] == 1){//7���Ϊ���ؽڵ�
		app_returnFromSenventhFire();//7��㷵�̺���
	}
}

/***Ѱ������**/
void app_outfireTwoWorkDoing(void){
	if(outfireRobotState.beginFlag == FREE && outfireRobotState.workMode != TEST){
		switch(outfireRobotState.robotTaskstep){
			case SIXTH_FIRE:app_findSixFire(); break;//6�Ż�㺯��(�ӷ���3��ʼ)
			case FOURTH_FIRE:app_findFourthFire(); break;//4��5��6��㺯��(�ɷֲ��ȥ6���)
			case EIGHTH_FIRE:app_findEighthFire(); break;//8�Ż�㺯��
			case SENVENTH_FIRE:app_findSeventhFire();break;//7�Ż�㺯��
			case RETURN:returnHome(&outfireRobotState);break;//���̺���
		}
	}

}
