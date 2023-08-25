#ifndef __PARAMETERS_H
#define __PARAMETERS_H

//保存默认参数  
#define DEAFULT_POWER	2500			//额定功率 25.00kw
#define DEAFULT_MAX_POW 2500			//线速最大功率 25.00kw(100%)
#define DEAFULT_LOW_POWER 100			//设定的最小功率 1.00kw
#define DEAFULT_MAX_SPEED 30000			//速比模式下的最大线速300.00 m/min

//滚筒参数，计算当前线速(m/min)=频率*脉冲线长度*60
#define DEAFULT_ROLLER_DIAMETER_LOACAL 40000		//本地滚轴直径0.40000M(400.00mm)  需要求出滚轴周长=滚轴直径*pi
#define DEAFULT_ROLLER_PULSE_LOCAL 50				//本地滚轴每周脉冲数50  计算出1个脉冲的线长度=滚轴周长/滚轴每周脉冲数
#define DEAFULT_ROLLER_DIAMETER_EXTERNAL 30000		//外地滚轴直径0.30000M(300.00mm)  需要求出滚轴周长=滚轴直径*pi
#define DEAFULT_ROLLER_PULSE_EXTERNAL 512			//外部滚轴每周脉冲数512 计算出1个脉冲的线长度=滚轴周长/滚轴每周脉冲数

#define DEAFULT_SPD_UP 50				//设定达速0.50m/min，需要再除以60得到m/s

//放电宽度
#define DEAFULT_ROLLER_WIDTH	500		//滚轴宽度5.00M即500cm

//定时器10ms
#define DEAFULT_SET_SPD_DELAY_TIME 300		//设定线速状态下的延时停止放电时间 3.00s
#define DEAFULT_SET_PULSE_DELAY_TIME 350	//设定脉冲状态下的延时停止放电时间 3.50s
#define DEAFULT_SET_REMAIN_DELAY_TIME 300	//设定脉冲采集更新时间 3.00s
#define DEAFULT_SET_LINE_DELAY_TIME 300		//设定线控状态下的延时停止放电时间 3.00s

//工作模式
#define DEAFULT_SET_STATE 0x00		//速度信号 [0本地 1外部脉冲 2外部0-10V 3外部4-20mA] 控制模式 [0手动 1联动] 功率模式 [0手动给定 1功率密度 2速比控制]

#define DEAFULT_MANUAL_POWER 0			//手动给定功率 0.00kw
#define DEAFULT_POWER_DENSITY 25	//功率密度kw/m2

#define DEAFULT_SET_DELAY_LENGTH1 	1000 		//设定换卷延迟放电长度1 10.00m
#define DEAFULT_SET_DELAY_LENGTH2	1000		//设定换卷延迟放电长度2 10.00m

#define DEAFULT_SET_PWR_HI_WARN 2500
#define DEAFULT_SET_PWR_LO_WARN 0

//运行时间
#define DEAFULT_SEC 0
#define DEAFULT_HOR 0
#define DEAFULT_DAY1 100
#define DEAFULT_DAY2 250
#define DEAFULT_DAY3 365

//密码
#define DEAFULT_PASSWORD1 114514
#define DEAFULT_PASSWORD2 1919
#define DEAFULT_PASSWORD3 810

#define DEAFULT_VERIFY_PASSWORD 0
//pulse
#define DEAFULT_PULSE_CORONA 0

#endif
