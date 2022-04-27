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

//variables for password module
extern uint8_t key_read;
extern char password[4];
extern char password_enter[4];
extern int door_status;

//variables for SDIO module
extern FATFS photoFATFS;
extern FIL photoFILE;
extern UINT numberofbytes;
extern BITMAPINFO bmp;
extern int photoNum;

//variables for camera module
extern volatile uint8_t Ov7725_vsync ;

//variables for CO sensor
extern ADC_HandleTypeDef hadc1;
extern float R0;

char read_keypad (void);		       //function to read keypad
void enterPassword(char*);		       //function to set and enter password
void verifyPassword(const char*, char*, int*, int*); //function to verify password & following actions
//int newbmp(const char*); 		       //function to write new bmp
int newbmp(void); 		       //function to write new bmp
//void createPhotoPath(int*, char*); 	       //function to create next photo path
void readbmp(void);			       //function to read and show bmp in SD card
void COsensor_INIT(float*);		       //function to set up CO sensor
int CO_ppm(float*);			       //function to calculate CO ppm
void showReadings(float*, int*, int*);	       //function to show readings
void check_homepage(void);		       //function to check homepage button press

#endif
