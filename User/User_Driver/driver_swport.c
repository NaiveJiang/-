#include "driver.h"
#include "app.h"
//IO�ڳ�ʼ��
void driver_port_Init(void){
	
	BSP_GPIO_Init(LKEN_PORT,GPIO_Mode_Out_PP);			//�����߿�ʹ�ܹ����
	BSP_GPIO_Init(DCVCHK_PORT,GPIO_Mode_IN_FLOATING);	//��ѹ��Դ���
	
	//�������LE2:LE1:LE0,LE3Ϊ1ʱ��Ч
	BSP_GPIO_Init(PBLE3_PORT,GPIO_Mode_Out_PP);		//����LE3
	BSP_GPIO_Init(LE0_PORT,GPIO_Mode_IN_FLOATING);	//LE0
	BSP_GPIO_Init(LE1_PORT,GPIO_Mode_IN_FLOATING);	//LE1
	BSP_GPIO_Init(LE2_PORT,GPIO_Mode_IN_FLOATING);	//LE2
	BSP_GPIO_Init(LE3_PORT,GPIO_Mode_IN_FLOATING);	//LE3������Чλ��LE3Ϊ1ʱLE0~2��Ч
	
	BSP_GPIO_Init(OPIN1_PORT,GPIO_Mode_IN_FLOATING);//��������1
	
	//�����ź����
	BSP_GPIO_Init(DJOPEN_PORT,GPIO_Mode_Out_PP);	//�򿪵缫			0.1s����
	BSP_GPIO_Init(DJCLOSE_PORT,GPIO_Mode_Out_PP);	//�رյ缫			0.1s����
	BSP_GPIO_Init(O2CJ3_PORT,GPIO_Mode_Out_PP);		//����������� 		0.1s����
	BSP_GPIO_Init(STOPCJ3_PORT,GPIO_Mode_Out_PP);	//����CJ1�ϵ�		0.1s����
	BSP_GPIO_Init(UPEDCJ2_PORT,GPIO_Mode_Out_PP);	//����CJ2			50ms����
	BSP_GPIO_Init(STARTS_PORT,GPIO_Mode_Out_PP);	//����ϵͳ���ʵ�Դ	0.1s����
	BSP_GPIO_Init(STOP_P_PORT,GPIO_Mode_Out_PP);	//�رչ��ʵ�Դͣ��	0.1s����
	
	//��̬���
	BSP_GPIO_Init(FYKL_PORT,GPIO_Mode_Out_PP);		//���θ�ѹ����
	BSP_GPIO_Init(STANDBY_PORT,GPIO_Mode_Out_PP);	//��ֹ3875�������
	BSP_GPIO_Init(FD_PORT,GPIO_Mode_Out_PP);		//�ѷŵ�,�����ŵ�̵���
	BSP_GPIO_Init(PLBJ_PORT,GPIO_Mode_Out_PP);		//CPU������ʵͱ���
	BSP_GPIO_Init(PHBJ_PORT,GPIO_Mode_Out_PP);		//CPU������ʸ߱���
	BSP_GPIO_Init(HJSHDL_PORT,GPIO_Mode_Out_PP);	//��������ʱ
	BSP_GPIO_Init(STOPCS_PORT,GPIO_Mode_Out_PP);	//CPU�����ֹ3875�������
	BSP_GPIO_Init(ALARM_PORT,GPIO_Mode_Out_PP);		//����ִ��ͣ���ʵ�Դ��ͣ��
	BSP_GPIO_Init(RESET_SYS_PORT,GPIO_Mode_Out_PP);	//CPU���ϵͳ��λ�ź�
	
	//�ź�������
	BSP_GPIO_Init(CJ3OK_PORT,GPIO_Mode_IN_FLOATING);	//���������⿪��
	BSP_GPIO_Init(PUPOK_PORT,GPIO_Mode_IN_FLOATING);	//���ϵ��⣬�ﵽ80%����CJ2
	BSP_GPIO_Init(CJ12OK_PORT,GPIO_Mode_IN_FLOATING);	//CJ1~CJ2�ϵ����
	BSP_GPIO_Init(LRUN_PORT,GPIO_Mode_IN_FLOATING);		//���������б�־
	BSP_GPIO_Init(DHAL_PORT,GPIO_Mode_IN_FLOATING);		//��ѹ��𱨾�
	BSP_GPIO_Init(HIAL_PORT,GPIO_Mode_IN_FLOATING);		//��ѹ�ŵ����
	BSP_GPIO_Init(IGBTBAL_PORT,GPIO_Mode_IN_FLOATING);	//IGBTB����
	BSP_GPIO_Init(IGBTAAL_PORT,GPIO_Mode_IN_FLOATING);	//IGBTA����
	BSP_GPIO_Init(TIAL_PORT,GPIO_Mode_IN_FLOATING);		//ԭ�ߵ�������
	BSP_GPIO_Init(IDCAL_PORT,GPIO_Mode_IN_FLOATING);	//IDC��������
	BSP_GPIO_Init(QSALARM_PORT,GPIO_Mode_IN_FLOATING);	//ȱ�౨��
	BSP_GPIO_Init(HJSH_PORT,GPIO_Mode_IN_FLOATING);		//�����ź�
	BSP_GPIO_Init(JTJC_PORT,GPIO_Mode_IN_FLOATING);		//��ͣ����
	BSP_GPIO_Init(IN_ALARM_PORT,GPIO_Mode_IN_FLOATING);	//ϵͳ����
	BSP_GPIO_Init(PVDD_PORT,GPIO_Mode_IN_FLOATING);		//���籨��
	
}

//�������˿�׼��
void driver_port_detection(void){
	STANDBY = 1;	//��ֹ3875�������
	STOPCS = 1;
	
	FYKL = 1;	//���θ�ѹ����		
	
	RESET_SYS = 1;		//Ӳ��ϵͳ��λ 0.1s
	delay_ms(100);
	RESET_SYS = 0;
	
	if(QSALARM) get_controlData()->error_sta |= QSALARM_ERROR;		//ȱ����
	if(DCVCHK) get_controlData()->error_sta |= DCVCHK_ERROR;		//��ѹ��Դ���
}

//�������
void pulse_output(volatile unsigned long *pulase_port,uint32_t pulse_time){
	*pulase_port = 1;
	vTaskDelay(pulse_time);
	*pulase_port = 0;
}


