#include "adc.h"
#include "gpio.h"
#include "util.h"

volatile u16 adcValue[2] = {0, 0};

//DMA传输的ADC配置
void adc_config(void)
{
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	DMA_InitTypeDef       DMA_InitStructure;
	
	gpio_an_no_init(GPIOB, GPIO_Pin_0|GPIO_Pin_1);
	
	DMA_Cmd(DMA2_Stream0, DISABLE);
	DMA_DeInit(DMA2_Stream0);
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)adcValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = 2;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	DMA_Cmd(DMA2_Stream0, ENABLE);
	
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;	//独立模式
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;	//两个采样阶段之间的延迟5个时钟
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; 		//DMA失能
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;	//ADCCLK=PCLK2/4=84/4=21Mhz
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;	//12位模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;			//扫描模式	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;		//连续转换
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;	//禁止触发检测，使用软件触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//右对齐	
	ADC_InitStructure.ADC_NbrOfConversion = 2;				//规则序列中有2个转换
	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_480Cycles );	//设置通道8采样顺序为1, 采样时间为480个周期	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 2, ADC_SampleTime_480Cycles );	//设置通道9采样顺序为1, 采样时间为480个周期	
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);
}

void adc_start_converte(void)   
{
	ADC_SoftwareStartConv(ADC1);
}

void adc_get_voltage(volatile u16 *adcValue, float *voltage)
{
	voltage[0] = adcValue[0] * 3.0f / 4096.0f;
	voltage[1] = adcValue[1] * 3.0f / 4096.0f;
}

//定时器触发的ADC配置
void adc_tim_trig_config(uint32_t period, uint32_t prescaler)
{
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef  ADC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	gpio_an_no_init(GPIOB, GPIO_Pin_0);
	
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;	//独立模式
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;	//两个采样阶段之间的延迟5个时钟
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; 		//DMA失能
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;	//ADCCLK=PCLK2/4=84/4=21Mhz
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;	//12位模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;			//关闭扫描模式	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;		//关闭连续转换
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;	//外部触发上升沿
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_CC2;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//右对齐	
	ADC_InitStructure.ADC_NbrOfConversion = 1;				//规则序列中有1个转换
	ADC_Init(ADC1, &ADC_InitStructure);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_84Cycles );	//设置通道5采样顺序为1, 采样时间为84个周期	
	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
	ADC_Cmd(ADC1, ENABLE);
	
	nvic_config(ADC_IRQn, 2);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = period-1;
	TIM_TimeBaseStructure.TIM_Prescaler = prescaler-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_Pulse = period/2;
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	
	TIM_Cmd(TIM2, ENABLE);    
	TIM_InternalClockConfig(TIM2);  
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  
	TIM_UpdateDisableConfig(TIM2, DISABLE); 
}

void ADC_IRQHandler(void)
{
	u16 adcVal;
	if(ADC_GetITStatus(ADC1, ADC_IT_EOC) == SET){
		adcVal = ADC1->DR;
		//todo
	}
}

