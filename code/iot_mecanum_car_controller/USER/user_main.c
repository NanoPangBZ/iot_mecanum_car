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
    motion_control_start();
    hmi_start();

    beep_notice( BEEP_SYS_INIT_FINNISH );

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