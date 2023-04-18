#include "page_welcome.h"
#include "hardware.h"

#include "FreeRTOS.h"
#include "task.h"

static void init( void )
{
    OLED12864_Show_String( 0 , 0 , "welcome!" , 2 );
}

static void deinit( void )
{
    OLED12864_Clear();
}

static void timer_handler( uint16_t ms )
{
    OLED12864_Clear_Page( 7 );
    OLED12864_Show_Num( 7 , 0 , xTaskGetTickCount() * portTICK_PERIOD_MS / 1000 , 1);
}

const page_t welcome_page = {
    init,
    deinit,
    timer_handler
};

