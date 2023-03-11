#include <stdio.h>
#include "stm32f1xx_hal.h"

#include "bsp.h"

#include "oled12864.h"

//os
#include "FreeRTOS.h"
#include "task.h"

void test_task(void*parm)
{
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
	bsp_init();
    OLED12864_Init();

	HAL_GPIO_WritePin( MT3_DIR1_GPIO_Port , MT3_DIR1_Pin , 0 );
    HAL_GPIO_WritePin( MT3_DIR2_GPIO_Port , MT3_DIR2_Pin , 1 );
	bsp_pwm_out( 2 , 10000 );

    HAL_GPIO_WritePin( MT4_DIR1_GPIO_Port , MT4_DIR1_Pin , 0 );
    HAL_GPIO_WritePin( MT4_DIR2_GPIO_Port , MT4_DIR2_Pin , 1 );
	bsp_pwm_out( 3 , 10000 );

    HAL_GPIO_WritePin( MT2_DIR1_GPIO_Port , MT2_DIR1_Pin , 0 );
    HAL_GPIO_WritePin( MT2_DIR2_GPIO_Port , MT2_DIR2_Pin , 1 );
	bsp_pwm_out( 0 , 10000 );

    HAL_GPIO_WritePin( MT1_DIR1_GPIO_Port , MT1_DIR1_Pin , 0 );
    HAL_GPIO_WritePin( MT1_DIR2_GPIO_Port , MT1_DIR2_Pin , 1 );
	bsp_pwm_out( 1 , 10000 );

    while(1)
    {
        HAL_Delay( 200 );
        OLED12864_Clear_Page(0);
        HAL_GPIO_TogglePin(TICK_LED_GPIO_Port,TICK_LED_Pin);
        TIM8->CNT = 0;
    }
	
}