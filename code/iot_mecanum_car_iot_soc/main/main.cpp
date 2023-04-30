#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "bsp.h"
#include "TcpServer.h"

#include "esp_log.h"
#define TAG "main"

void tcpApp( int sock )
{
    while(1)
    {
        send(sock, "HelloWorld!\r\n",  strlen("HelloWorld!\r\n") , 0);
        vTaskDelay( 500 / portTICK_PERIOD_MS );
    }
}

extern "C" void app_main(void)
{   
    TcpServer tcpServer;
    SysLed::sysLed.setFre( 1 , 200 );
    SysLed::sysLed.start();

    bsp_wifi_init();
    bsp_wifi_connect( "MOSS(4316)" , "4316yyds" );

    if( !bsp_wifi_connect( "MOSS(4316)" , "4316yyds" ) )
    {
        tcpServer.start( 144 , tcpApp , 4096 );
    }

    while(1)
    {
        vTaskDelay(1);
    }
}
