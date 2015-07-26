#include "remote_control.h"
#include "usart.h"
#include "motor.h"

void remote_control_init(void)
{
	usart1_init(115200);
}

#define MSG_STOP	('s')
#define MSG_FRONT	('a')
#define MSG_BACK	('b')
#define MSG_LEFT	('c')
#define MSG_RIGHT	('d')
void USART1_IRQHandler(void)
{
	uint8_t ch;
	if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==SET)
	{
		ch = USART_ReceiveData(USART1);
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		switch(ch){
			case MSG_STOP:
				break;
			case MSG_FRONT:
				break;
			case MSG_BACK:
				break;
			case MSG_LEFT:
				break;
			case MSG_RIGHT:
				break;
		}
	}
}
