#include "rgb_led.h"
#include "stm32f1xx_hal.h"
inline void RGB_green_on(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
}
inline void RGB_blue_on(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
}
inline void RGB_red_on(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
}
inline void RGB_green_toggle(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, !HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0));
}
inline void RGB_blue_toggle(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, !HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1));
}
inline void RGB_red_toggle(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, !HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5));
}
inline void RGB_green_off(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
}
inline void RGB_blue_off(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
}
inline void RGB_red_off(void)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
}