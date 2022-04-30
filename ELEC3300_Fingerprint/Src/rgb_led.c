#include "rgb_led.h"
#include "stm32f1xx_hal.h"
void RGB_green_on(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
}
void RGB_blue_on(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
}
void RGB_red_on(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
}
void RGB_green_toggle(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, !HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0));
}
void RGB_blue_toggle(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, !HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1));
}
void RGB_red_toggle(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, !HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5));
}
