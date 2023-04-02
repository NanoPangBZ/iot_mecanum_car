#include "tcp_server.h"

#include "esp_log.h"
#define TAG "tcp_server"

int tcp_server_init( void )
{
    int addr_family = 0;
    int ip_protocol = 0;

    addr_family = AF_INET;
    ip_protocol = IPPROTO_IP;

    int listen_sock =  socket(addr_family, SOCK_STREAM, ip_protocol);
    if(listen_sock < 0) 
    {
        ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
    }
    return 0;
}

