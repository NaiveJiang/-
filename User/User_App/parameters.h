#ifndef __PARAMETERS_H
#define __PARAMETERS_H

//保存默认参数  
#define DEAFULT_CONTROL_MODE 0		//控制模式 0手动 1联动
#define DEAFULT_POWER_MODE 0		//功率模式 0手动给定 1功率密度 2速比控制
#define DEAFULT_SPD_SIGNAL 0		//速度信号 0本地 1外部

#define DEAFULT_POWER_DENSITY 25	//功率密度kw/m2
#define DEAFULT_MAX_SPEED 300			//速比模式下的最大线速
//保留小数点后两位*100,实际使用需要*0.01f
#define DEAFULT_POWER	2500			//额定功率 25.00kw
#define DEAFULT_SPD_MAX 2500			//速比模式下的最大功率 25.00kw(100%)
#define DEAFULT_SPD_MIN 250				//速比模式下的最小功率 2.50kw(10%)
#define DEAFULT_MANUAL_POWER 0			//手动给定功率 0kw
#define DEAFULT_LOW_POWER 100			//设定的最小功率 1.00kw
//滚筒参数，计算当前线速(m/min)=频率*脉冲线长度*60
#define DEAFULT_SPD_UP 50				//设定达速0.5m/min，需要再除以60得到m/s
#define DEAFULT_ROLLER_DIAMETER_LOACAL 40		//本地滚轴直径0.4M  需要求出滚轴周长=滚轴直径*pi
#define DEAFULT_ROLLER_DIAMETER_EXTERNAL 30		//外地滚轴直径0.3M  需要求出滚轴周长=滚轴直径*pi
#define DEAFULT_ROLLER_WIDTH	5		//滚轴宽度5M

#define DEAFULT_ROLLER_PULSE_LOCAL 50		//本地滚轴每周脉冲数50  计算出1个脉冲的线长度=滚轴周长/滚轴每周脉冲数
#define DEAFULT_ROLLER_PULSE_EXTERNAL 512	//外部滚轴每周脉冲数512 计算出1个脉冲的线长度=滚轴周长/滚轴每周脉冲数

#define DEAFULT_SET_SPD_DELAY_TIME 300		//设定线速状态下的延时停止放电时间 3s
#define DEAFULT_SET_PULSE_DELAY_TIME 350	//设定脉冲状态下的延时停止放电时间 3.5s
#define DEAFULT_SET_REMAIN_DELAY_TIME 300	//设定脉冲采集更新时间 3s
#define DEAFULT_SET_LINE_DELAY_TIME 300		//设定线控状态下的延时停止放电时间 3s
#define DEAFULT_SET_DELAY_LENGTH1 	10 		//设定换卷延迟放电长度1 10m
#define DEAFULT_SET_DELAY_LENGTH2	10		//设定换卷延迟放电长度2 10m

#endif
