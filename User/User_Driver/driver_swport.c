#include "driver.h"
#include "app.h"
//IO口初始化
void driver_port_Init(void){
	
	BSP_GPIO_Init(LKEN_PORT,GPIO_Mode_Out_PP);			//允许线控使能光耦互锁
	BSP_GPIO_Init(RESET_DH_PORT,GPIO_Mode_Out_PP);		//打火复位
	BSP_GPIO_Init(DCVCHK_PORT,GPIO_Mode_IPD);	//低压电源检测
	
	//错误代码LE2:LE1:LE0,LE3为1时有效
	BSP_GPIO_Init(PBLE3_PORT,GPIO_Mode_Out_PP);		//屏蔽LE3
	BSP_GPIO_Init(LE0_PORT,GPIO_Mode_IPD);	//LE0
	BSP_GPIO_Init(LE1_PORT,GPIO_Mode_IPD);	//LE1
	BSP_GPIO_Init(LE2_PORT,GPIO_Mode_IPD);	//LE2
	BSP_GPIO_Init(LE3_PORT,GPIO_Mode_IPD);	//LE3报警有效位，LE3为1时LE0~2有效
	
	BSP_GPIO_Init(OPIN1_PORT,GPIO_Mode_IPD);//光耦输入1
	
	//脉冲信号输出
	BSP_GPIO_Init(DJOPEN_PORT,GPIO_Mode_Out_PP);	//打开电极			0.1s脉冲
	BSP_GPIO_Init(DJCLOSE_PORT,GPIO_Mode_Out_PP);	//关闭电极			0.1s脉冲
	BSP_GPIO_Init(O2CJ3_PORT,GPIO_Mode_Out_PP);		//启动臭氧风机 		0.1s脉冲
	BSP_GPIO_Init(STOPCJ3_PORT,GPIO_Mode_Out_PP);	//启动CJ1上电		0.1s脉冲
	BSP_GPIO_Init(UPEDCJ2_PORT,GPIO_Mode_Out_PP);	//启动CJ2			50ms脉冲
	BSP_GPIO_Init(STARTS_PORT,GPIO_Mode_Out_PP);	//启动系统功率电源	0.1s脉冲
	BSP_GPIO_Init(STOP_P_PORT,GPIO_Mode_Out_PP);	//关闭功率电源停机	0.1s脉冲
	
	//常态输出
	BSP_GPIO_Init(FYKL_PORT,GPIO_Mode_Out_PP);		//屏蔽负压开关
	BSP_GPIO_Init(STANDBY_PORT,GPIO_Mode_Out_PP);	//禁止3875移相输出
	BSP_GPIO_Init(FD_PORT,GPIO_Mode_Out_PP);		//已放电,启动放电继电器
	BSP_GPIO_Init(PLBJ_PORT,GPIO_Mode_Out_PP);		//CPU输出功率低报警
	BSP_GPIO_Init(PHBJ_PORT,GPIO_Mode_Out_PP);		//CPU输出功率高报警
	BSP_GPIO_Init(HJSHDL_PORT,GPIO_Mode_Out_PP);	//换卷动作延时
	BSP_GPIO_Init(STOPCS_PORT,GPIO_Mode_Out_PP);	//CPU输出禁止3875输出波形
	BSP_GPIO_Init(ALARM_PORT,GPIO_Mode_Out_PP);		//报警执行停功率电源，停机
	BSP_GPIO_Init(RESET_SYS_PORT,GPIO_Mode_Out_PP);	//CPU输出系统复位信号
	
	//信号输入检测
	BSP_GPIO_Init(CJ3OK_PORT,GPIO_Mode_IPD);	//臭氧风机检测开关
	BSP_GPIO_Init(PUPOK_PORT,GPIO_Mode_IPD);	//初上电检测，达到80%启动CJ2
	BSP_GPIO_Init(CJ12OK_PORT,GPIO_Mode_IPD);	//CJ1~CJ2上电完成
	BSP_GPIO_Init(LRUN_PORT,GPIO_Mode_IPD);		//生产线运行标志
	BSP_GPIO_Init(DHAL_PORT,GPIO_Mode_IPD);		//高压打火报警
	BSP_GPIO_Init(HIAL_PORT,GPIO_Mode_IPD);		//高压放电过流
	BSP_GPIO_Init(IGBTBAL_PORT,GPIO_Mode_IPD);	//IGBTB过流
	BSP_GPIO_Init(IGBTAAL_PORT,GPIO_Mode_IPD);	//IGBTA过流
	BSP_GPIO_Init(TIAL_PORT,GPIO_Mode_IPD);		//原边电流过流
	BSP_GPIO_Init(IDCAL_PORT,GPIO_Mode_IPD);	//IDC过流报警
	BSP_GPIO_Init(QSALARM_PORT,GPIO_Mode_IPD);	//缺相报警
	BSP_GPIO_Init(HJSH_PORT,GPIO_Mode_IPD);		//换卷信号
	BSP_GPIO_Init(JTJC_PORT,GPIO_Mode_IPD);		//急停报警
	BSP_GPIO_Init(IN_ALARM_PORT,GPIO_Mode_IPD);	//系统报警
	BSP_GPIO_Init(PVDD_PORT,GPIO_Mode_IPD);		//掉电报警
	
	//对IO口配置进行上锁
	BSP_GPIO_LockConfig(LKEN_PORT);
	BSP_GPIO_LockConfig(RESET_DH_PORT);
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
	
	//初始化管脚输出
	RESET_DH = 1;	//防止打火报警重复复位
	
}

//冷启动端口准备
void driver_port_detection(void){
	STANDBY = 1;	//禁止3875所有输出
	STOPCS = 1;
	
	PBLE3 = 1;	//屏蔽报警LE3	
	
//	STOP_P = 1;			//关闭功率电源 0.1s
	RESET_SYS = 1;		//硬件系统复位 0.1s
	delay_ms(100);
//	STOP_P = 0;
	RESET_SYS = 0;
	
	STOPCS = 0;	//3875输出波形建立驱动电压
}

//脉冲输出(高电平)
void pulse_outputHigh(volatile unsigned long *pulase_port,uint32_t pulse_time){
	*pulase_port = 1;
	vTaskDelay(pulse_time);
	*pulase_port = 0;
}
//脉冲输出(低电平)
void pulse_outputLow(volatile unsigned long *pulase_port,uint32_t pulse_time){
	*pulase_port = 0;
	vTaskDelay(pulse_time);
	*pulase_port = 1;
}
//错误设置 错误端口 错误代码 端口错误时的电平状态 0 或 1
void error_set(volatile unsigned long *error_port,uint32_t error_code,uint8_t state){
	if(*error_port == state) get_controlData()->error_sta |= error_code;
	else get_controlData()->error_sta &= ~error_code;		//如果该端口无错误，则清空错误报警
}

