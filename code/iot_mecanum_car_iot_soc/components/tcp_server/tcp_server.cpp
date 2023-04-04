#include "tcp_server.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

void tcp_server::tcp_server_task( void* param )
{
    tcp_server* instance = (tcp_server*)param;
    int err = listen( instance->listen_sock, 1);
    while(1)
    {

    }
}

tcp_server::tcp_server( uint16_t port )
{
    struct sockaddr_in *dest_addr_ip4 = (struct sockaddr_in *)&dest_addr;
    dest_addr_ip4->sin_addr.s_addr = htonl(INADDR_ANY);
    dest_addr_ip4->sin_family = AF_INET;
    dest_addr_ip4->sin_port = htons(port);
    ip_protocol = IPPROTO_IP;

    int listen_sock = socket( AF_INET , SOCK_STREAM, ip_protocol);

    //建立任务
    xTaskCreate(tcp_server_task, "tcp_server", 4096, (void*)this, 5, NULL);
}

tcp_server::~tcp_server()
{

}
