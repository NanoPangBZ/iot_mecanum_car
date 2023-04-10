#include "bsp.h"

void bsp_init( void )
{
    SysLed::sysLed.setFre( 1 , 100 );
    SysLed::sysLed.start();
    bsp_wifi_init();
}
