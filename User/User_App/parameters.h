#ifndef __PARAMETERS_H
#define __PARAMETERS_H

//����Ĭ�ϲ���  
#define DEAFULT_POWER	2500			//����� 25.00kw
#define DEAFULT_MAX_POW 2500			//��������� 25.00kw(100%)
#define DEAFULT_LOW_POWER 100			//�趨����С���� 1.00kw
#define DEAFULT_MAX_SPEED 30000			//�ٱ�ģʽ�µ��������300.00 m/min

//��Ͳ���������㵱ǰ����(m/min)=Ƶ��*�����߳���*60
#define DEAFULT_ROLLER_DIAMETER_LOACAL 40000		//���ع���ֱ��0.40000M(400.00mm)  ��Ҫ��������ܳ�=����ֱ��*pi
#define DEAFULT_ROLLER_PULSE_LOCAL 50				//���ع���ÿ��������50  �����1��������߳���=�����ܳ�/����ÿ��������
#define DEAFULT_ROLLER_DIAMETER_EXTERNAL 30000		//��ع���ֱ��0.30000M(300.00mm)  ��Ҫ��������ܳ�=����ֱ��*pi
#define DEAFULT_ROLLER_PULSE_EXTERNAL 512			//�ⲿ����ÿ��������512 �����1��������߳���=�����ܳ�/����ÿ��������

#define DEAFULT_SPD_UP 50				//�趨����0.50m/min����Ҫ�ٳ���60�õ�m/s

//�ŵ���
#define DEAFULT_ROLLER_WIDTH	500		//������5.00M��500cm

//��ʱ��10ms
#define DEAFULT_SET_SPD_DELAY_TIME 300		//�趨����״̬�µ���ʱֹͣ�ŵ�ʱ�� 3.00s
#define DEAFULT_SET_PULSE_DELAY_TIME 350	//�趨����״̬�µ���ʱֹͣ�ŵ�ʱ�� 3.50s
#define DEAFULT_SET_REMAIN_DELAY_TIME 300	//�趨����ɼ�����ʱ�� 3.00s
#define DEAFULT_SET_LINE_DELAY_TIME 300		//�趨�߿�״̬�µ���ʱֹͣ�ŵ�ʱ�� 3.00s

//����ģʽ
#define DEAFULT_SET_STATE 0x00		//�ٶ��ź� [0���� 1�ⲿ���� 2�ⲿ0-10V 3�ⲿ4-20mA] ����ģʽ [0�ֶ� 1����] ����ģʽ [0�ֶ����� 1�����ܶ� 2�ٱȿ���]

#define DEAFULT_MANUAL_POWER 0			//�ֶ��������� 0.00kw
#define DEAFULT_POWER_DENSITY 25	//�����ܶ�kw/m2

#define DEAFULT_SET_DELAY_LENGTH1 	1000 		//�趨�����ӳٷŵ糤��1 10.00m
#define DEAFULT_SET_DELAY_LENGTH2	1000		//�趨�����ӳٷŵ糤��2 10.00m

//����ʱ��
#define DEAFULT_SEC 0
#define DEAFULT_HOR 0
#define DEAFULT_DAY1 100
#define DEAFULT_DAY2 250
#define DEAFULT_DAY3 365

//����
#define DEAFULT_PASSWORD1 114514
#define DEAFULT_PASSWORD2 1919
#define DEAFULT_PASSWORD3 810

#define DEAFULT_VERIFY_PASSWORD 0

#endif
