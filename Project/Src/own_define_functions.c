#include "own_define_functions.h"
#include "as608.h"

//variables for password module
uint8_t key_read;
char password[4];
char password_enter[4];
int door_status = 0; //0=close, 1=open

//variables for SDIO module
FATFS photoFATFS;
FIL photoFILE;
UINT numberofbytes;
BITMAPINFO bmp;
int photoNum = 0;
int re_entry = 0;
uint16_t delete_finger = 0;
char * readPhotoPath[]={"photo1.bmp","photo2.bmp","photo3.bmp","photo4.bmp","photo5.bmp","photo6.bmp","photo7.bmp",
"photo8.bmp","photo9.bmp","photo10.bmp","photo11.bmp","photo12.bmp","photo13.bmp""photo14.bmp","photo15.bmp","photo16.bmp",
"photo17.bmp","photo18.bmp","photo19.bmp","photo20.bmp"};
extern UART_HandleTypeDef huart2;
//variables for camera module
volatile uint8_t Ov7725_vsync ; 



char read_keypad (void){
	/* Make ROW 1 LOW and all other ROWs HIGH */
	HAL_GPIO_WritePin (GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);  // Pull the R1 low
	HAL_GPIO_WritePin (GPIOA, GPIO_PIN_5, GPIO_PIN_SET);  	// Pull the R2 High
	HAL_GPIO_WritePin (GPIOA, GPIO_PIN_6, GPIO_PIN_SET);  	// Pull the R3 High
	HAL_GPIO_WritePin (GPIOA, GPIO_PIN_7, GPIO_PIN_SET);  	// Pull the R4 High
	
	if (HAL_GPIO_ReadPin (GPIOE, GPIO_PIN_5) == RESET){
		HAL_Delay(200);
		return '1';
	}else if (HAL_GPIO_ReadPin (GPIOE, GPIO_PIN_6) == RESET){
		HAL_Delay(200);
		return '2';
	}else if (HAL_GPIO_ReadPin (GPIOB, GPIO_PIN_2) == RESET){
		HAL_Delay(200);
		return '3';
	}else if (HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_12) == RESET){
		HAL_Delay(200);
		return 'A';
	}
	
	/* Make ROW 2 LOW and all other ROWs HIGH */
	HAL_GPIO_WritePin (GPIOA, GPIO_PIN_4, GPIO_PIN_SET);  	// Pull the R1 High
	HAL_GPIO_WritePin (GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);  // Pull the R2 low
	HAL_GPIO_WritePin (GPIOA, GPIO_PIN_6, GPIO_PIN_SET);  	// Pull the R3 High
	HAL_GPIO_WritePin (GPIOA, GPIO_PIN_7, GPIO_PIN_SET);  	// Pull the R4 High
	if (HAL_GPIO_ReadPin (GPIOE, GPIO_PIN_5) == RESET){
		HAL_Delay(200);
		return '4';
	}else if (HAL_GPIO_ReadPin (GPIOE, GPIO_PIN_6) == RESET){
		HAL_Delay(200);
		return '5';
	}else if (HAL_GPIO_ReadPin (GPIOB, GPIO_PIN_2) == RESET){
		HAL_Delay(200);
		return '6';
	}else if (HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_12) == RESET){
		HAL_Delay(200);
		return 'B';
	}
	
	/* Make ROW 3 LOW and all other ROWs HIGH */
  HAL_GPIO_WritePin (GPIOA, GPIO_PIN_4, GPIO_PIN_SET);  	// Pull the R1 High
	HAL_GPIO_WritePin (GPIOA, GPIO_PIN_5, GPIO_PIN_SET);  	// Pull the R2 High
	HAL_GPIO_WritePin (GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);  // Pull the R3 low
	HAL_GPIO_WritePin (GPIOA, GPIO_PIN_7, GPIO_PIN_SET);  	// Pull the R4 High
	if (HAL_GPIO_ReadPin (GPIOE, GPIO_PIN_5) == RESET){
		HAL_Delay(200);
		return '7';
	}else if (HAL_GPIO_ReadPin (GPIOE, GPIO_PIN_6) == RESET){
		HAL_Delay(200);
		return '8';
	}else if (HAL_GPIO_ReadPin (GPIOB, GPIO_PIN_2) == RESET){
		HAL_Delay(200);
		return '9';
	}else if (HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_12) == RESET){
		HAL_Delay(200);
		return 'C';
	}	
		
	/* Make ROW 4 LOW and all other ROWs HIGH */
  HAL_GPIO_WritePin (GPIOA, GPIO_PIN_4, GPIO_PIN_SET);  	// Pull the R1 High
	HAL_GPIO_WritePin (GPIOA, GPIO_PIN_5, GPIO_PIN_SET);  	// Pull the R2 High
	HAL_GPIO_WritePin (GPIOA, GPIO_PIN_6, GPIO_PIN_SET);  	// Pull the R3 High
	HAL_GPIO_WritePin (GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);  // Pull the R4 low
	if (HAL_GPIO_ReadPin (GPIOE, GPIO_PIN_5) == RESET){
		HAL_Delay(200);
		return '*';
	}else if (HAL_GPIO_ReadPin (GPIOE, GPIO_PIN_6) == RESET){
		HAL_Delay(200);
		return '0';
	}else if (HAL_GPIO_ReadPin (GPIOB, GPIO_PIN_2) == RESET){
		HAL_Delay(200);
		return '#';
	}else if (HAL_GPIO_ReadPin (GPIOA, GPIO_PIN_12) == RESET){
		HAL_Delay(200);
		return 'D';
	}	
	return 'N';
};


void enterPassword(char* arr){
	LCD_DrawString_Color(10,45,"Press * to delete the previous place", BACKGROUND, BLUE);
	LCD_DrawString_Color(10,78,"Press # after you complete", BACKGROUND, BLUE);

	int i = 0;
	while(i<=4){
		if (i < 4){
			arr[i] = ' ';
		}
		do{
			key_read = read_keypad();
			HAL_Delay(100);
		}while(key_read == 'N');
		if (key_read == '*' && i != 0){
			i--;
			LCD_Clear(100+i*8, 27, 8, 16, BACKGROUND);
			continue;
		}else if (i == 4){
			if (key_read == '#'){
				break;
			}
			continue;
		}else if (key_read != '*' && key_read != '#'){
			arr[i] = key_read;
			LCD_DrawChar_Color(100+i*8, 27, arr[i], BACKGROUND, BLUE);
			i++;
		}
	}
};

void verifyPassword(const char* password, char* password_enter, int* photoNum, int* door_status){
	while(1){
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0,GPIO_PIN_SET);	//green off
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5,GPIO_PIN_RESET);	//red on
		//refresh the lcd
		LCD_Clear(0,0,240,320, BACKGROUND);
		LCD_DrawString_Color(10,10,"Please enter your password :", BACKGROUND, BLUE);
		
		enterPassword(password_enter); //enter password
		
		if (strncmp(password, password_enter,sizeof(password)) == 0){
			LCD_DrawString_Color(10,200,"Correct password. Door is unlocked.", BACKGROUND, BLUE);

			*door_status = 1;
			break;
		}else{
			int i;
			for(i = 0; i<4; i++){password_enter[i] = ' ';}
			LCD_DrawString_Color(10,150,"Incorrect password. Please re-enter the password again.", BACKGROUND, BLUE);
			re_entry++;
			int photo_taken = scan_re_entry(photoNum);
			HAL_Delay(1000);
			if (photo_taken == 0){
				break;
			}
		}
	}
};

int scan_re_entry(int* photoNum){
	if (re_entry > 2){
		//start creating next file
		uint8_t thief_alarm[]="Thief detected!\n";
		HAL_UART_Transmit(&huart2,thief_alarm,sizeof(thief_alarm)-1,100);
		if(newbmp() == 0){
			LCD_DrawString(10, 220, readPhotoPath[*photoNum]);
			LCD_DrawString(10+8*strlen(readPhotoPath[*photoNum]),220," is created");
			if (*photoNum == 19){
				*photoNum = 0;
			}else{
				*photoNum += 1;
			}
			LCD_REG_Config();
		}
		re_entry = 0;
		return 0;
	}
	return 1;
};

int newbmp(void){
	__HAL_RCC_FSMC_CLK_DISABLE();
	while(Ov7725_Init() != SUCCESS); //initialize camera ov7725; if any error, loop forever
	Ov7725_vsync = 0;   //for detecting if camera finish capturing; if yes, it becomes 2 as detecting two failing edge in VSYNC
	if (f_mount(&photoFATFS,SDPath,1) == FR_OK){ // if sd card is successfully detected.
		const char * photoPath = readPhotoPath[photoNum];
		FRESULT error = f_open(&photoFILE, photoPath, FA_CREATE_ALWAYS | FA_WRITE);
		if (error == FR_OK ){
			bmp.bmfHeader.bfType =  0x4D42 ; 			//bmp type "BMP" 
			bmp.bmfHeader.bOffBits = sizeof(bmp.bmfHeader) + sizeof(bmp.bmiHeader) + sizeof(bmp.RGB_MASK); ; 				//The number of bytes occupied by the bitmap information structure
			bmp.bmfHeader.bfSize = bmp.bmfHeader.bOffBits + 320 * 240 * 2; 	//File size (information structure + pixel data) 
			bmp.bmfHeader.bfReserved1 =  0x0000 ; 		//Reserved, must be 0 
			bmp.bmfHeader.bfReserved2 =  0x0000;   			
			
			bmp.bmiHeader.biSize = sizeof(bmp.bmiHeader) ;   			//bitmap information header size of width
			bmp.bmiHeader.biWidth = 320. ;   		//bitmap 
			bmp.bmiHeader.biHeight = 240 ;   		//image height 
			bmp.bmiHeader.biPlanes = 1 ;   			//set another target level must be 1 
			bmp.bmiHeader.biBitCount = 16 ;         //bits per pixel 
			bmp.bmiHeader.biCompression = 3;   	//RGB565 format 
			bmp.bmiHeader.biSizeImage = 320 * 240 * 2 ; //The number of bytes occupied by the actual bitmap (only the bitmap pixel data is considered) 
			bmp.bmiHeader.biXPelsPerMeter = 0 ; 	//horizontal resolution 
			bmp.bmiHeader.biYPelsPerMeter = 0 ;  	//vertical resolution 
			bmp.bmiHeader.biClrImportant = 0 ;    	//color index number of the image display described have an important influence, 0 represents all colors as important 
			bmp.bmiHeader.biClrUsed = 0 ;   
			
			bmp.RGB_MASK[0] = 0X00F800;
			bmp.RGB_MASK[1] = 0X0007E0;
			bmp.RGB_MASK[2] = 0X00001F;

			if (f_write(&photoFILE, &bmp , sizeof(bmp), &numberofbytes) == FR_OK){
				FIFO_PREPARE; //ready for FIFO
				uint16_t i, j;	
				uint16_t Camera_Data;
					
				//if (Ov7725_vsync == 2){
					for (i = 0; i < 240; i++)
					{
						for (j = 0; j < 320; j++)
						{
								READ_FIFO_PIXEL(Camera_Data); //Read a pixel in RGB565 from camera
								f_write(&photoFILE, &Camera_Data, sizeof(Camera_Data), &numberofbytes); // write the raw imagedata.
						}
					}
					HAL_Delay(1000);
					Ov7725_vsync = 0;
				//}
				f_close(&photoFILE);
				__HAL_RCC_FSMC_CLK_ENABLE();
				return 0;
			}else{
				return 1;
			}
		}else{
			return 1;
		}
	}else{
		return 1;
	}
};

void readbmp(void){
	if (f_mount(&photoFATFS,SDPath,1) == FR_OK){ // if sd card is successfully detected.
		int readPhotoNum = 0;

		while(1){
			if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == SET){	//if k1 press, back to home page
				break;
			}
			if (readPhotoNum == 0 || HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == SET){		//if k2 press, next image
				
				FRESULT error = f_open(&photoFILE, readPhotoPath[readPhotoNum], FA_READ);
				if ( error == FR_OK ){
					bmp.bmfHeader.bfType =  0x4D42 ; 			//bmp type "BMP" 
					bmp.bmfHeader.bOffBits = sizeof(bmp.bmfHeader) + sizeof(bmp.bmiHeader) + sizeof(bmp.RGB_MASK); ; 				//The number of bytes occupied by the bitmap information structure
					bmp.bmfHeader.bfSize = bmp.bmfHeader.bOffBits + 320 * 240 * 2; 	//File size (information structure + pixel data) 
					bmp.bmfHeader.bfReserved1 =  0x0000 ; 		//Reserved, must be 0 
					bmp.bmfHeader.bfReserved2 =  0x0000;   			
					
					bmp.bmiHeader.biSize = sizeof(bmp.bmiHeader) ;   			//bitmap information header size of width
					bmp.bmiHeader.biWidth = 320. ;   		//bitmap 
					bmp.bmiHeader.biHeight = 240 ;   		//image height 
					bmp.bmiHeader.biPlanes = 1 ;   			//set another target level must be 1 
					bmp.bmiHeader.biBitCount = 16 ;         //bits per pixel 
					bmp.bmiHeader.biCompression = 3;   	//RGB565 format 
					bmp.bmiHeader.biSizeImage = 320 * 240 * 2 ; //The number of bytes occupied by the actual bitmap (only the bitmap pixel data is considered) 
					bmp.bmiHeader.biXPelsPerMeter = 0 ; 	//horizontal resolution 
					bmp.bmiHeader.biYPelsPerMeter = 0 ;  	//vertical resolution 
					bmp.bmiHeader.biClrImportant = 0 ;    	//color index number of the image display described have an important influence, 0 represents all colors as important 
					bmp.bmiHeader.biClrUsed = 0 ;   
					
					bmp.RGB_MASK[0] = 0X00F800;
					bmp.RGB_MASK[1] = 0X0007E0;
					bmp.RGB_MASK[2] = 0X00001F;
					if (f_read(&photoFILE, &bmp , sizeof(bmp), &numberofbytes) == FR_OK){
						uint16_t i, j;	
						uint16_t Photo_Data;
						LCD_Cam_Gram(); //function in lcd.c
						for (i = 0; i < 240; i++)
						{
							for (j = 0; j < 320; j++)
							{
								f_read(&photoFILE, &Photo_Data, sizeof(Photo_Data), &numberofbytes); // read the file imagedata.
								LCD_Write_Data(Photo_Data);
							}
						}
						f_close(&photoFILE);
						HAL_Delay(1500);
						LCD_DrawString(10, 180, readPhotoPath[readPhotoNum]);
						LCD_DrawString(10, 200, "Press K1 back to home page");
						LCD_DrawString(10, 220, "Press K2 to next image");
						if(readPhotoNum == 19){
							readPhotoNum = 0;
						}else{
							readPhotoNum += 1;
						}
					}else{
						LCD_Clear(0,0,240,320,BACKGROUND);
						LCD_DrawString(10, 100, "bmp file corrupted");
						break;
					}
				}else{
					if (readPhotoNum == 0){
						LCD_Clear(0,0,240,320,BACKGROUND);
					}else{
						LCD_Clear(0,0,320,240, BACKGROUND);
					}
					LCD_DrawString(30, 100, readPhotoPath[readPhotoNum]);
					LCD_DrawString(30+8*strlen(readPhotoPath[readPhotoNum]),100," doesn't existed");
					LCD_DrawString(60,120,"Back to homepage...");
					break;
				}
			}
		}
	}else{
		LCD_Clear(0,0,240,320,BACKGROUND);
		LCD_DrawString(10, 100, "Can't detect SD card");
	}
	HAL_Delay(1000);
};

void CO_detect(void){
	char str[20] = {0};
	uint32_t sensor_val = HAL_ADC_GetValue(&hadc1);
	sprintf(str, "%u",sensor_val);
	if (sensor_val >= 900 && sensor_val < 1020){
		uint8_t abnormal_CO[] = "Abnormal CO level detected\n";
		LCD_Clear(0,0,240,320, BACKGROUND);
		LCD_DrawString(10,10, (char*)abnormal_CO);
		LCD_DrawString(10,30,str);
		HAL_UART_Transmit(&huart2,abnormal_CO ,sizeof(abnormal_CO)-1,100);
		HAL_Delay(3000);
		LCD_homepage();
	}else if (sensor_val >= 1020 && sensor_val < 3500){
		LCD_Clear(0,0,240,320, BACKGROUND);
		uint8_t dangerous_CO[] = "Dangerous CO level detected\n";
		LCD_DrawString(10,10,(char*)dangerous_CO);
		LCD_DrawString(10,30,str);
		HAL_UART_Transmit(&huart2,dangerous_CO ,sizeof(dangerous_CO)-1,100);
		HAL_Delay(3000);
		LCD_homepage();
	}
}

void check_homepage(void){
	strType_XPT2046_Coordinate strDisplayCoordinate;
	
	if ( XPT2046_Get_TouchedPoint ( & strDisplayCoordinate, & strXPT2046_TouchPara ) )
	{
		if ( ( strDisplayCoordinate .x > 5 ) && ( strDisplayCoordinate .x < 235 ) ){
			if ( ( strDisplayCoordinate .y > 30 ) && ( strDisplayCoordinate .y < 60 ) ){					//1st button
				LCD_Clear(0, 0, 240, 320, BACKGROUND);
				LCD_DrawString_Color(10,10,"Please set a password with 4 places :", BACKGROUND, BLUE);
				enterPassword(password);
				LCD_DrawString_Color(10,150,"Password successfully set", BACKGROUND, BLUE);
			}else if ( ( strDisplayCoordinate .y > 70 ) && ( strDisplayCoordinate .y < 100 ) ){	  //2nd button
				//function to set new fingerprint
				LCD_Clear(0, 0, 240, 320, BACKGROUND);
				FR_state = 1;
				LCD_DrawString(10,10,"Please press your finger");
				while(FR_state == 1){
				}
				if (FR_state == 2)
				{
					LCD_DrawString(10,10,"Please press your finger again");
				}
				while(FR_state == 2){
				}
			}else if ( ( strDisplayCoordinate .y > 110 ) && ( strDisplayCoordinate .y < 140 ) ){	//3rd button
				//function to delete fingerprint
				LCD_Clear(0, 0, 240, 320, BACKGROUND);
				LCD_DrawString(10, 10, "Please press the finger\0");
				delete_finger = 1;
				while(delete_finger == 1){
					LCD_DrawChar(10,100,' ');
				}
				
			}else if ( ( strDisplayCoordinate .y > 150 ) && ( strDisplayCoordinate .y < 180 ) ){	//4th button
				//function to read SD card
				readbmp();
				LCD_REG_Config(); //reset LCD direction
			}
		}
		HAL_Delay(1500);
		LCD_homepage();
	}
};
