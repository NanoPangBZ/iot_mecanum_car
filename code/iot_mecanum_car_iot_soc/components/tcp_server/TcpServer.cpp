#include "TcpServer.h"
#include "esp_log.h"

#define TAG "TcpServer"

#define KEEPALIVE_IDLE              1
#define KEEPALIVE_INTERVAL          1
#define KEEPALIVE_COUNT             10

//tcp服务器的监听任务
void TcpServer::_serverListen( void* param )
{
    TcpServer* tcpServer = (TcpServer*)param;
    ESP_LOGI( TAG , "tcp server listen task running!" );
    ESP_LOGI( TAG , "listen port:%d" , tcpServer->_listenPort );

    int* listen_sock = &tcpServer->listen_sock;
    struct sockaddr_storage dest_addr;

    struct sockaddr_in *dest_addr_ip4 = (struct sockaddr_in *)&dest_addr;
    dest_addr_ip4->sin_addr.s_addr = htonl(INADDR_ANY);
    dest_addr_ip4->sin_family = AF_INET;
    dest_addr_ip4->sin_port = htons( tcpServer->_listenPort );

    *listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

    if (*listen_sock < 0) {
        ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
        tcpServer->_listenTask = NULL;
        vTaskDelete(NULL);
        return;
    }

    int opt = 1;
    setsockopt(*listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    ESP_LOGI(TAG, "listen socket created");

    int err = bind(*listen_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (err != 0) {
        ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
        ESP_LOGE(TAG, "IPPROTO: %d", AF_INET);
        close(*listen_sock);
        tcpServer->_listenTask = NULL;
        vTaskDelete( NULL );
    }
    ESP_LOGI(TAG, "Socket bound, port %d", tcpServer->_listenPort);

    err = listen(*listen_sock, 1);
    if (err != 0) {
        ESP_LOGE(TAG, "Error occurred during listen: errno %d", errno);
        close(*listen_sock);
        tcpServer->_listenTask = NULL;
        vTaskDelete( NULL );
        return;
    }

    while(1)
    {
        ESP_LOGI(TAG, "Socket listening...");
        struct sockaddr_storage source_addr; // Large enough for both IPv4 or IPv6
        socklen_t addr_len = sizeof(source_addr);
        int sock = accept(*listen_sock, (struct sockaddr *)&source_addr, &addr_len);
        if (sock < 0) {
            ESP_LOGE(TAG, "Unable to accept connection: errno %d", errno);
            break;
        }else{
            //初始化app上下文
            TcpServerAppCtx* ctx = new TcpServerAppCtx;
            ctx->_app = tcpServer->_app;
            ctx->sock = sock;
            ctx->keepAlive = 1;
            ctx->keepIdle = KEEPALIVE_IDLE;
            ctx->keepInterval = KEEPALIVE_INTERVAL;
            ctx->keepCount = KEEPALIVE_COUNT;
            ctx->server = tcpServer;

            xSemaphoreTake( tcpServer->appLock , -1 );

            tcpServer->tcpServerAppList.push_back( ctx );
            //创建处理这个客户端的请求的任务
            xTaskCreatePinnedToCore( 
                _serverAppBridge ,
                "tcp server app",
                tcpServer->_appStack,
                (void*)ctx,
                12,
                &ctx->taskHandle,
                tskNO_AFFINITY
            );

            xSemaphoreGive( tcpServer->appLock );
        }
    }

    tcpServer->_listenTask = NULL;
    vTaskDelete( NULL );
    return;
}

//tcp服务器收监听到连接请求后通过这个函数进入app
void TcpServer::_serverAppBridge(void* param)
{
    TcpServerAppCtx* ctx = (TcpServerAppCtx*)param;
    int sock = ctx->sock;
    TcpServer* server = ctx->server;

    ESP_LOGI( TAG , "tcp server bridge created!" );

    setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &ctx->keepAlive, sizeof(int));
    setsockopt(sock, IPPROTO_TCP, TCP_KEEPIDLE, &ctx->keepIdle, sizeof(int));
    setsockopt(sock, IPPROTO_TCP, TCP_KEEPINTVL, &ctx->keepInterval, sizeof(int));
    setsockopt(sock, IPPROTO_TCP, TCP_KEEPCNT, &ctx->keepCount, sizeof(int));
    
    ctx->_app( ctx->sock );

    xSemaphoreTake( server->appLock , -1 );
    shutdown(ctx->sock, 0);
    close(ctx->sock);
    server->tcpServerAppList.remove( ctx );
    delete ctx;
    xSemaphoreGive( server->appLock );

    ESP_LOGI( TAG , "tcp server bridge exit" );
    vTaskDelete( NULL );
}

bool TcpServer::start( uint16_t listen_port , TcpApp app , uint16_t stack )
{
    if( _listenTask != NULL )
        return false;

    _app = app;
    _appStack = stack;
    _listenPort = listen_port;

    xTaskCreatePinnedToCore(
            _serverListen,
            "tcp server listen",
            2048,
            this,
            12,
            &_listenTask,
            tskNO_AFFINITY
        );
    return true;
}

bool TcpServer::stop()
{
    if( _listenPort == NULL )
        return true;

    xSemaphoreTake( appLock , -1 );
    //杀死监听任务
    vTaskDelete( this->_listenTask );
    ESP_LOGI( TAG , "tcp server listen task delete!" );
    //杀死和回收正在运行的客户端处理任务
    for( std::list<TcpServerAppCtx*>::iterator it = tcpServerAppList.begin() ; it != tcpServerAppList.end() ; it++ )
    {
        shutdown( (*it)->sock, 0);
        close( (*it)->sock);
        if( (*it)->taskHandle )
            vTaskDelete( (*it)->taskHandle );
        delete (*it);
    }
    tcpServerAppList.clear();
    //关闭监听端口
    close(listen_sock);
    ESP_LOGI( TAG , "tcp server listen socket close!" );
    xSemaphoreGive( appLock );

    return true;
}

TcpServer::TcpServer()
{
    _app = NULL;
    _connectCount = 0;
    _listenTask = NULL;
    appLock = xSemaphoreCreateBinary();
    xSemaphoreGive( appLock );
}

TcpServer::~TcpServer(  )
{
    stop();
    vSemaphoreDelete( appLock );
}
