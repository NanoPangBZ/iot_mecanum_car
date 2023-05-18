#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "bsp.h"
#include "TcpServer.h"
#include "scp_trans.h"

#include "esp_log.h"
#define TAG "main"

static void uart_send_scp_pack( scp_pack_t* pack );
static SemaphoreHandle_t uart_tx_lock = NULL;

static void tcpApp( int sock )
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
    }
}

static int scp_recieve_respond( scp_pack_t* pack )
{
    return 0;
}

void uart_recieve_decoder_task( void* param )
{
    uint8_t recieve_buf[512];
    uint8_t decoder_buf[512];
    int count = 0;
    scp_pack_t pack = scp_trans_pack_create( decoder_buf , 512 );
    scp_trans_decoder_t decoder = scp_trans_decoder_create( &pack , scp_recieve_respond );
    while(1)
    {
        count = bsp_uart_recieve( recieve_buf , 64 , 20 );
        if( count > 0 )
            scp_trans_decoder_input( &decoder , recieve_buf , count );
    }
}

static void uart_send_scp_pack( scp_pack_t* pack )
{
    xSemaphoreTake( uart_tx_lock , -1 );
    scp_trans_send( pack , bsp_uart_send );
    xSemaphoreGive( uart_tx_lock );
}

extern "C" void app_main(void)
{   
    TcpServer tcpServer;
    SysLed::sysLed.setFre( 1 , 200 );
    SysLed::sysLed.start();

    bsp_wifi_init();
    bsp_uart_init();

    uart_tx_lock = xSemaphoreCreateBinary();

    // if( !bsp_wifi_connect( "MOSS(4316)" , "4316yyds" ) )
    // {
        // tcpServer.start( 144 , tcpApp , 4096 );
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
