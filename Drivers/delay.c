#include "delay.h"

void delay_config(void)
{
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;		//ʱ��ԴΪϵͳʱ��168MHz
	SysTick->LOAD = 167;								//����ֵΪ168-1��ÿ1us���һ��
}

void delay_ms(vu32 nTime)
{
	nTime *= 1000;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;			//ʹ��SysTick����ʼ����
	while(nTime--){
		while((SysTick->CTRL&0X010000) == 0);			//�ȴ�COUNTFLAG��־λ��1
	}
	SysTick->CTRL &= (~SysTick_CTRL_ENABLE_Msk);		//ʧ��SysTick��ֹͣ����
}

void delay_us(vu32 nTime)
{
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	while(nTime--){
		while((SysTick->CTRL&0X010000) == 0);
	}
	SysTick->CTRL &= (~SysTick_CTRL_ENABLE_Msk);
}
