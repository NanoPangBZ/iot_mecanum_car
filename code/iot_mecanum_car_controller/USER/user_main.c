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
#include "iot.h"

#include "FreeRTOS.h"
#include "task.h"

#include "elog.h"

static void init_task( void* param )
{
    motion_control_start();
    hmi_start();
    iot_start();

    motion_control_function_disable( YAW_LOCK );
    motion_control_function_disable( TARGET_POSITION );

    // beep_notice( BEEP_SYS_INIT_FINNISH );

    vTaskDelete( NULL );
}

void user_main()
{
	bsp_init();
    hardware_init();

    elog_init();
    elog_set_text_color_enabled(true);
    elog_output_lock_enabled(true);
    for( uint8_t temp = 0 ; temp < 6 ; temp++ )
        elog_set_fmt( temp , ~(ELOG_FMT_P_INFO) );
    elog_start();

    xTaskCreate( 
        init_task ,
        "sys_init_task",
        128,
        NULL,
        15,
        NULL
    );

    vTaskStartScheduler();
}