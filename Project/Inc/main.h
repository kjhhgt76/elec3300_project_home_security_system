/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Touchscreen_Pin GPIO_PIN_2
#define Touchscreen_GPIO_Port GPIOE
#define TouchscreenE3_Pin GPIO_PIN_3
#define TouchscreenE3_GPIO_Port GPIOE
#define TouchscreenE4_Pin GPIO_PIN_4
#define TouchscreenE4_GPIO_Port GPIOE
#define TouchscreenE4_EXTI_IRQn EXTI4_IRQn
#define keypad_C1_Pin GPIO_PIN_5
#define keypad_C1_GPIO_Port GPIOE
#define keypad_C2_Pin GPIO_PIN_6
#define keypad_C2_GPIO_Port GPIOE
#define Debugger_Pin GPIO_PIN_13
#define Debugger_GPIO_Port GPIOC
#define Camera_OE_Pin GPIO_PIN_1
#define Camera_OE_GPIO_Port GPIOC
#define Camera_VS_Pin GPIO_PIN_3
#define Camera_VS_GPIO_Port GPIOC
#define Camera_VS_EXTI_IRQn EXTI3_IRQn
#define key_1_on_board_Pin GPIO_PIN_0
#define key_1_on_board_GPIO_Port GPIOA
#define Buzzer_Pin GPIO_PIN_1
#define Buzzer_GPIO_Port GPIOA
#define keypad_R1_Pin GPIO_PIN_4
#define keypad_R1_GPIO_Port GPIOA
#define keypad_R2_Pin GPIO_PIN_5
#define keypad_R2_GPIO_Port GPIOA
#define keypad_R3_Pin GPIO_PIN_6
#define keypad_R3_GPIO_Port GPIOA
#define keypad_R4_Pin GPIO_PIN_7
#define keypad_R4_GPIO_Port GPIOA
#define Camera_WRST_Pin GPIO_PIN_4
#define Camera_WRST_GPIO_Port GPIOC
#define Camera_RCLK_Pin GPIO_PIN_5
#define Camera_RCLK_GPIO_Port GPIOC
#define RGB_G_for_door_Pin GPIO_PIN_0
#define RGB_G_for_door_GPIO_Port GPIOB
#define GPIO_for_fire_sensor_Pin GPIO_PIN_1
#define GPIO_for_fire_sensor_GPIO_Port GPIOB
#define GPIO_for_fire_sensor_EXTI_IRQn EXTI1_IRQn
#define keypad_C3_Pin GPIO_PIN_2
#define keypad_C3_GPIO_Port GPIOB
#define Camera_D2_Pin GPIO_PIN_10
#define Camera_D2_GPIO_Port GPIOB
#define Camera_D3_Pin GPIO_PIN_11
#define Camera_D3_GPIO_Port GPIOB
#define Camera_D4_Pin GPIO_PIN_12
#define Camera_D4_GPIO_Port GPIOB
#define Camera_D5_Pin GPIO_PIN_13
#define Camera_D5_GPIO_Port GPIOB
#define Camera_D6_Pin GPIO_PIN_14
#define Camera_D6_GPIO_Port GPIOB
#define Camera_D7_Pin GPIO_PIN_15
#define Camera_D7_GPIO_Port GPIOB
#define LCD__BL_Pin GPIO_PIN_12
#define LCD__BL_GPIO_Port GPIOD
#define TouchscreenD13_Pin GPIO_PIN_13
#define TouchscreenD13_GPIO_Port GPIOD
#define Camera_SCL_Pin GPIO_PIN_6
#define Camera_SCL_GPIO_Port GPIOC
#define Camera_SDA_Pin GPIO_PIN_7
#define Camera_SDA_GPIO_Port GPIOC
#define fingerprint_Pin GPIO_PIN_8
#define fingerprint_GPIO_Port GPIOA
#define fingerprint_EXTI_IRQn EXTI9_5_IRQn
#define Camera_RRST_Pin GPIO_PIN_11
#define Camera_RRST_GPIO_Port GPIOA
#define keypad_C4_Pin GPIO_PIN_12
#define keypad_C4_GPIO_Port GPIOA
#define Camera_WEN_Pin GPIO_PIN_3
#define Camera_WEN_GPIO_Port GPIOD
#define RGB_R_for_door_Pin GPIO_PIN_5
#define RGB_R_for_door_GPIO_Port GPIOB
#define camera_d0_Pin GPIO_PIN_6
#define camera_d0_GPIO_Port GPIOB
#define camera_d1_Pin GPIO_PIN_7
#define camera_d1_GPIO_Port GPIOB
#define TouchscreenE0_Pin GPIO_PIN_0
#define TouchscreenE0_GPIO_Port GPIOE
#define LCD_RST_Pin GPIO_PIN_1
#define LCD_RST_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern ADC_HandleTypeDef hadc3;

extern SD_HandleTypeDef hsd;

extern SRAM_HandleTypeDef hsram1;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
