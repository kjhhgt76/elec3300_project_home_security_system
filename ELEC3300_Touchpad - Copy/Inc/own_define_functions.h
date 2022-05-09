#ifndef __OWN_DEFINE_FUNCTIONS_H
#define __OWN_DEFINE_FUNCTIONS_H

#include "lcd_own.h"
#include "fatfs.h"
#include "bsp_ov7725.h"
#include "bsp_sccb.h"
#include "bmp.h"
#include "xpt2046.h"
#include "string.h"
#include "math.h"
#include "main.h"

extern int re_entry;

//variables for password module
extern uint8_t key_read;
extern char password[4];
extern char password_enter[4];
extern int door_status;
extern uint16_t delete_finger;
//variables for SDIO module
extern int photoNum;

//variables for camera module
extern volatile uint8_t Ov7725_vsync ;

char read_keypad (void);		       //function to read keypad
void enterPassword(char*);		       //function to set and enter password
void verifyPassword(const char*, char*, int*, int*); //function to verify password & following actions
int scan_re_entry(int*);					//function to scan re_rntry and take photo
int newbmp(void); 		       	       //function to write new bmp
void readbmp(void);			       //function to read and show bmp in SD card

void CO_detect(void);			       //function to detect CO

void check_homepage(void);		       //function to check homepage button press

#endif
