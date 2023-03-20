#include <stdio.h>
#include <string.h>
#include <math.h>

#include "stm32f1xx_hal.h"
#include "isr.h"
#include "bsp.h"
#include "hardware.h"
#include "alg.h"
#include "protocol.h"

#include "motion_control.h"

#include "FreeRTOS.h"
#include "task.h"

#include "adc.h"

static TaskHandle_t led_taskHandle;
void sys_led_tick(void* param)
{
    while(1)
    {
        LED_OFF();
        vTaskDelay( 800 / portTICK_PERIOD_MS );
        LED_ON();
        vTaskDelay( 60 / portTICK_PERIOD_MS );
    }
}

static void start_task( void* param )
{
    xTaskCreate( 
        sys_led_tick ,
        "sys_led",
        32,
        NULL,
        1,
        &led_taskHandle
    );
    motion_control_start();
    vTaskDelete( NULL );
}

void user_main()
{
	bsp_init();
    hardware_init();

    OLED12864_Show_String( 0 , 0 , "HelloWorld!" , 1 );

	while(1)
	{
		HAL_ADC_Start( &hadc3 );
        while( HAL_ADC_GetState( &hadc3 ) == HAL_BUSY );
        OLED12864_Clear_Page( 0 );
        OLED12864_Show_Num( 0 , 0 , HAL_ADC_GetValue( &hadc3 ) , 1 );
	}
	
    xTaskCreate( 
        start_task ,
        "sys_start",
        128,
        NULL,
        15,
        &led_taskHandle
    );

    vTaskStartScheduler();
}