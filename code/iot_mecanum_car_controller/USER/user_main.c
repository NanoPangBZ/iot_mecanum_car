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
#include "hmi.h"

#include "FreeRTOS.h"
#include "task.h"

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
    hmi_start();
    vTaskDelete( NULL );
}

void user_main()
{
	bsp_init();
    hardware_init();

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