#ifndef __PARAMETERS_H
#define __PARAMETERS_H

//保存默认参数  
#define DEAFULT_CONTROL_MODE 0		//控制模式 0手动 1联动
#define DEAFULT_POWER_MODE 0		//功率模式 0手动给定 1功率密度 2速比控制
#define DEAFULT_SPD_SIGNAL 0		//速度信号 0本地 1外部

#define DEAFULT_POWER_DENSITY 25	//功率密度kw/m2
//保留小数点后两位*100,实际使用需要*0.01f
#define DEAFULT_POWER	2500			//额定功率 25.00kw
#define DEAFULT_SPD_MAX 2500			//速比最大 25.00kw(100%)
#define DEAFULT_SPD_MIN 250				//速比最小 2.50kw(10%)
#define DEAFULT_MANUAL_POWER 0			//手动给定功率 0kw
//滚筒参数，计算当前线速(m/min)=频率*脉冲线长度*60
#define DEAFULT_SPD_UP_MAX 5000			//最高达速50m/min，需要再除以60得到m/s
#define DEAFULT_SPD_UP_MIN 50			//最低达速0.5m/min，需要再除以60得到m/s
#define DEAFULT_ROLLER_DIAMETER 40		//滚轴直径0.4M  需要求出滚轴周长=滚轴直径*pi
#define DEAFULT_ROLLER_PULSE 50			//滚轴每周脉冲数50  计算出1个脉冲的线长度=滚轴周长/滚轴每周脉冲数


#endif
