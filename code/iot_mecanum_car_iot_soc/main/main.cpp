#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "bsp.h"
#include "TcpServer.h"

#include "esp_log.h"
#define TAG "main"

void tcpApp( int sock )
{
    uint8_t buf[512];
    int len = 0;
    while(1)
    {
        //等待客户端数据和检查是否在线
        len = recv( sock , buf , 512 , 0 );
        if( len < 1 )
        {
            ESP_LOGI( TAG , "client logoff");
            return;
        }

        send(sock, buf ,  len , 0);
        ESP_LOGI( TAG , "Tick" );
        vTaskDelay( 1000 / portTICK_PERIOD_MS );
    }
}

extern "C" void app_main(void)
{   
    TcpServer tcpServer;
    SysLed::sysLed.setFre( 1 , 200 );
    SysLed::sysLed.start();

    bsp_wifi_init();
    bsp_uart_init();

    // if( !bsp_wifi_connect( "MOSS(4316)" , "4316yyds" ) )
    // {
    //     tcpServer.start( 144 , tcpApp , 4096 );
    // }

    // while(1)
    // {
    //     vTaskDelay(1);
    // }

    uint8_t temp[64];

    while(1)
    {
        bsp_uart_send( (uint8_t*)"HelloWorld!" , strlen("HelloWorld!") );
        ESP_LOGI( TAG , "recier func return  code : %d " , bsp_uart_recieve( temp , 60 , 10 ) );
        vTaskDelay( 1000 / portTICK_PERIOD_MS );
    }
}
