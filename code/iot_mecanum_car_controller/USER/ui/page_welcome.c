#include "gui.h"
#include "hardware.h"

#include "FreeRTOS.h"
#include "task.h"

static void init( void )
{
    OLED12864_Show_String( "welcome!" , 0 , 0 , ASSIC_8x6 );
}

static void deinit( void )
{
    OLED12864_Clear();
}

static void timer_handler( uint16_t ms )
{
}

static void event_handler( gui_evt_t evt )
{
    gui_set_page( &home_page );
}

const page_t welcome_page = {
    init,
    deinit,
    timer_handler,
    event_handler
};

