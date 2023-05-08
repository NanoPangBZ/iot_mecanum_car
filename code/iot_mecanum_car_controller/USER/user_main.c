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

#include "elog.h"

static void init_task( void* param )
{
    motion_control_start();
    hmi_start();

    // beep_notice( BEEP_SYS_INIT_FINNISH );

    // vTaskDelay( 12000 / portTICK_PERIOD_MS );

    // elog_info( "test" , "is a info log" );
    // elog_warn( "test" , "is a warn log" );
    // elog_error( "test" , "is a error log" );
    // elog_debug( "test" , "is a debug log" );
    // elog_flush();

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