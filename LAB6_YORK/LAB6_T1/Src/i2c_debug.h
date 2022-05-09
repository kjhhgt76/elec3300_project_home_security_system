#ifndef __I2C_DEBUG_
#define __I2C_DEBUG_

#include "stm32f1xx_hal.h"
#include "lcd.h"
void i2c_debug_print(HAL_StatusTypeDef state)
{
	if (state == HAL_BUSY)
		LCD_DrawString(60, 150, "HAL_BUSY\0");
	else if (state == HAL_ERROR)
		LCD_DrawString(60, 150, "HAL_ERROR\0");
	else if (state == HAL_TIMEOUT)
		LCD_DrawString(60, 150, "HAL_TIMEOUT\0");
	else if (state == HAL_OK)
		LCD_DrawString(60, 150, "HAL_OK\0");
}
#endif
