#include "hmc5883.h"
#include "math.h"

uint8_t HMC5883L_Addr = 0x1E; 
uint8_t	CRA = 0x70; 
uint8_t	CRB = 0xA0;
uint8_t	read_mode = 0x01;

void HMC5883L_init(void)
{
	HAL_I2C_Mem_Write(&hi2c1,HMC5883L_Addr<<1,0x00,1,&CRA,1,100);
	HAL_Delay(20);
	HAL_I2C_Mem_Write(&hi2c1,HMC5883L_Addr<<1,0x01,1,&CRB,1,100);
}

int16_t read_x_y(void)
{
		uint8_t x_MSB;
		uint8_t x_LSB;
		uint8_t y_MSB;
		uint8_t y_LSB;
		double input_of_atan;
		int16_t angle = 0;
		HAL_I2C_Mem_Write(&hi2c1,HMC5883L_Addr<<1,0x02,1,&read_mode,1,100);
		HAL_Delay(10);
		HAL_I2C_Mem_Read(&hi2c1,HMC5883L_Addr<<1,0x03,1,&x_MSB,1,100);
		HAL_I2C_Mem_Read(&hi2c1,HMC5883L_Addr<<1,0x04,1,&x_LSB,1,100);
		HAL_I2C_Mem_Read(&hi2c1,HMC5883L_Addr<<1,0x07,1,&y_MSB,1,100);
		HAL_I2C_Mem_Read(&hi2c1,HMC5883L_Addr<<1,0x08,1,&y_LSB,1,100);
		
		int16_t x = (x_MSB<<8) + x_LSB;
		int16_t y = (y_MSB<<8) + y_LSB;
	
		if (x > 0 && y > 0){
			input_of_atan = y*1.0/x;
			angle = atan(input_of_atan) * (180.0 / 3.14159265);
		}else if (x < 0 && y > 0){
			input_of_atan = x/y * -1;
			angle = 90 + atan(input_of_atan) * (180.0 / 3.14159265);
		}else if (x < 0 && y < 0){
			input_of_atan = y/x;
			angle = 180 + atan(input_of_atan) * (180.0 / 3.14159265);
		}else if (x > 0 && y < 0){
			input_of_atan = y/x * -1;
			angle = 359 - atan(input_of_atan) * (180.0 / 3.14159265);
		}
		return angle;
}
