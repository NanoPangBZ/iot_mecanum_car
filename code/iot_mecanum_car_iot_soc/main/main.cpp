#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "bsp.h"
#include "TcpServer.h"
#include "scp_trans.h"

#include "esp_log.h"
#define TAG "main"

static TcpServer tcpServer;
static void uart_send_scp_pack( scp_pack_t* pack );
static int tcp_send( uint8_t* data , uint16_t len );
static int scp_recieve_respond( scp_pack_t* pack );
static void uart_recieve_decode_task( void* param );
static void send_scp_pack( scp_pack_t* pack );
static SemaphoreHandle_t uart_tx_lock = NULL;
static SemaphoreHandle_t tcp_tx_lock = NULL;
static int _s_sock = -1;

static void tcpApp( int sock )
{
    uint8_t buf[512];
    uint8_t decoder_buf[512];
    scp_pack_t pack = scp_trans_pack_create( decoder_buf , 512 );
    scp_trans_decoder_t decoder = scp_trans_decoder_create( &pack , scp_recieve_respond );
    int len = 0;
    _s_sock = sock;
    while(1)
    {
        //等待客户端数据和检查是否在线
        len = recv( sock , buf , 512 , 0 );
        if( len < 1 )
        {
            ESP_LOGI( TAG , "client logoff");
            return;
        }
        scp_trans_decoder_input( &decoder , buf , len );
        // send(sock, buf ,  len , 0);
        ESP_LOGI( TAG , "Tick" );
    }
}

static int scp_recieve_respond( scp_pack_t* pack )
{
    return 0;
}

static int tcp_send( uint8_t* data , uint16_t len )
{
    if( _s_sock != -1 )
        return send(_s_sock , data , len , 0 );
    return -1;
}

static void uart_recieve_decode_task( void* param )
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

static void send_scp_pack( scp_pack_t* pack )
{
    if( pack->control_word )
    {
        xSemaphoreTake( uart_tx_lock , -1 );
        scp_trans_send( pack , bsp_uart_send );
        xSemaphoreGive( uart_tx_lock );
    }else
    {
        if( _s_sock != -1 )
        {
            xSemaphoreTake( tcp_tx_lock , -1 );
            scp_trans_send( pack , tcp_send );
            xSemaphoreGive( tcp_tx_lock );
        }
    }
}

extern "C" void app_main(void)
{   
    SysLed::sysLed.setFre( 1 , 200 );
    SysLed::sysLed.start();

    bsp_wifi_init();
    bsp_uart_init();

    uart_tx_lock = xSemaphoreCreateBinary();
    tcp_tx_lock = xSemaphoreCreateBinary();

    xTaskCreatePinnedToCore(
        uart_recieve_decode_task,
        "uart rx",
        512,
        NULL,
        13,
        NULL,
        tskNO_AFFINITY
    );

    while( bsp_wifi_connect( "MOSS(4316)" , "4316yyds" ) != 0 )
    {
        ESP_LOGI( TAG , "retry to connect wifi." );
        vTaskDelay( 500 / portTICK_PERIOD_MS );
    }

    ESP_LOGI( TAG , "wifi connected , wait tcp client connect" );
    tcpServer.start( 144 , tcpApp , 4096 );
}
