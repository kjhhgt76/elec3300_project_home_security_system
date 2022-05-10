#include "as608.h"
#include "stm32f1xx_hal.h"
#include <stdio.h>
#include <string.h>
#include "own_define_functions.h"


uint32_t AS608Addr = 0XFFFFFFFF; 
char str2[6] = {0};
uint8_t USART1_RX_BUF[USART1_MAX_RECV_LEN]; 		
uint8_t USART1_TX_BUF[USART1_MAX_SEND_LEN]; 			  

__IO uint16_t USART1_RX_STA=0;




static void MYUSART_SendData(uint8_t data)
{
  while((USART1->SR & 0X40) == 0);
  USART1->DR = data;
}
static void SendHead(void)
{
  MYUSART_SendData(0xEF);
  MYUSART_SendData(0x01);
}
static void SendAddr(void)
{
  MYUSART_SendData(AS608Addr >> 24);
  MYUSART_SendData(AS608Addr >> 16);
  MYUSART_SendData(AS608Addr >> 8);
  MYUSART_SendData(AS608Addr);
}
static void SendFlag(uint8_t flag)
{
  MYUSART_SendData(flag);
}
static void SendLength(int length)
{
  MYUSART_SendData(length >> 8);
  MYUSART_SendData(length);
}
static void Sendcmd(uint8_t cmd)
{
  MYUSART_SendData(cmd);
}
static void SendCheck(uint16_t check)
{
  MYUSART_SendData(check >> 8);
  MYUSART_SendData(check);
}
static uint8_t *JudgeStr(uint16_t waittime)
{
  char *data;
  uint8_t str[8];
  str[0] = 0xef;
  str[1] = 0x01;
  str[2] = AS608Addr >> 24;
  str[3] = AS608Addr >> 16;
  str[4] = AS608Addr >> 8;
  str[5] = AS608Addr;
  str[6] = 0x07;
  str[7] = '\0';
//  USART1_RX_STA = 0;
	
	HAL_StatusTypeDef error = HAL_UART_Receive(&huart1,(uint8_t *)USART1_RX_BUF,USART1_MAX_RECV_LEN,waittime/4);//串口三接收数据

	if(!memcmp(str,USART1_RX_BUF,7))
	{
		  data = strstr((const char*)USART1_RX_BUF, (const char*)str);
      if(data)
        return (uint8_t*)data;
	}
	//
//  while(--waittime)
//  {
//    HAL_Delay(1);
//    {
//      USART1_RX_STA = 0;
//      data = strstr((const char*)USART1_RX_BUF, (const char*)str);
//      if(data)
//        return (uint8_t*)data;
//    }
//  }
  return 0;
}

uint8_t PS_GetImage(void)
{
  uint16_t temp;
  uint8_t  ensure;
  uint8_t  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);
  SendLength(0x03);
  Sendcmd(0x01);
  temp =  0x01 + 0x03 + 0x01;
  SendCheck(temp);
  data = JudgeStr(2000);

  if(data)
    ensure = data[9];
  else
    ensure = 0xff;
  return ensure;
}

uint8_t PS_GenChar(uint8_t BufferID)
{
  uint16_t temp;
  uint8_t  ensure;
  uint8_t  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);
  SendLength(0x04);
  Sendcmd(0x02);
  MYUSART_SendData(BufferID);
  temp = 0x01 + 0x04 + 0x02 + BufferID;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
    ensure = data[9];
  else
    ensure = 0xff;
  return ensure;
}

uint8_t PS_Match(void)
{
  uint16_t temp;
  uint8_t  ensure;
  uint8_t  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);
  SendLength(0x03);
  Sendcmd(0x03);
  temp = 0x01 + 0x03 + 0x03;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
    ensure = data[9];
  else
    ensure = 0xff;
  return ensure;
}

uint8_t PS_Search(uint8_t BufferID, uint16_t StartPage, uint16_t PageNum, SearchResult *p)
{
  uint16_t temp;
  uint8_t  ensure;
  uint8_t  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);
  SendLength(0x08);
  Sendcmd(0x04);
  MYUSART_SendData(BufferID);
  MYUSART_SendData(StartPage >> 8);
  MYUSART_SendData(StartPage);
  MYUSART_SendData(PageNum >> 8);
  MYUSART_SendData(PageNum);
  temp = 0x01 + 0x08 + 0x04 + BufferID
         + (StartPage >> 8) + (uint8_t)StartPage
         + (PageNum >> 8) + (uint8_t)PageNum;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
  {
    ensure = data[9];
    p->pageID   = (data[10] << 8) + data[11];
    p->mathscore = (data[12] << 8) + data[13];
  }
  else
    ensure = 0xff;
  return ensure;
}

uint8_t PS_RegModel(void)
{
  uint16_t temp;
  uint8_t  ensure;
  uint8_t  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);
  SendLength(0x03);
  Sendcmd(0x05);
  temp = 0x01 + 0x03 + 0x05;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
    ensure = data[9];
  else
    ensure = 0xff;
  return ensure;
}

uint8_t PS_StoreChar(uint8_t BufferID, uint16_t PageID)
{
  uint16_t temp;
  uint8_t  ensure;
  uint8_t  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);
  SendLength(0x06);
  Sendcmd(0x06);
  MYUSART_SendData(BufferID);
  MYUSART_SendData(PageID >> 8);
  MYUSART_SendData(PageID);
  temp = 0x01 + 0x06 + 0x06 + BufferID
         + (PageID >> 8) + (uint8_t)PageID;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
    ensure = data[9];
  else
    ensure = 0xff;
  return ensure;
}

uint8_t PS_DeletChar(uint16_t PageID, uint16_t N)
{
  uint16_t temp;
  uint8_t  ensure;
  uint8_t  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);
  SendLength(0x07);
  Sendcmd(0x0C);
  MYUSART_SendData(PageID >> 8);
  MYUSART_SendData(PageID);
  MYUSART_SendData(N >> 8);
  MYUSART_SendData(N);
  temp = 0x01 + 0x07 + 0x0C
         + (PageID >> 8) + (uint8_t)PageID
         + (N >> 8) + (uint8_t)N;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
    ensure = data[9];
  else
    ensure = 0xff;
  return ensure;
}

uint8_t PS_Empty(void)
{
  uint16_t temp;
  uint8_t  ensure;
  uint8_t  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);
  SendLength(0x03);
  Sendcmd(0x0D);
  temp = 0x01 + 0x03 + 0x0D;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
	{
    ensure = data[9];
	}
  else
    ensure = 0xff;
  return ensure;
}

uint8_t PS_WriteReg(uint8_t RegNum, uint8_t DATA)
{
  uint16_t temp;
  uint8_t  ensure;
  uint8_t  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);
  SendLength(0x05);
  Sendcmd(0x0E);
  MYUSART_SendData(RegNum);
  MYUSART_SendData(DATA);
  temp = RegNum + DATA + 0x01 + 0x05 + 0x0E;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
    ensure = data[9];
  else
    ensure = 0xff;
  if(ensure == 0)
    printf("\r\nok！");
  else
    printf("\r\n%s", EnsureMessage(ensure));
  return ensure;
}

uint8_t PS_ReadSysPara(SysPara *p)
{
  uint16_t temp;
  uint8_t  ensure;
  uint8_t  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);
  SendLength(0x03);
  Sendcmd(0x0F);
  temp = 0x01 + 0x03 + 0x0F;
  SendCheck(temp);
  data = JudgeStr(1000);
  if(data)
  {
    ensure = data[9];
    p->PS_max = (data[14] << 8) + data[15];
    p->PS_level = data[17];
    p->PS_addr = (data[18] << 24) + (data[19] << 16) + (data[20] << 8) + data[21];
    p->PS_size = data[23];
    p->PS_N = data[25];
  }
  else
    ensure = 0xff;
  if(ensure == 0x00)
  {
    printf("\r\n=%d", p->PS_max);
    printf("\r\n=%d", p->PS_level);
    printf("\r\n=%x", p->PS_addr);
    printf("\r\n=%d", p->PS_N * 9600);
  }
  else
    printf("\r\n%s", EnsureMessage(ensure));
  return ensure;
}

uint8_t PS_SetAddr(uint32_t PS_addr)
{
  uint16_t temp;
  uint8_t  ensure;
  uint8_t  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);
  SendLength(0x07);
  Sendcmd(0x15);
  MYUSART_SendData(PS_addr >> 24);
  MYUSART_SendData(PS_addr >> 16);
  MYUSART_SendData(PS_addr >> 8);
  MYUSART_SendData(PS_addr);
  temp = 0x01 + 0x07 + 0x15
         + (uint8_t)(PS_addr >> 24) + (uint8_t)(PS_addr >> 16)
         + (uint8_t)(PS_addr >> 8) + (uint8_t)PS_addr;
  SendCheck(temp);
  AS608Addr = PS_addr; 
  data = JudgeStr(2000);
  if(data)
    ensure = data[9];
  else
    ensure = 0xff;
  AS608Addr = PS_addr;
  if(ensure == 0x00)
    printf("\r\nok！");
  else
    printf("\r\n%s", EnsureMessage(ensure));
  return ensure;
}

uint8_t PS_WriteNotepad(uint8_t NotePageNum, uint8_t *Byte32)
{
  uint16_t temp;
  uint8_t  ensure, i;
  uint8_t  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);
  SendLength(36);
  Sendcmd(0x18);
  MYUSART_SendData(NotePageNum);
  for(i = 0; i < 32; i++)
  {
    MYUSART_SendData(Byte32[i]);
    temp += Byte32[i];
  }
  temp = 0x01 + 36 + 0x18 + NotePageNum + temp;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
    ensure = data[9];
  else
    ensure = 0xff;
  return ensure;
}

uint8_t PS_ReadNotepad(uint8_t NotePageNum, uint8_t *Byte32)
{
  uint16_t temp;
  uint8_t  ensure, i;
  uint8_t  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);
  SendLength(0x04);
  Sendcmd(0x19);
  MYUSART_SendData(NotePageNum);
  temp = 0x01 + 0x04 + 0x19 + NotePageNum;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
  {
    ensure = data[9];
    for(i = 0; i < 32; i++)
    {
      Byte32[i] = data[10 + i];
    }
  }
  else
    ensure = 0xff;
  return ensure;
}

uint8_t PS_HighSpeedSearch(uint8_t BufferID, uint16_t StartPage, uint16_t PageNum, SearchResult *p)
{
  uint16_t temp;
  uint8_t  ensure;
  uint8_t  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);
  SendLength(0x08);
  Sendcmd(0x1b);
  MYUSART_SendData(BufferID);
  MYUSART_SendData(StartPage >> 8);
  MYUSART_SendData(StartPage);
  MYUSART_SendData(PageNum >> 8);
  MYUSART_SendData(PageNum);
  temp = 0x01 + 0x08 + 0x1b + BufferID
         + (StartPage >> 8) + (uint8_t)StartPage
         + (PageNum >> 8) + (uint8_t)PageNum;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
  {
    ensure = data[9];
    p->pageID 	= (data[10] << 8) + data[11];
    p->mathscore = (data[12] << 8) + data[13];
  }
  else
    ensure = 0xff;
  return ensure;
}

uint8_t PS_ValidTempleteNum(uint16_t *ValidN)
{
  uint16_t temp;
  uint8_t  ensure;
  uint8_t  *data;
  SendHead();
  SendAddr();
  SendFlag(0x01);
  SendLength(0x03);
  Sendcmd(0x1d);
  temp = 0x01 + 0x03 + 0x1d;
  SendCheck(temp);
  data = JudgeStr(2000);
  if(data)
  {
    ensure = data[9];
    *ValidN = (data[10] << 8) + data[11];
  }
  else
    ensure = 0xff;

  if(ensure == 0x00)
  {
		LCD_Clear(0, 0, 240, 320, WHITE);
		char str[30];
		sprintf(str, "number of models: %d", *ValidN);
		LCD_DrawString(10, 10, str);
  }
  else
    printf("\r\n%s", EnsureMessage(ensure));
  return ensure;
}

uint8_t PS_HandShake(uint32_t *PS_Addr)
{
  SendHead();
  SendAddr();
  MYUSART_SendData(0X01);
  MYUSART_SendData(0X00);
  MYUSART_SendData(0X00);
  HAL_Delay(200);
  if(USART1_RX_STA & 0X8000) 
  {
		
    if(
      USART1_RX_BUF[0] == 0XEF
      && USART1_RX_BUF[1] == 0X01
      && USART1_RX_BUF[6] == 0X07
    )
    {
      *PS_Addr = (USART1_RX_BUF[2] << 24) + (USART1_RX_BUF[3] << 16)
                 + (USART1_RX_BUF[4] << 8) + (USART1_RX_BUF[5]);
      USART1_RX_STA = 0;
      return 0;
    }
    USART1_RX_STA = 0;
  }
  return 1;
}

const char *EnsureMessage(uint8_t ensure)
{
  const char *p;
  return p;
}

void ShowErrMessage(uint8_t ensure)
{
  //OLED_ShowMixedCH(5,0,(uint8_t*)EnsureMessage(ensure));
}


SysPara AS608Para;

int16_t press_FR(void)
{
  SearchResult seach;
  uint8_t ensure;
  char str[20];
  while(1)
  {
		
    ensure = PS_GetImage();
		
    if(ensure == 0x00) 
    {
      ensure = PS_GenChar(CharBuffer1);
      if(ensure == 0x00) 
      {
        ensure = PS_HighSpeedSearch(CharBuffer1, 0, 99, &seach);
        if(ensure == 0x00) 
        {
					LCD_Clear(0, 0, 240, 320, WHITE);
          LCD_DrawString(10, 10, "Verification succeeded.\0");
					LCD_Clear(0, 0, 240, 320, WHITE);
          sprintf(str, " ID:%d score:%d ", seach.pageID, seach.mathscore);
          LCD_DrawString(10, 10, str);
					if (delete_finger != 1)
						door_status = 1;
          HAL_Delay(1500);
					return seach.pageID;
        }
        else
        {
					LCD_Clear(0, 0, 240, 320, WHITE);
          LCD_DrawString(10, 10, "Verification failed.\0");
					re_entry++;
					scan_re_entry(&photoNum);
          HAL_Delay(1500);
					return -1;
        }
      }
      else
			{
				LCD_Clear(0, 0, 240, 320, WHITE);
				LCD_DrawString(10, 10, "Cannot communicate with the module!\0");
				return -1;
			}
      LCD_Clear(0, 0, 240, 320, WHITE);
      LCD_DrawString(10, 10, "Please press your finger.\0");
			return -1;
    }
		else
		{
			LCD_Clear(0, 0, 240, 320, WHITE);
			LCD_DrawString(10, 10, "No response!\0");
			return -1;
		}
  }
  LCD_Clear(0, 0, 240, 320, WHITE	);
	return -1;
}

void Del_FR(uint16_t ID_NUM)
{
  uint8_t  ensure;
  ensure = PS_DeletChar(ID_NUM, 1); 
  if(ensure == 0)
  {
		LCD_Clear(0, 0, 240, 320, WHITE);
		LCD_DrawString(10, 10, "Finger deleted!");
		
  }
  else
    ShowErrMessage(ensure);
  HAL_Delay(1500);
}

void delete_ALL(void)
{
	 uint8_t ensure;
   ensure = PS_Empty(); 
   if(ensure == 0)
   {
			LCD_Clear(0, 0, 240, 320, WHITE);
			LCD_DrawString(10, 10, "Deleted all models\0");
   }
   else
	 {
      ShowErrMessage(ensure);
      HAL_Delay(1500);
   }
}
