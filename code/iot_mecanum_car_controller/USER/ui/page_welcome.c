#include "lori_gui.h"
#include "hardware.h"
#include <stdio.h>
#include <stdlib.h>
#include "motion_control.h"
#include "isr.h"

#include "FreeRTOS.h"
#include "task.h"

static void init( void )
{
    OLED12864_Show_String( "Welcome!" , 0 , 0 , ASSIC_16X8 );
}

static void deinit( void )
{
    OLED12864_Clear();
}

static void timer_handler( uint16_t ms )
{
    static uint8_t w = 16;
    static uint8_t dir = 0;
    uint8_t buf[32];
    sprintf( (char*)buf , "time : %d s" , xTaskGetTickCount() * portTICK_PERIOD_MS / 1000 );
    OLED12864_Show_String( (char*)buf , 0 , 63 - 8 , ASSIC_8x6 );
    sprintf( (char*)buf , "sf yaw : %.1f " , motion_get_yaw() );
    OLED12864_Show_String( (char*)buf , 0 , 16 , ASSIC_8x6 );
    sprintf( (char*)buf , "hd yaw : %.1f " , jy901s_yaw);
    OLED12864_Show_String( (char*)buf , 0 , 32 , ASSIC_8x6 );
}

static void event_handler( gui_evt_t evt )
{
    extern page_t home_page;
    gui_set_page( &home_page );
}

const page_t welcome_page = {
    init,
    deinit,
    timer_handler,
    event_handler
};

