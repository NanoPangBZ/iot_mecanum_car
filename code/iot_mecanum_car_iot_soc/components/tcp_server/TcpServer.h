#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include <stdint.h>

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

typedef void (*TcpApp)( int sock );

class TcpServer{
public:
    TcpServer();
    ~TcpServer();
    bool start( uint16_t listen_port , TcpApp app , uint16_t app_stack );
    bool stop();
private:
    static void _serverListen( void* param );
    static void _serverAppBridge(void* param);
    
    TaskHandle_t _listenTask;
    uint16_t _appStack;
    TcpApp _app;
    uint16_t _listenPort;
    int _connectCount;   //连接数量
};

#endif  //_TCP_SERVER_H_

