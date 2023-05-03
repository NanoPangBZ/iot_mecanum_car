#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include <stdint.h>
#include <list>

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

class TcpServer;

typedef void (*TcpApp)( int sock );

typedef struct{
    TcpApp _app;
    int sock;
    int keepAlive;
    int keepIdle;
    int keepInterval;
    int keepCount;
    TaskHandle_t taskHandle;
    TcpServer*  server;
    struct sockaddr_in peerAddr;
    int peer_len;
    char ipAddrStr[INET_ADDRSTRLEN];
}TcpServerAppCtx;

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
    int listen_sock;
    uint16_t _appStack;
    TcpApp _app;
    uint16_t _listenPort;
    int _connectCount;   //连接数量
    std::list<TcpServerAppCtx*> tcpServerAppList;
    SemaphoreHandle_t   appLock;
};

#endif  //_TCP_SERVER_H_

