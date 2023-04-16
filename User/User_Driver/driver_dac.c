#include "driver.h"


//DAC1��������
void driver_DAC1_Configuration(void){
	DAC_InitTypeDef DAC_InitType;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);
	
	BSP_GPIO_Init(PGD3V3,GPIO_Mode_AIN);
	
	DAC_InitType.DAC_Trigger = DAC_Trigger_None;	//��ʹ�ô������� TEN1=0
	DAC_InitType.DAC_WaveGeneration = DAC_WaveGeneration_None;//��ʹ�ò��η���
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;//���Ρ���ֵ����
	DAC_InitType.DAC_OutputBuffer = DAC_OutputBuffer_Disable ;	//DAC1�������ر� BOFF1=1
    DAC_Init(DAC_Channel_1,&DAC_InitType);	 //��ʼ��DACͨ��1
	
	DAC_Cmd(DAC_Channel_1,ENABLE);
	DAC_SetChannel1Data(DAC_Align_12b_R, 0);  //12λ�Ҷ������ݸ�ʽ����DACֵ
	
}

//DAC2��������
void driver_DAC2_Configuration(void){
	DAC_InitTypeDef DAC_InitType;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);
	
	BSP_GPIO_Init(POUT3V3,GPIO_Mode_AIN);
	
	DAC_InitType.DAC_Trigger = DAC_Trigger_None;	//��ʹ�ô������� TEN1=0
	DAC_InitType.DAC_WaveGeneration = DAC_WaveGeneration_None;//��ʹ�ò��η���
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;//���Ρ���ֵ����
	DAC_InitType.DAC_OutputBuffer = DAC_OutputBuffer_Disable ;	//DAC1�������ر� BOFF1=1
    DAC_Init(DAC_Channel_2,&DAC_InitType);	 //��ʼ��DACͨ��1
	
	DAC_Cmd(DAC_Channel_2,ENABLE);
	DAC_SetChannel2Data(DAC_Align_12b_R, 0);  //12λ�Ҷ������ݸ�ʽ����DACֵ
	
}

//DAC1���
void dac_ch1_voltageOut(float setVoltage){
	uint16_t voltageOut;
	voltageOut = (uint16_t)(setVoltage/(3.3f)*4095);
	DAC_SetChannel1Data(DAC_Align_12b_R,voltageOut);
}

//DAC2���
void dac_ch2_voltageOut(float setVoltage){
	uint16_t voltageOut;
	voltageOut = (uint16_t)(setVoltage/(3.3f)*4095);
	DAC_SetChannel2Data(DAC_Align_12b_R,voltageOut);
}

