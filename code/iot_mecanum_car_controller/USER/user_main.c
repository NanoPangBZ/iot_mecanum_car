#include <stdio.h>
#include <string.h>
#include <math.h>

#include "stm32f1xx_hal.h"
#include "bsp.h"
#include "hardware.h"
#include "alg.h"
#include "protocol.h"

#include "FreeRTOS.h"
#include "task.h"

float yaw;

static TaskHandle_t led_taskHandle;
void sys_led_tick(void* param)
{
    while(1)
    {
        LED_OFF();
        vTaskDelay( 800 / portTICK_PERIOD_MS );
        LED_ON();
        OLED12864_Show_Num( 7 , 64 , xTaskGetTickCount()/1000 , 1 );
        vTaskDelay( 60 / portTICK_PERIOD_MS );
    }
}

void user_main()
{
	bsp_init();
    hardware_init();

    xTaskCreate( 
        sys_led_tick ,
        "sys_led",
        64,
        NULL,
        1,
        &led_taskHandle
    );

    vTaskStartScheduler();
}