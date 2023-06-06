#ifndef __DRIVER_SWPORT_H
#define __DRIVER_SWPORT_H

#include "bsp.h"

#define LKEN_PORT BSP_GPIOA8		//�����߿�ʹ�ܹ����
#define LKEN PAout(8)
#define RESET_DH_PORT BSP_GPIOA15	//���λ	0��Ч
#define RESET_DH PAout(15)
#define DCVCHK_PORT BSP_GPIOB9		//��ѹ��Դ���
#define DCVCHK PBin(9)

//�������LE2:LE1:LE0
#define PBLE3_PORT BSP_GPIOB12		//����LE3
#define PBLE3 PBout(12)
#define LE0_PORT BSP_GPIOB13		//LE0
#define LE0 PBin(13)
#define LE1_PORT BSP_GPIOB14		//LE1
#define LE1 PBin(14)
#define LE2_PORT BSP_GPIOB15		//LE2
#define LE2 PBin(15)
#define LE3_PORT BSP_GPIOE12		//LE3������Чλ��LE3Ϊ1ʱLE0~2��Ч
#define LE3 PEin(12)

#define OPIN1_PORT BSP_GPIOC7		//��������1
#define OPIN1 PCin(7)

//�����ź����
#define DJOPEN_PORT BSP_GPIOB5		//�򿪵缫		0.1s
#define DJOPEN PBout(5)
#define DJCLOSE_PORT BSP_GPIOB8		//�رյ缫		0.1s
#define DJCLOSE PBout(8)
#define O2CJ3_PORT BSP_GPIOD0		//�����������	0.1s
#define O2CJ3 PDout(0)
#define STOPCJ3_PORT BSP_GPIOD1		//�رճ������	0.1s
#define STOPCJ3 PDout(1)
#define UPEDCJ2_PORT BSP_GPIOD3		//����CJ2		50ms
#define UPEDCJ2 PDout(3)
#define STARTS_PORT BSP_GPIOD5		//����ϵͳ���ʵ�Դ��ͬʱ����CJ1
#define STARTS PDout(5)
#define STOP_P_PORT BSP_GPIOD6		//�رչ��ʵ�Դͣ��
#define STOP_P PDout(6)
#define RESET_SYS_PORT BSP_GPIOD14	//CPU���ϵͳ��λ�ź�
#define RESET_SYS PDout(14)


//��̬���
#define FYKL_PORT BSP_GPIOD4		//���θ�ѹ����
#define FYKL PDout(4)
#define STANDBY_PORT BSP_GPIOD7		//��ֹ3875�������
#define STANDBY PDout(7)
#define FD_PORT BSP_GPIOD8			//�ѷŵ�,�����ŵ�̵���
#define FD PDout(8)
#define PLBJ_PORT BSP_GPIOD9		//CPU������ʵͱ���
#define PLBJ PDout(9)
#define PHBJ_PORT BSP_GPIOD10		//CPU������ʸ߱���
#define PHBJ PDout(10)
#define HJSHDL_PORT BSP_GPIOD11		//��������ʱ
#define HJSHDL PDout(11)
#define STOPCS_PORT BSP_GPIOD12		//CPU�����ֹ3875�������
#define STOPCS PDout(12)
#define ALARM_PORT BSP_GPIOD13		//����ִ��ͣ���ʵ�Դ��ͣ��
#define ALARM PDout(13)

//�ź�������
#define CJ3OK_PORT BSP_GPIOE0		//���������⿪��
#define CJ3OK PEin(0)
#define PUPOK_PORT BSP_GPIOE1		//���ϵ��⣬�ﵽ80%����CJ2
#define PUPOK PEin(1)
#define CJ12OK_PORT BSP_GPIOE2		//CJ1~CJ2�ϵ����
#define CJ12OK PEin(2)
#define LRUN_PORT BSP_GPIOE3		//���������б�־
#define LRUN PEin(3)
#define DHAL_PORT BSP_GPIOE4		//��ѹ��𱨾�
#define DHAL PEin(4)
#define HIAL_PORT BSP_GPIOE5		//��ѹ�ŵ����
#define HIAL PEin(5)
#define IGBTBAL_PORT BSP_GPIOE6		//IGBTB����
#define IGBTBAL PEin(6)
#define IGBTAAL_PORT BSP_GPIOE7		//IGBTA����
#define IGBTAAL PEin(7)
#define TIAL_PORT BSP_GPIOE8		//ԭ�ߵ�������
#define TIAL PEin(8)
#define IDCAL_PORT BSP_GPIOE9		//IDC��������
#define IDCAL PEin(9)
#define QSALARM_PORT BSP_GPIOE10	//ȱ�౨��
#define QSALARM PEin(10)
#define HJSH_PORT BSP_GPIOE11		//�����ź�
#define HJSH PEin(11)
#define JTJC_PORT BSP_GPIOE13		//��ͣ����
#define JTJC PEin(13)
#define IN_ALARM_PORT BSP_GPIOE14	//ϵͳ����
#define IN_ALARM PEin(14)
#define PVDD_PORT BSP_GPIOE15		//���籨��
#define PVDD PEin(15)

void driver_port_Init(void);
void driver_port_detection(void);
void pulse_outputHigh(volatile unsigned long *pulase_port,uint32_t pulse_time);
void pulse_outputLow(volatile unsigned long *pulase_port,uint32_t pulse_time);
void error_set(volatile unsigned long *error_port,uint32_t error_code,uint8_t state);
#endif

