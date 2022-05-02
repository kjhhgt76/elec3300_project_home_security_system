/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "as608.h"
#include "rgb_led.h"
#include "lcd.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */ 
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line[9:5] interrupts.
  */
void EXTI9_5_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI9_5_IRQn 0 */
	if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_8) != RESET)
	{
		if (FR_state == 0)
		{
			LCD_Clear(0, 0, 240, 320, WHITE);
			LCD_DrawString(10, 10, "Detected finger!\0");
			press_FR();
		}
		else if (FR_state == 1)
		{
			uint8_t ensure = 0;
			LCD_Clear(0, 0, 240, 320, WHITE);
			LCD_DrawString(10, 10, "First press\0");
      ensure = PS_GetImage();
      if(ensure == 0x00)
      {
        ensure = PS_GenChar(CharBuffer1);
        if(ensure == 0x00)
        {
					LCD_Clear(0, 0, 240, 320, WHITE);
					LCD_DrawString(10, 10, "First press succeeded.\0");
					FR_state = 2;
        }
        else
				{
					LCD_Clear(0, 0, 240, 320, WHITE);
					LCD_DrawString(10, 10, "First press failed.\0");
					FR_state = 0;
				}
      }
      else 
			{
				LCD_Clear(0, 0, 240, 320, WHITE);
				LCD_DrawString(10, 10, "Response error\0");
				FR_state = 0;
			}
			
		}
		else if (FR_state == 2)
		{
			uint8_t ensure = 0;
			LCD_Clear(0, 0, 240, 320, WHITE);
			LCD_DrawString(10, 10, "Second press\0");
			ensure = PS_GetImage();
			 if(ensure == 0x00)
      {
        ensure = PS_GenChar(CharBuffer2);
        if(ensure == 0x00)
        {
					LCD_Clear(0, 0, 240, 320, WHITE);
					LCD_DrawString(10, 10, "Second press succeeded.\0");
					LCD_Clear(0, 0, 240, 320, WHITE);
					LCD_DrawString(10, 10, "Start matching two fingerprints.\0");
					ensure = PS_Match();
					if(ensure == 0x00)
					{
						LCD_Clear(0, 0, 240, 320, WHITE);
						LCD_DrawString(10, 10, "Matching succeeded.\0");
						LCD_Clear(0, 0, 240, 320, WHITE);
						LCD_DrawString(10, 10, "Start generating model.\0");
						ensure = PS_RegModel();
						if (ensure == 0x00)
						{
							LCD_Clear(0, 0, 240, 320, WHITE);
							LCD_DrawString(10, 10, "Model generated.\0");
							LCD_Clear(0, 0, 240, 320, WHITE);
							LCD_DrawString(10, 10, "Start saving model.\0");
							ensure = PS_StoreChar(CharBuffer2, current_id);
							if(ensure == 0x00)
							{
								char str[50];
								LCD_Clear(0, 0, 240, 320, WHITE);
								sprintf(str, "Successfully saved model, id: %d", current_id++);
								LCD_DrawString(10, 10, str);
								
							}
							else
							{
								LCD_Clear(0, 0, 240, 320, WHITE);
								LCD_DrawString(10, 10, "Saving model failed.\0");
							}
						}
						else
						{
							LCD_Clear(0, 0, 240, 320, WHITE);
							LCD_DrawString(10, 10, "Model generation failed.\0");
						}
					}
					else
					{
						LCD_Clear(0, 0, 240, 320, WHITE);
						LCD_DrawString(10, 10, "Matching failed.\0");
					}
					HAL_Delay(500);
        }
        else
				{
					LCD_Clear(0, 0, 240, 320, WHITE);
					LCD_DrawString(10, 10, "Second press failed.\0");
				}
      }
      else 
			{
				LCD_Clear(0, 0, 240, 320, WHITE);
				LCD_DrawString(10, 10, "Response error\0");
			}
			FR_state = 0;
		}
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_8);
		HAL_GPIO_EXTI_Callback(GPIO_PIN_8);
	}
  /* USER CODE END EXTI9_5_IRQn 0 */
  //HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
  /* USER CODE BEGIN EXTI9_5_IRQn 1 */

  /* USER CODE END EXTI9_5_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
