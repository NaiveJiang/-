/*
*********************************************************************************************************
*
*	模块名称 : 主程序模块。
*	文件名称 : main.c
*	版    本 : V1.0
*	说    明 : 本实验主要学习FreeRTOS的移植
*              实验目的：
*                 1. 学习FreeRTOS的移植。
*              实验内容：
*                 1. 创建了如下四个任务：
*                    vTaskTaskUserIF 任务: 接口消息处理，这里用作LED闪烁	
*                    vTaskLED        任务: LED闪烁
*                    vTaskMsgPro     任务: 信息处理，这里是用作LED闪烁
*                    vTaskStart      任务: 启动任务，也就是最高优先级任务，这里用作LED闪烁
*              注意事项：
*                 1. 本实验推荐使用串口软件SecureCRT，要不串口打印效果不整齐。此软件在
*                    V4开发板光盘里面有。
*                 2. 务必将编辑器的缩进参数和TAB设置为4来阅读本文件，要不代码显示不整齐。
*
*	修改记录 :
*		版本号    日期         作者            说明
*       V1.0    2016-03-15   Eric2013    1. ST固件库到V3.6.1版本
*                                        2. BSP驱动包V1.2
*                                        3. FreeRTOS版本V8.2.3
*
*	Copyright (C), 2016-2020, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/
#include "includes.h"

/*
*********************************************************************************************************
*	函 数 名: main
*	功能说明: 标准c程序入口。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
RCC_ClocksTypeDef get_rcc_clock;
void app_Init(void)
{
	
    
	
	bsp_Init();
	driver_Init();
	app_ParameterInit();
	app_inputTaskInit();     //2
	RCC_GetClocksFreq(&get_rcc_clock);
			//4
								//删除当前任务
}
int main(void)
{
	
	/* 
	  在启动调度前，为了防止初始化STM32外设时有中断服务程序执行，这里禁止全局中断(除了NMI和HardFault)。
	  这样做的好处是：
	  1. 防止执行的中断服务程序中有FreeRTOS的API函数。
	  2. 保证系统正常启动，不受别的中断影响。
	  3. 关于是否关闭全局中断，大家根据自己的实际情况设置即可。
	  在移植文件port.c中的函数prvStartFirstTask中会重新开启全局中断。通过指令cpsie i开启，__set_PRIMASK(1)
	  和cpsie i是等效的。
     */
	__set_PRIMASK(0);  //开启全局中断
	
	/* 硬件初始化 */
	//bsp_Init(); 
	
	/* 创建任务 */
	app_Init();
//	xTaskCreate(app_Init,"INIT",INIT_STACK_SIZE,NULL,INIT_PRIORITY,NULL);
	
    /* 启动调度，开始执行任务 */
   vTaskStartScheduler();

	/* 
	  如果系统正常启动是不会运行到这里的，运行到这里极有可能是用于定时器任务或者空闲任务的
	  heap空间不足造成创建失败，此要加大FreeRTOSConfig.h文件中定义的heap大小：
	  #define configTOTAL_HEAP_SIZE	      ( ( size_t ) ( 17 * 1024 ) )
	*/
//	while(1);
}

