#include "driver.h"
#include "app.h"
//IO�ڳ�ʼ��
void driver_port_Init(void){
	
	BSP_GPIO_Init(LKEN_PORT,GPIO_Mode_Out_PP);			//�����߿�ʹ�ܹ����
	BSP_GPIO_Init(DCVCHK_PORT,GPIO_Mode_IPD);	//��ѹ��Դ���
	
	//�������LE2:LE1:LE0,LE3Ϊ1ʱ��Ч
	BSP_GPIO_Init(PBLE3_PORT,GPIO_Mode_Out_PP);		//����LE3
	BSP_GPIO_Init(LE0_PORT,GPIO_Mode_IPD);	//LE0
	BSP_GPIO_Init(LE1_PORT,GPIO_Mode_IPD);	//LE1
	BSP_GPIO_Init(LE2_PORT,GPIO_Mode_IPD);	//LE2
	BSP_GPIO_Init(LE3_PORT,GPIO_Mode_IPD);	//LE3������Чλ��LE3Ϊ1ʱLE0~2��Ч
	
	BSP_GPIO_Init(OPIN1_PORT,GPIO_Mode_IPD);//��������1
	
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
	BSP_GPIO_Init(CJ3OK_PORT,GPIO_Mode_IPD);	//���������⿪��
	BSP_GPIO_Init(PUPOK_PORT,GPIO_Mode_IPD);	//���ϵ��⣬�ﵽ80%����CJ2
	BSP_GPIO_Init(CJ12OK_PORT,GPIO_Mode_IPD);	//CJ1~CJ2�ϵ����
	BSP_GPIO_Init(LRUN_PORT,GPIO_Mode_IPD);		//���������б�־
	BSP_GPIO_Init(DHAL_PORT,GPIO_Mode_IPD);		//��ѹ��𱨾�
	BSP_GPIO_Init(HIAL_PORT,GPIO_Mode_IPD);		//��ѹ�ŵ����
	BSP_GPIO_Init(IGBTBAL_PORT,GPIO_Mode_IPD);	//IGBTB����
	BSP_GPIO_Init(IGBTAAL_PORT,GPIO_Mode_IPD);	//IGBTA����
	BSP_GPIO_Init(TIAL_PORT,GPIO_Mode_IPD);		//ԭ�ߵ�������
	BSP_GPIO_Init(IDCAL_PORT,GPIO_Mode_IPD);	//IDC��������
	BSP_GPIO_Init(QSALARM_PORT,GPIO_Mode_IPD);	//ȱ�౨��
	BSP_GPIO_Init(HJSH_PORT,GPIO_Mode_IPD);		//�����ź�
	BSP_GPIO_Init(JTJC_PORT,GPIO_Mode_IPD);		//��ͣ����
	BSP_GPIO_Init(IN_ALARM_PORT,GPIO_Mode_IPD);	//ϵͳ����
	BSP_GPIO_Init(PVDD_PORT,GPIO_Mode_IPD);		//���籨��
	
	//��IO�����ý�������
	BSP_GPIO_LockConfig(LKEN_PORT);
	BSP_GPIO_LockConfig(DCVCHK_PORT);
	BSP_GPIO_LockConfig(PBLE3_PORT);
	BSP_GPIO_LockConfig(LE0_PORT);
	BSP_GPIO_LockConfig(LE1_PORT);
	BSP_GPIO_LockConfig(LE2_PORT);
	BSP_GPIO_LockConfig(LE3_PORT);
	BSP_GPIO_LockConfig(OPIN1_PORT);
	BSP_GPIO_LockConfig(DJOPEN_PORT);
	BSP_GPIO_LockConfig(O2CJ3_PORT);
	BSP_GPIO_LockConfig(STOPCJ3_PORT);
	BSP_GPIO_LockConfig(UPEDCJ2_PORT);
	BSP_GPIO_LockConfig(STARTS_PORT);
	BSP_GPIO_LockConfig(STOP_P_PORT);
	BSP_GPIO_LockConfig(FYKL_PORT);
	BSP_GPIO_LockConfig(STANDBY_PORT);
	BSP_GPIO_LockConfig(FD_PORT);
	BSP_GPIO_LockConfig(PLBJ_PORT);
	BSP_GPIO_LockConfig(PHBJ_PORT);
	BSP_GPIO_LockConfig(HJSHDL_PORT);
	BSP_GPIO_LockConfig(STOPCS_PORT);
	BSP_GPIO_LockConfig(ALARM_PORT);
	BSP_GPIO_LockConfig(RESET_SYS_PORT);
	BSP_GPIO_LockConfig(CJ3OK_PORT);
	BSP_GPIO_LockConfig(CJ12OK_PORT);
	BSP_GPIO_LockConfig(LRUN_PORT);
	BSP_GPIO_LockConfig(DHAL_PORT);
	BSP_GPIO_LockConfig(HIAL_PORT);
	BSP_GPIO_LockConfig(IGBTBAL_PORT);
	BSP_GPIO_LockConfig(IGBTAAL_PORT);
	BSP_GPIO_LockConfig(TIAL_PORT);
	BSP_GPIO_LockConfig(IDCAL_PORT);
	BSP_GPIO_LockConfig(QSALARM_PORT);
	BSP_GPIO_LockConfig(HJSH_PORT);
	BSP_GPIO_LockConfig(JTJC_PORT);
	BSP_GPIO_LockConfig(IN_ALARM_PORT);
	BSP_GPIO_LockConfig(PVDD_PORT);
	
}

//�������˿�׼��
void driver_port_detection(void){
	STANDBY = 1;	//��ֹ3875�������
	STOPCS = 1;
	
	PBLE3 = 1;	//���α���LE3	
	
	RESET_SYS = 1;		//Ӳ��ϵͳ��λ 0.1s
	delay_ms(100);
	RESET_SYS = 0;
}

//�������
void pulse_output(volatile unsigned long *pulase_port,uint32_t pulse_time){
	*pulase_port = 1;
	vTaskDelay(pulse_time);
	*pulase_port = 0;
}
//��������
void error_set(volatile unsigned long *error_port,uint32_t error_code){
	if(!(*error_port)) get_controlData()->error_sta &= ~error_code;
	else get_controlData()->error_sta |= error_code;
}

