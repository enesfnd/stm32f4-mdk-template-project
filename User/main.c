#include "stm32f4xx.h"
#include "util.h"
#include "usart.h"
#include "delay.h"
#include "stm32f4_discovery.h"
#include "gpio.h"
#include "..\Modules\SD-Card\sd.h"

#include "micros_time.h"

int main()
{
	rcc_clock_enable();
	delay_config();
	uart4_init(115200);
	print_clock_freq();
	
	STM_EVAL_LEDInit(LED3);
	micros_time_init();
	
//	if(SD_Init() != 0){
//		printf("sd card init error!\r\n");
//		while(1);
//	}
//	show_sdcard_info();	//打印SD卡相关信息

	while(1){
		STM_EVAL_LEDToggle(LED3);
		get_interval_time();
		delay_ms(1000);
		printf("time=%d\r\n", get_interval_time());
	}
}

