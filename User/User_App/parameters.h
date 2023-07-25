#ifndef __PARAMETERS_H
#define __PARAMETERS_H

//����Ĭ�ϲ���  
#define DEAFULT_SET_STATE 0x00		//�ٶ��ź� 0���� 1�ⲿ���� 2�ⲿ0-10V 3�ⲿ4-20mA ����ģʽ 0�ֶ� 1���� ����ģʽ 0�ֶ����� 1�����ܶ� 2�ٱȿ���

#define DEAFULT_POWER_DENSITY 25	//�����ܶ�kw/m2
#define DEAFULT_MAX_SPEED 300			//�ٱ�ģʽ�µ��������
//����С�������λ*100,ʵ��ʹ����Ҫ*0.01f
#define DEAFULT_POWER	2500			//����� 25.00kw
#define DEAFULT_SPD_MAX_POW 2500			//�ٱ�ģʽ�µ������ 25.00kw(100%)
#define DEAFULT_SPD_MIN_POW 250				//�ٱ�ģʽ�µ���С���� 2.50kw(10%)
#define DEAFULT_MANUAL_POWER 0			//�ֶ��������� 0kw
#define DEAFULT_LOW_POWER 100			//�趨����С���� 1.00kw
#define DEAFULT_SET_DELAY_LENGTH1 	1000 		//�趨�����ӳٷŵ糤��1 10.00m
#define DEAFULT_SET_DELAY_LENGTH2	1000		//�趨�����ӳٷŵ糤��2 10.00m
//��Ͳ���������㵱ǰ����(m/min)=Ƶ��*�����߳���*60
#define DEAFULT_SPD_UP 50				//�趨����0.5m/min����Ҫ�ٳ���60�õ�m/s
#define DEAFULT_ROLLER_DIAMETER_LOACAL 40		//���ع���ֱ��0.4M  ��Ҫ��������ܳ�=����ֱ��*pi
#define DEAFULT_ROLLER_DIAMETER_EXTERNAL 30		//��ع���ֱ��0.3M  ��Ҫ��������ܳ�=����ֱ��*pi
#define DEAFULT_ROLLER_WIDTH	500		//������5.00M

#define DEAFULT_ROLLER_PULSE_LOCAL 50		//���ع���ÿ��������50  �����1��������߳���=�����ܳ�/����ÿ��������
#define DEAFULT_ROLLER_PULSE_EXTERNAL 512	//�ⲿ����ÿ��������512 �����1��������߳���=�����ܳ�/����ÿ��������

#define DEAFULT_SET_SPD_DELAY_TIME 300		//�趨����״̬�µ���ʱֹͣ�ŵ�ʱ�� 3s
#define DEAFULT_SET_PULSE_DELAY_TIME 350	//�趨����״̬�µ���ʱֹͣ�ŵ�ʱ�� 3.5s
#define DEAFULT_SET_REMAIN_DELAY_TIME 300	//�趨����ɼ�����ʱ�� 3s
#define DEAFULT_SET_LINE_DELAY_TIME 300		//�趨�߿�״̬�µ���ʱֹͣ�ŵ�ʱ�� 3s


#endif
