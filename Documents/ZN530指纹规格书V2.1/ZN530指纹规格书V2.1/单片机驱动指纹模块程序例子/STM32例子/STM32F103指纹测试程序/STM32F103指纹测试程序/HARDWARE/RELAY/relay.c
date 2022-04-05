
#include "relay.h"

/************************************************************************  
DESC:LED IO初始化
继电器接在PC3上。
*************************************************************************/
void RELAY_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				     
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		   
 GPIO_Init(GPIOC, &GPIO_InitStructure);					         
 GPIO_SetBits(GPIOC,GPIO_Pin_3);						            
}


