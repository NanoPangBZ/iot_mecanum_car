#include "stm32f1xx_hal.h"

#include "tim.h"

#include "oled12864.h"
#include "debug_log.h"
#include <stdio.h>

#include "bsp_uart.h"

#include "FreeRTOS.h"
#include "task.h"

void test_task(void*parm)
{
	OLED12864_Init();
    HAL_GPIO_WritePin( MT3_DIR1_GPIO_Port , MT3_DIR1_Pin , 0 );
    HAL_GPIO_WritePin( MT3_DIR2_GPIO_Port , MT3_DIR2_Pin , 1 );
    htim5.Instance->CCR3 = htim5.Instance->ARR ;
    HAL_TIM_PWM_Start( &htim5 ,  TIM_CHANNEL_3 );
    HAL_TIM_Encoder_Start( &htim8 , TIM_CHANNEL_ALL );
    while(1)
    {
        // INFO_LOG( __FILE__ , "as" );
        // vTaskDelay( 1000 / portTICK_PERIOD_MS  );
        HAL_GPIO_WritePin( TICK_LED_GPIO_Port , TICK_LED_Pin , 0 );
        vTaskDelay( 80 / portTICK_PERIOD_MS );
        HAL_GPIO_WritePin( TICK_LED_GPIO_Port , TICK_LED_Pin , 1 );
        vTaskDelay( 500 / portTICK_PERIOD_MS );
		OLED12864_Show_String(  0 , 3 , "HelloWorld!" , 2 );

    }
}

TaskHandle_t _handle = NULL;

void user_main()
{
    OLED12864_Init();
    HAL_GPIO_WritePin( MT3_DIR1_GPIO_Port , MT3_DIR1_Pin , 0 );
    HAL_GPIO_WritePin( MT3_DIR2_GPIO_Port , MT3_DIR2_Pin , 1 );
    htim5.Instance->CCR3 = htim5.Instance->ARR ;
    HAL_TIM_PWM_Start( &htim5 ,  TIM_CHANNEL_3 );
    HAL_TIM_Encoder_Start( &htim8 , TIM_CHANNEL_ALL );

    while(1)
    {
        HAL_Delay( 20 );
        OLED12864_Show_Num( 0 , 0 , TIM8->CNT , 1 );
        TIM8->CNT = 0;
    }

    // xTaskCreate(
    //     test_task,
    //     NULL,
    //     1024,
    //     "HelloWorld",
    //     15,
    //     &_handle
    //  );

    // vTaskStartScheduler();

	// HAL_GPIO_WritePin( TICK_LED_GPIO_Port , TICK_LED_Pin , 0 );
	
}