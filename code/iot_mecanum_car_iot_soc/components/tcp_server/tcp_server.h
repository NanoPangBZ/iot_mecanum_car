#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lwip/sockets.h"

class tcp_server{
public:
    tcp_server( uint16_t port );
    ~tcp_server();

private:
    TaskHandle_t _taskHandle;
    static void tcp_server_task( void* param );

    char addr_str[128];
    int ip_protocol = 0;            //tcp协议
    int keepAlive = 1;              //保活计数
    int keepIdle;
    int keepInterval;
    int keepCount;
    int listen_sock;
    struct sockaddr_storage dest_addr;
};

#endif  //_TCP_SERVER_H_


