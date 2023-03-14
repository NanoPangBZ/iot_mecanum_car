/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#define OLED_SPI hspi2
#define PWM_TIM htim5
#define BEEP_TIM htim1
#define HALL1_TIM htim2
#define HALL2_TIM htim3
#define HALL3_TIM htim4
#define HALL4_TIM htim8
#define BAT_ADC_Pin GPIO_PIN_0
#define BAT_ADC_GPIO_Port GPIOC
#define KEY_ADC_Pin GPIO_PIN_1
#define KEY_ADC_GPIO_Port GPIOC
#define MT1_DIR1_Pin GPIO_PIN_2
#define MT1_DIR1_GPIO_Port GPIOC
#define MT1_DIR2_Pin GPIO_PIN_3
#define MT1_DIR2_GPIO_Port GPIOC
#define MT_PWM1_Pin GPIO_PIN_0
#define MT_PWM1_GPIO_Port GPIOA
#define MT_PWM2_Pin GPIO_PIN_1
#define MT_PWM2_GPIO_Port GPIOA
#define MT_PWM3_Pin GPIO_PIN_2
#define MT_PWM3_GPIO_Port GPIOA
#define MT_PWM4_Pin GPIO_PIN_3
#define MT_PWM4_GPIO_Port GPIOA
#define MT2_DIR1_Pin GPIO_PIN_4
#define MT2_DIR1_GPIO_Port GPIOA
#define MT2_DIR2_Pin GPIO_PIN_5
#define MT2_DIR2_GPIO_Port GPIOA
#define MT3_DIR1_Pin GPIO_PIN_6
#define MT3_DIR1_GPIO_Port GPIOA
#define MT3_DIR2_Pin GPIO_PIN_7
#define MT3_DIR2_GPIO_Port GPIOA
#define MT4_DIR1_Pin GPIO_PIN_4
#define MT4_DIR1_GPIO_Port GPIOC
#define MT4_DIR2_Pin GPIO_PIN_5
#define MT4_DIR2_GPIO_Port GPIOC
#define CHG_STATE_Pin GPIO_PIN_0
#define CHG_STATE_GPIO_Port GPIOB
#define BOOT1_Pin GPIO_PIN_2
#define BOOT1_GPIO_Port GPIOB
#define OLED_CS_Pin GPIO_PIN_12
#define OLED_CS_GPIO_Port GPIOB
#define OLED_SCK_Pin GPIO_PIN_13
#define OLED_SCK_GPIO_Port GPIOB
#define OLED_DC_Pin GPIO_PIN_14
#define OLED_DC_GPIO_Port GPIOB
#define OLED_MOSI_Pin GPIO_PIN_15
#define OLED_MOSI_GPIO_Port GPIOB
#define HALL4_CH_Pin GPIO_PIN_6
#define HALL4_CH_GPIO_Port GPIOC
#define HALL4_CHN_Pin GPIO_PIN_7
#define HALL4_CHN_GPIO_Port GPIOC
#define TICK_LED_Pin GPIO_PIN_8
#define TICK_LED_GPIO_Port GPIOC
#define ESP_BOOT_Pin GPIO_PIN_9
#define ESP_BOOT_GPIO_Port GPIOC
#define BEEP_PWM_Pin GPIO_PIN_8
#define BEEP_PWM_GPIO_Port GPIOA
#define HALL1_CH_Pin GPIO_PIN_15
#define HALL1_CH_GPIO_Port GPIOA
#define HALL1_CHN_Pin GPIO_PIN_3
#define HALL1_CHN_GPIO_Port GPIOB
#define HALL2_CH_Pin GPIO_PIN_4
#define HALL2_CH_GPIO_Port GPIOB
#define HALL2_CHN_Pin GPIO_PIN_5
#define HALL2_CHN_GPIO_Port GPIOB
#define HALL3_CH_Pin GPIO_PIN_6
#define HALL3_CH_GPIO_Port GPIOB
#define HALL3_CHN_Pin GPIO_PIN_7
#define HALL3_CHN_GPIO_Port GPIOB
#define OLED_RES_Pin GPIO_PIN_8
#define OLED_RES_GPIO_Port GPIOB
#define ESP_RST_Pin GPIO_PIN_9
#define ESP_RST_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
