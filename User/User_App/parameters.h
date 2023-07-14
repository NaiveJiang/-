#ifndef __PARAMETERS_H
#define __PARAMETERS_H

//����Ĭ�ϲ���  
#define DEAFULT_CONTROL_MODE 0		//����ģʽ 0�ֶ� 1����
#define DEAFULT_POWER_MODE 0		//����ģʽ 0�ֶ����� 1�����ܶ� 2�ٱȿ���
#define DEAFULT_SPD_SIGNAL 0		//�ٶ��ź� 0���� 1�ⲿ

#define DEAFULT_POWER_DENSITY 25	//�����ܶ�kw/m2
#define DEAFULT_MAX_SPEED 300			//�ٱ�ģʽ�µ��������
//����С�������λ*100,ʵ��ʹ����Ҫ*0.01f
#define DEAFULT_POWER	2500			//����� 25.00kw
#define DEAFULT_SPD_MAX 2500			//�ٱ�ģʽ�µ������ 25.00kw(100%)
#define DEAFULT_SPD_MIN 250				//�ٱ�ģʽ�µ���С���� 2.50kw(10%)
#define DEAFULT_MANUAL_POWER 0			//�ֶ��������� 0kw
#define DEAFULT_LOW_POWER 100			//�趨����С���� 1.00kw
//��Ͳ���������㵱ǰ����(m/min)=Ƶ��*�����߳���*60
#define DEAFULT_SPD_UP 50				//�趨����0.5m/min����Ҫ�ٳ���60�õ�m/s
#define DEAFULT_ROLLER_DIAMETER_LOACAL 40		//���ع���ֱ��0.4M  ��Ҫ��������ܳ�=����ֱ��*pi
#define DEAFULT_ROLLER_DIAMETER_EXTERNAL 30		//��ع���ֱ��0.3M  ��Ҫ��������ܳ�=����ֱ��*pi
#define DEAFULT_ROLLER_WIDTH	5		//������5M

#define DEAFULT_ROLLER_PULSE_LOCAL 50		//���ع���ÿ��������50  �����1��������߳���=�����ܳ�/����ÿ��������
#define DEAFULT_ROLLER_PULSE_EXTERNAL 512	//�ⲿ����ÿ��������512 �����1��������߳���=�����ܳ�/����ÿ��������

#define DEAFULT_SET_SPD_DELAY_TIME 300		//�趨����״̬�µ���ʱֹͣ�ŵ�ʱ�� 3s
#define DEAFULT_SET_PULSE_DELAY_TIME 350	//�趨����״̬�µ���ʱֹͣ�ŵ�ʱ�� 3.5s
#define DEAFULT_SET_REMAIN_DELAY_TIME 300	//�趨����ɼ�����ʱ�� 3s
#define DEAFULT_SET_LINE_DELAY_TIME 300		//�趨�߿�״̬�µ���ʱֹͣ�ŵ�ʱ�� 3s
#define DEAFULT_SET_DELAY_LENGTH1 	10 		//�趨�����ӳٷŵ糤��1 10m
#define DEAFULT_SET_DELAY_LENGTH2	10		//�趨�����ӳٷŵ糤��2 10m

#endif
