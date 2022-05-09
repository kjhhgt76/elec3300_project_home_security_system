#ifndef __HMC5883_H
#define	__HMC5883_H
#include "stm32f1xx_hal.h"
#include "lcd.h"
extern I2C_HandleTypeDef hi2c1;

void HMC5883L_init(void);
void read_x_y(uint8_t* x_MSB, uint8_t* x_LSB, uint8_t* y_MSB, uint8_t* y_LSB);
#endif