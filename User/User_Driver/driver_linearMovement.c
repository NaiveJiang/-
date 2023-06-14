#include "driver.h"

uint16_t g_usAdcValue;	
__IO uint16_t adc1_raw_value[10][6]; //采集的ADC值
__IO uint16_t adc3_raw_value[10][6]; //采集的ADC值
__IO float average_dis[4];  //距离平均值
adcFilterData_t adcFilters[4]; //滤波结构体
adcFilterData_t speedFilters[4];


/*******************ADC1*******************/
//ADC1结算使能
uint8_t adc1_settlement_sw = 0;
//ADC1采样次数
__IO uint8_t adc1_settlement = 0;
//ADC1单次采样数据
__IO float adc_TBI_DC3V3;//变压器原边电流
__IO float adc_TBV_DC3V3;//变压器原边电压
__IO float adc_POV3V3;//输出功率AD3.3V
__IO float adc_HI_DC3V3;//高压放电电流
__IO float adc_LPV3V3;//生产线控制输出功率电压给定
__IO float adc_TEMP;//散热器温度检测输入
//ADC1采样数组
__IO float adc_full_TBI_DC3V3[SAMP_FULL_COUNT];
__IO float adc_full_TBV_DC3V3[SAMP_FULL_COUNT];
__IO float adc_full_POV3V3[SAMP_FULL_COUNT];
__IO float adc_full_HI_DC3V3[SAMP_FULL_COUNT];
__IO float adc_full_LPV3V3[SAMP_FULL_COUNT];
__IO float adc_full_TEMP[SAMP_FULL_COUNT];
//ADC1滤波采样数据
__IO float adc_filter_TBI_DC3V3;
__IO float adc_filter_TBV_DC3V3;
__IO float adc_filter_POV3V3;
__IO float adc_filter_HI_DC3V3;
__IO float adc_filter_LPV3V3;
__IO float adc_filter_TEMP;
/*******************ADC3*******************/
//ADC3结算使能
uint8_t adc3_settlement_sw = 0;
//ADC3采样次数
__IO uint8_t adc3_settlement = 0;
//ADC3单次采样数据
__IO float adc_IDC_ADC;//IDC电流
__IO float adc_VDC_ADC;//VDC电压
__IO float adc_HV_DC3V3;//高压放电电压
__IO float adc_LSPI3V3;//生产线控输出功率速度电流给定
__IO float adc_LSPV3V3;//生产线控输出功率速度电压给定
__IO float adc_LPIMA3V3;//生产线控制输出功率电流给定
//ADC3采样数组
__IO float adc_full_IDC_ADC[SAMP_FULL_COUNT];
__IO float adc_full_VDC_ADC[SAMP_FULL_COUNT];
__IO float adc_full_HV_DC3V3[SAMP_FULL_COUNT];
__IO float adc_full_LSPI3V3[SAMP_FULL_COUNT];
__IO float adc_full_LSPV3V3[SAMP_FULL_COUNT];
__IO float adc_full_LPIMA3V3[SAMP_FULL_COUNT];
//ADC3滤波采样数据
__IO float adc_filter_IDC_ADC;
__IO float adc_filter_VDC_ADC;
__IO float adc_filter_HV_DC3V3;
__IO float adc_filter_LSPI3V3;
__IO float adc_filter_LSPV3V3;
__IO float adc_filter_LPIMA3V3;

//创建定时器，定时观察ADC值
void drive_ADC_sampling_TIM_init(TIM_TypeDef* SAMP_TIMx){
	BSP_TIM_INT_Init(SAMP_TIMx,SAMP_TIMER_PEROID,SAMP_TIMER_PRESCALER,SAMP_TIMER_PREEMPTIONPRIORITY,SAMP_TIMER_SUBPRIORITY);
}

/*
*********************************************************************************************************
*	函 数 名: drive_ADC1_Configuration
*	功能说明: 配置ADC1, PA6,PA7,PB0,PB1,PC4,PC5作为ADC1通道输入
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void drive_ADC1_Configuration(void){
	ADC_InitTypeDef ADC_InitStructure;
	__IO uint16_t ADCConvertedValue;

	//ADC1 DMA配置
    /* ADC1  DMA1 Channel Config */  
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //使能DMA时钟
 
    DMA_DeInit(DMA1_Channel1);   //将DMA的通道1寄存器重设为缺省值：ADC1连接DMA通道1
    DMA_InitStructure.DMA_PeripheralBaseAddr =  (u32)&ADC1->DR;  //DMA外设ADC基地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&adc1_raw_value;      //DMA内存基地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //内存作为数据传输的目的地
    DMA_InitStructure.DMA_BufferSize = 10*6;  //此值为完整一轮DMA传输的次数
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址不变
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //外设数据位宽度16位，即DMA传输尺寸
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //数据宽度16位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  //工作在循环缓存模式，一轮结束后自动开始下轮传输
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMA通道 x拥有高优先级
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x禁止内存到内存
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);  //根据DMA_InitStruct中参数DMA通道
	DMA_Cmd(DMA1_Channel1, ENABLE);//启动DMA通道
    /* 使能 ADC1 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);

	/* 配置IO口为模拟输入 */
	BSP_GPIO_Init(TBI_DC3V3, GPIO_Mode_AIN);
	BSP_GPIO_Init(TBV_DC3V3, GPIO_Mode_AIN);
	BSP_GPIO_Init(POV3V3, GPIO_Mode_AIN);
	BSP_GPIO_Init(HI_DC3V3, GPIO_Mode_AIN);
	BSP_GPIO_Init(LPV3V3, GPIO_Mode_AIN);
	BSP_GPIO_Init(TEMP, GPIO_Mode_AIN);
	
	/* 配置ADC1, 不用DMA, 用软件触发 */
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 6;
	
	ADC_Init(ADC1, &ADC_InitStructure);

	/* 配置ADC1 规则通道 */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_239Cycles5);		//变压器原边电流					300
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 2, ADC_SampleTime_239Cycles5);		//变压器原边电压					800
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 3, ADC_SampleTime_239Cycles5);		//输出功率AD3.3V检测				45K
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 4, ADC_SampleTime_239Cycles5);		//高压放电电流检测				10A				
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 5, ADC_SampleTime_239Cycles5);	//生产线控制输出功率电压给定			40K
	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 6, ADC_SampleTime_239Cycles5);	//散热器温度检测输入					85
	

	/* 使能ADC1 DMA功能 */
	ADC_DMACmd(ADC1, ENABLE);

	/* 使能 ADC1 */
	ADC_Cmd(ADC1, ENABLE);

	/* 使能ADC1 复位校准寄存器 */
	ADC_ResetCalibration(ADC1);
	/* 检查ADC1的复位寄存器 */
	while(ADC_GetResetCalibrationStatus(ADC1));

	/* 启动ADC1校准 */
	ADC_StartCalibration(ADC1);
	/* 检查校准是否结束 */
	while(ADC_GetCalibrationStatus(ADC1));

	/* 软件启动ADC转换 */
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/*
*********************************************************************************************************
*	函 数 名: drive_ADC3_Configuration
*	功能说明: 配置ADC3, PA0,PA1,PC0~PC3作为ADC3通道输入
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void drive_ADC3_Configuration(void){
	ADC_InitTypeDef ADC_InitStructure;
	__IO uint16_t ADCConvertedValue;

	//ADC3 DMA配置
    /* ADC3  DMA2 Channel Config */  
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE); //使能DMA时钟
 
    DMA_DeInit(DMA2_Channel5);   //将DMA的通道1寄存器重设为缺省值：ADC1连接DMA通道1
    DMA_InitStructure.DMA_PeripheralBaseAddr =  (u32)&ADC3->DR;  //DMA外设ADC基地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&adc3_raw_value;      //DMA内存基地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //内存作为数据传输的目的地
    DMA_InitStructure.DMA_BufferSize = 10*6;  //此值为完整一轮DMA传输的次数
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址不变
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //外设数据位宽度16位，即DMA传输尺寸
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //数据宽度16位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  //工作在循环缓存模式，一轮结束后自动开始下轮传输
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMA通道 x拥有高优先级
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x禁止内存到内存
    DMA_Init(DMA2_Channel5, &DMA_InitStructure);  //根据DMA_InitStruct中参数DMA通道
	DMA_Cmd(DMA2_Channel5, ENABLE);//启动DMA通道
    /* 使能 ADC3 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3 , ENABLE);

	/* 配置IO口为模拟输入 */
	BSP_GPIO_Init(IDC_ADC, GPIO_Mode_AIN);
	BSP_GPIO_Init(VDC_ADC, GPIO_Mode_AIN);
	BSP_GPIO_Init(HV_DC3V3, GPIO_Mode_AIN);
	BSP_GPIO_Init(LSPI3V3, GPIO_Mode_AIN);
	BSP_GPIO_Init(LSPV3V3, GPIO_Mode_AIN);
	BSP_GPIO_Init(LPIMA3V3, GPIO_Mode_AIN);
	
	/* 配置ADC3*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 6;
	
	ADC_Init(ADC3, &ADC_InitStructure);

	/* 配置ADC3 规则通道 */
	ADC_RegularChannelConfig(ADC3, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);		//IDC电流检测				120
	ADC_RegularChannelConfig(ADC3, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5);		//VDC电压检测				590
	ADC_RegularChannelConfig(ADC3, ADC_Channel_10, 3, ADC_SampleTime_239Cycles5);	//高压放电电压					12500
	ADC_RegularChannelConfig(ADC3, ADC_Channel_11, 4, ADC_SampleTime_239Cycles5);	//生产线控输出功率速度电流给定		100,4~20
	ADC_RegularChannelConfig(ADC3, ADC_Channel_12, 5, ADC_SampleTime_239Cycles5);	//生产线控输出功率速度电压给定		0.66~3.3
	ADC_RegularChannelConfig(ADC3, ADC_Channel_13, 6, ADC_SampleTime_239Cycles5);	//生产线控制输出功率电流给定		0.66~3.3
	

	/* 使能ADC3 DMA功能 */
	ADC_DMACmd(ADC3, ENABLE);

	/* 使能 ADC3 */
	ADC_Cmd(ADC3, ENABLE);

	/* 使能ADC3 复位校准寄存器 */
	ADC_ResetCalibration(ADC3);
	/* 检查ADC3的复位寄存器 */
	while(ADC_GetResetCalibrationStatus(ADC3));

	/* 启动ADC3校准 */
	ADC_StartCalibration(ADC3);
	/* 检查校准是否结束 */
	while(ADC_GetCalibrationStatus(ADC3));

	/* 软件启动ADC转换 */
	ADC_SoftwareStartConvCmd(ADC3, ENABLE);
}

/*
*********************************************************************************************************
*	函 数 名: AdcPro
*	功能说明: ADC采样处理，插入1ms systick 中断进行调用
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void AdcPro(void)
{
	static uint16_t buf[SAMP_COUNT];
	static uint8_t write;
	uint32_t sum;
	uint8_t i;

	buf[write] = ADC_GetConversionValue(ADC1);
	if (++write >= SAMP_COUNT)
	{
		write = 0;
	}

	/* 下面这段代码采用求平均值的方法进行滤波
		也可以改善下，选择去掉最大和最下2个值，使数据更加精确
	*/
	sum = 0;
	for (i = 0; i < SAMP_COUNT; i++)
	{
		sum += buf[i];
	}
	g_usAdcValue = sum / SAMP_COUNT;	/* ADC采样值由若干次采样值平均 */

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	/* 软件启动下次ADC转换 */
}

/*
*********************************************************************************************************
*	函 数 名: GetADC
*	功能说明: 读取ADC采样的平均值
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
uint16_t GetADC(void)
{
	uint16_t ret;

	/* 因为	g_AdcValue 变量在systick中断中改写，为了避免主程序读变量时被中断程序打乱导致数据错误，因此需要
	关闭中断进行保护 */

	/* 进行临界区保护，关闭中断 */
	__set_PRIMASK(1);  /* 关中断 */

	ret = g_usAdcValue;

	__set_PRIMASK(0);  /* 开中断 */

	return ret;
}



void getadc1_average(void){  //采集得到平均值
	uint8_t i,j;
	float sum =0;
	float real[6]; //6路ADC
	for (i = 0; i < 6; i++){
		for (j = 0; j < 10;j++){
			sum += adc1_raw_value[j][i];
		}
		
		real[i] = sum * 0.1f;  //得到平均值
		sum=0;
	}
	//赋值
	adc_TBI_DC3V3 = real[0] * 0.073260073260f;
	adc_TBV_DC3V3 = real[1] * 0.195360195360f;
	adc_POV3V3 = real[2] * 10.989010989011f;
	adc_HI_DC3V3 = real[3] * 0.002442002442f;
	adc_LPV3V3 = real[4] * 0.000244200244f;		//得到百分比  0~10v	即0v~3.3v
	adc_TEMP = real[5] * 0.000805860806f;
}

void getadc3_average(void){  //采集得到平均值
	int i;
	int j;
	float sum =0;
	float real[6]; //6路ADC
	for (i = 0; i < 6; i++){
		for (j = 0; j < 10;j++){
			sum += adc3_raw_value[j][i];
		}
		real[i] = sum * 0.1;//mm  
		sum=0;
	}
	//赋值
	adc_IDC_ADC = real[0] * 0.029304029304f;
	adc_VDC_ADC = real[1] * 0.146520146520f;	//600V
	adc_HV_DC3V3 = real[2] * 3.052503052503f;
	adc_LSPI3V3 = (real[3] - 819.0f) * 0.000305250305f;		//得到百分比	 4~20mA 即0.66v~3.3v
	adc_LSPV3V3 = real[4] * 0.000244200244f;				//得到百分比  0~10v	即0v~3.3v
	adc_LPIMA3V3 = (real[5] - 819.0f) * 0.000305250305f;	//得到百分比	 4~20mA 即0.66v~3.3v
}

void adcLowPassFilterInit(void)  //低通滤波参数初始化
{
		float a;
/**********************  ADC_1 *******************************/

    a = 2.0f * 0.05f * 200.0f;

    adcFilters[ADC_1].gx1 = 1.0f / (1.0f + a);
    adcFilters[ADC_1].gx2 = 1.0f / (1.0f + a);
    adcFilters[ADC_1].gx3 = (1.0f - a) / (1.0f + a);
    adcFilters[ADC_1].previousInput  = 0.0f;
    adcFilters[ADC_1].previousOutput = 0.0f;
		
/**********************  ADC_2 *******************************/
    a = 2.0f * 0.05f * 200.0f;

    adcFilters[ADC_2].gx1 = 1.0f / (1.0f + a);
    adcFilters[ADC_2].gx2 = 1.0f / (1.0f + a);
    adcFilters[ADC_2].gx3 = (1.0f - a) / (1.0f + a);
    adcFilters[ADC_2].previousInput  = 0.0f;
    adcFilters[ADC_2].previousOutput = 0.0f;		
/**********************  ADC_3 *******************************/
    a = 2.0f * 0.3f * 1000.0f;

    adcFilters[ADC_3].gx1 = 1.0f / (1.0f + a);
    adcFilters[ADC_3].gx2 = 1.0f / (1.0f + a);
    adcFilters[ADC_3].gx3 = (1.0f - a) / (1.0f + a);
    adcFilters[ADC_3].previousInput  = 0.0f;
    adcFilters[ADC_3].previousOutput = 0.0f;		
		
/**********************  ADC_4 *******************************/
    a = 2.0f * 0.3f * 1000.0f;

    adcFilters[ADC_4].gx1 = 1.0f / (1.0f + a);
    adcFilters[ADC_4].gx2 = 1.0f / (1.0f + a);
    adcFilters[ADC_4].gx3 = (1.0f - a) / (1.0f + a);
    adcFilters[ADC_4].previousInput  = 0.0f;
    adcFilters[ADC_4].previousOutput = 0.0f;		
		
		
		/**********************  SPEED_1 *******************************/

    a = 2.0f * 0.05f * 500.0f;

    speedFilters[SPEED_1].gx1 = 1.0f / (1.0f + a);
    speedFilters[SPEED_1].gx2 = 1.0f / (1.0f + a);
    speedFilters[SPEED_1].gx3 = (1.0f - a) / (1.0f + a);
    speedFilters[SPEED_1].previousInput  = 0.0f;
    speedFilters[SPEED_1].previousOutput = 0.0f;
		
/**********************  SPEED_2 *******************************/
    a = 2.0f * 0.05f * 500.0f;

    speedFilters[SPEED_2].gx1 = 1.0f / (1.0f + a);
    speedFilters[SPEED_2].gx2 = 1.0f / (1.0f + a);
    speedFilters[SPEED_2].gx3 = (1.0f - a) / (1.0f + a);
    speedFilters[SPEED_2].previousInput  = 0.0f;
    speedFilters[SPEED_2].previousOutput = 0.0f;		
/**********************  SPEED_3 *******************************/
    a = 2.0f * 0.05f * 1000.0f;

    speedFilters[SPEED_3].gx1 = 1.0f / (1.0f + a);
    speedFilters[SPEED_3].gx2 = 1.0f / (1.0f + a);
    speedFilters[SPEED_3].gx3 = (1.0f - a) / (1.0f + a);
    speedFilters[SPEED_3].previousInput  = 0.0f;
    speedFilters[SPEED_3].previousOutput = 0.0f;		
		
/**********************  SPEED_4 *******************************/
    a = 2.0f * 0.05f * 1000.0f;

    speedFilters[SPEED_4].gx1 = 1.0f / (1.0f + a);
    speedFilters[SPEED_4].gx2 = 1.0f / (1.0f + a);
    speedFilters[SPEED_4].gx3 = (1.0f - a) / (1.0f + a);
    speedFilters[SPEED_4].previousInput  = 0.0f;
    speedFilters[SPEED_4].previousOutput = 0.0f;		
}

float adcFilter(float input, struct adcFilterData *filterParameters) //二阶低通滤波
{
    float output;

    output = filterParameters->gx1 * input +
             filterParameters->gx2 * filterParameters->previousInput -
             filterParameters->gx3 * filterParameters->previousOutput;

    filterParameters->previousInput  = input;
    filterParameters->previousOutput = output;

    return output;
}

void adcLowPassFilter(formatTrans32Struct_t *distence, formatTrans32Struct_t *speed){  //获得最终距离和速度值
	int i;
//	getadc_average(average_dis);
	  
  for(i = 0; i<2; i++)
	{
	  distence[i].float_temp = (adcFilter(average_dis[i] , &adcFilters[i]));//mm
	}
	  distence[1].float_temp += 0.2f;
}


