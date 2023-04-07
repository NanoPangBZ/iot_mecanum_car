#include "tcp_server.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

#include "esp_log.h"
#define TAG "tcp_server"

void tcp_server::tcp_server_task( void* param )
{
    while(1)
    {
        vTaskDelay(1);
    }
    vTaskDelete( NULL );

}

tcp_server::tcp_server( tcp_server_business_t business )
{
    _taskHandle = NULL;
    _business = business;
    _state = TCP_SERVER_STOP;
    listen_sock = -1;
    sock = -1;
}

void tcp_server::start( uint16_t listen_port , uint16_t task_stack_size )
{
    if( getState() == TCP_SERVER_STOP )
    {
        struct sockaddr_in *dest_addr_ip4 = (struct sockaddr_in *)&dest_addr;
        dest_addr_ip4->sin_addr.s_addr = htonl(INADDR_ANY);
        dest_addr_ip4->sin_family = AF_INET;
        dest_addr_ip4->sin_port = htons(listen_port);
        ip_protocol = IPPROTO_IP;

        listen_sock = socket( AF_INET , SOCK_STREAM, ip_protocol);

        //建立任务
        xTaskCreate(tcp_server_task, "tcp_server", task_stack_size, (void*)this, 5, &_taskHandle );

        _state = TCP_SERVER_LISTEN;
    }
}

void tcp_server::stop()
{
    if( getState() != TCP_SERVER_STOP )
    {
        vTaskDelete( _taskHandle );
        _taskHandle = NULL;
        lwip_close( listen_sock );
        _state = TCP_SERVER_STOP;
    }
}

int tcp_server::getState()
{
    return -1;
}

tcp_server::~tcp_server()
{
}


