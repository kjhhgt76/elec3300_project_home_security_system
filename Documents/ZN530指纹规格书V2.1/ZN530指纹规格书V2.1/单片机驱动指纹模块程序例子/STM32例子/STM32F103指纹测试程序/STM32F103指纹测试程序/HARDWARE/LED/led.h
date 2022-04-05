#ifndef __LED_H
#define __LED_H	 
#include "stm32f10x.h"


//LED端口定义
#define LED0 PCout(0)// PC0
#define LED1 PCout(1)// PC0
#define LED2 PCout(2)// PC0


void LED_Init(void);//初始化

		 				    
#endif
