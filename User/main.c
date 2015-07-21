#include "stm32f4xx.h"
#include "util.h"
#include "usart.h"
#include "delay.h"
#include "stm32f4_discovery.h"
#include "gpio.h"

int main()
{
	rcc_clock_enable();
	delay_config();
	uart4_init(115200);
	print_clock_freq();
	
	while(1){
		delay_ms(250);
	}
}

