#include "bsp.h"

void bsp_init( void )
{
    SysLed::sysLed.setFre( 1 , 100 );
    SysLed::sysLed.start();
    bsp_wifi_init();
    bsp_wifi_connect( "MOSS(4316)" , "4316yyds" );
    bsp_wifi_connect( "Redmi Note 11T Pro" , "09296666" );
    bsp_wifi_connect( "MOSS(4316)" , "4316yyds" );
    bsp_wifi_connect( "Redmi Note 11T Pro" , "09296666" );
    bsp_wifi_connect( "MOSS(4316)" , "4316yyds" );
}
