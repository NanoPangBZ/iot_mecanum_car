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

static void start_task( void* param )
{
    for( uint8_t temp = 0 ;temp < 3 ; temp++ )
    {
        bsp_beep_on( 200 );
        vTaskDelay( 120 / portTICK_PERIOD_MS );
        bsp_beep_off();
        vTaskDelay( 100 / portTICK_PERIOD_MS );
    }

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
        NULL
    );

    vTaskStartScheduler();
}