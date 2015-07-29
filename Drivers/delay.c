#include "delay.h"

void delay_config(void)
{
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;		//时钟源为系统时钟168MHz
	SysTick->LOAD = 167;								//重载值为168-1，每1us溢出一次
}

void delay_ms(vu32 nTime)
{
	nTime *= 1000;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;			//使能SysTick，开始计数
	while(nTime--){
		while((SysTick->CTRL&0X010000) == 0);			//等待COUNTFLAG标志位置1
	}
	SysTick->CTRL &= (~SysTick_CTRL_ENABLE_Msk);		//失能SysTick，停止计数
}

void delay_us(vu32 nTime)
{
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	while(nTime--){
		while((SysTick->CTRL&0X010000) == 0);
	}
	SysTick->CTRL &= (~SysTick_CTRL_ENABLE_Msk);
}
