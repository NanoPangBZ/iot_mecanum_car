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

typedef void* (*TcpApp_Init)( int sock );
typedef void (*TcpApp)( int sock , void* user);
typedef void (*TcpApp_Deinit)( int sock , void* user);

class TcpServer{
public:
    TcpServer();
    ~TcpServer();

    /**
     * @brief 启动tcp服务
     * @param listen_port 要监听的端口
     * @param init_func 用户业务逻上下文初始化函数,这个函数需要返回user指针
     * @param app_func 业务逻辑
     * @param deinit_func 业务逻辑逆初始化函数
     * @param app_stack_size 要为每个业务逻辑分配的任务堆栈大小
     * @note    这个接口会在后台启动一个监听任务，当监听到有tcp客户端连接后，
     *          会创建一个用于处理这个客户端请求的任务，这个任务首先会调用初
     *          始化函数，然后进入app_func，当app_func退出后调用逆初始化函数
     *          。这个接口能支持多客户端连接，通过传入的sock区分不同的客户端。
     * @anchor 庞 2023/5/19
    */
    bool start( uint16_t listen_port ,
                TcpApp_Init init_func,
                TcpApp app_func ,
                TcpApp_Deinit deinit_func ,
                uint16_t app_stack_size );

    bool stop();
private:
    static void _serverListen( void* param );
    static void _serverAppBridge(void* param);
    
    typedef struct{
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
        void* user;
    }TcpServerAppCtx;

    TaskHandle_t _listenTask;
    int listen_sock;
    uint16_t _appStackSize;
    TcpApp_Init _appInit;
    TcpApp _app;
    TcpApp_Deinit _appDeinit;
    uint16_t _listenPort;
    int _connectCount;   //连接数量
    std::list<TcpServerAppCtx*> tcpServerAppList;
    SemaphoreHandle_t   appLock;
};

#endif  //_TCP_SERVER_H_

