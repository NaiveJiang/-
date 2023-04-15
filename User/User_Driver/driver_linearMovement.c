#include "driver.h"

uint16_t g_usAdcValue;	
__IO uint16_t adc1_raw_value[10][6]; //�ɼ���ADCֵ
__IO uint16_t adc3_raw_value[10][6]; //�ɼ���ADCֵ
__IO float average_dis[4];  //����ƽ��ֵ
adcFilterData_t adcFilters[4]; //�˲��ṹ��
adcFilterData_t speedFilters[4];
/*
*********************************************************************************************************
*	�� �� ��: drive_ADC1_Configuration
*	����˵��: ����ADC1, PA6,PA7,PB0,PB1,PC4,PC5��ΪADC1ͨ������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void drive_ADC1_Configuration(void){
	ADC_InitTypeDef ADC_InitStructure;
	__IO uint16_t ADCConvertedValue;

	//ADC1 DMA����
    /* ADC1  DMA1 Channel Config */  
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //ʹ��DMAʱ��
 
    DMA_DeInit(DMA1_Channel1);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ��ADC1����DMAͨ��1
    DMA_InitStructure.DMA_PeripheralBaseAddr =  (u32)&ADC1->DR;  //DMA����ADC����ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&adc1_raw_value;      //DMA�ڴ����ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //�ڴ���Ϊ���ݴ����Ŀ�ĵ�
    DMA_InitStructure.DMA_BufferSize = 10*6;  //��ֵΪ����һ��DMA����Ĵ���
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ����
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ����
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //��������λ���16λ����DMA����ߴ�
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //���ݿ��16λ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  //������ѭ������ģʽ��һ�ֽ������Զ���ʼ���ִ���
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMAͨ�� xӵ�и����ȼ�
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��x��ֹ�ڴ浽�ڴ�
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);  //����DMA_InitStruct�в���DMAͨ��
	DMA_Cmd(DMA1_Channel1, ENABLE);//����DMAͨ��
    /* ʹ�� ADC1 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);

	/* ����IO��Ϊģ������ */
	BSP_GPIO_Init(TRANS_IADC, GPIO_Mode_AIN);
	BSP_GPIO_Init(TRANS_VADC, GPIO_Mode_AIN);
	BSP_GPIO_Init(OUTPUT_POWER_3V3, GPIO_Mode_AIN);
	BSP_GPIO_Init(HV_DISCHARGE_IADC, GPIO_Mode_AIN);
	BSP_GPIO_Init(LPV3V3, GPIO_Mode_AIN);
	BSP_GPIO_Init(TEMP, GPIO_Mode_AIN);
	
	/* ����ADC1, ����DMA, ��������� */
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 6;
	
	ADC_Init(ADC1, &ADC_InitStructure);

	/* ����ADC1 ����ͨ�� */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_55Cycles5);		//��ѹ��ԭ�ߵ���
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 2, ADC_SampleTime_55Cycles5);		//��ѹ��ԭ�ߵ�ѹ
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 3, ADC_SampleTime_55Cycles5);		//�������AD3.3V���
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 4, ADC_SampleTime_55Cycles5);		//��ѹ�ŵ�������
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 5, ADC_SampleTime_55Cycles5);	//�����߿���������ʵ�ѹ����
	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 6, ADC_SampleTime_55Cycles5);	//ɢ�����¶ȼ������
	

	/* ʹ��ADC1 DMA���� */
	ADC_DMACmd(ADC1, ENABLE);

	/* ʹ�� ADC1 */
	ADC_Cmd(ADC1, ENABLE);

	/* ʹ��ADC1 ��λУ׼�Ĵ��� */
	ADC_ResetCalibration(ADC1);
	/* ���ADC1�ĸ�λ�Ĵ��� */
	while(ADC_GetResetCalibrationStatus(ADC1));

	/* ����ADC1У׼ */
	ADC_StartCalibration(ADC1);
	/* ���У׼�Ƿ���� */
	while(ADC_GetCalibrationStatus(ADC1));

	/* �������ADCת�� */
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/*
*********************************************************************************************************
*	�� �� ��: drive_ADC3_Configuration
*	����˵��: ����ADC3, PA0,PA1,PC0~PC3��ΪADC3ͨ������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void drive_ADC3_Configuration(void){
	ADC_InitTypeDef ADC_InitStructure;
	__IO uint16_t ADCConvertedValue;

	//ADC3 DMA����
    /* ADC3  DMA2 Channel Config */  
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE); //ʹ��DMAʱ��
 
    DMA_DeInit(DMA2_Channel5);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ��ADC1����DMAͨ��1
    DMA_InitStructure.DMA_PeripheralBaseAddr =  (u32)&ADC3->DR;  //DMA����ADC����ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&adc3_raw_value;      //DMA�ڴ����ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //�ڴ���Ϊ���ݴ����Ŀ�ĵ�
    DMA_InitStructure.DMA_BufferSize = 10*6;  //��ֵΪ����һ��DMA����Ĵ���
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ����
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ����
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //��������λ���16λ����DMA����ߴ�
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //���ݿ��16λ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  //������ѭ������ģʽ��һ�ֽ������Զ���ʼ���ִ���
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMAͨ�� xӵ�и����ȼ�
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��x��ֹ�ڴ浽�ڴ�
    DMA_Init(DMA2_Channel5, &DMA_InitStructure);  //����DMA_InitStruct�в���DMAͨ��
	DMA_Cmd(DMA2_Channel5, ENABLE);//����DMAͨ��
    /* ʹ�� ADC3 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3 , ENABLE);

	/* ����IO��Ϊģ������ */
	BSP_GPIO_Init(IDC_ADC, GPIO_Mode_AIN);
	BSP_GPIO_Init(VDC_ADC, GPIO_Mode_AIN);
	BSP_GPIO_Init(HV_DC3V3, GPIO_Mode_AIN);
	BSP_GPIO_Init(LSPI3V3, GPIO_Mode_AIN);
	BSP_GPIO_Init(LSPV3V3, GPIO_Mode_AIN);
	BSP_GPIO_Init(LPIMA3V3, GPIO_Mode_AIN);
	
	/* ����ADC3*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 6;
	
	ADC_Init(ADC3, &ADC_InitStructure);

	/* ����ADC3 ����ͨ�� */
	ADC_RegularChannelConfig(ADC3, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);		//IDC�������
	ADC_RegularChannelConfig(ADC3, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);		//VDC��ѹ���
	ADC_RegularChannelConfig(ADC3, ADC_Channel_10, 3, ADC_SampleTime_55Cycles5);	//��ѹ�ŵ��ѹ
	ADC_RegularChannelConfig(ADC3, ADC_Channel_11, 4, ADC_SampleTime_55Cycles5);	//�����߿���������ٶȵ�������
	ADC_RegularChannelConfig(ADC3, ADC_Channel_12, 5, ADC_SampleTime_55Cycles5);	//�����߿���������ٶȵ�ѹ����
	ADC_RegularChannelConfig(ADC3, ADC_Channel_13, 6, ADC_SampleTime_55Cycles5);	//�����߿���������ʵ�������
	

	/* ʹ��ADC3 DMA���� */
	ADC_DMACmd(ADC3, ENABLE);

	/* ʹ�� ADC3 */
	ADC_Cmd(ADC3, ENABLE);

	/* ʹ��ADC3 ��λУ׼�Ĵ��� */
	ADC_ResetCalibration(ADC3);
	/* ���ADC3�ĸ�λ�Ĵ��� */
	while(ADC_GetResetCalibrationStatus(ADC3));

	/* ����ADC3У׼ */
	ADC_StartCalibration(ADC3);
	/* ���У׼�Ƿ���� */
	while(ADC_GetCalibrationStatus(ADC3));

	/* �������ADCת�� */
	ADC_SoftwareStartConvCmd(ADC3, ENABLE);
}

/*
*********************************************************************************************************
*	�� �� ��: AdcPro
*	����˵��: ADC������������1ms systick �жϽ��е���
*	��    �Σ���
*	�� �� ֵ: ��
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

	/* ������δ��������ƽ��ֵ�ķ��������˲�
		Ҳ���Ը����£�ѡ��ȥ����������2��ֵ��ʹ���ݸ��Ӿ�ȷ
	*/
	sum = 0;
	for (i = 0; i < SAMP_COUNT; i++)
	{
		sum += buf[i];
	}
	g_usAdcValue = sum / SAMP_COUNT;	/* ADC����ֵ�����ɴβ���ֵƽ�� */

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	/* ��������´�ADCת�� */
}

/*
*********************************************************************************************************
*	�� �� ��: GetADC
*	����˵��: ��ȡADC������ƽ��ֵ
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
uint16_t GetADC(void)
{
	uint16_t ret;

	/* ��Ϊ	g_AdcValue ������systick�ж��и�д��Ϊ�˱��������������ʱ���жϳ�����ҵ������ݴ��������Ҫ
	�ر��жϽ��б��� */

	/* �����ٽ����������ر��ж� */
	__set_PRIMASK(1);  /* ���ж� */

	ret = g_usAdcValue;

	__set_PRIMASK(0);  /* ���ж� */

	return ret;
}



void getadc1_average(__IO float *real){  //�ɼ��õ�ƽ��ֵ
	int i;
	int j;
	float sum =0;
	for (i = 0; i < 6; i++)
	{
		for (j = 0; j < 10;j++)
		{
			sum += adc1_raw_value[j][i];
		}
		real[i] = sum * 0.1;//mm  
		sum=0;
  }	
}

void getadc3_average(__IO float *real){  //�ɼ��õ�ƽ��ֵ
	int i;
	int j;
	float sum =0;
	for (i = 0; i < 6; i++)
	{
		for (j = 0; j < 10;j++)
		{
			sum += adc3_raw_value[j][i];
		}
		real[i] = sum * 0.1;//mm  
		sum=0;
  }	
  
}

void adcLowPassFilterInit(void)  //��ͨ�˲�������ʼ��
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

float adcFilter(float input, struct adcFilterData *filterParameters) //���׵�ͨ�˲�
{
    float output;

    output = filterParameters->gx1 * input +
             filterParameters->gx2 * filterParameters->previousInput -
             filterParameters->gx3 * filterParameters->previousOutput;

    filterParameters->previousInput  = input;
    filterParameters->previousOutput = output;

    return output;
}

void adcLowPassFilter(formatTrans32Struct_t *distence, formatTrans32Struct_t *speed)  //������վ�����ٶ�ֵ
{
	int i;
//	getadc_average(average_dis);
	  
  for(i = 0; i<2; i++)
	{
	  distence[i].float_temp = (adcFilter(average_dis[i] , &adcFilters[i]));//mm
	}
	  distence[1].float_temp += 0.2f;
}

