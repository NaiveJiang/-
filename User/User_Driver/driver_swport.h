#ifndef __DRIVER_SWPORT_H
#define __DRIVER_SWPORT_H

#include "bsp.h"

#define LKEN_PORT BSP_GPIOA8		//允许线控使能光耦互锁
#define LKEN PAout(8)
#define DCVCHK_PORT BSP_GPIOB9		//低压电源检测
#define DCVCHK PBin(9)

//错误代码LE2:LE1:LE0
#define PBLE3_PORT BSP_GPIOB12		//屏蔽LE3
#define PBLE3 PBout(12)
#define LE0_PORT BSP_GPIOB13		//LE0
#define LE0 PBin(13)
#define LE1_PORT BSP_GPIOB14		//LE1
#define LE1 PBin(14)
#define LE2_PORT BSP_GPIOB15		//LE2
#define LE2 PBin(15)
#define LE3_PORT BSP_GPIOE12		//LE3报警有效位，LE3为1时LE0~2有效
#define LE3 PEin(12)

#define OPIN1_PORT BSP_GPIOC7		//光耦输入1
#define OPIN1 PCin(7)

//脉冲信号输出
#define DJOPEN_PORT BSP_GPIOB5		//打开电极		0.1s
#define DJOPEN PBout(5);
#define DJCLOSE_PORT BSP_GPIOB8		//关闭电极		0.1s
#define DJCLOSE PBout(8);
#define O2CJ3_PORT BSP_GPIOD0		//启动臭氧风机	0.1s
#define O2CJ3 PDout(0)
#define STOPCJ3_PORT BSP_GPIOD1		//关闭臭氧风机	0.1s
#define STOPCJ3 PDout(1)
#define UPEDCJ2_PORT BSP_GPIOD3		//启动CJ2		50ms
#define UPEDCJ2 PDout(3)
#define STARTS_PORT BSP_GPIOD5		//启动系统功率电源，同时启动CJ1
#define STARTS PDout(5)
#define STOP_P_PORT BSP_GPIOD6		//关闭功率电源停机
#define STOP_P PDout(6)
#define RESET_SYS_PORT BSP_GPIOD14	//CPU输出系统复位信号
#define RESET_SYS PDout(14)


//常态输出
#define FYKL_PORT BSP_GPIOD4		//屏蔽负压开关
#define FYKL PDout(4)
#define STANDBY_PORT BSP_GPIOD7		//禁止3875移相输出
#define STANDBY PDout(7)
#define FD_PORT BSP_GPIOD8			//已放电,启动放电继电器
#define FD PDout(8)
#define PLBJ_PORT BSP_GPIOD9		//CPU输出功率低报警
#define PLBJ PDout(9)
#define PHBJ_PORT BSP_GPIOD10		//CPU输出功率高报警
#define PHBJ PDout(10)
#define HJSHDL_PORT BSP_GPIOD11		//换卷动作延时
#define HJSHDL PDout(11)
#define STOPCS_PORT BSP_GPIOD12		//CPU输出禁止3875输出波形
#define STOPCS PDout(12)
#define ALARM_PORT BSP_GPIOD13		//报警执行停功率电源，停机
#define ALARM PDout(13)

//信号输入检测
#define CJ3OK_PORT BSP_GPIOE0		//臭氧风机检测开关
#define CJ3OK PEin(0)
#define PUPOK_PORT BSP_GPIOE1		//初上电检测，达到80%启动CJ2
#define PUPOK PEin(1)
#define CJ12OK_PORT BSP_GPIOE2		//CJ1~CJ2上电完成
#define CJ12OK PEin(2)
#define LRUN_PORT BSP_GPIOE3		//生产线运行标志
#define LRUN PEin(3)
#define DHAL_PORT BSP_GPIOE4		//高压打火报警
#define DHAL PEin(4)
#define HIAL_PORT BSP_GPIOE5		//高压放电过流
#define HIAL PEin(5)
#define IGBTBAL_PORT BSP_GPIOE6		//IGBTB过流
#define IGBTBAL PEin(6)
#define IGBTAAL_PORT BSP_GPIOE7		//IGBTA过流
#define IGBTAAL PEin(7)
#define TIAL_PORT BSP_GPIOE8		//原边电流过流
#define TIAL PEin(8)
#define IDCAL_PORT BSP_GPIOE9		//IDC过流报警
#define IDCAL PEin(9)
#define QSALARM_PORT BSP_GPIOE10	//缺相报警
#define QSALARM PEin(10)
#define HJSH_PORT BSP_GPIOE11		//换卷信号
#define HJSH PEin(11)
#define JTJC_PORT BSP_GPIOE13		//急停报警
#define JTJC PEin(13)
#define IN_ALARM_PORT BSP_GPIOE14	//系统报警
#define IN_ALARM PEin(14)
#define PVDD_PORT BSP_GPIOE15		//掉电报警
#define PVDD PEin(15)

void driver_port_Init(void);
void driver_port_detection(void);
void pulse_output(volatile unsigned long *pulase_port,uint32_t pulse_time);
void error_set(volatile unsigned long *error_port,uint32_t error_code);
#endif

