#ifndef __AS608_H
#define __AS608_H
#include <stdio.h>
#include <string.h>
#include "stm32f1xx_hal.h"

#define CharBuffer1 0x01
#define CharBuffer2 0x02

#define USART1_MAX_RECV_LEN		400					
#define USART1_MAX_SEND_LEN		400					
#define USART1_RX_EN 			1					

extern uint32_t AS608Addr;
extern UART_HandleTypeDef huart1;

typedef struct  
{
	uint16_t pageID;
	uint16_t mathscore;
}SearchResult;

typedef struct
{
	uint16_t PS_max;
	uint8_t  PS_level;
	uint32_t PS_addr;
	uint8_t  PS_size;
	uint8_t  PS_N;
}SysPara;

extern SysPara AS608Para;
extern uint16_t current_id;
volatile extern uint8_t FR_state;
void PS_StaGPIO_Init(void);
	
uint8_t PS_GetImage(void); 
 
uint8_t PS_GenChar(uint8_t BufferID);

uint8_t PS_Match(void);

uint8_t PS_Search(uint8_t BufferID,uint16_t StartPage,uint16_t PageNum,SearchResult *p);
 
uint8_t PS_RegModel(void);
 
uint8_t PS_StoreChar(uint8_t BufferID,uint16_t PageID);

uint8_t PS_DeletChar(uint16_t PageID,uint16_t N);

uint8_t PS_Empty(void);

uint8_t PS_WriteReg(uint8_t RegNum,uint8_t DATA);
 
uint8_t PS_ReadSysPara(SysPara *p);

uint8_t PS_SetAddr(uint32_t addr); 

uint8_t PS_WriteNotepad(uint8_t NotePageNum,uint8_t *content);

uint8_t PS_ReadNotepad(uint8_t NotePageNum,uint8_t *note); 

uint8_t PS_HighSpeedSearch(uint8_t BufferID,uint16_t StartPage,uint16_t PageNum,SearchResult *p);
  
uint8_t PS_ValidTempleteNum(uint16_t *ValidN);

uint8_t PS_HandShake(uint32_t *PS_Addr); 

const char *EnsureMessage(uint8_t ensure);

void Add_FR(void);

int16_t press_FR(void);

void Del_FR(uint16_t ID_NUM);

void delete_ALL(void);

#endif
