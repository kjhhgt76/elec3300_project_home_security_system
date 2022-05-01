#include "stm32f1xx_hal.h"
#include "key.h"

inline uint8_t key1_scan(void)
{
	return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET;
}

inline uint8_t key2_scan(void)
{
	return HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_SET;
}
