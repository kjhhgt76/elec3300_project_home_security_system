/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   wifi 串口透传例程
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F407 开发板
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"
#include "./systick/bsp_SysTick.h"
#include "./dwt_delay/core_delay.h"
#include "./DHT11/bsp_dht11.h"
#include "./esp8266/bsp_esp8266.h"
#include "./esp8266/bsp_esp8266_test.h"
#include "./led/bsp_led.h"

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{	
  /* 初始化 */
  Debug_USART_Config ();                                                      //初始化串口1
  CPU_TS_TmrInit();                                                     //初始化DWT计数器，用于延时函数
  LED_GPIO_Config();                                                    //初始化RGB彩灯
	ESP8266_Init ();                                                      //初始化WiFi模块使用的接口和外设
	DHT11_GPIO_Config ();                                                        //初始化DHT11

	
	printf ( "\r\n野火 WF-ESP8266 WiFi模块测试例程\r\n" );                 //打印测试例程提示信息

  ESP8266_StaTcpClient_Unvarnish_ConfigTest();                          //对ESP8266进行配置
  
  
	SysTick_Init ();                                                      //配置 SysTick 为 10ms 中断一次，在中断里读取传感器数据
  
  
  while ( 1 )
  {
    
    if( read_dht11_finish ) // read_dht11_finish == 1 or read_dht11_finish == -1
    {
      
      ESP8266_SendDHT11DataTest(); // ESP8266 发送一次温湿度数据
      //printf("read_dht11_finish=%d\r\n", read_dht11_finish);    //Debug
      read_dht11_finish = 0;       // 清零标志位
      
      LED1_TOGGLE;
    }
    
  }
  
	
}



/*********************************************END OF FILE**********************/

