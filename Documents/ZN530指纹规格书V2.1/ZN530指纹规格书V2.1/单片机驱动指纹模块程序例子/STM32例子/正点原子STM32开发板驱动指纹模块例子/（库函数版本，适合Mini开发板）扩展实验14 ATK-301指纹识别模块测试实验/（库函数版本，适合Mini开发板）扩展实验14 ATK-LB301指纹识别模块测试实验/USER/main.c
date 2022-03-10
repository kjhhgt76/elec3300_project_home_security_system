#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "lcd.h"
#include "key.h"
#include "touch.h"	  
#include "malloc.h" 
#include "usmart.h"  
#include "MMC_SD.h"   
#include "ff.h"  
#include "exfuns.h"    
#include "fontupd.h"
#include "text.h"	
#include "usart2.h"
#include "LB301.h"
#include "timer.h"

//ALIENTEK Mini STM32开发板扩展实验14
//ATK-LB301指纹识别模块实验-库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com
//广州市星翼电子科技有限公司    
//作者：正点原子 @ALIENTEK 

#define usart2_baund  57600//串口2波特率，根据指纹模块波特率更改（注意：指纹模块默认57600）
SysPara LB301Para;//指纹模块LB301参数
u16 ValidN;//模块内有效模板个数
u8** kbd_tbl;
const  u8* kbd_menu[15]={"del_fp"," : ","add_fp","1","2","3","4","5","6","7","8","9","DEL","0","Enter",};//按键表
const  u8* kbd_delFR[15]={"BACK"," : ","del_all","1","2","3","4","5","6","7","8","9","DEL","0","Enter",};//按键表

void Add_FR(void);	//录指纹
void Del_FR(void);	//删除指纹
void press_FR(void);//刷指纹
void ShowErrMessage(u8 ensure);//显示确认码错误信息
void LB301_load_keyboard(u16 x,u16 y,u8 **kbtbl);//加载虚拟键盘
u8  LB301_get_keynum(u16 x,u16 y);//获取键盘数
u16 GET_NUM(void);//获取数值  

int main(void)
{		 
	u8 ensure;
	u8 key_num;
	char *str;
	u16 count = 0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init();  	    //初始化延时函数
	uart_init(115200);	//初始化串口1波特率为115200，用于支持USMART
	usart2_init(usart2_baund);//初始化串口2,用于与指纹模块通讯
	KEY_Init();					//按键初始化 
 	LCD_Init();					//LCD初始化 	
	tp_dev.init();			//初始化触摸屏
	usmart_dev.init(72);//初始化USMART
	mem_init();					//初始化内存池 
	exfuns_init();			//为fatfs相关变量申请内存  
 	f_mount(fs[1],"1:",1);  //挂载FLASH.
	POINT_COLOR=RED;
	if(!(tp_dev.touchtype&0x80))//如果是电阻屏
	{
		Show_Str_Mid(0,30,"Adjust the touch screen?",16,240);
		POINT_COLOR=BLUE;
		Show_Str_Mid(0,60,"YES: KEY1   NO: KEY0",16,240);	
		while(1)
		{
			key_num=KEY_Scan(0);
			if(key_num==KEY0_PRES)
				break;
			if(key_num==KEY1_PRES)
			{
				LCD_Clear(WHITE);
				TP_Adjust();  	 //屏幕校准 
				TP_Save_Adjdata();//保存校准参数
				break;				
			}
		}
	}
	/*加载指纹识别实验界面*/
	LCD_Clear(WHITE);
	POINT_COLOR=RED;
	Show_Str_Mid(0,0,"LB301 Fingerprint module test",16,240);	    			    	 
	Show_Str_Mid(0,20,"Author: @ALIENTEK",16,240);				    	 
 	POINT_COLOR=BLUE;
	Show_Str_Mid(0,40,"Connect with LB301....",16,240);	
	while(PS_GetImage() == 0xff)//与LB301模块握手（读取图像指令有数据返回说明模块已经连接）
	{
		LCD_Fill(0,40,240,80,WHITE);
		Show_Str_Mid(0,40,"Cannot connect with LB301!",16,240);
		delay_ms(1000);
		LCD_Fill(0,40,240,80,WHITE);
		Show_Str_Mid(0,40,"Try to connect again....",16,240);	
		delay_ms(1000);	  
	}
	LCD_Fill(0,40,240,320,WHITE);
	Show_Str_Mid(0,40,"Connect success!",16,240);//通讯成功
	str=mymalloc(30);
	sprintf(str,"Baudrate:%d   Addr:%x",usart2_baund,LB301Addr);//显示波特率
	Show_Str(0,60,240,16,(u8*)str,16,0);
	delay_ms(100);
	ensure=PS_ValidTempleteNum(&ValidN);//读库指纹个数
	if(ensure!=0x00)
		ShowErrMessage(ensure);//显示确认码错误信息	
	ensure=PS_ReadSysPara(&LB301Para);  //读LB301模块参数 
	if(ensure==0x00)
	{
		mymemset(str,0,50);
		sprintf(str,"RemainNum:%d    Level:%d",LB301Para.PS_max-ValidN,LB301Para.PS_level);//显示剩余指纹数量和安全等级
		Show_Str(0,80,240,16,(u8*)str,16,0);
	}
	else
		ShowErrMessage(ensure);	
	myfree(str);
	LB301_load_keyboard(0,170,(u8**)kbd_menu);//加载虚拟键盘
	while(1)
	{
		key_num=LB301_get_keynum(0,170);	
		if(key_num)
		{
			if(key_num==1)Del_FR();		//删指纹
			if(key_num==3)Add_FR();		//录指纹									
		}
		//查询刷指纹
		if(++count > 10) 
		{
			count = 0;
			press_FR();		
		}
		delay_ms(1);				 
	} 	
}


//加载按键界面（尺寸x,y为240*150）
//x,y:界面起始坐标（240*320分辨率的时候，x必须为0）
void LB301_load_keyboard(u16 x,u16 y,u8 **kbtbl)
{
	u16 i;
	POINT_COLOR=RED;
	kbd_tbl=kbtbl;
	LCD_Fill(x,y,x+240,y+150,WHITE);
	LCD_DrawRectangle(x,y,x+240,y+150);						   
	LCD_DrawRectangle(x+80,y,x+160,y+150);	 
	LCD_DrawRectangle(x,y+30,x+240,y+60);
	LCD_DrawRectangle(x,y+90,x+240,y+120);
	POINT_COLOR=BLUE;
	for(i=0;i<15;i++)
	{
		if(i==1)//按键表第2个‘:’不需要中间显示
			Show_Str(x+(i%3)*80+2,y+7+30*(i/3),80,30,(u8*)kbd_tbl[i],16,0);	
		else
			Show_Str_Mid(x+(i%3)*80,y+7+30*(i/3),(u8*)kbd_tbl[i],16,80);
	} 
}
//按键状态设置
//x,y:键盘坐标
//key:键值(0~14)
//sta:状态，0，松开；1，按下；
void LB301_key_staset(u16 x,u16 y,u8 keyx,u8 sta)
{		  
	u16 i=keyx/3,j=keyx%3;
	if(keyx>16)return;
	if(sta &&keyx!=1)//按键表第2个‘:’不需要清除
		LCD_Fill(x+j*80+1,y+i*30+1,x+j*80+78,y+i*30+28,GREEN);
	else if(keyx!=1)
		LCD_Fill(x+j*80+1,y+i*30+1,x+j*80+78,y+i*30+28,WHITE);
	if(keyx!=1)//不是‘：’
		Show_Str_Mid(x+j*80,y+7+30*i,(u8*)kbd_tbl[keyx],16,80);	
}
//得到触摸屏的输入
//x,y:键盘坐标
//返回值:（1~15,对应按键表）
u8 LB301_get_keynum(u16 x,u16 y)
{
	u16 i,j;
	static u8 key_x=0;//0,没有任何按键按下
	u8 key=0;
	tp_dev.scan(0); 		 
	if(tp_dev.sta&TP_PRES_DOWN)//触摸屏被按下
	{	
		for(i=0;i<5;i++)
		{
			for(j=0;j<3;j++)
			{
			 	if(tp_dev.x[0]<(x+j*80+80)&&tp_dev.x[0]>(x+j*80)&&tp_dev.y[0]<(y+i*30+30)&&tp_dev.y[0]>(y+i*30))
				{	
					key=i*3+j+1;	 
					break;	 		   
				}
			}
			if(key)
			{	   
				if(key_x==key)key=0;
				else 
				{
					LB301_key_staset(x,y,key_x-1,0);
					key_x=key;
					LB301_key_staset(x,y,key_x-1,1);
				}
				break;
			}
	  }  
	}else if(key_x) 
	{
		LB301_key_staset(x,y,key_x-1,0);
		key_x=0;
	} 
	return key; 
}
//获取键盘数值
u16 GET_NUM(void)
{
	u8  key_num=0;
	u16 num=0;
	while(1)
	{
		key_num=LB301_get_keynum(0,170);	
		if(key_num)
		{
			if(key_num==1)return 0xFFFF;//‘返回’键
			if(key_num==3)return 0xFF00;//		
			if(key_num>3&&key_num<13&&num<99)//‘1-9’键(限制输入3位数)
				num =num*10+key_num-3;		
			if(key_num==13)num =num/10;//‘Del’键			
			if(key_num==14&&num<99)num =num*10;//‘0’键
			if(key_num==15)return num;  //‘Enter’键
		}
		LCD_ShowNum(80+15,170+7,num,6,16);
	}	
}
//显示确认码错误信息
void ShowErrMessage(u8 ensure)
{
	LCD_Fill(0,120,lcddev.width,160,WHITE);
	Show_Str_Mid(0,120,(u8*)EnsureMessage(ensure),16,240);
}
//录指纹
void Add_FR(void)
{
	u8 i=0,ensure ,processnum=1,pressCout=1, str_buffer[40];
	u16 ID;
	while(1)
	{
		switch (processnum)
		{
			case 1: //连续按压4次指纹分别存到4个charBuffer里 
				i++;
				LCD_Fill(0,100,lcddev.width,160,WHITE);
				sprintf((char*)str_buffer,"Pleas touch finger(%d)",pressCout);
				Show_Str_Mid(0,100,str_buffer,16,240);
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					i=0;
					delay_ms(100);//这里需要延时一下，模块内部处理图像需要时间
					ensure=PS_GenChar(CharBuffer1);//生成特征
					if(ensure==0x00)
					{
						LCD_Fill(0,120,lcddev.width,160,WHITE);
						Show_Str_Mid(0,120,"Fingerprint correct",16,240);//指纹正确
						pressCout++;
						if(pressCout >=5)
						{
							pressCout = 0;
							processnum=2;//跳到第二步
						}					
					}else ShowErrMessage(ensure);				
				}else ShowErrMessage(ensure);						
				break;

			case 2:
				LCD_Fill(0,100,lcddev.width,160,WHITE);
				Show_Str_Mid(0,100,"Generate fingerprint template",16,240);//产生一个指纹模板
				ensure=PS_RegMB();
				if(ensure==0x00) 
				{
					LCD_Fill(0,120,lcddev.width,160,WHITE);
					Show_Str_Mid(0,120,"Generate fingerprint success",16,240);//生成指纹模板成功
					processnum=3;//跳到第三步
				}else {processnum=0;ShowErrMessage(ensure);}
				delay_ms(1000);
				break;
				
			case 3:	
				LCD_Fill(0,100,lcddev.width,160,WHITE);
				Show_Str_Mid(0,100,"Intput ID and save with ENTER!",16,240);//输入ID并按“Enter”保存
				Show_Str_Mid(0,120,"0=< ID <=499",16,240);	
				do
					ID=GET_NUM();
				while(!(ID<300));//输入DI必须小于300
				ensure=PS_StoreMB(CharBuffer1,ID);//储存模板
				if(ensure==0x00) 
				{			
					LCD_Fill(0,100,lcddev.width,160,WHITE);					
					Show_Str_Mid(0,120,"Add fingerprint success!!!",16,240);//添加指纹成功
					PS_ValidTempleteNum(&ValidN);//读库指纹个数
					LCD_ShowNum(80,80,LB301Para.PS_max-ValidN,3,16);//显示剩余指纹个数
					delay_ms(1500);//延时后清除显示	
					LCD_Fill(0,100,240,160,WHITE);
					return ;
				}else {processnum=0;ShowErrMessage(ensure);}					
				break;				
		}
		delay_ms(200);
		if(i>=20)//超过20次没有按手指则退出
		{
			LCD_Fill(0,100,lcddev.width,160,WHITE);
			break;	
		}					
	}
}

#define USE_AUTO_IDETIFY  1 // 1使用自动验证方式  0使用分布式验证方式

//刷指纹
void press_FR(void)
{
	SearchResult seach;
	u8 ensure;
	char *str;
#if (USE_AUTO_IDETIFY == 1) //自动验证指纹	
	ensure=PS_AutoIdentify(&seach);
	if(ensure==0x00)//自动验证成功
	{
		LCD_Fill(0,100,lcddev.width,160,WHITE);
		Show_Str_Mid(0,100,"Search fingerprint success",16,240);
		str=mymalloc(50);
		sprintf(str,"Match ID:%d  Match score:%d",seach.pageID,seach.mathscore);
		Show_Str_Mid(0,140,(u8*)str,16,240);
		myfree(str);
		delay_ms(500);
		LCD_Fill(0,100,lcddev.width,160,WHITE);
	}
	else if(ensure != 0xff)
	{
		ShowErrMessage(ensure);
		delay_ms(500);
		LCD_Fill(0,100,lcddev.width,160,WHITE);
	}
#else  //分布式验证指纹	
	ensure=PS_GetImage();
	if(ensure==0x00)//获取图像成功 
	{	
		ensure=PS_GenChar(CharBuffer1);
		if(ensure==0x00) //生成特征成功
		{		
			ensure=PS_HighSpeedSearch(CharBuffer1,0,300,&seach);
			if(ensure==0x00)//搜索成功
			{				
				LCD_Fill(0,100,lcddev.width,160,WHITE);
				Show_Str_Mid(0,100,"Search fingerprint success",16,240);//搜索指纹成功				
				str=mymalloc(50);
				sprintf(str,"Match ID:%d  Match score:%d",seach.pageID,seach.mathscore);//显示匹配指纹的ID和分数
				Show_Str_Mid(0,140,(u8*)str,16,240);
				myfree(str);
			}
			else 
				ShowErrMessage(ensure);					
	  }
		else
			ShowErrMessage(ensure);
	 delay_ms(1000);//延时后清除显示
	 LCD_Fill(0,100,lcddev.width,160,WHITE);
	}
#endif	
}

//删除指纹
void Del_FR(void)
{
	u8  ensure;
	u16 num;
	LCD_Fill(0,100,lcddev.width,160,WHITE);
	Show_Str_Mid(0,100,"Delete fingerprint",16,240);//显示删除指纹
	Show_Str_Mid(0,120,"Input ID and touch Enter!",16,240);//显示输入ID并按下“Enter”
	Show_Str_Mid(0,140,"0=< ID <=299",16,240);
	delay_ms(50);
	LB301_load_keyboard(0,170,(u8**)kbd_delFR);
	num=GET_NUM();//获取返回的数值
	if(num==0xFFFF)
		goto MENU ; //返回主页面
	else if(num==0xFF00)
		ensure=PS_Empty();//清空指纹库
	else 
		ensure=PS_DeletChar(num,1);//删除单个指纹
	if(ensure==0)
	{
		LCD_Fill(0,120,lcddev.width,160,WHITE);
		Show_Str_Mid(0,140,"Delete fingerprint success!!!",16,240);//删除指纹成功		
	}
  else
		ShowErrMessage(ensure);	
	delay_ms(1500);//延时后清除显示
	PS_ValidTempleteNum(&ValidN);//读库指纹个数
	LCD_ShowNum(80,80,LB301Para.PS_max-ValidN,3,16);//显示剩余指纹个数
MENU:	
	LCD_Fill(0,100,lcddev.width,160,WHITE);
	delay_ms(50);
	LB301_load_keyboard(0,170,(u8**)kbd_menu);
}





