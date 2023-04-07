#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lwip/sockets.h"

#define TCP_SERVER_ERROR    -1
#define TCP_SERVER_LISTEN   0
#define TCP_SERVER_RUNNING  1
#define TCP_SERVER_STOP     2

typedef int (*tcp_server_business_t)( int socket , void* user);

class tcp_server{
public:
    tcp_server( tcp_server_business_t business );
    ~tcp_server();

    void start( uint16_t listen_port , uint16_t task_stack_size );
    void stop();
    int  getState();

private:
    TaskHandle_t _taskHandle;
    static void tcp_server_task( void* param );
    tcp_server_business_t _business;

    char addr_str[128];
    int ip_protocol = 0;            //tcp协议
    int keepAlive = 1;              //保活计数
    int keepIdle;
    int keepInterval;
    int keepCount;
    int listen_sock;
    int sock;
    struct sockaddr_storage dest_addr;

    int _state;
};

#endif  //_TCP_SERVER_H_


