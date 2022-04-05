#ifndef __RELAY_H
#define __RELAY_H	 
#include "stm32f10x.h"

#include "sys.h"


#define RELAY PCout(3)
#define RELAYON GPIO_SetBits(GPIOC,GPIO_Pin_3)
#define RELAYOFF  GPIO_ResetBits(GPIOC,GPIO_Pin_3)

void RELAY_Init(void);//≥ı ºªØ







#endif





