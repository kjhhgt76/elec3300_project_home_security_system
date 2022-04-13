#include "own_define_functions.h"

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
int photoNum = 1;

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
	LCD_DrawString(10,45,"Press * to delete the previous place");
	LCD_DrawString(10,78,"Press # after you complete");

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
			LCD_DrawChar(100+i*8, 27, ' ');
			continue;
		}else if (i == 4){
			if (key_read == '#'){
				break;
			}
			continue;
		}else if (key_read != '*' && key_read != '#'){
			arr[i] = key_read;
			LCD_DrawChar(100+i*8, 27, arr[i]);
			i++;
		}
	}
};

void verifyPassword(const char* password, char* password_enter, int* photoNum, int* door_status){
	int re_entry = 0;
	
	while(1){
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0,GPIO_PIN_SET);	//green off
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5,GPIO_PIN_RESET);	//red on
		//refresh the lcd
		LCD_Clear(0,0,240,320, BACKGROUND);
		LCD_DrawString(10,10,"Please enter your password :");
		
		enterPassword(password_enter); //enter password
		
		if (strncmp(password, password_enter,sizeof(password)) == 0){
			LCD_DrawString(10,100,"Correct password. Door is unlocked.");
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5,GPIO_PIN_SET);	//red off
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);		//green on = door open
			*door_status = 1;
			break;
		}else{
			int i;
			for(i = 0; i<4; i++){password_enter[i] = ' ';}
			LCD_DrawString(10,100,"Incorrect password. Please re-enter the password again.");
			re_entry++;
			if (re_entry > 2){
				// the file path of the image data.
				char photoPath[15];
				createPhotoPath(photoNum, photoPath);
				*photoNum += 1;
				//start creating next file
				if(newbmp(photoPath) == 0){
					LCD_DrawString(10,220,photoPath);
					LCD_DrawString(10+8*strlen(photoPath),220," is created");
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5,GPIO_PIN_SET); // red off = photo taken
					break;
				}
			}
			HAL_Delay(1000);
		}
	}
};

int newbmp(const char* photoPath){
	while(Ov7725_Init() != SUCCESS); //initialize camera ov7725; if any error, loop forever
	Ov7725_vsync = 0;   //for detecting if camera finish capturing; if yes, it becomes 2 as detecting two failing edge in VSYNC
	if (f_mount(&photoFATFS,SDPath,1) == FR_OK){ // if sd card is successfully detected.
		if (f_open(&photoFILE, photoPath, FA_WRITE |FA_CREATE_ALWAYS) == FR_OK ){
			bmp.bmfHeader.bfType =  0x4D42 ; 			//bmp type "BM" 
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
					
				LCD_Cam_Gram(); //function in lcd_own.c to initialize lcd to show the photo
				if (Ov7725_vsync == 2){
					for (i = 0; i < 240; i++)
					{
						for (j = 0; j < 320; j++)
						{
								READ_FIFO_PIXEL(Camera_Data); //Read a pixel in RGB565 from camera
								f_write(&photoFILE, &Camera_Data, sizeof(Camera_Data), &numberofbytes); // write the raw imagedata.
								LCD_Write_Data(Camera_Data);
						}
					}
				}
				f_close(&photoFILE);
				Ov7725_vsync = 0;
				LCD_REG_Config(); //to change the lcd setting back
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

void createPhotoPath(int* photoNum, char* photoPath){
	//convert photoNum to string
	char photoNum_str[3];
	sprintf(photoNum_str, "%d", *photoNum);
	//combine all elements
	const char photoPath_name[5] = "photo";
	const char extension[4] = ".bmp";
	sprintf(photoPath, "%s%s%s",photoPath_name, photoNum_str, extension);
	
};

void check_homepage(void){
		strType_XPT2046_Coordinate strDisplayCoordinate;
	
	if ( XPT2046_Get_TouchedPoint ( & strDisplayCoordinate, & strXPT2046_TouchPara ) )
	{
		if ( ( strDisplayCoordinate .x > 5 ) && ( strDisplayCoordinate .x < 235 ) ){
			if ( ( strDisplayCoordinate .y > 30 ) && ( strDisplayCoordinate .y < 60 ) ){					//1st button
					LCD_Clear(0, 0, 240, 320, WHITE);
					LCD_DrawString(10,10,"Please set a password with 4 places :");
					enterPassword(password);
					LCD_DrawString(10,100,"Password successfully set");
					HAL_Delay(2000);
					LCD_homepage();
			}else if ( ( strDisplayCoordinate .y > 70 ) && ( strDisplayCoordinate .y < 100 ) ){	  //2nd button
				//function to set new fingerprint
			}else if ( ( strDisplayCoordinate .y > 110 ) && ( strDisplayCoordinate .y < 140 ) ){	//3rd button
				//function to delete fingerprint
			}else if ( ( strDisplayCoordinate .y > 150 ) && ( strDisplayCoordinate .y < 180 ) ){	//4th button
				//function to read SD card
			}else if ( ( strDisplayCoordinate .y > 190 ) && ( strDisplayCoordinate .y < 220 ) ){	//5th button
				//function to show readings
			}
		}

	}
};

